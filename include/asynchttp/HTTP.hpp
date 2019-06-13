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
    std::pair<std::string, std::string> tempHeader;
    int onbody(http_parser*, const char *at, size_t length);
    int onurl(http_parser*, const char *at, size_t length);
    int onheaders_field(http_parser*, const char *at, size_t length);
    int onheaders_value(http_parser*, const char *at, size_t length);
    int onstatus(http_parser *, const char *at, size_t length);
    void keepAliveInit();
    void doRead();
    void doWrite();
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
        
        settings.on_header_value=std::bind(&Session::onheaders_value,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
        settings.on_header_field=std::bind(&Session::onheaders_field,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
        settings.on_status=std::bind(&Session::onstatus,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
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
