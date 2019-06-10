//
//  HTTP.cpp
//  async-http
//
//  Created by 贾皓翔 on 2019/6/6.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include "HTTP.hpp"

void Session::doRead(){
    auto self=shared_from_this();
    descriptor_.AsyncWaitRead([this,self](int fd){
        size_t readsize=read(fd,&buf[hasRead],buf.capacity());
        if(readsize<=0){
            return ;
        }
        hasRead+=readsize;
        if (readsize==buf.capacity()) {
            buf.resize(buf.capacity()*2);
            doRead();
        }else{
            size_t hasParsered=http_parser_execute(parser, &settings, &buf[0], hasRead);
            if(hasParsered!=hasRead){
                doRead();
            }else{
                std::cout<<requestbody_.url_<<std::endl;
                doWrite();
            }
        }
    });
}
void Session::doWrite(){
    auto self=shared_from_this();
    descriptor_.AsyncWaitWrite([self,this](int fd){
        if(hasWrite==0){
            HttpDispatcher *dispatcher=HttpDispatcherImpl::Create();
            ResponseBody response= dispatcher->dispatch(requestbody_);
            responsePacket_=response.getPacket();
        }
        size_t s= write(fd, &responsePacket_[hasWrite], responsePacket_.size()-hasWrite);
        if (s<=0) {
            return;
        }
        hasWrite+=s;
        if (hasWrite<responsePacket_.size()) {
            doWrite();
        }else{
//            if (requestbody_.headers_["Connection"].compare("keep-alive")==0) {
//                this->keepAliveInit();
//                doRead();
//            }
        }
        
    });
}

int Session::onbody(http_parser *, const char *at, size_t length){
    requestbody_.parameters=std::string(at,length);
    return 0;
}
int Session::onstatus(http_parser *, const char *at, size_t length){
    return 0;
}
int Session::onheaders_field(http_parser *, const char *at, size_t length){
    tempHeader.first=std::string(at,length);
    return 0;
}
int Session::onheaders_value(http_parser *, const char *at, size_t length){
    tempHeader.second=std::string(at,length);
    if (tempHeader.first.compare("Cookie")==0) {
        size_t lastfen=0;
        std::string &a=tempHeader.second;
        for (size_t i=0; i<=a.size();i++) {
            if (i==(a.size())||a[i]==';') {
                for(size_t j=lastfen;j<i;j++){
                    if (a[j]=='=') {
                        requestbody_.cookies_.insert(std::make_pair(std::string(&a[lastfen],j-lastfen), std::string(&a[j+1],i-j-1)));
                        lastfen=i+1;
                        break;
                    }
                }
            }
        }
    }else{
        requestbody_.headers_.insert(tempHeader);
        
    }
    return 0;
}
int Session::onurl(http_parser *, const char * at,size_t length){
    //requestbody_.url_=std::string(at,length);
    size_t i=0;
    size_t posP=length;
    for(i=0;i<length;i++){
        if(at[i]=='?'){
            break;
        }
    }
    if(i!=length)
        posP=i;
    requestbody_.url_=std::string(at,posP);
    size_t posPoint=requestbody_.url_.rfind('.');
    if (posPoint==std::string::npos) {
        requestbody_.isFile=false;
    }else{
        requestbody_.isFile=true;
        requestbody_.suffix=requestbody_.url_.substr(posPoint,requestbody_.url_.size());
    }
    
    /*
     此处应该分析一波参数。
     
     */
    if (posP!=length) {
        requestbody_.parameters=std::string(&at[posP+1],length-posP);
    }
    return 0;
}


void Session::keepAliveInit(){
    http_parser_init(parser, HTTP_REQUEST);
    responsePacket_.clear();
    buf.clear();
    hasRead=0;
    hasWrite=0;
    requestbody_=RequestBody();
    
}
