//
//  HttpDispatcher.cpp
//  async-http
//
//  Created by 贾皓翔 on 2019/6/6.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include "HttpDispatcher.hpp"
#include <sstream>

std::string ResponseBody::getPacket(){
    std::stringstream ss;
    ss<<"HTTP/1.1 "<<status<<" OK\r\n";
    if (cookies_.size()!=0) {
        ss<<"Cookie:";
        for(auto i=cookies_.begin();i!=cookies_.end();i++){
            ss<<i->first<<"="<<i->second<<";";
        }
         ss<<"\r\n";
    }
   
    for(auto i:otherHeaders_){
        ss<<i.first<<": "<<i.second<<"\r\n";
    }
    ss<<"Content-Length: "<<body_.size()<<"\r\n";
    ss<<"\r\n";
    ss<<body_;
    packet_=ss.str();
    return packet_;
}



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
        
        return response;
    }
}

void HttpDispatcherImpl::SetDefaultPath(const std::string &url){
    this->defaultPath_=url;
}

