#pragma once
#include <ctime>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <thread>
#include <chrono>

namespace head {
	// Запись имен нужных файлов
	void writeNames(std::vector<std::string> &text_names,
					std::string              fileInputName,
					std::ifstream            &list
	) {

		while (!list.eof())
		{
			std::string list_names;
			list >> list_names;
			std::string one_name = list_names;
			one_name.resize(fileInputName.size());
			if (one_name == fileInputName) {
				text_names.push_back(list_names);
			}
		}
	}

	// Открытие каждого текстового файла для нахождения и запись каждого числа в вектор значений
	// В дальнейшем в вектор заходят векторы векторов значений
	std::vector<std::vector<std::vector<std::vector<int>>>> opendAndRead(std::vector<std::string> &text_names) {

		std::vector<std::vector<std::vector<std::vector<int>>>> my_vector;

		for (auto& _names : text_names) {
			std::vector<std::vector<std::vector<int>>> vec_vector;

			std::ifstream fin(_names, std::ios::in);
			int num;
			while (fin >> num) {
				std::vector<std::vector<int>> _vec;
				std::vector<int> vec;
				vec.push_back(num);
				for (int i = 0; i < 11; i++) {
					fin >> num;
					vec.push_back(num);
				}
				_vec.push_back(vec);
				vec_vector.push_back(_vec);
			}

			my_vector.push_back(vec_vector);
			fin.close();
		}

		return my_vector;
	}

	// Генерация граждан
	std::vector<std::string> names_persones(std::vector<std::vector<std::vector<int>>> vec_vector) {

		std::vector<std::string> names;

		for (int i = 0; i < int(vec_vector.size()); i++) {
			std::string str = "Гражданин " + std::to_string(i + 1);
			names.push_back(str);
		}

		return names;
	}

	// Высчитвание средней зп и запись в map для вывода в .csv файл
	void average(size_t						     first,
				 size_t						     last,
				 std::vector<std::string>	     &names,
				 std::multimap<int, std::string> &average_salary_humans,
				 std::vector<std::vector<std::vector<int>>> vec_vector) {

		for (auto i = first; i < last; i++) {
			int average_salary = 0;

			std::vector<int> vec = vec_vector[i].front();
			for (int j = 0; j < 12; j++) {
				int num = vec[j];
				average_salary += num;
			}

			average_salary = average_salary / 12;
			average_salary_humans.insert(std::pair<int, std::string>(average_salary, names[i]));
		}
	}

	// зп ниже среднего
	int below_the_set_value(std::string param,
							std::multimap<int, std::string> &average_salary_humans) {
		int count_for_param = 0;
		int int_param = std::stoi(param);

		for (auto n : average_salary_humans) {
			if (n.first < int_param)
				count_for_param++;
		}

		return count_for_param;
	}
}
