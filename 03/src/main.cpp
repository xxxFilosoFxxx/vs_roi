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
	std::list<std::string>		  human_low_salary;
	std::list<std::string>		  human_high_salary;
	std::vector<std::vector<int>> vector;
	int below_average = 0;

	std::string fileInputName = argv_p[2];
	std::string param = argv_p[6];

	std::ifstream list("list.txt", std::ios::in);
	if (!list.is_open()) {
		std::cerr << "*.txt files not found" << std::endl;
		return -1;
	}

	writeNames(text_names, names, fileInputName, list);
	vector = opendAndRead(text_names);

	// Максимальное кол-во оптимально открытых потоков и разделение размерности вектора
	const auto Current_Hardware = std::thread::hardware_concurrency();
	size_t vecSize = 0;
	if (vector.size() % Current_Hardware == 0) {
		vecSize = vector.size() / Current_Hardware;
	}
	else {
		vecSize = (vector.size() / Current_Hardware) + 1;
	}

	size_t first = 0;
	size_t last = vecSize;

	//using Tasks = std::vector<Value_average>;
	std::vector<Value_average> tasks;

	for (unsigned int i = 0; i < Current_Hardware; ++i) {
		tasks.push_back(Value_average(names, vector, first, last));
		first += vecSize;
		last = std::min(last + vecSize, vector.size());
	}

	auto start1 = std::chrono::steady_clock::now();

	Names_average namesAverage = QtConcurrent::blockingMappedReduced(tasks, mapAverage, reduceAverage);

	auto end1 = std::chrono::steady_clock::now();
	std::chrono::duration<double, std::milli> elapsed_seconds = end1 - start1;

	std::thread t1 = std::thread(percent, namesAverage.list_average_salary_humans, std::ref(namesAverage.average_salary_humans), std::ref(human_low_salary), std::ref(human_high_salary));
	std::thread t2 = std::thread(below_the_set_value, param, std::ref(namesAverage.list_average_salary_humans), std::ref(below_average));

	auto start2 = std::chrono::steady_clock::now();

	t1.join();
	t2.join();

	auto end2 = std::chrono::steady_clock::now();
	elapsed_seconds += end2 - start2;

	// Запись времени работы программы в ms
	std::ofstream fout_time("time.txt", std::ios::out);
	fout_time << "Количество файлов: " << names.size() << std::endl << "Время работы программы: " << elapsed_seconds.count() << "ms";
	fout_time.close();

	// Работа с csv файлом
	std::string fileOutputName = argv_p[4];
	std::ofstream fout(fileOutputName, std::ios::out);

	fout << "Гражданин, Среднемесячная зарплата" << "\n";
	for (auto & average_salary_human : namesAverage.average_salary_humans) {
		fout << average_salary_human.first << ", " << average_salary_human.second << "\n";
	}

	fout << "\n";
	fout << "5% граждан с max зп, 5% граждан с min зп" << "\n";

	auto it1 = human_high_salary.begin();
	auto it2 = human_low_salary.begin();

	while (it1 != human_high_salary.end() && it2 != human_low_salary.end()) {
		fout << *it1 << ", " << *it2 << "\n";
		++it1;
		++it2;
	}
	fout << "\n";

	fout << "Количество граждан с зп ниже среднего" << "\n";
	fout << below_average << "\n";

	fout.close();

	return 0;
}