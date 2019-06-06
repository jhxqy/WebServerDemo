//
//  HttpDispatcher.cpp
//  async-http
//
//  Created by 贾皓翔 on 2019/6/6.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include "HttpDispatcher.hpp"
int HttpDispatcherImpl::Register(const std::string &url, CallBackType func){
    if (map_.count(url)==1) {
        map_[url]=func;
        return 1;
    }
    map_[url]=func;
    return 0;
}


ResponseBody HttpDispatcherImpl::dispatch(const RequestBody &request){
    ResponseBody response;
    if (map_.count(request.url_)==1) {
        map_[request.url_](request,response);
        return response;
    }else{
        //默认情况
        return response;
    }
}
