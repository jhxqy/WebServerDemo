//
//  test2.cpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/6/5.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include <stdio.h>
#include "HTTP.hpp"
using namespace std;

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
        response.otherHeaders_["Content-Type"]="text/html";
        response.out("are you ok?");
    });
    http->Register("/jump", [](RequestBody &request,ResponseBody &response){
        response.sendRedirect("/world");
    });
    ctx.run();
}
