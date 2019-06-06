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
using namespace std;
int main(){
    http_parser *pa=static_cast<http_parser*>(malloc(sizeof(http_parser)));
    http_parser_init(pa, HTTP_REQUEST);
    char buf[]="GET /2016121034000012.png HTTP/1.1\r\nHost: admin.omsg.cn\r\nAccept: */*\r";
    char buf2[]="\nConnection: Keep-Alive\r\n\r\n";
    http_parser_settings s;
//    s.on_body=[](http_parser*, const char *at, size_t length){
//        return 0;
//    };
    memset(&s,0 , sizeof(s));
    size_t n=http_parser_execute(pa, &s, buf, sizeof(buf)-1);
    cout<<n<<endl;
    cout<<sizeof(buf)-1<<endl;
    n=http_parser_execute(pa, &s, buf2, sizeof(buf2)-1);
    cout<<n<<endl;
    cout<<sizeof(buf2)-1<<endl;
}
