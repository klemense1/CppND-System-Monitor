#include "../include/linux_parser.h"
#include "../include/process.h"
#include "gtest/gtest.h"

class ProcessTest : public ::testing::Test {
 public:
  ProcessTest() {
    pids_ = LinuxParser::Pids();
    pid_selected_ = &(*((pids_.begin())));
    std::cout << "pid " << *pid_selected_ << std::endl;
  }

 protected:
  std::vector<int> pids_;
  int *pid_selected_;
};

TEST_F(ProcessTest, TestConstructor) {
  int pid = *pid_selected_;
  Process process = Process(pid);
  EXPECT_EQ(process.Pid(), pid);
}

TEST_F(ProcessTest, TestCpuUtilization) {
  int pid = *pid_selected_;
  Process process = Process(pid);
  auto cpu = process.CpuUtilization();
  std::cout << "cpu " << cpu << std::endl;
  EXPECT_GT(cpu, 0.0);
}

TEST_F(ProcessTest, TestCommand) {
  int pid = *pid_selected_;
  Process process = Process(pid);
  auto cmd = process.Command();
  std::cout << "cmd " << cmd << std::endl;
  EXPECT_GT(cmd.length(), 0) << cmd;
}

TEST_F(ProcessTest, TestRam) {
  int pid = *pid_selected_;
  Process process = Process(pid);
  auto ram = process.Ram();
  std::cout << "ram " << ram << std::endl;
  EXPECT_GT(std::stoi(ram), 0);
}

TEST_F(ProcessTest, TestUser) {
  int pid = *pid_selected_;
  Process process = Process(pid);
  auto user = process.User();
  std::cout << "user " << user << std::endl;
  EXPECT_GT(user.length(), 0);
}

TEST_F(ProcessTest, TestUpTime) {
  int pid = *pid_selected_;
  Process process = Process(pid);
  auto uptime = process.UpTime();
  std::cout << "uptime " << uptime << std::endl;
  EXPECT_GT(uptime, 0);
}