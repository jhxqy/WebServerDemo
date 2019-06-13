//
//  IOContext.cpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/5/28.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include "IOContext.hpp"
#include <memory>
namespace HTTP {
    IOContext::IOContext():dispatcher_(std::make_shared<KqueueDispatcher>()){
        
    }
    void IOContext::add(Event e){
        dispatcher_->add(e);
    }
    void IOContext::run(){
        dispatcher_->dispatch(nullptr);
    }
    

}
