#pragma once
#include <ctime>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <list>
#include <chrono>

#include <QCoreApplication>
#include <QtConcurrent>
#include <QStringList>
#include <QTextCodec>
#include <QString>

namespace head {
	struct Names_average {
		std::map<std::string, int> average_salary_humans;
		std::list<int> list_average_salary_humans;

		Names_average() = default;

		Names_average(std::map<std::string, int> average_salary_humans,
					  std::list<int> list_average_salary_humans) {
			this->average_salary_humans = average_salary_humans;
			this->list_average_salary_humans = list_average_salary_humans;
		}
	};

	struct Value_average {
		std::vector<std::string> names;
		std::vector<std::vector<int>> vector;
		size_t first;
		size_t last;

		Value_average() = default;

		Value_average(std::vector<std::string> names,
					  std::vector<std::vector<int>> vector,
					  size_t first,
					  size_t last) {
			this->names = names;
			this->vector = vector;
			this->first = first;
			this->last = last;
		}
	};

	// Запись имен нужных файлов
	void writeNames(std::vector<std::string> &text_names,
					std::vector<std::string> &names,
					std::string              fileInputName,
					std::ifstream            &list) {

		while (!list.eof())
		{
			std::string list_names;
			list >> list_names;
			std::string one_name = list_names;
			one_name.resize(fileInputName.size());
			if (one_name == fileInputName) {
				text_names.push_back(list_names);
				names.push_back(list_names.substr(0, list_names.length() - 4));
			}
		}
	}

	// Открытие каждого текстового файла для нахождения и запись каждой числа в вектор значений
	// В дальнейшем вектора заходят в очередь векторов
	std::vector<std::vector<int>> opendAndRead(std::vector<std::string> &text_names) {

		std::vector<std::vector<int>> vector;

		for (auto& _names : text_names) {
			std::ifstream fin(_names, std::ios::in);
			std::vector<int> vec;
			for (int i = 0; i < 12; i++) {
				int num;
				fin >> num;
				vec.push_back(num);
			}
			vector.push_back(vec);
			fin.close();
		}

		return vector;
	}

	// Высчитвание средней зп и запись в map для вывода в .csv файл
	Names_average mapAverage(const Value_average &vectorAverage) {

		std::map<std::string, int> average_salary_humans;
		std::list<int> list_average_salary_humans;

		for (auto i = vectorAverage.first; i < vectorAverage.last; ++i) {
			int average_salary = 0;
			std::vector<int> vec = vectorAverage.vector[i];
			for (int j = 0; j < 12; j++) {
				int num = vec.at(j);
				average_salary += num;
			}

			average_salary = average_salary / 12;
			average_salary_humans[vectorAverage.names.at(i)] = average_salary;
			list_average_salary_humans.push_back(average_salary);
		}

		Names_average namesAverage = Names_average(average_salary_humans, list_average_salary_humans);
		return namesAverage;
	}

	void reduceAverage(Names_average &n1, const Names_average &n2) {
		// Объединение map
		std::map<std::string, int> val = n1.average_salary_humans;
		val.insert(n2.average_salary_humans.begin(), n2.average_salary_humans.end());
		n1.average_salary_humans = val;

		// Объединение list
		n1.list_average_salary_humans.insert(n1.list_average_salary_humans.end(), n2.list_average_salary_humans.begin(), n2.list_average_salary_humans.end());
	}

	// 5% на max и min зп
	void percent(std::list<int> list_average_salary_humans,
				 std::map<std::string, int> &average_salary_humans,
				 std::list<std::string> &human_low_salary,
				 std::list<std::string> &human_high_salary) {

		size_t size = list_average_salary_humans.size();
		size = size_t(size * 0.05);

		std::list<int>         low_salary;
		std::list<int>         high_salary;

		list_average_salary_humans.sort();
		low_salary = list_average_salary_humans;
		low_salary.resize(size);

		list_average_salary_humans.reverse();
		high_salary = list_average_salary_humans;
		high_salary.resize(size);

		for (auto & average_salary_human : average_salary_humans) {
			for (int n : low_salary) {
				if (average_salary_human.second == n)
					human_low_salary.push_back(average_salary_human.first);
			}

			for (int n : high_salary) {
				if (average_salary_human.second == n)
					human_high_salary.push_back(average_salary_human.first);
			}
		}
	}

	// зп ниже среднего
	void below_the_set_value(std::string param,
							 std::list<int> &list_average_salary_humans,
							 int &count_for_param) {

		int int_param = std::stoi(param);

		for (int n : list_average_salary_humans) {
			if (n < int_param)
				count_for_param++;
		}
	}
}