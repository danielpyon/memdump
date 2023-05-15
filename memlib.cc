#include "memlib.h"

bool memlib::MemoryTool::ReadMem(char* const start, const uint32_t len,
                                 std::unique_ptr<char*>* result) {
    struct iovec local, remote;
    
    char* buf = new char[len];

    local.iov_base = buf;
    local.iov_len = len;
    remote.iov_base = static_cast<void*>(start);
    remote.iov_len = len;

    ssize_t status = process_vm_readv(_pid, &local, 1, &remote, 1, 0);

    if (status == -1 || status != len) {
        return false;
    }

    *result = std::make_unique<char*>(buf);
    return true;
}

bool memlib::MemoryTool::WriteMem(char* start, const uint32_t len,
                                  const char* data) {
    return false;
}
