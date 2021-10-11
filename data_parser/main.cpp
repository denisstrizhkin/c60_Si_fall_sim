#include <vector>
#include <sstream>
#include <iostream>
#include <cstring>
#include <fstream>

std::vector<std::string> SplitString(const std::string& line);
void WriteOutput(const std::string& output_file_path,
    std::vector<double> output_vec);
void WriteOutput(const std::string& output_file_path,
    std::vector<std::string> output_vec);

// C z coord distrib
void CalcCDistrib(const std::string& dump_file_path,
    const std::string& output_file_path);
std::vector<std::vector<double>> GetStepsCoordsVec(
    const std::string& input_file_path);
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

void WriteOutput(const std::string& output_file_path,
    std::vector<double> output_vec)
{
  std::ofstream output_file;
  output_file.open(output_file_path);
  for (double line : output_vec) output_file << line << '\n';
  output_file.close();
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

void CalcCDistrib(const std::string& dump_file_path,
    const std::string& output_file_path)
{
  std::vector<std::vector<double>> steps_coords_vec =
      GetStepsCoordsVec(dump_file_path);
  std::vector<double> average_vec = GetAverageVec(steps_coords_vec);
  WriteOutput(output_file_path, average_vec);
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

std::vector<std::vector<double>> GetStepsCoordsVec(
  const std::string& input_file_path)
{
  std::string line;
  std::ifstream input_file;
  std::vector<std::vector<double>> steps_coords_vec;
  // loop variables
  int current_step = -1;
  bool b_analyze_line = true;
  // loop for reading the input file
  input_file.open(input_file_path);
  while(std::getline(input_file, line))
  {
    if (line == "ITEM: TIMESTEP")
    {
      b_analyze_line = false;
      steps_coords_vec.push_back(std::vector<double>());
      current_step++;
    }
    else if (line.substr(0, 11) == "ITEM: ATOMS")
      b_analyze_line = true;
    else if (b_analyze_line == true)
      steps_coords_vec[current_step].push_back(std::stod(line));
  }
  input_file.close();

  return steps_coords_vec;
}