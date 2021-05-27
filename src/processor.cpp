#include "processor.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  const LinuxParser::CpuRawData curr_cpu_data =
      LinuxParser::ParseCpuRawData("cpu");
  long PrevIdle = prev_cpu_data_.Idle();
  long Idle = curr_cpu_data.Idle();
  long PrevTotal = prev_cpu_data_.Idle() + prev_cpu_data_.NonIdle();
  long Total = curr_cpu_data.Idle() + curr_cpu_data.NonIdle();

  long totald = Total - PrevTotal;
  long idled = Idle - PrevIdle;

  float CPU_Percentage = static_cast<float>(totald - idled) / totald;
  return CPU_Percentage;
}