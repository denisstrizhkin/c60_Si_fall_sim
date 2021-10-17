#include <vector>
#include <sstream>
#include <iostream>
#include <cstring>
#include <fstream>
#include <map>

class Dump
{
public:
  Dump();
  Dump(const std::string& dump_file_path);

  Dump GetSpecificStepsVals(const unsigned type,
    const std::vector<std::string>& keys) const;
  Dump GetAverageOfDump() const;

  double AtomValAt(const std::string& key,
      const size_t step, const size_t atom) const;
  double& AtomValAt(const size_t step,
      const size_t atom, const size_t val) const;
  
private:
  struct Step
  {
    unsigned long time;
    std::vector<std::vector<double>> atoms;
  };

  std::map<std::string, size_t> keys;
  std::vector<Step> steps;
};

std::vector<std::string> SplitString(const std::string& line);