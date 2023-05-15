#include "memlib.h"

bool memlib::MemoryTool::ReadMem(char* const start, const uint32_t len,
                                 char** result) {
    struct iovec local, remote;
    
    char* buf = new char[len];

    local.iov_base = buf;
    local.iov_len = len;
    remote.iov_base = start;
    remote.iov_len = len;

    ssize_t status = process_vm_readv(_pid, &local, 1, &remote, 1, 0);

    if (status == -1 || status != len) {
        return false;
    }

    *result = buf;
    return true;
}

bool memlib::MemoryTool::WriteMem(char* start, const uint32_t len,
                                  const char* data) {
    return false;
}

memlib::Process::Process(pid_t pid) {
    _pid = pid;

    // get process name
    std::string filename("/proc/");
    filename += std::to_string(pid);
    filename += "/comm";

    std::ifstream proc(filename);
    std::string line;

    if (proc.is_open()) {
        if (getline(proc, line)) {
            _name = line;
        }
        proc.close();
    }
}

std::list<memlib::VMMapEntry>* memlib::Process::GetVMMap() {
    return nullptr;
}

