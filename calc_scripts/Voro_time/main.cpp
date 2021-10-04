#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>

// *** FUNCTIONS ***

// read lines of a lammps dump file
std::string* ReadSteps(char* input_file_path, int num_of_steps);
// write result into a file
void WriteAverageArray(std::string* steps_array, int num_of_steps,
  char* output_file_path);
// count steps in a lammps dump file
int CountSteps(char* input_file_path);
std::vector<std::string> SplitString(const std::string& line);

// ***

int main(int argc, char** argv)
{
  // get number of steps in a dump file
  int num_of_steps = CountSteps(argv[1]);

  // get step lines array
  std::string* steps_array = ReadSteps(argv[1], num_of_steps);
 
  // write result into a file
  WriteAverageArray(steps_array, num_of_steps, argv[2]);
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

void WriteAverageArray(std::string* steps_array, int num_of_steps,
  char* output_file_path)
{
  std::ofstream output_file;
  
  output_file.open(output_file_path);

  output_file << "timestep vacs ints\n";
  for (size_t i = 0; i < num_of_steps; i++)
  {
    std::vector<std::string> line_strs = SplitString(steps_array[i]);

    output_file << std::setw(10) << std::left << line_strs[0];
    for (size_t i = 1; i < line_strs.size(); i++)
      output_file << std::setw(10) << std::left << line_strs[i];
    output_file << '\n';
  }

  output_file.close();
}

int CountSteps(char* input_file_path)
{
  std::string line;
  std::ifstream input_file;

  int num_of_steps = 0;

  input_file.open(input_file_path);
  while(std::getline(input_file, line))
  {
    if (line == "ITEM: TIMESTEP")
      num_of_steps++;
  }
  input_file.close();

  return num_of_steps;
}

std::string* ReadSteps(char* input_file_path, int num_of_steps)
{
  // file variables
  std::string line;
  std::ifstream input_file;

  // loop variables
  int current_step = -1;
  bool b_analyze_line = true;
  bool b_read_timestep = false;

  // steps array
  std::string* steps_array = new std::string[num_of_steps];

  // loop for reading the input file
  input_file.open(input_file_path);
  while(std::getline(input_file, line))
  {
    if (line == "ITEM: TIMESTEP")
    {
      b_analyze_line = false;
      b_read_timestep = true;
      current_step++;
    }
    else if (b_read_timestep == true)
    {
      steps_array[current_step] = line;
      b_read_timestep = false;
    }
    else if (line.substr(0, 11) == "ITEM: ATOMS")
    {
      b_analyze_line = true;
    }
    else if (b_analyze_line == true)
    {
      steps_array[current_step] += " " + line;
    }
  }
  input_file.close();

  return steps_array;
}
