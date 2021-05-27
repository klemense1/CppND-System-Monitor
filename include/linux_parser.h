#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

struct CpuRawData {
  long NonIdle() const { return user + nice + system + irq + softirq + steal; }
  long Idle() const { return idle + iowait; }
  long user;        // normal processes executing in user mode
  long nice;        // niced processes executing in user mode
  long system;      // processes executing in kernel mode
  long idle;        // twiddling thumbs
  long iowait;      // In a word, iowait stands for waiting for I/O to complete.
  long irq;         // servicing interrupts
  long softirq;     // servicing softirqs
  long steal;       // involuntary wait
  long guest;       // running a normal guest
  long guest_nice;  // running a niced guest
};

struct ProcessStatusData {
  long utime;   // CPU time spent in user code, measured in clock ticks
  long stime;   // CPU time spent in kernel code, measured in clock ticks
  long cutime;  // Waited-for children's CPU time spent in user code (in clock
                // ticks)
  long cstime;  // Waited-for children's CPU time spent in kernel code (in clock
                // ticks)
  long starttime;  // Time when the process started, measured in clock ticks
  std::string DebugString() {
    std::string s = "utime: " + std::to_string(utime) +
                    ", stime: " + std::to_string(stime) +
                    ", cutime: " + std::to_string(cutime) +
                    ", cstime: " + std::to_string(cstime) +
                    ", starttime: " + std::to_string(starttime);
    return s;
  };
};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int ParseValueForKey(const std::string& filename, const std::string& skey);
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
CpuRawData ParseCpuRawData(const std::string& cpu_name);
ProcessStatusData ParseProcessStatusData(int pid);
// std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
int Uid(int pid);
std::string User(int uid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif