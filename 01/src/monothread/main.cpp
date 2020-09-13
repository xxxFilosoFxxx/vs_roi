#include <vector> 
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <list>
#pragma warning (disable: 4996)

// ������� �������������� ������ � �����
int StrToInt(char *s)
{
	int temp = 0; // �����
	int i = 0;
	int sign = 0; // ���� ����� 0- �������������, 1 � �������������
	if (s[i] == '-')
	{
		sign = 1;
		i++;
	}
	while (s[i] >= 0x30 && s[i] <= 0x39)
	{
		temp = temp + (s[i] & 0x0F);
		temp = temp * 10;
		i++;
	}

	if (strlen(s) != i) {
		std::cerr << "Negative parameter" << std::endl;
		system("pause");
		return -1;
	}

	temp = temp / 10;
	if (sign == 1) {
		std::cerr << "Negative parameter" << std::endl;
		system("pause");
		return -1;
	}
	return(temp);
}

int main(int argc_p, char ** argv_p) {
	std::cout << "\n ---------------------------------\n";
	const std::size_t argumetsAmount_cl = argc_p;
	for (std::size_t i = 0; i < argumetsAmount_cl; ++i)
		std::cout << "\t" << argv_p[i] << "\n";
	std::cout << " ---------------------------------\n";

	if (strcmp(argv_p[1], "--input") != 0 || strcmp(argv_p[3], "--output") != 0 || strcmp(argv_p[5], "--param") != 0) {
		std::cerr << "Invalid command line arguments" << std::endl;
		system("pause");
		return -1;
	}

	// ��� ����� ����� ����� ������� ������
	if (strcmp(argv_p[2], "") == 0) {
		std::cerr << "Invalid command line arguments" << std::endl;
		system("pause");
		return -1;
	}

	// ��� �������� ��������� ����� �� ��� *.csv
	if (strcmp(argv_p[4], "") == 0) {
		std::cerr << "Invalid command line arguments" << std::endl;
		system("pause");
		return -1;
	}
	else {
		std::string fileCfgName = argv_p[4];
		std::string name_csv = fileCfgName.substr(fileCfgName.length() - 4, 4);

		if (name_csv != ".csv") {
			std::cerr << "Invalid file *.csv" << std::endl;
			system("pause");
			return -1;
		}
	}

	// ���������� ���� ������� + ..\..\test
	std::string path = getenv("WORK_DIR");
	path += "\\";

	// ������ ���� ��������� ������ 
	std::system("dir /b /o:d %WORK_DIR%\\*.txt > %WORK_DIR%\\list.txt");
	std::vector<std::string>	text_names;
	std::vector<std::string>	names;
	std::list<int>				list_average_salary_humans;
	std::list<int>				low_salary;
	std::list<int>				high_salary;
	std::list<std::string>		human_low_salary;
	std::list<std::string>		human_high_salary;
	std::string					list_names;

	std::map<std::string, int>  average_salary_humans;

	std::string fileInputName = argv_p[2];

	// ������ ���� ������ ������ � map
	std::ifstream list(path + "list.txt", std::ios::in);
	if (!list.is_open()) {
		std::cerr << "*.txt files not found" << std::endl;
		system("pause");
		return -1;
	}
	else {
		while (!list.eof())
		{
			list >> list_names;
			std::string one_name = list_names;
			one_name.resize(fileInputName.size());
			if (one_name == fileInputName) {
				text_names.push_back(list_names);
				names.push_back(list_names.substr(0, list_names.length() - 4));
			}
		}
	}

	auto it1 = text_names.begin();
	auto it2 = names.begin();

	// �������� ������� ���������� ����� ��� ���������� �������� �������� � ����������� ��� [�������(�������� �����)] -> ��. ��
	// � ������ �������� �������� � ������ ��� ������ � �������
	while (it1 != text_names.end() && it2 != names.end()) {
		std::ifstream fin(path + *it1, std::ios::in);
		int average_salary = 0;
		for (int i = 0; i < 12; i++) {
			int num;
			fin >> num;
			average_salary += num;
		}
		average_salary = average_salary / 12;
		// std::cout << "A citizen '" << *it2 << "' has an average salary -> " << average_salary << std::endl;
		average_salary_humans[*it2] = average_salary;
		list_average_salary_humans.push_back(average_salary);
		fin.close();
		++it1;
		++it2;
	}

	int size = list_average_salary_humans.size();
	size = size * 0.05;
	// std::cout << i_map << std::endl;

	char* param = argv_p[6];
	int count_for_param = 0;
	int int_param = StrToInt(param);

	// �� ���� ��������
	for (int n : list_average_salary_humans) {
		if (n < int_param)
			count_for_param++;
	}

	list_average_salary_humans.sort();
	low_salary = list_average_salary_humans;
	low_salary.resize(size);

	list_average_salary_humans.reverse();
	high_salary = list_average_salary_humans;
	high_salary.resize(size);

	// 5% �� max � min ��
	for (auto it = average_salary_humans.begin(); it != average_salary_humans.end(); it++) {
		for (int n : low_salary) {
			if (it->second == n)
				human_low_salary.push_back(it->first);
		}

		for (int n : high_salary) {
			if (it->second == n)
				human_high_salary.push_back(it->first);
		}
	}

	// ������ � csv ������
	std::string fileOutputName = argv_p[4];
	std::ofstream fout(path + fileOutputName, std::ios::out);

	fout << "���������, �������������� ��������" << "\n";
	for (auto it = average_salary_humans.begin(); it != average_salary_humans.end(); it++) {
		fout << it->first << ", " << it->second << "\n";
	}

	fout << "\n";
	fout << "5% ������� � max ��, 5% ������� � min ��" << "\n";

	auto it3 = human_high_salary.begin();
	auto it4 = human_low_salary.begin();

	while (it3 != human_high_salary.end() && it4 != human_low_salary.end()) {
		fout << *it3 << ", " << *it4 << "\n";
		++it3;
		++it4;
	}
	fout << "\n";

	fout << "���������� ������� � �� ���� ��������" << "\n";
	fout << count_for_param << "\n";

	fout.close();

	return 0;
}