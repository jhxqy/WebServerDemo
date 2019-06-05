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
#include <iostream>
#include <map>
#include "include/Async.hpp"
using namespace std;
class Session:public enable_shared_from_this<Session>{
    IOContext &ctx_;
    Descriptor descriptor_;
    http_parser_settings settings;
    http_parser *parser;
    size_t readfromclient;
    std::map<string, string> cookies;
    char buf[8192];
    
    int onbody(http_parser*, const char *at, size_t length){return 0;}
    int onurl(http_parser*, const char *at, size_t length){
        cout<<string(at,length)<<endl;
        return 0;}

    void doRead(){
        auto self=shared_from_this();
        descriptor_.AsyncWaitRead([this,self](int fd){
            readfromclient+=read(fd,buf,8192);
            if(readfromclient<=0){
                return;
            }
            size_t hasParsered=http_parser_execute(parser, &settings, buf, readfromclient);
            if(hasParsered!=readfromclient){
                doRead();
            }else{
                doWrite();
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
    Session(IOContext &ctx,int fd):ctx_(ctx),descriptor_(ctx,fd),readfromclient(0){
        parser=(http_parser*)malloc(sizeof(http_parser));
        http_parser_init(parser,HTTP_REQUEST);
        settings.on_url=bind(&Session::onurl, this,placeholders::_1,placeholders::_2,placeholders::_3);
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
