//
//  main.cpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/5/28.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include <iostream>
#include "Async.hpp"
using namespace std;
class Session:public enable_shared_from_this<Session>{
    IOContext &ctx_;
    Descriptor descriptor_;
    size_t needSend;
    char buf[1024];
    void doRead(){
        auto self=shared_from_this();
        descriptor_.AsyncWaitRead([this,self](int fd){
            needSend=read(fd, buf, 1024);
            buf[needSend]=0;
            cout<<buf;
            if(needSend!=0){
                self->doWrite();
            }else{
                cout<<"用户退出"<<endl;
            }
        });
    }
    void doWrite(){
        auto self=shared_from_this();
        descriptor_.AsyncWaitWrite([this,self](int fd){
            size_t st=write(fd, buf, needSend);
            if(st!=0){
                needSend-=st;
                if(needSend>0){
                    doWrite();
                }else{
                    doRead();
                }
            }
        });
    }
public:
    void run(){
        doRead();
    }
    Session(IOContext &ctx,int fd):ctx_(ctx),descriptor_(ctx,fd){
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
