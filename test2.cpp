//
//  test2.cpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/6/5.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include <stdio.h>
#include "http_parser.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <iostream>
#include <map>
#include "include/Async.hpp"
#include "include/HttpDispatcher.hpp"
using namespace std;


class Session:public enable_shared_from_this<Session>{
    IOContext &ctx_;
    Descriptor descriptor_;
    http_parser_settings settings;
    http_parser *parser;
    size_t hasRead=0;
    RequestBody requestbody_;
    size_t hasWrite=0;
    std::vector<char> buf;
    int onbody(http_parser*, const char *at, size_t length){
        
        return 0;
        
    }
    int onurl(http_parser*, const char *at, size_t length){
        requestbody_.url_=std::string(at,length);
        return 0;
        
    }
    int onCookies(http_parser*,const char *at,size_t length){
        
        return 0;
    }
    void doRead(){
        auto self=shared_from_this();
        descriptor_.AsyncWaitRead([this,self](int fd){
            size_t readsize=read(fd,&buf[hasRead],buf.capacity());
            if(readsize<=0){
                return ;
            }
            hasRead+=readsize;
            if (readsize==buf.capacity()) {
                buf.resize(buf.capacity()*2);
                doRead();
            }else{
                size_t hasParsered=http_parser_execute(parser, &settings, &buf[0], hasRead);
                if(hasParsered!=hasRead){
                    doRead();
                }else{
                    cout<<requestbody_.url_<<endl;
                    doWrite();
                }
            }
        });
    }
    void doWrite(){
        auto self=shared_from_this();
        descriptor_.AsyncWaitWrite([self,this](int fd){
            char responsebody[]="HTTP/1.1 200 OK\r\nServer: Tiny Web\r\nConnection: close\r\nContent-type: text/plain\r\nContent-Length: 10\r\n\r\nhelloworld";
            write(fd, responsebody, sizeof(responsebody)-1);
            
        });
    };
public:
    void run(){
        doRead();
    }
    Session(IOContext &ctx,int fd):ctx_(ctx),descriptor_(ctx,fd),buf(2048){
        parser=(http_parser*)malloc(sizeof(http_parser));
        http_parser_init(parser,HTTP_REQUEST);
        memset(&settings,0,sizeof(settings));
        settings.on_url=bind(&Session::onurl, this,placeholders::_1,placeholders::_2,placeholders::_3);
        settings.on_body=bind(&Session::onbody, this,placeholders::_1,placeholders::_2,placeholders::_3);
    }
    ~Session(){
        cout<<"链接已断开"<<endl;
        close(descriptor_.NativeFd());
    }
};

class Serv{
    IOContext &ctx_;
    Acceptor a;
    void listen(){
        a.AsyncWaitAccept([this](int fd){
            int socket= accept(fd, nullptr, nullptr);
            cout<<"有用户进入了"<<endl;
            make_shared<Session>(ctx_,socket)->run();
            this->listen();
        });
    }
public:
    Serv(IOContext &ctx,const char * serv):ctx_(ctx),a(ctx,nullptr,serv){
        listen();
    }
};
int main(){
    IOContext ctx;
    Serv s(ctx,"9998");
    ctx.run();
}
