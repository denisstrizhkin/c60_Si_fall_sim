#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <random>
#include <chrono>

const double kBlockSize = 5.43;
const size_t kNumOfAtomsIndex = 2;
const size_t k1stAtomIndex = 16;
const size_t kType = 1;
const size_t kX = 2;
const size_t kY = 3;
const size_t kZ = 4;

// read lines of a lammps input file
std::string* ReadLines(char* input_file_path, size_t num_of_lines);
// write new input file
void WriteLinesArray(std::string* lines_array,
  size_t num_of_lines, char* output_file_path);
// count lines in input file
int CountLines(char* input_file_path);
// split lines
std::vector<std::string> SplitString(const std::string& line);
// get number of atoms in input file
size_t GetNumOfAtoms(std::string* lines_array);
// randomize carbon coordinates
void MoveAtoms(std::string* lines_array, size_t num_of_atoms);
// actual random functions
double RandomizeCoordinate()
{  
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine random_engine(seed);

  std::uniform_real_distribution<double> uniform_double_distrib(0.5, 1);
  double d_delta = uniform_double_distrib(random_engine) * kBlockSize;

  double signs[2] = {1, -1};
  std::uniform_int_distribution<int> uniform_int_distrib(0, 1);
  d_delta *= signs[uniform_int_distrib(random_engine)];

  return d_delta;
}

int main(int argc, char** argv)
{
  // get number of steps in a dump file
  size_t num_of_lines = CountLines(argv[1]);

  // get step lines array
  std::string* lines_array = ReadLines(argv[1], num_of_lines);

  size_t num_of_atoms = GetNumOfAtoms(lines_array);
  
  MoveAtoms(lines_array, num_of_atoms);

  // write result into a file
  WriteLinesArray(lines_array, num_of_lines, argv[2]);
}

void MoveAtoms(std::string* lines_array, size_t num_of_atoms)
{
  int temp = 0;

  double delta_x = RandomizeCoordinate();
  double delta_y = RandomizeCoordinate();
  double delta_z = RandomizeCoordinate();

  for (size_t i = k1stAtomIndex; i < num_of_atoms + k1stAtomIndex; i++)
  {
    std::vector<std::string> line_vals = SplitString(lines_array[i]);

    if (line_vals[kType] == "2")
    {
      std::cout << "in coord change function " << temp++;

      std::cout << " " << line_vals[kX] << " " << line_vals[kY] << " " << line_vals[kZ];

      line_vals[kX] = std::to_string(std::stod(line_vals[kX]) + delta_x);
      line_vals[kY] = std::to_string(std::stod(line_vals[kY]) + delta_y);
      line_vals[kZ] = std::to_string(std::stod(line_vals[kZ]) + delta_z);
   
      std::cout << " " << line_vals[kX] << " " << line_vals[kY] << " " << line_vals[kZ] << '\n';

      std::string new_line = line_vals[0];

      for (size_t j = 1; j < line_vals.size(); j++)
        new_line += ' ' + line_vals[j];

      lines_array[i] = new_line;
    }
  }
}

unsigned long GetNumOfAtoms(std::string* lines_array)
{
  std::vector<std::string> line3_vals = SplitString(lines_array[kNumOfAtomsIndex]);

  return std::stoul(line3_vals[0]);
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

void WriteLinesArray(std::string* lines_array,
  size_t num_of_lines, char* output_file_path)
{
  std::ofstream output_file;
  
  output_file.open(output_file_path);
  for (int i = 0; i < num_of_lines; i++)
  {
    output_file << lines_array[i] << '\n';
  }
  output_file.close();
}

int CountLines(char* input_file_path)
{
  std::string line;
  std::ifstream input_file;

  int num_of_lines = 0;

  input_file.open(input_file_path);
  while(std::getline(input_file, line))
  {
    num_of_lines++;
  }
  input_file.close();

  return num_of_lines;
}

std::string* ReadLines(char* input_file_path, size_t num_of_lines)
{
  // file variables
  std::string line;
  std::ifstream input_file;

  // loop variables
  int current_line = 0;

  // lines array
  std::string* lines_array = new std::string[num_of_lines];

  // loop for reading the input file
  input_file.open(input_file_path);
  while(std::getline(input_file, line))
  {
    lines_array[current_line++] = line; 
  }
  input_file.close();

  return lines_array;
}
