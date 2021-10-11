#include "main.h"

void WriteOutput(const std::string& output_file_path,
    std::vector<std::string> output_vec);

// C z coord distrib
void CalcCDistrib(const std::string& dump_file_path,
    const std::string& output_file_path);
std::vector<double> GetAverageVec(
    std::vector<std::vector<double>> steps_coords_vec);

int main(int argc, char** argv)
{
  if (argc != 4)
  {
    std::cout << "three arguments expected\n";
    return 0;
  }

  if (std::strlen(argv[1]) == 1)
    switch (argv[1][0])
    {
      case 'r':
        break;
      case 'v':
        break;
      case 'c':
        CalcCDistrib(argv[2], argv[3]);
        break;
      default:
        std::cout << "there is no such 1st argument - " << argv[1][0] << '\n';
        return 0;
    }
  else
    std::cout << "1st argument must be 1 letter\n";

  return 0;
}

double Dump::AtomValAt(const std::string& key,
    const size_t step, const size_t atom)
{
  return steps[step].atoms[atom][keys[key]];
}

void WriteOutput(const std::string& output_file_path,
    std::vector<std::string> output_vec)
{
  std::ofstream output_file;
  output_file.open(output_file_path);
  for (std::string line : output_vec) output_file << line << '\n';
  output_file.close();
} 

std::vector<std::string> SplitString(const std::string& line)
{
  std::vector<std::string> line_strs;
  std::stringstream line_stream(line);
  std::string str;

  while (std::getline(line_stream, str, ' '))
    line_strs.push_back(str);
  
  return line_strs;
}

Dump ReadDump(const std::string& dump_file_path)
{
  std::string line;
  std::ifstream input_file;
  Dump dump;
  // loop variables
  size_t current_step = 0;
  size_t current_atom;
  bool b_analyze_line = true;
  bool b_keys_read = false;
  bool b_read_timestep = false;
  // loop for reading the input file
  input_file.open(dump_file_path);
  while(std::getline(input_file, line))
  {
    if (line == "ITEM: TIMESTEP")
    {
      b_analyze_line = false;
      b_read_timestep = true;

      dump.steps.push_back(Step());
      current_step++;
      size_t current_atom = 0;
    }
    else if (b_read_timestep == true)
    {
      dump.steps[current_step].time = std::stoul(line);
      b_read_timestep = false;
    }
    else if (line.substr(0, 11) == "ITEM: ATOMS")
    {
      b_analyze_line = true;
      std::vector<std::string> keys_vec = SplitString(line.substr(12));
      for (size_t i = 0; i < keys_vec.size(); i++)
        dump.keys[keys_vec[i]] = i;
    }
    else if (b_analyze_line == true)
    {
      std::vector<std::string> vals_vec = SplitString(line);
      
      dump.steps[current_step].atoms.push_back(std::vector<double>());
      for (std::string value : vals_vec)
        dump.steps[current_step].
            atoms[current_atom].push_back(std::stod(value));

      current_atom++;
    }
  }
  input_file.close();

  return dump;
}

Dump GetAverageOfDump(const Dump& dump)
{
  Dump new_dump;
  new_dump.keys = dump.keys;
  new_dump.steps.push_back(Step());
  
}

void CalcCDistrib(const std::string& dump_file_path,
    const std::string& output_file_path)
{
  Dump dump = ReadDump(dump_file_path);
  Dump c_z_dump = GetSpecificStepsVals(-1, {"z"}, dump);
  Dump average_dump = GetAverageOfDump(c_z_dump);
  WriteDump(average_dump, output_file_path);
}

Dump GetSpecificStepsVals(const unsigned type, 
    const std::vector<std::string>& keys, const Dump& dump)
{
  Dump new_dump;
  for (std::string key : keys) new_dump.keys.insert({key, 0});

  for (size_t i = 0; i < dump.steps.size(); i++)
  {
    new_dump.steps.push_back(Step());

    for (size_t j = 0; j < dump.steps[i].atoms.size(); i++)
    {
      size_t current_atom = 0;

      if (dump.AtomValAt("type", i, j) == type || type == -1)
      {
        new_dump.steps[i].atoms.push_back(std::vector<double>());

        size_t current_val = 0;
        for (std::string key : keys)
        {
          new_dump.keys[key] = current_val++;
          new_dump.steps[i].atoms[current_atom].push_back(
              dump.AtomValAt(key, i, j));
        }

        current_atom++;
      }
    }
  }

  return new_dump;
}

std::vector<double> GetAverageVec(
    std::vector<std::vector<double>> steps_coords_vec)
{
  std::vector<double> average_vec;

  for (const double value : steps_coords_vec[0])
    average_vec.push_back(value);

  for (size_t i = 0; i < average_vec.size(); i++)
  {
    for (size_t j = 1; j < steps_coords_vec.size(); j++)
      average_vec[i] += steps_coords_vec[j][i];

    average_vec[i] /= steps_coords_vec.size();
  }

  return average_vec;
}