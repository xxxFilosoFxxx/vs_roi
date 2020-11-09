#include <iostream>
#include <chrono>
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
	std::vector<std::string>		text_names;
	std::vector<std::string>		names;
	std::multimap<int, std::string> average_salary_humans;

	std::string fileInputName = argv_p[2];
	std::string param = argv_p[6];

	std::ifstream list("list.txt", std::ios::in);
	if (!list.is_open()) {
		std::cerr << "*.txt files not found" << std::endl;
		return -1;
	}

	writeNames(text_names, fileInputName, list);

	std::vector<std::vector<std::queue<std::vector<int>>>> my_vector = opendAndRead(text_names);

	std::chrono::duration<double, std::milli> elapsed_seconds;
	int salary_limit = 0;

	// Открытие файла для записи времени работы
	std::string fileOutputName = argv_p[4];
	std::ofstream fout_time(fileOutputName, std::ios::out);
	fout_time << "Количество значений, " << "время работы программы в `ms`" << std::endl;

	for (int i = 0; i < int(my_vector.size()); i++) {
		names = names_persones(my_vector[i]);

		auto start = std::chrono::steady_clock::now();

		average(names, average_salary_humans, my_vector[i]);
		salary_limit = below_the_set_value(param, average_salary_humans);

		auto end = std::chrono::steady_clock::now();
		elapsed_seconds = end - start;

		// Запись времени работы программы в ms
		fout_time <<  names.size() << ", " <<  elapsed_seconds.count() << std::endl;
	}

	fout_time.close();

	// Работа с csv файлом
	/*std::string fileOutputName = argv_p[4];
	std::ofstream fout(fileOutputName, std::ios::out);

	fout << "Гражданин, Среднемесячная зарплата" << "\n";
	for (auto & average_salary_human : average_salary_humans) {
		fout << average_salary_human.second << ", " << average_salary_human.first << "\n";
	}

	fout << "\n";
	fout << "5% граждан с max зп, 5% граждан с min зп" << "\n";

	auto it1 = average_salary_humans.rbegin();
	auto it2 = average_salary_humans.begin();

	size_t size = average_salary_humans.size();
	size = size_t(size * 0.05);
	size_t count = 0;

	while (count < size) {
		fout << it1->second << ", " << it2->second << "\n";
		++it1;
		++it2;
		count++;
	}
	fout << "\n";

	fout << "Количество граждан с зп ниже среднего" << "\n";
	fout << salary_limit << "\n";

	fout.close();*/

	return 0;
}
