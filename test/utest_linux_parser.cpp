#include "gtest/gtest.h"
#include "../include/linux_parser.h"

class LinuxParserTest : public ::testing::Test {
  protected:
    
};

TEST_F(LinuxParserTest, TestMemoryUtilization) {
    EXPECT_TRUE(LinuxParser::MemoryUtilization() > 0.0);
}

TEST_F(LinuxParserTest, TestUpTime) {
    EXPECT_TRUE(LinuxParser::UpTime() > 0);
}

// TEST_F(LinuxParserTest, TestJiffies) {
//     EXPECT_TRUE(LinuxParser::Jiffies() > 0);
// }

// TEST_F(LinuxParserTest, TestActiveJiffies) {
//     EXPECT_TRUE(LinuxParser::ActiveJiffies() > 0);
// }

// TEST_F(LinuxParserTest, TestActiveJiffiesPid) {
//     EXPECT_TRUE(LinuxParser::ActiveJiffies() > 0);
// }

// TEST_F(LinuxParserTest, TestIdleJiffies) {
//     EXPECT_TRUE(LinuxParser::IdleJiffies() > 0);
// }

TEST_F(LinuxParserTest, TestParseCpuRawData) {
    const LinuxParser::CpuRawData data = LinuxParser::ParseCpuRawData("cpu");
    EXPECT_TRUE(data.Idle() > 0);
}

TEST_F(LinuxParserTest, TestParseProcessStatusData) {
    auto pids = LinuxParser::Pids();
    int pid = *pids.end();
    LinuxParser::ProcessStatusData data = LinuxParser::ParseProcessStatusData(pid);
    EXPECT_TRUE(data.utime > 0);
    EXPECT_TRUE(data.stime > 0);
    EXPECT_TRUE(data.cutime > 0);
    EXPECT_TRUE(data.cstime > 0);
    EXPECT_TRUE(data.starttime > 0);
    std::string s = data.DebugString();
    std::cout << "pid: " << pid << " ... " << s << std::endl;
}

TEST_F(LinuxParserTest, TestTotalProcesses) {
    EXPECT_GT(LinuxParser::TotalProcesses(), 0);
}

TEST_F(LinuxParserTest, TestRunningProcesses) {
    EXPECT_TRUE(LinuxParser::RunningProcesses() > 0);
}
