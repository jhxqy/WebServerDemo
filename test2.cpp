//
//  test2.cpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/6/5.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include <stdio.h>
#include "asynchttp/HTTP.hpp"
using namespace std;
using namespace HTTP;
int main(){
    IOContext ctx;
    Serv s(ctx,"8088");
    HttpDispatcher *http=HttpDispatcherImpl::Create();
    http->SetDefaultPath("./web");
    http->Register("/hello", [](RequestBody &request,ResponseBody &response){
        response.status=200;
        response.otherHeaders_["Content-Type"]="text/html";
        response.out("hello Nice my HTTP!");
    });
    
    http->Register("/world", [](RequestBody &request,ResponseBody &response){
        response.status=200;
        response.otherHeaders_["Content-Type"]="text/html;charset=utf-8";
        for(auto i:request.headers_){
            response.out("%s:%s<br>",i.first.c_str(),i.second.c_str());
        }
        for(auto i:request.cookies_){
            response.out("%s=%s<br>",i.first.c_str(),i.second.c_str());
            
        }
    });
    http->Register("/jump", [](RequestBody &request,ResponseBody &response){
        response.sendRedirect("/world");
        
    });
    http->Register("/param", [](RequestBody &request,ResponseBody& response){
        response.out("param:%s",request.parameters.c_str());
    });
    
    http->Register("/forward", [](RequestBody &req,ResponseBody &res){
        res.out("这是跳转前第一个接口<br>");
        res.forward(req,"/world");
    });
    http->RegisterFilter("/hello", [](RequestBody &request,ResponseBody &response){
        cout<<"调用第一个过滤器！"<<endl;
        return FILTER_NEXT;
    });
    http->RegisterFilter("/index.html", [](RequestBody &request,ResponseBody &response){
        if (request.parameters.compare("hello")!=0) {
            cout<<request.parameters<<endl;
            response.forward(request, "/hello");
            return FILTER_END;
        }
        cout<<"NICE!"<<endl;
        return FILTER_NEXT;
    });
    
    ctx.run();
}
