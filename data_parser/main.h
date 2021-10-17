#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <map>

class Dump
{
public:
  Dump() {};
  Dump(const std::string& dump_file_path);

  Dump GetSpecificStepsVals(const short type,
    const std::vector<std::string>& keys) const;
  Dump GetAverageOfDump() const;

  double& AtomValAt(const std::string& key,
      const size_t step, const size_t atom);
  double& AtomValAt(const size_t step,
      const size_t atom, const size_t val);
  double AtomValAt(const std::string& key,
      const size_t step, const size_t atom) const;
  double AtomValAt(const size_t step,
      const size_t atom, const size_t val) const;
  void WriteTo(const std::string& output_file_path) const;
  
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
void WriteOutput(const std::string& output_file_path,
    std::vector<std::string> output_vec);
// C z coord distrib
void CalcCDistrib(const std::string& dump_file_path,
    const std::string& output_file_path);
