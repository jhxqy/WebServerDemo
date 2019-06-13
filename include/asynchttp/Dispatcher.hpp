//
//  Dispatcher.hpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/5/28.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#ifndef Dispatcher_hpp
#define Dispatcher_hpp
#include <functional>
#include <stdio.h>
#include <memory>
#include <list>
#include <stdio.h>
#include <sys/event.h>
#include <stdexcept>
#include <sys/types.h>
#include <vector>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>

namespace HTTP {
    struct Event{
        int fd_;
        enum Type{
            WRITE,READ,SIGNAL,TIMEOUT
        };
        Type type_;
        std::function<void(int)> func_;
        bool finished;
    public:
        Event(int fd,Type t,std::function<void(int)> func):fd_(fd),type_(t),func_(func),finished(false){
        }
    public:
        
    };
    
    class DispatcherBase{
    public:
        DispatcherBase(){}
        virtual void add(Event e)=0;
        virtual void remove(Event e)=0;
        virtual int dispatch(struct timespec *)=0;
        virtual ~DispatcherBase(){}
    };
}


#endif /* Dispatcher_hpp */
