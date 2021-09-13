#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <array>

int main(int argc, char** argv)
{
	// variables for reading dump file
	std::string line;
	std::ifstream dumpFile;

	int curStep = -1;
	
	bool bAnalyzeLine = true;

	// vars for z c60 coordinates vector
	std::array<std::vector<float>, 11> lastC60pos;

	// reading the dump file line by line
	dumpFile.open(argv[1]);
	while(std::getline(dumpFile, line))
	{
		if (line == "ITEM: TIMESTEP")
		{
			bAnalyzeLine = false;
			curStep++;
		}
		else if (line.substr(0, 11) == "ITEM: ATOMS")
		{
			bAnalyzeLine = true;
		}
		else if (bAnalyzeLine)
		{
			lastC60pos[curStep].push_back(stod(line));
		}
	}

	int numOfC = lastC60pos[0].size();

	for (int i = 1; i < lastC60pos.size(); i++)
		numOfC = std::min(numOfC, (int) lastC60pos[i].size());

	std::array<float, 60> finalCzPos = {};

	for (int i = 0; i < numOfC; i++)
	{
		for (int j = 0; j < lastC60pos.size(); j++)
			finalCzPos[i] += lastC60pos[j][i];

		finalCzPos[i] /= lastC60pos.size();

		std::cout << finalCzPos[i] << '\n';
	}

	return 1;
}
