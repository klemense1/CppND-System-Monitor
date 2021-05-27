#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const {
  LinuxParser::ProcessStatusData data =
      LinuxParser::ParseProcessStatusData(pid_);

  // total time spent for the process in seconds
  long total_time = (data.utime + data.stime + data.cutime + data.cstime) /
                    sysconf(_SC_CLK_TCK);

  // total elapsed time in seconds since the process started:
  long seconds = UpTime();
  // std::cout << "total time " << total_time << " seconds " << seconds
  //           << data.DebugString() << std::endl;

  // CPU usage percentage:
  float cpu_usage = (static_cast<float>(total_time) / seconds);
  return cpu_usage;
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// DONE: Return this process's memory utilization in MB
string Process::Ram() { return LinuxParser::Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() {
  // Find the UID associated with the process
  int uid = LinuxParser::Uid(pid_);
  // Find the user corresponding to that UID
  return LinuxParser::User(uid);
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() const {
  LinuxParser::ProcessStatusData data =
      LinuxParser::ParseProcessStatusData(pid_);
  long int start_time_s = data.starttime / sysconf(_SC_CLK_TCK);
  long seconds = LinuxParser::UpTime() - start_time_s;
  return seconds;
}

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  if (this->CpuUtilization() < a.CpuUtilization()) {
    return true;
  } else {
    return false;
  }
}