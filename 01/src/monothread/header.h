#pragma once

#include <ctime>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <queue>

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
	// В дальнейшем в вектор заходят очереди векторов значений
	std::vector<std::vector<std::queue<std::vector<int>>>> opendAndRead(std::vector<std::string> &text_names) {

		std::vector<std::vector<std::queue<std::vector<int>>>> my_vector;

		for (auto& _names : text_names) {
			std::vector<std::queue<std::vector<int>>> vec_queue;

			std::ifstream fin(_names, std::ios::in);
			int num;
			while (fin >> num) {
				std::queue<std::vector<int>> queue;
				std::vector<int> vec;
				vec.push_back(num);
				for (int i = 0; i < 11; i++) {
					fin >> num;
					vec.push_back(num);
				}
				queue.push(vec);
				vec_queue.push_back(queue);
			}

			my_vector.push_back(vec_queue);
			fin.close();
		}

		return my_vector;
	}

	// Генерация граждан
	std::vector<std::string> names_persones(std::vector<std::queue<std::vector<int>>> vec_queue) {

		std::vector<std::string> names;

		for (int i = 0; i < int(vec_queue.size()); i++) {
			std::string str = "Гражданин " + std::to_string(i + 1);
			names.push_back(str);
		}

		return names;
	}

	// Высчитвание средней зп и запись в map для вывода в .csv файл
	void average(std::vector<std::string>        &names,
				 std::multimap<int, std::string> &average_salary_humans,
				 std::vector<std::queue<std::vector<int>>> &vec_queue) {

		for (int i = 0; i < int(vec_queue.size()); i++) {
			int average_salary = 0;

			std::vector<int> vec = vec_queue[i].front();
			for (int i = 0; i < 12; i++) {
				int num = vec.at(i);
				average_salary += num;
			}
			vec_queue[i].pop();
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
