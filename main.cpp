//
//  main.cpp
//  EZasync
//
//  Created by 贾皓翔 on 2019/5/28.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include <iostream>
#include "Async.hpp"
#include "http_parser.h"
#include "HttpDispatcher.hpp"
using namespace std;
int main(){
    ResponseBody r;
    r.status=200;
    r.otherHeaders_["Content-Type"]="text/html";
    r.out("helloworld%d",100);
    cout<<r.getPacket()<<endl;
}
