//
//  main.cpp
//  MyDemo
//
//  Created by Sprax Lines on 6/6/16.
//  Copyright © 2016 Sprax Lines. All rights reserved.
//

#include "coutOne.h"
#include "printOne.h"

int main(int argc, const char * argv[])
{
    const char *cstr = "Press one if you meant 'one'.";
    printOne(cstr);

    const std::string sstr(cstr);
    coutOne(sstr);
    
    return 0;
}
