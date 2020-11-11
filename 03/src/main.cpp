#include <iostream>
#include "header.h"

int main(int argc_p, char **argv_p) {
	using namespace head;

	QCoreApplication app_l(argc_p, argv_p);
	static const auto * const codel_scl = QTextCodec::codecForName("IBM 866");
	std::cout << "\n ---------------------------------\n";
	QStringList argumentsList_l;
	const std::size_t argumetsAmount_cl = argc_p;
	for (std::size_t i = 0; i < argumetsAmount_cl; ++i)
		argumentsList_l.push_back(codel_scl->toUnicode(argv_p[i]));
	for (auto oneArg : argumentsList_l)
		std::cout << "\t" << codel_scl->fromUnicode(oneArg).constData() << "\n";
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
	std::vector<std::string>	  text_names;
	std::vector<std::string>	  names;
	std::vector<int>			  vec_average;

	std::string fileInputName = argv_p[2];
	std::string param = argv_p[6];

	std::ifstream list("list.txt", std::ios::in);
	if (!list.is_open()) {
		std::cerr << "*.txt files not found" << std::endl;
		return -1;
	}

	writeNames(text_names, fileInputName, list);

	std::vector<std::vector<std::queue<std::vector<int>>>> my_vector = opendAndRead(text_names);

	int salary_limit = 0;

	// Открытие файла для записи времени работы
	std::string fileOutputName = argv_p[4];
	std::ofstream fout_time(fileOutputName, std::ios::out);
	fout_time << "Количество значений, " << "время работы программы в `ms`" << std::endl;
	for (int i = 0; i < int(my_vector.size()); i++) {
		// Максимальное кол-во оптимально открытых потоков и разделение размерности вектора
		const auto Current_Hardware = std::thread::hardware_concurrency();
		size_t vecSize = 0;
		if (my_vector[i].size() % Current_Hardware == 0) {
			vecSize = my_vector[i].size() / Current_Hardware;
		}
		else {
			vecSize = (my_vector[i].size() / Current_Hardware) + 1;
		}

		size_t first = 0;
		size_t last = vecSize;

		std::vector<Value_average> tasks;

		names = names_persones(my_vector[i]);

		auto start1 = std::chrono::steady_clock::now();

		average(my_vector[i], vec_average);

		auto end1 = std::chrono::steady_clock::now();
		std::chrono::duration<double, std::milli> elapsed_seconds = end1 - start1;

		for (unsigned int j = 0; j < Current_Hardware; ++j) {
			tasks.push_back(Value_average(names, vec_average, std::stoi(param), first, last));
			first += vecSize;
			last = std::min(last + vecSize, my_vector[i].size());
		}

		auto start2 = std::chrono::steady_clock::now();

		salary_limit = QtConcurrent::blockingMappedReduced(tasks, mapAverage, reduceAverage);

		auto end2 = std::chrono::steady_clock::now();
		elapsed_seconds += end2 - start2;

		// Предположительная запись и освобождение глобальной переменной
		average_salary_humans.clear();

		// Запись времени работы программы в ms
		fout_time << names.size() << ", " << elapsed_seconds.count() << std::endl;
	}

	fout_time.close();

	return 0;
}