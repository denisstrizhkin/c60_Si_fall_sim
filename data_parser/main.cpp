#include "main.h"

const double kBlockSize = 5.43;
const size_t kX = 2;
const size_t kY = 3;

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
        RandomizeInput(argv[2], argv[3]);
        break;
      case 'v':
        WriteVoro(argv[2], argv[3]);
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

double& Dump::AtomValAt(const std::string& key,
    const size_t step, const size_t atom)
{
  return this->steps.at(step).atoms.at(atom).at(this->keys.at(key));
}

double& Dump::AtomValAt(const size_t step,
    const size_t atom, const size_t val)
{
  return this->steps.at(step).atoms.at(atom).at(val);
}

double Dump::AtomValAt(const std::string& key,
    const size_t step, const size_t atom) const
{
  return this->steps.at(step).atoms.at(atom).at(this->keys.at(key));
}

double Dump::AtomValAt(const size_t step,
    const size_t atom, const size_t val) const
{
  return this->steps.at(step).atoms.at(atom).at(val);
}

void Dump::WriteTo(const std::string& output_file_path) const
{
  const unsigned kWidth = 15;
  std::ofstream output_file;
  output_file.open(output_file_path);
  // write header
  output_file << std::setw(kWidth) << std::left << "step";
  for (auto key : this->keys)
    output_file << std::setw(kWidth) << std::left << key.first;
  output_file << '\n';  
  // write steps
  for (Step step : this->steps)
  {
    for (size_t i = 0; i < step.atoms.size(); i++)
    {
      if (i == 0) output_file << std::setw(kWidth) << std::left << step.time;
      else output_file << std::setw(kWidth) << std::left << ' ';
      // write atom vals
      for (double value : step.atoms.at(i))
        output_file << std::setw(kWidth) << std::left << value;

      output_file << '\n';
    }
  }
  output_file.close();
}

std::vector<std::string> ReadLines(const std::string& input_file_path)
{
  std::vector<std::string> lines_vec;
  std::ifstream input_file;
  std::string line;
  // read input file lines cycle
  input_file.open(input_file_path);
  while(std::getline(input_file, line)) lines_vec.push_back(line);
  input_file.close();

  return lines_vec;
}

void WriteLines(const std::string& output_file_path,
    std::vector<std::string> output_vec)
{
  std::ofstream output_file;
  output_file.open(output_file_path);
  for (std::string line : output_vec) output_file << line << '\n';
  output_file.close();
} 

double RandomizeCoordinate()
{  
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine random_engine(seed);
  std::uniform_real_distribution<double> uniform_double_distrib(-0.5, 0.5);
  
  return uniform_double_distrib(random_engine) * kBlockSize;
}

void MoveAtoms(std::vector<std::string>& lines_vec, const size_t type)
{
  int temp = 0;

  double delta_x = RandomizeCoordinate();
  double delta_y = RandomizeCoordinate();
 
  for (size_t i = 16; i < lines_vec.size(); i++)
  {
    if (lines_vec[i].size() == 0) break;
    std::vector<std::string> line_vals = SplitString(lines_vec[i]);

    if (line_vals[type] == "2")
    {
      std::cout << "in coord change function " << temp++;
      std::cout << " " << line_vals[kX] << " " << line_vals[kY];

      line_vals[kX] = std::to_string(std::stod(line_vals[kX]) + delta_x);
      line_vals[kY] = std::to_string(std::stod(line_vals[kY]) + delta_y); 
   
      std::cout << " " << line_vals[kX] << " " << line_vals[kY] << '\n';

      std::string new_line = line_vals[0];

      for (size_t j = 1; j < line_vals.size(); j++)
        new_line += ' ' + line_vals[j];

      lines_vec[i] = new_line;
    }
  }
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

Dump::Dump(const std::string& dump_file_path)
{
  std::string line;
  std::ifstream input_file;
   // loop variables
  size_t current_step = -1;
  size_t current_atom = 0;
  bool b_analyze_line = true;
  bool b_keys_read = true;
  bool b_read_timestep = false;
  // loop for reading the input file
  input_file.open(dump_file_path);
  while(std::getline(input_file, line))
  {
    if (line == "ITEM: TIMESTEP")
    {
      b_analyze_line = false;
      b_read_timestep = true;

      this->steps.push_back(Step());
      current_step++;
      current_atom = 0;
    }
    else if (b_read_timestep == true)
    {
      this->steps.at(current_step).time = std::stoul(line);
      b_read_timestep = false;
    }
    else if (line.substr(0, 11) == "ITEM: ATOMS")
    {
      b_analyze_line = true;

      if (b_keys_read == true)
      {
        std::vector<std::string> keys_vec = SplitString(line.substr(12));
        for (size_t i = 0; i < keys_vec.size(); i++)
          this->keys.insert({keys_vec.at(i), i}); 

        b_keys_read == false;
      }
    }
    else if (b_analyze_line == true)
    {
      std::vector<std::string> vals_vec = SplitString(line);
      
      this->steps.at(current_step).atoms.push_back(std::vector<double>());
      for (std::string value : vals_vec)
        this->steps.at(current_step).
            atoms.at(current_atom).push_back(std::stod(value));

      current_atom++;
    }
  }
  input_file.close();
}

Dump Dump::GetAverageOfDump() const
{
  Dump new_dump;
  new_dump.keys = this->keys;
  new_dump.steps.push_back(Step());
  new_dump.steps.at(0).time = this->steps.at(0).time;
  // populate step 0 atoms vector
  for (size_t i = 0; i < this->steps.at(0).atoms.size(); i++)
  {
    new_dump.steps.at(0).atoms.push_back(std::vector<double>());
    for (size_t j = 0; j < this->steps.at(0).atoms.at(0).size(); j++)
    {
      new_dump.steps.at(0).atoms.at(i).push_back(0);
    } 
  }
  // cycle through atoms
  for (size_t i = 0; i < new_dump.steps.at(0).atoms.size(); i++)
  {
    // cycle through steps
    for (size_t j = 0; j < this->steps.size(); j++)
    {
      for (size_t k = 0; k < this->steps.at(j).atoms.at(i).size(); k++)
        new_dump.AtomValAt(0, i, k) += this->AtomValAt(j, i, k);
    }

    for (size_t k = 0; k < new_dump.steps.at(0).atoms.at(i).size(); k++)
        new_dump.AtomValAt(0, i, k) /= this->steps.size();
  }

  return new_dump;
}

void CalcCDistrib(const std::string& dump_file_path,
    const std::string& output_file_path)
{
  Dump dump(dump_file_path);
  dump.WriteTo("./d0.vals");
  Dump c_z_dump = dump.GetSpecificStepsVals(-1, {"v_c_z_dist"});
  c_z_dump.WriteTo("./d1.vals");
  Dump average_dump = c_z_dump.GetAverageOfDump();
  average_dump.WriteTo(output_file_path);
}

void RandomizeInput(const std::string& input_file_path,
    const std::string& output_file_path)
{
  std::vector<std::string> lines_vector = ReadLines(input_file_path);
  MoveAtoms(lines_vector, 1);
  WriteLines(output_file_path, lines_vector);
}

void WriteVoro(const std::string& dump_file_path,
    const std::string& output_file_path)
{
  Dump dump(dump_file_path);
  dump.WriteTo(output_file_path);
}

Dump Dump::GetSpecificStepsVals(const short type, 
    const std::vector<std::string>& keys) const
{
  // make new dump and copy keys
  Dump new_dump;
  for (std::string key : keys) new_dump.keys.insert({key, 0});
  // cycle through steps
  for (size_t i = 0; i < this->steps.size(); i++)
  {
    new_dump.steps.push_back(Step());
    new_dump.steps.at(i).time = this->steps.at(i).time;
    size_t current_atom = 0;
    // cycle through atoms of i_step
    for (size_t j = 0; j < this->steps.at(i).atoms.size(); j++)
    {
      // check if atom type satisfies type argument
      short tmp_type = (type == -1 ? -1 : this->AtomValAt("type", i, j));
      if (tmp_type == type || type == -1)
      {
        new_dump.steps.at(i).atoms.push_back(std::vector<double>());

        size_t current_val = 0;
        for (std::string key : keys)
        {
          new_dump.keys.at(key) = current_val++;
          new_dump.steps.at(i).atoms.at(current_atom).push_back(
              this->AtomValAt(key, i, j));
        }

        current_atom++;
      }
    }
  }

  return new_dump;
}

