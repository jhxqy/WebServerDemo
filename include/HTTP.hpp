//
//  HTTP.hpp
//  async-http
//
//  Created by 贾皓翔 on 2019/6/6.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#ifndef HTTP_hpp
#define HTTP_hpp

#include <stdio.h>
#include "Async.hpp"
#include "HttpDispatcher.hpp"
#include "http_parser.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <iostream>
#include <map>


class Session:public std::enable_shared_from_this<Session>{
    IOContext &ctx_;
    Descriptor descriptor_;
    http_parser_settings settings;
    http_parser *parser;
    size_t hasRead=0;
    RequestBody requestbody_;
    size_t hasWrite=0;
    std::vector<char> buf;
    std::string responsePacket_;
    int onbody(http_parser*, const char *at, size_t length){
        return 0;
    }
    int onurl(http_parser*, const char *at, size_t length){
        //requestbody_.url_=std::string(at,length);
        size_t i=0;
        size_t posP=length;
        for(i=0;i<length;i++){
            if(at[i]=='?'){
                break;
            }
        }
        if(i!=length)
            posP=i;
        requestbody_.url_=std::string(at,posP);
        size_t posPoint=requestbody_.url_.rfind('.');
        if (posPoint==std::string::npos) {
            requestbody_.isFile=false;
        }else{
            requestbody_.isFile=true;
        requestbody_.suffix=requestbody_.url_.substr(posPoint,requestbody_.url_.size());
        }
        
        /*
         此处应该分析一波参数。
         
         */
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
                    std::cout<<requestbody_.url_<<std::endl;
                    doWrite();
                }
            }
        });
    }
    void doWrite(){
        auto self=shared_from_this();
        descriptor_.AsyncWaitWrite([self,this](int fd){
            if(hasWrite==0){
                HttpDispatcher *dispatcher=HttpDispatcherImpl::Create();
                ResponseBody response= dispatcher->dispatch(requestbody_);
                responsePacket_=response.getPacket();
                
            }
            size_t s= write(fd, &responsePacket_[hasWrite], responsePacket_.size()-hasWrite);
            if (s<=0) {
                return;
            }
            hasWrite+=s;
            if (hasWrite<responsePacket_.size()) {
                doWrite();
            }
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
        settings.on_url=std::bind(&Session::onurl, this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
        settings.on_body=std::bind(&Session::onbody, this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    }
    ~Session(){
        std::cout<<"链接已断开"<<std::endl;
        close(descriptor_.NativeFd());
    }
};

class Serv{
    IOContext &ctx_;
    Acceptor a;
    void listen(){
        a.AsyncWaitAccept([this](int fd){
            int socket= accept(fd, nullptr, nullptr);
            std::cout<<"有用户进入了"<<std::endl;
            std::make_shared<Session>(ctx_,socket)->run();
            this->listen();
        });
    }
public:
    Serv(IOContext &ctx,const char * serv):ctx_(ctx),a(ctx,nullptr,serv){
        listen();
    }
};


#endif /* HTTP_hpp */
