#include <fstream>
#include <vector>
#include <iostream>

// constants
const int kNumOfSteps = 11;

// functions
std::vector<float>* ReadStepsCoordinates(char* input_file_path);
int GetNumOfCarbon(std::vector<float>* steps_coordinates_array);
float* GetAverageArray(std::vector<float>* steps_coordinates_array,
  int num_of_carbon);
void WriteAverageArray(float* average_array, int num_of_carbon,
  char* output_file_path);

int main(int argc, char** argv)
{
  std::vector<float>* steps_coordinates_array
    = ReadStepsCoordinates(argv[1]);

  int num_of_carbon = GetNumOfCarbon(steps_coordinates_array);

  float* average_array = GetAverageArray(steps_coordinates_array,
    num_of_carbon);

  WriteAverageArray(average_array, num_of_carbon, argv[2]);
}

void WriteAverageArray(float* average_array, int num_of_carbon,
  char* output_file_path)
{
  std::ofstream output_file;
  
  output_file.open(output_file_path);
  for (int i = 0; i < num_of_carbon; i++)
    output_file << average_array[i] << '\n';

  output_file.close();
}

float* GetAverageArray(std::vector<float>* steps_coordinates_array,
  int num_of_carbon)
{
  float* average_array = new float[num_of_carbon];

  for (int i = 0; i < num_of_carbon; i++)
    average_array[i] = 0.0f;

  for (int i = 0; i < num_of_carbon; i++)
  {
    for (int j = 0; j < kNumOfSteps; j++)
      average_array[i] += steps_coordinates_array[j][i];

    average_array[i] /= kNumOfSteps;
  }

  return average_array;
}

int GetNumOfCarbon(std::vector<float>* steps_coordinates_array)
{
  int num_of_carbon = steps_coordinates_array[0].size();
 
  for (int i = 1; i < kNumOfSteps; i++)
  {
    num_of_carbon = std::min(num_of_carbon,
      static_cast<int>(steps_coordinates_array[i].size()));
  }

  return num_of_carbon;
}

std::vector<float>* ReadStepsCoordinates(char* input_file_path)
{
  // file variables
  std::string line;
  std::ifstream input_file;

  // loop variables
  int current_step = -1;
  bool b_analyze_line = true;

  // values array
  std::vector<float>* steps_coordinates_array
    = new std::vector<float>[kNumOfSteps];

  // loop for reading the input file
  input_file.open(input_file_path);
  while(std::getline(input_file, line))
  {
    if (line == "ITEM: TIMESTEP")
    {
      b_analyze_line = false;
      current_step++;
    }
    else if (line.substr(0, 11) == "ITEM: ATOMS")
    {
      b_analyze_line = true;
    }
    else if (b_analyze_line == true)
    {
      steps_coordinates_array[current_step]
        .push_back(std::stod(line));
    }
  }
  input_file.close();

  return steps_coordinates_array;
}
