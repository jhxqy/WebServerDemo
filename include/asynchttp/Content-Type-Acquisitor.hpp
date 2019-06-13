//
//  Content-Type-Acquisitor.hpp
//  async-http
//
//  Created by 贾皓翔 on 2019/6/8.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#ifndef Content_Type_Acquisitor_hpp
#define Content_Type_Acquisitor_hpp

#include <stdio.h>
#include <string>
#include <unordered_map>
namespace HTTP{
    class ContentTypeAscquisitor{
        std::unordered_map<std::string, std::string> map_;
    public:
        ContentTypeAscquisitor();
        std::string get(const std::string &suffix){
            if(map_.count(suffix)!=0){
                return map_[suffix];
            }else{
                return "";
            }
        }
    };
}


#endif /* Content_Type_Acquisitor_hpp */
