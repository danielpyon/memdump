#ifndef MEMLIB_H
#define MEMLIB_H

#include <sys/uio.h>
#include <stdint.h>
#include <memory>

namespace memlib {

class MemoryTool {
  public:
    MemoryTool(pid_t pid) : _pid(pid) { };
    ~MemoryTool() { };
    MemoryTool(const MemoryTool& other) = delete;
    MemoryTool& operator=(const MemoryTool& rhs) = delete;

    bool ReadMem(char* start, const uint32_t len,
                 std::unique_ptr<char*>* result);
    bool WriteMem();

  private:
    pid_t _pid;
};

}

#endif // MEMLIB_H
