//
//  Kqueue.cpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/5/29.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include "Kqueue.hpp"
constexpr int maxEvent=10000;
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
    std::vector<struct kevent> v(maxEvent);
    while (!events_.empty()) {
        int n=0;
        for(auto i=events_.begin();i!=events_.end();i++,n++){
            if(i->type_==Event::Type::WRITE){
                EV_SET(&v[n], i->fd_, EVFILT_WRITE, EV_ADD|EV_CLEAR, NOTE_WRITE, 0,nullptr);
            }else if(i->type_==Event::Type::READ){
                EV_SET(&v[n],i->fd_, EVFILT_READ, EV_ADD|EV_CLEAR, 0, 0, nullptr);
            }
            v[n].udata=new decltype(i)(i);
            kevent(kd_, &v[n], 1, nullptr, 0, nullptr);
        }
        
        
        int res=kevent(kd_, nullptr, 0, &v[0], maxEvent, t);
        for (int i=0;i<res;i++) {
//            for(auto iter=events_.begin();iter!=events_.end();iter++){
//                if (iter->fd_==v[i].ident) {
//                    iter->func_(iter->fd_);
//                    v[i].flags=EV_DELETE;
//                    kevent(kd_, &v[i], 1,nullptr , 0,nullptr);
//                    events_.erase(iter);
//                    break;
//                }
//            }
            std::list<Event>::iterator *iter=static_cast<std::list<Event>::iterator*>(v[i].udata);
            (*iter)->func_((*iter)->fd_);
            events_.erase(*iter);
            delete static_cast<std::list<Event>::iterator*>(v[i].udata);
            v[i].flags=EV_DELETE;
            v[i].udata=nullptr;
            kevent(kd_, &v[i], 1,nullptr , 0,nullptr);
        }
    }
    return 0;
}

KqueueDispatcher::~KqueueDispatcher(){
    
}
