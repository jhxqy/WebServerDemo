//
//  IOContext.hpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/5/28.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#ifndef IOContext_hpp
#define IOContext_hpp
#include "Select.hpp"
#include "Kqueue.hpp"
#include "Socket.hpp"
#include "Async.hpp"
#include <stdio.h>
namespace HTTP{
    class IOContext{
        std::shared_ptr<DispatcherBase> dispatcher_;
    public:
        IOContext();
        void add(Event e);
        void run();
    };
}

#endif /* IOContext_hpp */
