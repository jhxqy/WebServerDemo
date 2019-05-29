//
//  Kqueue.hpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/5/29.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#ifndef Kqueue_hpp
#define Kqueue_hpp

#include <stdio.h>
#include "Dispatcher.hpp"
class KqueueDispatcher:public DispatcherBase{
    std::list<Event> events_;
    
public:
    KqueueDispatcher();
    virtual void add(Event e) override;
    virtual void remove(Event e) override;
    virtual int dispatch(struct timespec *) override;
    virtual ~KqueueDispatcher();
};

#endif /* Kqueue_hpp */
