//
//  HTTP.cpp
//  async-http
//
//  Created by 贾皓翔 on 2019/6/6.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include "HTTP.hpp"
int Session::onbody(http_parser *, const char *at, size_t length){
    std::cout<<std::string(at,length)<<std::endl;
    return 0;
}
int Session::onCookies(http_parser *, const char *at, size_t length){
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
    return 0;
}
