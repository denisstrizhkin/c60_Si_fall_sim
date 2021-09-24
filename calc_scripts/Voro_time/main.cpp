#include <fstream>
#include <iostream>
#include <iomanip>

// *** FUNCTIONS ***

// read lines of a lammps dump file
std::string* ReadSteps(char* input_file_path, int num_of_steps);
// write result into a file
void WriteAverageArray(std::string* steps_array, int num_of_steps,
  char* output_file_path);
// count steps in a lammps dump file
int CountSteps(char* input_file_path);

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

void WriteAverageArray(std::string* steps_array, int num_of_steps,
  char* output_file_path)
{
  std::ofstream output_file;
  
  output_file.open(output_file_path);

  output_file << "timestep vacs ints\n";
  for (int i = 0; i < num_of_steps; i++)
  {
    int space_pos = steps_array[i].find(' ');

    std::string str1 = steps_array[i].substr(0, space_pos);
    std::string str2 = steps_array[i].substr(space_pos + 1);

    space_pos = str2.find(' ');

    output_file << std::setw(10) << std::left << str1
      << std::setw(10) << std::left << str2.substr(0, space_pos)
      <<str2.substr(space_pos + 1) << '\n';
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
