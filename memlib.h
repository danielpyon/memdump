#ifndef MEMLIB_H
#define MEMLIB_H

#include <sys/uio.h>
#include <dirent.h>
#include <stdint.h>
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <istream>
#include <iterator>

#define READABLE(x) ((x >> 2) & 1)
#define WRITABLE(x) ((x >> 1) & 1)
#define EXECUTABLE(x) ((x) & 1)

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
    char* start;
    char* end;
    uint8_t perms;
    std::string name;
    friend std::ostream& operator<<(std::ostream &out,
                                    const VMMapEntry& entry) {
        if (!entry.name.empty()) {
            out << entry.name << ": ";
        } else {
            out << "(Unknown): ";
        }

        out << std::hex << static_cast<void*>(entry.start) << "-";
        out << static_cast<void*>(entry.end) << " ";

        if (READABLE(entry.perms))
            out << "r";
        if (WRITABLE(entry.perms))
            out << "w";
        if (EXECUTABLE(entry.perms))
            out << "x";
        return out;
    }
};

class Process {
  public:
    Process(pid_t pid);
    std::string GetName() const { return _name; };
    std::vector<VMMapEntry>* GetVMMap() const;
    pid_t GetPID() const { return _pid; };

  private:
    pid_t _pid;
    std::string _name;
};

std::ostream& operator<<(std::ostream& out, const memlib::Process& proc);

std::vector<pid_t>* get_all_pids();

}

#endif // MEMLIB_H
