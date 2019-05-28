//
//  Socket.cpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/5/28.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include "Socket.hpp"
#include <stdexcept>
#include <sys/types.h>
#include <vector>
#include <sys/socket.h>
#include <netdb.h>
#include "IOContext.hpp"
Acceptor::Acceptor(IOContext &ctx,const char *host,const char *serv):ctx_(ctx){
    const int on=1;
    addrinfo hint,*res=nullptr;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family=AF_UNSPEC;
    hint.ai_socktype=SOCK_STREAM;
    hint.ai_flags=AI_PASSIVE;
    int error;
    if((error=getaddrinfo(host, serv, &hint,  &res))!=0){
        
    }
    for(;res!=nullptr;res=res->ai_next){
        nativeFd_=socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (nativeFd_<0) {
            throw std::runtime_error("socket error");
        }
        
        setsockopt(nativeFd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        if(bind(nativeFd_, res->ai_addr, res->ai_addrlen)==0){
            break;
        }
        close(nativeFd_);
        
    }
    if(res==nullptr){
        throw std::runtime_error("create acceptor error");
    }
    freeaddrinfo(res);
    listen(nativeFd_, 1024);
}

void Acceptor::AsyncWaitAccept(std::function<void (int)> func){
    ctx_.add(Event(nativeFd_,Event::READ,func));
}

void Descriptor::AsyncWaitRead(std::function<void (int)> func){
    ctx_.add(Event(nativeFd_,Event::Type::READ,func));
}
void Descriptor::AsyncWaitWrite(std::function<void (int)> func){
    ctx_.add(Event(nativeFd_,Event::Type::WRITE,func));
}

