#include <vector>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>

int main(int argc_p, char ** argv_p) {
	std::cout << "\n ---------------------------------\n";
	const std::size_t argumetsAmount_cl = argc_p;
	for (std::size_t i = 0; i < argumetsAmount_cl; ++i)
		std::cout << "\t" << argv_p[i] << "\n";
	std::cout << " ---------------------------------\n";

	if (std::string(argv_p[1]) != "--config" || std::string(argv_p[3]) != "--outfile") {
		std::cerr << "Invalid command line arguments" << std::endl;
		return -1;
	}

	if (std::string(argv_p[2]) == "") {
		std::cerr << "Invalid command line arguments" << std::endl;
		return -1;
	}
	else {
		std::string fileCfgName = argv_p[2];
		std::string name_cfg = fileCfgName.substr(fileCfgName.length() - 4, 4);

		if (name_cfg != ".cfg") {
			std::cerr << "Invalid file *.cfg" << std::endl;
			return -1;
		}
	}

	if ((std::string(argv_p[4]) == "") {
		std::cerr << "Invalid command line arguments" << std::endl;
		return -1;
	}

	srand((unsigned int)time(NULL));
	const int max = 200000;
	const int min = 30000;
	double random_values[12];
	std::string fileCfgName = argv_p[2];
	std::string fileOutName = argv_p[4];

	std::vector<std::string> names;
	int s;

	std::ifstream file(fileCfgName, std::ios::in);
	if (!file.is_open()) {
		std::cerr << "Invalid file *.cfg" << std::endl;
		return -1;
	}
	else
	{
		file >> s;
		for (int i = 1; i < s + 1; i++) {
			names.push_back(fileOutName + std::to_string(i) + ".txt");
		}

		file.close();
	}

	for (auto it : names) {
		std::ofstream fout(it, std::ios::out);
		for (int i = 0; i < 12; i++) {
			random_values[i] = min + (double)rand() / (RAND_MAX + 1)*(max - min);
			fout << int(random_values[i]) << std::endl;
		}
		fout.close();
	}

	return 0;
}