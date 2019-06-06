//
//  HttpDispatcher.hpp
//  async-http
//
//  Created by 贾皓翔 on 2019/6/6.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#ifndef HttpDispatcher_hpp
#define HttpDispatcher_hpp

#include <stdio.h>
#include <map>
#include <unordered_map>
#include <string>
#include <functional>
struct RequestBody{
    std::string url_;
    std::map<std::string,std::string> cookies_;
    std::string userAgent_;
    
};
struct ResponseBody{
    int status;
    std::map<std::string,std::string> cookies_;
    std::map<std::string,std::string> otherHeaders_;
    std::string body_;
    std::string packet_;
    std::string getPacket();
    template<typename...Args>
    void out(Args&&...rest){
        char buf[2048]; //此处一次添加最多2048个字节
        snprintf(buf, 2048, std::forward<Args>(rest)...);
        body_+=buf;
    }
};


class HttpDispatcher{
public:
    virtual int Register(const std::string& url,std::function<void(RequestBody,ResponseBody&)> )=0;
    virtual ResponseBody dispatch(const RequestBody &request)=0;
    virtual ~HttpDispatcher(){}
};

class HttpDispatcherImpl:public HttpDispatcher{
    HttpDispatcherImpl(){}
    using CallBackType=std::function<void(RequestBody,ResponseBody&)>;
    std::unordered_map<std::string, CallBackType> map_;
public:
    static HttpDispatcher* Create(){
        static HttpDispatcherImpl *Singleton=nullptr;
        if(Singleton==nullptr){
            Singleton=new HttpDispatcherImpl();
        }
        return Singleton;
    }
    ResponseBody dispatch(const RequestBody &request) override;
    int Register(const std::string &url,CallBackType) override;
    ~HttpDispatcherImpl(){}
    
};


#endif /* HttpDispatcher_hpp */
