#include "memlib.h"

static constexpr int kProcMapNameIdx = 5;
static constexpr int kProcMapPermsIdx = 1;
static constexpr int kProcMapAddrIdx = 0;
static std::vector<std::string> split(std::string const &input);

// get the permission bits from a string
static uint8_t perms(const std::string& rwx);

// get start, end address from string
static std::pair<char*, char*> start_end_address(const std::string& address);

static bool is_number(const std::string& str);

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

std::vector<memlib::VMMapEntry>* memlib::Process::GetVMMap() const {
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
                curr.name = toks[kProcMapNameIdx];
            }

            if (toks.size() >= kProcMapPermsIdx + 1) {
                curr.perms = perms(toks[kProcMapPermsIdx]);

                auto pr(start_end_address(toks[kProcMapAddrIdx]));
                curr.start = pr.first;
                curr.end = pr.second;
            }

            vmmap->push_back(curr);
        }
        proc.close();
    }

    return vmmap;
}

std::ostream& memlib::operator<<(std::ostream& out,
                                 const memlib::Process& proc) {
    out << "Process " << proc.GetName() << std::endl;

    std::unique_ptr<std::vector<memlib::VMMapEntry>> vmmap(proc.GetVMMap());

    for (const auto& entry : *vmmap) {
        out << entry;
        out << std::endl;
    }

    return out;
}

std::vector<pid_t>* memlib::get_all_pids() {
    DIR *d = opendir("/proc/");
    struct dirent *dir;
    auto pids = new std::vector<pid_t>;

    struct stat st;

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            char* name = dir->d_name;
            std::string curr_path("/proc/");
            curr_path += name;

            if (stat(curr_path.c_str(), &st) == -1) {
                perror("stat");
                continue;
            }

            if (st.st_uid == getuid() && is_number(name)) {
                pid_t pid = static_cast<pid_t>(atoi(name));
                pids->push_back(pid);
            }
        }
        closedir(d);
    }

    return pids;
}

static std::vector<std::string> split(std::string const &input) { 
    std::istringstream buffer(input);
    std::vector<std::string> ret((std::istream_iterator<std::string>(buffer)), 
                                  std::istream_iterator<std::string>());
    return ret;
}

static uint8_t perms(const std::string& rwx) {
    uint8_t ret = 0;
    if (rwx.at(0) == 'r')
        ret |= 0b100;
    if (rwx.at(1) == 'w')
        ret |= 0b10;
    if (rwx.at(2) == 'x')
        ret |= 0b1;
    return ret;
}

static std::pair<char*, char*> start_end_address(const std::string& address) {
    std::string delim("-");
    auto delim_idx = address.find(delim);
    std::string start(address.substr(0, delim_idx));
    std::string end(address.substr(delim_idx + 1));

    uint64_t start_address, end_address;
    start_address = std::stoul(start, nullptr, 16);
    end_address = std::stoul(end, nullptr, 16);

    return std::make_pair(reinterpret_cast<char*>(start_address),
                          reinterpret_cast<char*>(end_address));
}

static bool is_number(const std::string& str) {
    if (str.empty()) {
        return false;
    }

    for (auto it(str.begin()); it != str.end(); it++) {
        if (*it < '0' || *it > '9') {
            return false;
        }
    }

    return true;
}
