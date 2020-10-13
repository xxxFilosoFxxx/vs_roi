#include <iostream>
#include <chrono>
#include <thread>
#include "header.h"

int main(int argc_p, char ** argv_p) {
	using namespace head;

	std::cout << "\n ---------------------------------\n";
	const std::size_t argumetsAmount_cl = argc_p;
	for (std::size_t i = 0; i < argumetsAmount_cl; ++i)
		std::cout << "\t" << argv_p[i] << "\n";
	std::cout << " ---------------------------------\n";

	if (std::string(argv_p[1]) != "--input" || std::string(argv_p[3]) != "--output" || std::string(argv_p[5]) != "--param") {
		std::cerr << "Invalid command line arguments" << std::endl;
		return -1;
	}

	// Для общей части имени входных файлов
	if (std::string(argv_p[2]).empty() || std::string(argv_p[4]).empty() || std::string(argv_p[6]).empty()) {
		std::cerr << "Invalid command line arguments" << std::endl;
		return -1;
	}

	// Название .csv файла
	std::string fileCfgName = argv_p[4];
	std::string name_csv = fileCfgName.substr(fileCfgName.length() - 4, 4);

	if (name_csv != ".csv") {
		std::cerr << "Invalid file *.csv" << std::endl;
		return -1;
	}

	// Запись всех текстовых файлов
	std::system("dir /b /o:d *.txt > list.txt");
	std::vector<std::string>	 text_names;
	std::vector<std::string>	 names;
	std::list<int>				 list_average_salary_humans;
	std::map<std::string, int>   average_salary_humans;
	std::queue<std::vector<int>> queue;

	std::string fileInputName = argv_p[2];
	std::string param = argv_p[6];

	std::ifstream list("list.txt", std::ios::in);
	if (!list.is_open()) {
		std::cerr << "*.txt files not found" << std::endl;
		return -1;
	}

	writeNames(text_names, names, fileInputName, list);
	queue = opendAndRead(text_names);

	std::vector<std::thread> Threads;
	unsigned int Current_Hardware = std::thread::hardware_concurrency();
	for (unsigned int i = 0; i < Current_Hardware; i++) {
		Threads.push_back(std::thread(average, std::ref(names), std::ref(average_salary_humans), std::ref(list_average_salary_humans), std::ref(queue)));
	}

	auto start = std::chrono::steady_clock::now();

	for (auto& threads : Threads) {
		threads.join();
	}
	Names_percent namesPercent = percent(list_average_salary_humans, average_salary_humans);
	int salary_limit = below_the_set_value(param, list_average_salary_humans);

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double, std::milli> elapsed_seconds = end - start;

	// Запись времени работы программы в ms
	std::ofstream fout_time("time.txt", std::ios::out);
	fout_time << "Количество файлов: " << names.size() << std::endl << "Время работы программы: " << elapsed_seconds.count() << "ms";
	fout_time.close();

	// Работа с csv файлом
	std::string fileOutputName = argv_p[4];
	std::ofstream fout(fileOutputName, std::ios::out);

	fout << "Гражданин, Среднемесячная зарплата" << "\n";
	for (auto & average_salary_human : average_salary_humans) {
		fout << average_salary_human.first << ", " << average_salary_human.second << "\n";
	}

	fout << "\n";
	fout << "5% граждан с max зп, 5% граждан с min зп" << "\n";

	auto it1 = namesPercent.human_high_salary.begin();
	auto it2 = namesPercent.human_low_salary.begin();

	while (it1 != namesPercent.human_high_salary.end() && it2 != namesPercent.human_low_salary.end()) {
		fout << *it1 << ", " << *it2 << "\n";
		++it1;
		++it2;
	}
	fout << "\n";

	fout << "Количество граждан с зп ниже среднего" << "\n";
	fout << salary_limit << "\n";

	fout.close();

	return 0;
}
