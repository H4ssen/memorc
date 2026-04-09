#include <vector>
#include <cstring>
#include <array>
#include <iostream>
//#include <thread>
#include <chrono>
#include "memorc/memorc.h"
#include <gtest/gtest.h>
#include <algorithm>

std::string suiteName = "MemOrcTest";
memorc::MemOrc& morc = memorc::MemOrc::getInstance();

//basic zero copy 
TEST(suiteName,ZeroCopyNoCrush) {
    char src[] = " Hello ";
    char dest[] = " World ";

    //expected Value
    char expected[] = " World ";
    morc.submit_copy(dest,src,0);
    morc.wait_for_completion();
    EXPECT_EQ(memcmp(dest,expected,sizeof(dest)),0);
}

//Overlapping memory
TEST(suiteName,OverlapMemory) {
    char buffer[] = "abcdef";
    char expected[] = "ababcd";
    morc.submit_copy(buffer+2,buffer,4);
    morc.wait_for_completion();
    //memcpy(buffer+2,buffer,4);
    std::cout<<" bufefr "<<buffer<<std::endl;
    EXPECT_EQ(memcmp(expected,buffer,sizeof(buffer)),0);
}

//Various size test :
TEST(suiteName, VariousSizes) {
    size_t it=0;    
    for (size_t size : {1, 8, 64, 1024, 1024 * 1024}) {
        std::vector<char> src(size, 'A');
        std::vector<char> dest(size);
        morc.submit_copy(dest.data(), src.data(), size);
        morc.wait_for_completion();
        //memcpy(dest.data(), src.data(), size);
        size_t c = 0;
        EXPECT_EQ(std::memcmp(src.data(), dest.data(), size), 0);
    }
}

//