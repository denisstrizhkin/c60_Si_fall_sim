#include <vector>
#include <sstream>
#include <iostream>
#include <cstring>
#include <fstream>
#include <map>

struct Dump
{
  std::map<std::string, size_t> keys;
  std::vector<Step> steps;

  double AtomValAt(const std::string& key,
      const size_t step, const size_t atom);
};

struct Step
{
  unsigned long time;
  std::vector<std::vector<double>> atoms;
};

Dump ReadDump(const std::string& dump_file_path);
Dump GetSpecificStepsVals(const unsigned type,
    const std::vector<std::string>& keys, const Dump& dump);
Dump GetAverageOfDump(const Dump& dump);

std::vector<std::string> SplitString(const std::string& line);