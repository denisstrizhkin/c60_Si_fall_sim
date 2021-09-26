#include <fstream>
#include <iostream>
#include <iomanip>

const double kBlockSize = 5.43;

// read lines of a lammps input file
std::string* ReadLines(char* input_file_path, int num_of_lines);
// write new input file
void WriteLinesArray(std::string* lines_array,
  int num_of_lines, char* output_file_path);
// count lines in input file
int CountLines(char* input_file_path);

int main(int argc, char** argv)
{
  // get number of steps in a dump file
  int num_of_lines = CountLines(argv[1]);

  // get step lines array
  std::string* lines_array = ReadLines(argv[1], num_of_lines);
 
  // write result into a file
  WriteLinesArray(lines_array, num_of_lines, argv[2]);
}

void WriteLinesArray(std::string* lines_array,
  int num_of_lines, char* output_file_path)
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

std::string* ReadLines(char* input_file_path, int num_of_lines)
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
