#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>
using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  long s = seconds;

  int h = seconds / (60 * 60);
  s -= h * (60 * 60);

  int m = s / (60);
  s -= m * (60);

  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << h << ':' << std::setw(2) << m
     << ':' << std::setw(2) << s;
  return ss.str();
}