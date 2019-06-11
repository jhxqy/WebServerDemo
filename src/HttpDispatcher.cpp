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

struct StatusTable{
    std::string table[506];
    StatusTable(){
        table[100]="Continue";
        table[101]="Switching Protocols";
        table[200]="OK";
        table[201]="Created";
        table[202]="Accepted";
        table[203]="Non-Authoritative Information";
        table[204]="No Content";
        table[205]="Reset Content";
        table[206]="Partial Content";
        table[300]="Multiple Choices";
        table[301]="Moved Permanently";
        table[302]="Found";
        table[303]="See Other";
        table[304]="Not Modified";
        table[305]="Use Proxy";
        table[306]="Unused";
        table[307]="Temporary Redirect";
        table[400]="Bad Request";
        table[401]="Unauthorized";
        table[402]="Payment Required";
        table[403]="Forbidden";
        table[404]="Not Found";
        table[405]="Method Not Allowed";
        table[406]="Not Acceptable";
        table[407]="Proxy Authentication Required";
        table[408]="Request Time-out";
        table[409]="Conflict";
        table[410]="Gone";
        table[411]="Length Required";
        table[412]="Precondition Failed";
        table[413]="Request Entity Too Large";
        table[414]="Request-URI Too Large";
        table[415]="Unsupported Media Type";
        table[416]="Requested range not satisfiable";
        table[417]="Expectation Failed";
        table[500]="Internal Server Error";
        table[501]="Not Implemented";
        table[502]="Bad Gateway";
        table[503]="Service Unavailable";
        table[504]="Gateway Time-out";
        table[505]="HTTP Version not supported";
    }
};

std::string StatusMessage(int status){
    static StatusTable st;
    if (st.table[status]=="") {
        return "OK";
    }else{
        return st.table[status];
    }

}
void ResponseBody::forward(RequestBody &request,const std::string &url){
    request.nativeURL_=url;
    httpDispatcher_.dispatch(request, *this);
}
std::string ResponseBody::getPacket(){
    std::stringstream ss;
    ss<<"HTTP/1.1 "<<status<<" "<<StatusMessage(status)<<"\r\n";
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
void HttpDispatcherImpl::parserUrl(RequestBody &request){
    size_t length=request.nativeURL_.size();
    char *at=&request.nativeURL_[0];
    size_t i=0;
    size_t posP=length;
    for(i=0;i<length;i++){
        if(at[i]=='?'){
            break;
        }
    }
    if(i!=length)
        posP=i;
    request.url_=std::string(at,posP);
    size_t posPoint=request.url_.rfind('.');
    if (posPoint==std::string::npos) {
        request.isFile=false;
    }else{
        request.isFile=true;
        request.suffix=request.url_.substr(posPoint,request.url_.size());
    }
    
    if (posP!=length) {
        request.parameters=std::string(&at[posP+1],length-posP);
    }
}


ResponseBody HttpDispatcherImpl::dispatch(RequestBody &request,ResponseBody &response){
   // ResponseBody response(*this);
    parserUrl(request);
    if(request.isFile==false){
        if (map_.count(request.url_)==1) {
//            response.otherHeaders_["Content-Type"]="text/html;charset=utf-8";
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
