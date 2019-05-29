//
//  Kqueue.cpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/5/29.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include "Kqueue.hpp"
KqueueDispatcher::KqueueDispatcher(){
    kd_=kqueue();
    if(kd_==-1){
        throw std::runtime_error("create kqueue error");
    }
    
}
void KqueueDispatcher::add(Event e){
    events_.push_back(e);
}
void KqueueDispatcher::remove(Event e){
  //  Event.remove(e);
}

int KqueueDispatcher::dispatch(struct timespec *t ){
    
    while (!events_.empty()) {
        std::vector<struct kevent> v(events_.size());
        int n=0;
        for(auto i=events_.begin();i!=events_.end();i++,n++){
            if(i->type_==Event::Type::WRITE){
                EV_SET(&v[n], i->fd_, EVFILT_WRITE, EV_ADD|EV_CLEAR, NOTE_WRITE, 0, nullptr);
            }else if(i->type_==Event::Type::READ){
                EV_SET(&v[n],i->fd_, EVFILT_READ, EV_ADD|EV_CLEAR, 0, 0, nullptr);
            }
            kevent(kd_, &v[n], 1, nullptr, 0, nullptr);
        }
        
        
        int res=kevent(kd_, nullptr, 0, &v[0], (int)events_.size(), t);
        for (int i=0;i<res; i++) {
            for(auto iter=events_.begin();iter!=events_.end();iter++){
                if (iter->fd_==v[i].ident) {
                    iter->func_(iter->fd_);
                    events_.erase(iter);
                    break;
                }
            }
        }
    }
    return 0;
}

KqueueDispatcher::~KqueueDispatcher(){
    
}
