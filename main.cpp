//
//  main.cpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/5/28.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include <iostream>
#include "asynchttp/HTTP.hpp"
#include <cstdlib>
#include <unordered_map>

using namespace HTTP;
using namespace std;
class Random{
public:
    Random(){
        srand(time(nullptr));
    }
};
int createRand(){
    static Random r;
    return rand();
}


int main(){
    IOContext ctx;
    Serv s(ctx,"8088");
    HttpDispatcher *http=HttpDispatcherImpl::Create();
    std::unordered_map<string,int> m;
    http->Register("/count", [&m](RequestBody &request,ResponseBody&response){
        
        if (m.count(request.cookies_["SESSION"])==0) {
            cout<<"Session不存在 创建新SESSION"<<endl;
            string newSession=to_string(createRand());
            request.cookies_.insert(make_pair("SESSION",newSession));
        }
        response.cookies_.insert(make_pair("SESSION", request.cookies_["SESSION"]));
        response.out("这是您的第%d次访问！",m[request.cookies_["SESSION"]]);
//        printf("这是您的第%d次访问！\n",m[request.cookies_["SESSION"]]);
//        cout<<"request COOKIE:"<<endl;
//        for(auto i:request.cookies_){
//            cout<<" "<<i.first<<"="<<i.second<<endl;
//        }
//        cout<<endl;
//        cout<<"response COOKIE:"<<endl;;
//        for(auto i:response.cookies_){
//            cout<<" "<<i.first<<"="<<i.second<<endl;
//        }
//        cout<<endl;
//        cout<<"session map:"<<endl;;
//        for(auto i:m){
//            cout<<" "<<i.first<<"="<<i.second<<endl;
//        }
//        cout<<endl;
        
        m[request.cookies_["SESSION"]]=m[request.cookies_["SESSION"]]+1;
        
    });
    ctx.run();

}
