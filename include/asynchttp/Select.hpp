//
//  Select.hpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/5/28.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#ifndef Select_hpp
#define Select_hpp
#include "Dispatcher.hpp"

namespace HTTP {
    class SelectDispatcher:public DispatcherBase{
        
        std::list<Event> events_;
    public:
        SelectDispatcher();
        virtual void add(Event e) override;
        virtual void remove(Event e) override;
        virtual int dispatch(struct timespec *) override;
        virtual ~SelectDispatcher();
    };
}


#endif /* Select_hpp */
