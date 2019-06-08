//
//  HttpDispatcher.cpp
//  async-http
//
//  Created by 贾皓翔 on 2019/6/6.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include "HttpDispatcher.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
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

ResponseBody HttpDispatcherImpl::dispatch(RequestBody &request){
    ResponseBody response;
    if(request.isFile==false){
        if (map_.count(request.url_)==1) {
            map_[request.url_](request,response);
            return response;
        }else{
            
            return response;
        }
    }
    //处理文件情况;
    fromPath(request, response);
    return response;
    
    
}

void HttpDispatcherImpl::SetDefaultPath(const std::string &url){
    this->defaultPath_=url;
}



void HttpDispatcherImpl::fromPath(RequestBody &request,ResponseBody &response){
    //如果没有设置默认目录，则直接返回空文件
    if (defaultPath_.compare("")==0) {
        return;
    }
    std::string &suffix=request.suffix;
    //不存在后缀的情况，即打开的不是一个文件
    std::string type=cta_.get(suffix);
    if (type=="") {
        return ;
    }
    response.otherHeaders_["Content-Type"]=type;
    std::ifstream in(defaultPath_+request.url_);
    if (!in) {
        return;
    }

    
    response.body_=std::string(std::istreambuf_iterator<char>(in),std::istreambuf_iterator<char>());
    return;
    
    
    
    
}
