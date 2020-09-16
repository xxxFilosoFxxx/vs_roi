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

	if (strcmp(argv_p[1], "--config") != 0 || strcmp(argv_p[3], "--outfile") != 0) {
		std::cerr << "Invalid command line arguments" << std::endl;
		system("pause");
		return -1;
	}

	if (strcmp(argv_p[2], "") == 0) {
		std::cerr << "Invalid command line arguments" << std::endl;
		system("pause");
		return -1;
	}
	else {
		std::string fileCfgName = argv_p[2];
		std::string name_cfg = fileCfgName.substr(fileCfgName.length() - 4, 4);

		if (name_cfg != ".cfg") {
			std::cerr << "Invalid file *.cfg" << std::endl;
			system("pause");
			return -1;
		}
	}

	if (strcmp(argv_p[4], "") == 0) {
		std::cerr << "Invalid command line arguments" << std::endl;
		system("pause");
		return -1;
	}

	srand(time(NULL));
	const int max = 200000;
	const int min = 30000;
	int random_values[12];
	std::string fileCfgName = argv_p[2];
	std::string fileOutName = argv_p[4];

	std::vector<std::string> names;
	std::string s;


	std::ifstream file(fileCfgName, std::ios::in);
	if (!file.is_open()) {
		std::cerr << "Invalid file *.cfg" << std::endl;
		system("pause");
		return -1;
	}
	else
	{
		while (getline(file, s))
		{
			names.push_back(fileOutName + s + ".txt");
		}
		file.close();
	}

	/*for (auto it = names.begin(); it != names.end(); it++) {
		std::cout << *it << std::endl;
	}*/

	for (auto it = names.begin(); it != names.end(); it++) {
		std::ofstream fout(*it, std::ios::out);
		for (int i = 0; i < 12; i++) {
			random_values[i] = min + (double)rand() / (RAND_MAX + 1)*(max - min);
			fout << random_values[i] << std::endl;
		}
		fout.close();
	}

	return 0;
}