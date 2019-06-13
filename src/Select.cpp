//
//  Select./Users/jiahaoxiang/个人文件/weblearn/EZasync/EZasync/Dispatcher.cppcpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/5/28.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include "Select.hpp"
namespace HTTP{
    
    void SelectDispatcher::add(Event e){
        events_.push_back(e);
    }
    void SelectDispatcher::remove(Event e){
        // lists.remove(e);
    }
    
    int SelectDispatcher::dispatch(struct timespec *t){
        while (!events_.empty()) {
            fd_set ReadSet;
            fd_set WriteSet;
            __DARWIN_FD_ZERO(&ReadSet);
            __DARWIN_FD_ZERO(&WriteSet);
            int maxFd=0;
            for(auto &i:events_){
                maxFd=maxFd>i.fd_?maxFd:i.fd_;
                switch (i.type_) {
                    case Event::READ:
                        //   std::cout<<"Read "<<i.fd_<<std::endl;;
                        __DARWIN_FD_SET(i.fd_, &ReadSet);
                        break;
                    case Event::WRITE:
                        //   std::cout<<"Write "<<i.fd_<<std::endl;
                        __DARWIN_FD_SET(i.fd_, &WriteSet);
                        break;
                    default:
                        break;
                }
            }
            //    std::cout<<std::endl;
            maxFd++;
            int res=select(maxFd, &ReadSet, &WriteSet, nullptr, nullptr);
            
            if(res==-1){
                throw std::runtime_error("select error");
            }
            for(auto i=events_.begin();i!=events_.end();++i){
                if(i->type_==Event::Type::READ&&__DARWIN_FD_ISSET(i->fd_, &ReadSet)){
                    i->func_(i->fd_);
                    __DARWIN_FD_CLR(i->fd_, &ReadSet);
                    events_.erase(i);
                }else if(i->type_==Event::Type::WRITE&&__DARWIN_FD_ISSET(i->fd_, &WriteSet)){
                    i->func_(i->fd_);
                    __DARWIN_FD_CLR(i->fd_, &WriteSet);
                    events_.erase(i);
                }
                
            }
            events_.remove_if([](Event &e){
                return e.finished;
            });
        }
        return 0;
    }
    SelectDispatcher::~SelectDispatcher(){}
    SelectDispatcher::SelectDispatcher(){}

}
