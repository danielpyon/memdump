#include <iostream>

#include "memlib.h"

int main() {
    memlib::MemoryTool mt(24381);
    char* result;
    mt.ReadMem((char*)0x7fffffffd92b, 4, &result);

    std::cout << result << std::endl;
    memlib::Process p(24381);

    delete[] result;
}
