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
#include "Content-Type-Acquisitor.hpp"
#include <functional>
struct RequestBody{
    std::string url_;
    std::map<std::string,std::string> cookies_;
    std::map<std::string,std::string> headers_;
    bool isFile;
    std::string suffix;
    std::string parameters;
    RequestBody():isFile(false){
        
    }
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
    void sendRedirect(const std::string &newurl){
        status=302;
        otherHeaders_["Location"]=newurl;
    }
    ResponseBody(){
        status=200;
        otherHeaders_["Content-Type"]="text/html";
    }
};


class HttpDispatcher{
public:
    using CallBackType=std::function<void(RequestBody&,ResponseBody&)>;
    virtual int Register(const std::string& url,CallBackType )=0;
    virtual ResponseBody dispatch(RequestBody &request)=0;
    virtual void SetDefaultPath(const std::string &url)=0;
    virtual ~HttpDispatcher(){}
    
};

class HttpDispatcherImpl:public HttpDispatcher{
    HttpDispatcherImpl(){
        
    }
    std::unordered_map<std::string, CallBackType> map_;
    std::string defaultPath_;
    void fromPath(RequestBody &request,ResponseBody &response);
    std::unordered_map<std::string, std::string> contentTypeTable_;
    ContentTypeAscquisitor cta_;

public:
    static HttpDispatcherImpl* Create(){
        static HttpDispatcherImpl *Singleton=nullptr;
        if(Singleton==nullptr){
            Singleton=new HttpDispatcherImpl();
        }
        return Singleton;
    }
    void SetDefaultPath(const std::string&url) override;
    ResponseBody dispatch(RequestBody &request) override;
    int Register(const std::string &url,CallBackType) override;
    ~HttpDispatcherImpl(){}
    
};


#endif /* HttpDispatcher_hpp */
