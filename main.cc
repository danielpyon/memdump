#include <iostream>

#include "memlib.h"

int main() {
    memlib::MemoryTool mt(24381);
    std::unique_ptr<char*> result;
    mt.ReadMem((char*)0x7fffffffd92b, 4, &result);
    (*result)[3] = '\x00';

    std::cout << *result << std::endl;
}

