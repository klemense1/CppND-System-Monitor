#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::string filename = kProcDirectory + kMeminfoFilename;
  int mem_total = ParseValueForKey(filename, "MemTotal:");
  int mem_free = ParseValueForKey(filename, "MemFree:");
  // int mem_available = ParseValueForKey(filename, "MemAvailable:");
  // int buffers = ParseValueForKey(filename, "Buffers:");
  return static_cast<float>(mem_total - mem_free) / mem_total;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string uptime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    // std::cout << line << std::endl;
    std::istringstream linestream(line);
    linestream >> uptime;
    return std::stoi(uptime);
  }
  return 0;
}

// OBSOLETE: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { return 0; }

// OBSOLETE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// OBSOLETE: Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() { return 0; }

// OBSOLETE: Read and return the number of idle jiffies for the system
// long LinuxParser::IdleJiffies() { return 0; }

// OBSOLETE: Read and return CPU utilization
// vector<string> LinuxParser::CpuUtilization() {
// return {};
// }

LinuxParser::CpuRawData LinuxParser::ParseCpuRawData(
    const std::string& cpu_name) {
  string line;
  string key;
  string user, value, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  LinuxParser::CpuRawData cpu_data;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == cpu_name) {
          linestream >> user >> nice >> system >> idle >> iowait >> irq >>
              softirq >> steal >> guest >> guest_nice;
          cpu_data.user = std::stoi(user);
          cpu_data.nice = std::stoi(nice);
          cpu_data.system = std::stoi(system);
          cpu_data.idle = std::stoi(idle);
          cpu_data.iowait = std::stoi(iowait);
          cpu_data.irq = std::stoi(irq);
          cpu_data.softirq = std::stoi(softirq);
          cpu_data.steal = std::stoi(steal);
          cpu_data.guest = std::stoi(guest);
          cpu_data.guest_nice = std::stoi(guest_nice);
        }
      }
    }
  }
  return cpu_data;
}

LinuxParser::ProcessStatusData LinuxParser::ParseProcessStatusData(int pid) {
  string line;
  string key;
  string pid_in, comm, state, ppid, pgrp, session, tty_nr, tpgid, flags, minflt,
      cminflt, majflt, cmajflt, utime, stime, cutime, cstime, priority, nice,
      num_threads, itrealvalue, starttime;
  LinuxParser::ProcessStatusData process_data;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> pid_in >> comm >> state >> ppid >> pgrp >> session >>
        tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >>
        utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads >>
        itrealvalue >> starttime;
    process_data.utime = std::stoi(utime);
    process_data.stime = std::stoi(stime);
    process_data.cutime = std::stoi(cutime);
    process_data.cstime = std::stoi(cstime);
    process_data.starttime = std::stoi(starttime);
  }
  return process_data;
}

int LinuxParser::ParseValueForKey(const std::string& filename,
                                  const std::string& skey) {
  string line;
  string key;
  string value;
  std::ifstream filestream(filename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == skey) {
          std::replace(value.begin(), value.end(), '_', ' ');
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return ParseValueForKey(kProcDirectory + kStatFilename, "processes");
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return ParseValueForKey(kProcDirectory + kStatFilename, "procs_running");
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::string filename =
      kProcDirectory + std::to_string(pid) + kCmdlineFilename;

  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  return line;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::string filename = kProcDirectory + std::to_string(pid) + kStatusFilename;
  int ram = LinuxParser::ParseValueForKey(filename, "VmSize:");
  ram /= 1e3;  // convert from kB to MB
  // std::cout << "LinuxParser filename " << filename << std::endl;
  // std::cout << "LinuxParser Ram " << ram << std::endl;
  return std::to_string(ram);
}

// DONE: Read and return the user ID associated with a process
int LinuxParser::Uid(int pid) {
  std::string filename = kProcDirectory + std::to_string(pid) + kStatusFilename;
  int uid = LinuxParser::ParseValueForKey(filename, "Uid:");
  // std::cout << "LinuxParser filename " << filename << std::endl;
  // std::cout << "LinuxParser uid " << uid << std::endl;
  return uid;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int uid) {
  string line;
  string key;
  string dummy;
  string value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      // while (linestream >> key >> dummy >> value) {
      while (linestream >> value >> dummy >> key) {
        if (key == std::to_string(uid)) {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// OBSOLETE: Read and return the uptime of a process
// long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
