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

std::string HttpDispatcherImpl::getSuffix(std::string &s){
    size_t pos=s.rfind('.');
    if (pos==std::string::npos) {
        return "";
    }
    return s.substr(pos+1,s.size());
}

void HttpDispatcherImpl::fromPath(RequestBody &request,ResponseBody &response){
    //如果没有设置默认目录，则直接返回空文件
    if (defaultPath_.compare("")==0) {
        return;
    }
    std::string &suffix=request.suffix;
    //不存在后缀的情况，即打开的不是一个文件
    if (suffix=="") {
        return;
    }
    if (suffix.compare("html")==0 ||suffix.compare("htm")==0) {
        response.otherHeaders_["Content-Type"]="text/html;charset=UTF-8";
        
    }else if(suffix.compare("js")==0){
        response.otherHeaders_["Content-Type"]="application/javascript;charset=UTF-8";
    }else if(suffix.compare("css")==0){
        response.otherHeaders_["Content-Type"]="text/css; charset=UTF-8";
    }else if(suffix.compare("txt")==0){
        response.otherHeaders_["Content-Type"]="text/plain; charset=UTF-8";
    }else if(suffix.compare("manifest")==0){
        response.otherHeaders_["Content-Type"]="text/cache-mainifset; charset=UTF-8";
    }else{
        response.otherHeaders_["Content-Type"]="text/plain; charset=UTF-8";
    }
    
    
    
}
