//
//  Socket.hpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/5/28.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#ifndef Socket_hpp
#define Socket_hpp

#include <stdio.h>
#include <functional>

#include <unistd.h>
class IOContext;
class Acceptor{
    IOContext &ctx_;
    int nativeFd_;
public:
    ~Acceptor(){
        close(nativeFd_);
    }
    explicit Acceptor(IOContext &ctx,const char *host,const char *serv);
    void AsyncWaitAccept(std::function<void(int)>);
};

class Descriptor{
    int nativeFd_;
    IOContext &ctx_;
public:
    explicit Descriptor(IOContext &ctx,int fd):nativeFd_(fd),ctx_(ctx){
    }
    void AsyncWaitRead(std::function<void(int)>);
    void AsyncWaitWrite(std::function<void(int)>);
    ~Descriptor(){
    };
};
#endif /* Socket_hpp */
