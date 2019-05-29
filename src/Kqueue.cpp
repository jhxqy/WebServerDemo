//
//  Kqueue.cpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/5/29.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include "Kqueue.hpp"
KqueueDispatcher::KqueueDispatcher(){}
void KqueueDispatcher::add(Event e){
    events_.push_back(e);
}
void KqueueDispatcher::remove(Event e){
  //  Event.remove(e);
}
