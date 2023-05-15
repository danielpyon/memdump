#include <iostream>

#include "memlib.h"

int main() {
    auto pids = memlib::get_all_pids();
    for(const auto& x : *pids) {
        std::cout << x << " ";
        // std::cout << memlib::Process(x);
    }
    std::cout << std::endl;
    delete pids;
    return 0;

    memlib::Process p(60331);
    std::cout << p << std::endl;

    memlib::MemoryTool mt(p.GetPID());
    char* result;
    mt.ReadMem((char*)0x7fffffffd92b, 4, &result);
    result[3] = '\x00';
    std::cout << result << std::endl;

/*
    auto vmmap = p.GetVMMap();
    for (const auto& e : *vmmap) {
        std::cout << e.name << std::endl;
    }
*/

    delete[] result;
}
