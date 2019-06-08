//
//  Content-Type-Acquisitor.cpp
//  async-http
//
//  Created by 贾皓翔 on 2019/6/8.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include "Content-Type-Acquisitor.hpp" 
#include <fstream>
#include <stdexcept>
ContentTypeAscquisitor::ContentTypeAscquisitor(){
    std::ifstream in("./Content-type.txt");
    if (!in) {
        throw std::runtime_error("没有找到Content-type文件");
    }
    std::string first,second;
    while (in>>first) {
        in>>second;
        map_.insert(std::make_pair(first, second));
    }
}
