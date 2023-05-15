#include "memlib.h"

static constexpr int kProcMapNameIdx = 5;
static constexpr int kProcMapPIDIdx = 1;
static constexpr int kProcMapAddrIdx = 0;
static std::vector<std::string> split(std::string const &input);

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

std::vector<memlib::VMMapEntry>* memlib::Process::GetVMMap() {
    std::string filename("/proc/");
    filename += std::to_string(_pid);
    filename += "/maps";

    std::ifstream proc(filename);
    std::string line;
    
    auto vmmap = new std::vector<memlib::VMMapEntry>();

    if (proc.is_open()) {
        while (getline(proc, line)) {
            auto toks(split(line));
            memlib::VMMapEntry curr;

            if (toks.size() >= kProcMapNameIdx + 1) {
                curr.name = std::string(toks[kProcMapNameIdx]);
            }

            if (toks.size() >= kProcMapAddrIdx + 1) {
                // toks[kProcMapAddrIdx];
            }

            vmmap->push_back(curr);
        }
        proc.close();
    }

    return vmmap;
}

static std::vector<std::string> split(std::string const &input) { 
    std::istringstream buffer(input);
    std::vector<std::string> ret((std::istream_iterator<std::string>(buffer)), 
                                  std::istream_iterator<std::string>());
    return ret;
}
