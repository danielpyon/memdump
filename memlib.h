#ifndef MEMLIB_H
#define MEMLIB_H

#include <sys/uio.h>
#include <stdint.h>
#include <memory>
#include <list>
#include <iostream>
#include <fstream>
#include <string>

namespace memlib {

class MemoryTool {
  public:
    MemoryTool(pid_t pid) : _pid(pid) { };
    ~MemoryTool() { };
    MemoryTool(const MemoryTool& other) = delete;
    MemoryTool& operator=(const MemoryTool& rhs) = delete;

    bool ReadMem(char* start, const uint32_t len, char** result);
    bool WriteMem(char* start, const uint32_t len, const char* data);

  private:
    pid_t _pid;
};

struct VMMapEntry {
    char* start, end;
    uint8_t perms;
    std::string name;
};

class Process {
  public:
    Process(pid_t pid);
    std::string GetName() { return _name; };
    std::list<memlib::VMMapEntry>* GetVMMap();

  private:
    pid_t _pid;
    std::string _name;
};

}

#endif // MEMLIB_H
