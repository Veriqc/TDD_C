#pragma once
#include <unordered_set>
#include <vector>
#include <array>
#include <bitset>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <regex>
#include"time.h"
#include <math.h>

using namespace std;

//定义一些矩阵
constexpr dd::Matrix2x2 Imat = { {{ 1, 0 }, { 0, 0 } }, {{ 0, 0 }, { 1, 0 } } };
constexpr dd::Matrix2x2 Xmat = { {{ 0, 0 }, { 1, 0 } }, {{ 1, 0 }, { 0, 0 } } };
constexpr dd::Matrix2x2 Ymat = { {{ 0, 0 }, { 0, 1 } }, {{ 0, -1 }, { 0, 0 } } };
constexpr dd::Matrix2x2 Zmat = { {{ 1, 0 }, { 0, 0 } }, {{ 0, 0 }, { 0, -1 } } };
constexpr dd::Matrix2x2 Hmat = { {{ dd::SQRT_2, 0 }, { dd::SQRT_2,  0 }},
								{{ dd::SQRT_2, 0 }, { -dd::SQRT_2, 0 }} };
constexpr dd::Matrix2x2 Smat = { {{ 1, 0 }, { 0, 0 } }, {{ 0, 0 }, { 0, 1 } } };
constexpr dd::Matrix2x2 Sdgmat = { {{ 1, 0 }, { 0, 0 } }, {{ 0, 0 }, { 0, -1 } } };
constexpr dd::Matrix2x2 Tmat = { {{ 1, 0 }, { 0, 0 } }, {{ 0, 0 }, { dd::SQRT_2, dd::SQRT_2 } } };
constexpr dd::Matrix2x2 Tdgmat = { {{ 1, 0 }, { 0, 0 } }, {{ 0, 0 }, { dd::SQRT_2, -dd::SQRT_2 } } };


struct gate {
	std::string name;
	short int qubits[2];
	//std::vector<dd::Index> index_set;
};

int qubits_num = 0;
int gates_num = 0;
clock_t   start, finish;
void print_index_set(std::vector<dd::Index> index_set) {
	for (int k = 0; k < index_set.size(); k++) {
		std::cout << index_set[k].key << "   " << index_set[k].idx << std::endl;

	}
}
//是为了从qasm文件的行条目中提取出门的name和qubit
vector<string> split(const string& s, const string& seperator) {
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size()) {
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}

//导入一个qasm文件
std::map<int, gate> import_circuit(std::string  file_name) {

	qubits_num = 0;
	gates_num = 0;

	std::map<int, gate> gate_set;

	std::ifstream  infile;

	infile.open(file_name);

	std::string line;
	std::getline(infile, line);
	std::getline(infile, line);
	std::getline(infile, line);
	std::getline(infile, line);
	while (std::getline(infile, line))
	{
		gate temp_gate;

		vector<string> g = split(line, " ");
		smatch result;

		temp_gate.name = g[0];

		if (g[0] == "cx") {
			regex pattern("q\\[(\\d+)\\], ?q\\[(\\d+)\\];");
			if (regex_match(g[1], result, pattern))
			{
				//std::cout << result[1] << endl;
				//std::cout << result[2] << endl;
				if (stoi(result[1]) > qubits_num) {
					qubits_num = stoi(result[1]);
				}
				if (stoi(result[2]) > qubits_num) {
					qubits_num = stoi(result[2]);
				}
				temp_gate.qubits[0] = stoi(result[1]);
				temp_gate.qubits[1] = stoi(result[2]);
			}

		}
		else {
			regex pattern("q\\[(\\d+)\\];");
			if (regex_match(g[1], result, pattern))
			{
				//std::cout << result[1] << endl;
				if (stoi(result[1]) > qubits_num) {
					qubits_num = stoi(result[1]);
				}
				temp_gate.qubits[0] = stoi(result[1]);
			}
		}

		gate_set[gates_num] = temp_gate;
		gates_num++;
	}
	infile.close();
	qubits_num += 1;
	return gate_set;
}

std::map<int, std::vector<dd::Index>> get_index(std::map<int, gate> gate_set) {

	std::map<int, std::vector<dd::Index>> Index_set;

	std::map<std::string, short> hyper_idx;

	int* qubit_idx = new   int[qubits_num]();


	for (int k = 0; k < gates_num; k++)
	{
		//std::cout << k << std::endl;
		string nam = gate_set[k].name;
		//std::cout << nam << std::endl;
		//std::cout << gate_set[k].qubits[0]<<"    "<<gate_set[k].qubits[1] << endl;
		if (nam == "cx") {
			int con_q = gate_set[k].qubits[0];
			int tar_q = gate_set[k].qubits[1];
			string cont_idx = "x";
			cont_idx += to_string(con_q);
			cont_idx += to_string(0);
			cont_idx += to_string(qubit_idx[con_q]);
			string targ_idx1 = "x";
			targ_idx1 += to_string(tar_q);
			targ_idx1 += to_string(0);
			targ_idx1 += to_string(qubit_idx[tar_q]);
			qubit_idx[tar_q] += 1;
			string targ_idx2 = "x";
			targ_idx2 += to_string(tar_q);
			targ_idx2 += to_string(0);
			targ_idx2 += to_string(qubit_idx[tar_q]);
			Index_set[k] = { {cont_idx,hyper_idx[cont_idx]},{cont_idx,hyper_idx[cont_idx] + 1},{cont_idx,hyper_idx[cont_idx] + 2},{targ_idx1,hyper_idx[targ_idx1]},{targ_idx2,hyper_idx[targ_idx2]} };
			//std::cout << cont_idx<<" " << hyper_idx[cont_idx] << " " << cont_idx << " " << hyper_idx[cont_idx] + 1 << " " << cont_idx << " " << hyper_idx[cont_idx] + 2 << " " << targ_idx1 << " " << hyper_idx[targ_idx1] << " " << targ_idx2 << " " <<hyper_idx[targ_idx2] << " " << std::endl;
			hyper_idx[cont_idx] += 2;

		}
		else {
			int tar_q = gate_set[k].qubits[0];
			string targ_idx1 = "x";
			string targ_idx2 = "x";

			targ_idx1 += to_string(tar_q);
			targ_idx1 += to_string(0);
			targ_idx1 += to_string(qubit_idx[tar_q]);
			qubit_idx[tar_q] += 1;
			targ_idx2 += to_string(tar_q);
			targ_idx2 += to_string(0);
			targ_idx2 += to_string(qubit_idx[tar_q]);
			Index_set[k] = { {targ_idx1,hyper_idx[targ_idx1]},{targ_idx2,hyper_idx[targ_idx2]} };
			if (nam == "z" || nam == "s" || nam == "sdg" || nam == "t" || nam == "tdg" || (nam[0] == 'u' && nam[1] == '1') || (nam[0] == 'r' && nam[1] == 'z')) {
				Index_set[k] = { {targ_idx1,hyper_idx[targ_idx1]},{targ_idx1,hyper_idx[targ_idx1] + 1} };
				qubit_idx[tar_q] -= 1;
				hyper_idx[targ_idx1] += 1;
			}
			else {
				Index_set[k] = { {targ_idx1,hyper_idx[targ_idx1]},{targ_idx2,hyper_idx[targ_idx2]} };
			}
		}
		//print_index_set(Index_set[k]);
	}
	return Index_set;
}


std::map<int, map<int, std::vector<int>>>  cir_partition1(std::map<int, gate> gate_set, int cx_cut_max) {
	std::map<int, map<int, std::vector<int>>>   par;
	int cx_cut = 0;
	int block = 0;
	for (int k = 0; k < gates_num; k++)
	{
		string nam = gate_set[k].name;
		if (nam != "cx") {
			if (gate_set[k].qubits[0] < qubits_num / 2) {
				par[block][0].push_back(k);
			}
			else {
				par[block][1].push_back(k);
			}
		}
		else {
			if (gate_set[k].qubits[0] < qubits_num / 2 && gate_set[k].qubits[1] < qubits_num / 2) {
				par[block][0].push_back(k);
			}
			else if (gate_set[k].qubits[0] >= qubits_num / 2 && gate_set[k].qubits[1] >= qubits_num / 2) {
				par[block][1].push_back(k);
			}
			else {
				if (cx_cut < cx_cut_max) {
					if (gate_set[k].qubits[1] >= qubits_num / 2) {
						par[block][1].push_back(k);
					}
					else {
						par[block][0].push_back(k);
					}
					cx_cut += 1;
				}
				else {
					block += 1;
					cx_cut = 1;
					if (gate_set[k].qubits[1] >= qubits_num / 2) {
						par[block][1].push_back(k);
					}
					else {
						par[block][0].push_back(k);
					}
				}
			}
		}
	}

	//for (int k = 0; k < par.size(); k++) {
	//	for (int k1 = 0; k1 < par[k][0].size(); k1++) {
	//		cout << par[k][0][k1] << "  ";
	//	}
	//	cout << endl;
	//	for (int k1 = 0; k1 < par[k][1].size(); k1++) {
	//		cout << par[k][1][k1] << "  ";
	//	}
	//	cout << endl;
	//	cout << "--------" << endl;
	//}


	return par;
}

int min(int a, int b) {
	if (a <= b) {

		return a;
	}
	else {
		return b;
	}
}
int max(int a, int b) {
	if (a >= b) {

		return a;
	}
	else {
		return b;
	}
}

std::map<int, map<int, std::vector<int>>>  cir_partition2(std::map<int, gate> gate_set, int cx_cut_max, int c_part_width) {
	std::map<int, map<int, std::vector<int>>>   par;
	int cx_cut = 0;
	int block = 0;
	int c_part_min = qubits_num / 2;
	int c_part_max = qubits_num / 2;
	for (int k = 0; k < gates_num; k++)
	{
		string nam = gate_set[k].name;
		if (cx_cut <= cx_cut_max) {

			if (nam != "cx") {
				if (gate_set[k].qubits[0] <= qubits_num / 2) {
					par[block][0].push_back(k);
				}
				else {
					par[block][1].push_back(k);
				}
			}
			else {
				if (gate_set[k].qubits[0] <= qubits_num / 2 && gate_set[k].qubits[1] <= qubits_num / 2) {
					par[block][0].push_back(k);
				}
				else if (gate_set[k].qubits[0] > qubits_num / 2 && gate_set[k].qubits[1] > qubits_num / 2) {
					par[block][1].push_back(k);
				}
				else {
					if (gate_set[k].qubits[1] >= qubits_num / 2) {
						par[block][1].push_back(k);
					}
					else {
						par[block][0].push_back(k);
					}
					cx_cut += 1;
				}
			}
		}
		else {
			if (nam != "cx") {
				if (gate_set[k].qubits[0] < c_part_min) {
					par[block][0].push_back(k);
				}
				else if (gate_set[k].qubits[0] > c_part_max) {
					par[block][1].push_back(k);
				}
				else {
					par[block][2].push_back(k);
				}
			}
			else if (gate_set[k].qubits[0] >= c_part_min && gate_set[k].qubits[0] <= c_part_max && gate_set[k].qubits[1] >= c_part_min && gate_set[k].qubits[1] <= c_part_max) {
				par[block][2].push_back(k);
			}
			else if (gate_set[k].qubits[0] < c_part_min && gate_set[k].qubits[1] < c_part_min)
			{
				par[block][0].push_back(k);
			}
			else if (gate_set[k].qubits[0] > c_part_max && gate_set[k].qubits[1] > c_part_max)
			{
				par[block][1].push_back(k);
			}
			else {
				int temp_c_min = min(c_part_min, min(gate_set[k].qubits[0], gate_set[k].qubits[1]));
				int temp_c_max = max(c_part_max, max(gate_set[k].qubits[0], gate_set[k].qubits[1]));
				if ((temp_c_max - temp_c_min) > c_part_width) {
					block += 1;
					cx_cut = 0;
					c_part_min = qubits_num / 2;
					c_part_max = qubits_num / 2;
					if (gate_set[k].qubits[0] <= qubits_num / 2 && gate_set[k].qubits[1] <= qubits_num / 2) {
						par[block][0].push_back(k);
					}
					else if (gate_set[k].qubits[0] > qubits_num / 2 && gate_set[k].qubits[1] > qubits_num / 2) {
						par[block][1].push_back(k);
					}
					else {
						if (gate_set[k].qubits[1] >= qubits_num / 2) {
							par[block][1].push_back(k);
						}
						else {
							par[block][0].push_back(k);
						}
						cx_cut += 1;
					}
				}
				else {
					par[block][2].push_back(k);
					c_part_min = temp_c_min;
					temp_c_max = temp_c_max;
				}
			}
		}
	}
	return par;
}


dd::TDD apply(dd::Package* dd, dd::TDD tdd, string nam, std::vector<dd::Index> index_set) {

	std::map<std::string, int> gate_type;
	gate_type["x"] = 1;
	gate_type["y"] = 2;
	gate_type["z"] = 3;
	gate_type["h"] = 4;
	gate_type["s"] = 5;
	gate_type["sdg"] = 6;
	gate_type["t"] = 7;
	gate_type["tdg"] = 8;

	dd::TDD temp_tdd;

	if (nam == "cx") {
		temp_tdd = dd->cnot_2_TDD(index_set, 1);
	}
	else {
		switch (gate_type[nam]) {
		case 1:
			temp_tdd = dd->Matrix2TDD(Xmat, index_set);
			break;
		case 2:
			temp_tdd = dd->Matrix2TDD(Ymat, index_set);
			break;
		case 3:
			temp_tdd = dd->diag_matrix_2_TDD(Zmat, index_set);
			break;
		case 4:
			temp_tdd = dd->Matrix2TDD(Hmat, index_set);
			break;
		case 5:
			temp_tdd = dd->diag_matrix_2_TDD(Smat, index_set);
			break;
		case 6:
			temp_tdd = dd->diag_matrix_2_TDD(Sdgmat, index_set);
			break;
		case 7:
			temp_tdd = dd->diag_matrix_2_TDD(Tmat, index_set);
			break;
		case 8:
			temp_tdd = dd->diag_matrix_2_TDD(Tdgmat, index_set);
			break;
		default:
			if (nam[0] == 'r' and nam[1] == 'z') {
				regex pattern("rz\\((-?\\d.\\d+)\\)");
				smatch result;
				regex_match(nam, result, pattern);
				float theta = stof(result[1]);
				dd::Matrix2x2 Rzmat = { {{ 1, 0 }, { 0, 0 } }, {{ 0, 0 }, { cos(theta), sin(theta) } } };
				temp_tdd = dd->diag_matrix_2_TDD(Rzmat, index_set);
				break;
			}
			if (nam[0] == 'u' and nam[1] == '1') {
				regex pattern("u1\\((-?\\d.\\d+)\\)");
				smatch result;
				regex_match(nam, result, pattern);
				float theta = stof(result[1]);
				dd::Matrix2x2 U1mat = { {{ 1, 0 }, { 0, 0 } }, {{ 0, 0 }, { cos(theta), sin(theta) } } };
				temp_tdd = dd->diag_matrix_2_TDD(U1mat, index_set);
				break;
			}
			if (nam[0] == 'u' and nam[1] == '3') {
				regex pattern("u3\\((-?\\d.\\d+), ?(-?\\d.\\d+), ?(-?\\d.\\d+)\\)");
				smatch result;
				regex_match(nam, result, pattern);
				float theta = stof(result[1]);
				float phi = stof(result[2]);
				float lambda = stof(result[3]);
				dd::Matrix2x2 U3mat = { {{ cos(theta / 2), 0 }, { -cos(lambda) * sin(theta / 2),-sin(lambda) * sin(theta / 2)} }, {{ cos(phi) * sin(theta / 2),sin(phi) * sin(theta / 2) }, { cos(lambda + phi) * cos(theta / 2),sin(lambda + phi) * cos(theta / 2) } } };
				temp_tdd = dd->Matrix2TDD(U3mat, index_set);
				break;
			}
		}
	}
	tdd = dd->cont(tdd, temp_tdd);

	return tdd;
}

std::map<std::string, int> get_var_order() {

	std::map<std::string, int> var;


	//设置变量顺序
	int order_num = 1;
	for (int k = qubits_num - 1; k >= 0; k--) {
		string idx_nam;
		idx_nam = "x";
		idx_nam += to_string(k);
		var[idx_nam] = order_num;
		order_num += 1;
		for (int k2 = 0; k2 <= gates_num; k2++) {
			idx_nam = "x";
			idx_nam += to_string(k);
			idx_nam += to_string(0);
			idx_nam += to_string(k2);
			var[idx_nam] = order_num;
			order_num += 1;
			//cout << idx_nam << endl;
		}
		idx_nam = "y";
		idx_nam += to_string(k);
		var[idx_nam] = order_num;
		order_num += 1;
	}

	var["-1"] = order_num;

	return var;
}


int* Simulate_with_partition1(std::string path, std::string  file_name) {

	std::map<int, gate> gate_set = import_circuit(path + file_name);
	std::map<int, std::vector<dd::Index>> Index_set = get_index(gate_set);
	int cx_cut_max = qubits_num / 2 + 1;
	std::map<int, map<int, std::vector<int>>>  par = cir_partition1(gate_set, cx_cut_max);

	int* nodes = new int[2];
	nodes[0] = 0;
	nodes[1] = 0;

	std::map<std::string, int> var;

	var = get_var_order();

	auto* dd = new dd::Package(var);

	dd::TDD tdd = { dd->DDone ,{} };

	start = clock();
	for (int k = 0; k < par.size(); k++) {
		//std::cout << "block:" << k << std::endl;
		dd::TDD temp_tdd1 = { dd->DDone ,{} };
		for (int k1 = 0; k1 < par[k][0].size(); k1++) {
			int gate_idx = par[k][0][k1];
			string nam = gate_set[gate_idx].name;
			temp_tdd1 = apply(dd, temp_tdd1, nam, Index_set[gate_idx]);
		}


		dd::TDD temp_tdd2 = { dd->DDone ,{} };
		for (int k1 = 0; k1 < par[k][1].size(); k1++) {
			int gate_idx = par[k][1][k1];
			string nam = gate_set[gate_idx].name;
			temp_tdd2 = apply(dd, temp_tdd2, nam, Index_set[gate_idx]);
		}

		dd::TDD temp_tdd = dd->cont(temp_tdd1, temp_tdd2);
		tdd = dd->cont(tdd, temp_tdd);
		int node_num_max = dd->size(tdd.e);
		if (node_num_max > nodes[0]) {
			nodes[0] = node_num_max;
		}
		finish = clock();
		double time = (double)(finish - start) / CLOCKS_PER_SEC;
		if (time > 1800) {
			return nodes;
		}
	}

	int node_num_final = dd->size(tdd.e);

	nodes[1] = node_num_final;

	//dd->export2Dot(tdd.e, file_name, false);
	//dd->export2Dot(tdd.e, "par1", false);
	dd->garbageCollect(true);
	delete dd;
	std::cout << "Done!!!" << std::endl;
	return nodes;
}

int* Simulate_with_partition2(std::string path, std::string  file_name) {

	std::map<int, gate> gate_set = import_circuit(path + file_name);
	std::map<int, std::vector<dd::Index>> Index_set = get_index(gate_set);
	int cx_cut_max = qubits_num / 2 + 1;
	std::map<int, map<int, std::vector<int>>>  par = cir_partition2(gate_set, cx_cut_max, qubits_num / 2);

	int* nodes = new int[2];
	nodes[0] = 0;
	nodes[1] = 0;

	std::map<std::string, int> var;

	var = get_var_order();

	auto* dd = new dd::Package(var);

	dd::TDD tdd = { dd->DDone ,{} };

	start = clock();
	for (int k = 0; k < par.size(); k++) {

		dd::TDD temp_tdd1 = { dd->DDone ,{} };
		for (int k1 = 0; k1 < par[k][0].size(); k1++) {
			int gate_idx = par[k][0][k1];
			string nam = gate_set[gate_idx].name;
			temp_tdd1 = apply(dd, temp_tdd1, nam, Index_set[gate_idx]);
		}


		dd::TDD temp_tdd2 = { dd->DDone ,{} };
		for (int k1 = 0; k1 < par[k][1].size(); k1++) {
			int gate_idx = par[k][1][k1];
			string nam = gate_set[gate_idx].name;
			temp_tdd2 = apply(dd, temp_tdd2, nam, Index_set[gate_idx]);
		}

		dd::TDD temp_tdd3 = { dd->DDone ,{} };
		for (int k1 = 0; k1 < par[k][2].size(); k1++) {
			int gate_idx = par[k][2][k1];
			string nam = gate_set[gate_idx].name;
			temp_tdd3 = apply(dd, temp_tdd3, nam, Index_set[gate_idx]);
		}


		dd::TDD temp_tdd = dd->cont(temp_tdd1, temp_tdd2);
		temp_tdd = dd->cont(temp_tdd, temp_tdd3);
		tdd = dd->cont(tdd, temp_tdd);

		int node_num_max = dd->size(tdd.e);
		if (node_num_max > nodes[0]) {
			nodes[0] = node_num_max;
		}
		finish = clock();
		double time = (double)(finish - start) / CLOCKS_PER_SEC;
		if (time > 1800) {
			return nodes;
		}
	}

	int node_num_final = dd->size(tdd.e);

	nodes[1] = node_num_final;

	//dd->export2Dot(tdd.e, file_name, false);

	dd->garbageCollect(true);
	delete dd;
	std::cout << "Done!!!" << std::endl;
	return nodes;
}

//计算qubit_num
int get_qubits_num(std::string  file_name) {

	int qubits_num = 0;

	std::ifstream  infile;

	infile.open(file_name);

	std::string line;
	std::getline(infile, line);
	std::getline(infile, line);
	std::getline(infile, line);
	std::getline(infile, line);
	while (std::getline(infile, line))
	{

		vector<string> g = split(line, " ");

		smatch result;

		if (g[0] == "cx") {

			regex pattern("q\\[(\\d+)\\],q\\[(\\d+)\\];");
			if (regex_match(g[1], result, pattern))
			{
				if (stoi(result[1]) > qubits_num) {
					qubits_num = stoi(result[1]);
				}
				if (stoi(result[2]) > qubits_num) {
					qubits_num = stoi(result[2]);
				}
			}

		}
		else {
			regex pattern("q\\[(\\d+)\\];");
			if (regex_match(g[1], result, pattern))
			{
				if (stoi(result[1]) > qubits_num) {
					qubits_num = stoi(result[1]);
				}
			}
		}
	}
	infile.close();
	qubits_num += 1;
	return qubits_num;
}

//计算gate_num
int get_gates_num(std::string  file_name) {

	int gates_num = 0;

	std::ifstream  infile;

	infile.open(file_name);

	std::string line;
	std::getline(infile, line);
	std::getline(infile, line);
	std::getline(infile, line);
	std::getline(infile, line);
	while (std::getline(infile, line))
	{
		gates_num += 1;
	}
	infile.close();
	return gates_num;
}

//计算一个电路的tdd
int* Simulate_with_tdd(std::string path, std::string  file_name) {

	std::map<int, gate> gate_set = import_circuit(path + file_name);
	std::map<int, std::vector<dd::Index>> Index_set = get_index(gate_set);

	int* nodes = new int[2];
	nodes[0] = 0;
	nodes[1] = 0;

	std::map<std::string, int> var;

	var = get_var_order();

	//开始仿真，每一步都要处理指标，生成当前门的temp_tdd,并与现有的结果收缩得到完整tdd
	auto* dd = new dd::Package(var);

	dd::TDD tdd = { dd->DDone ,{} };
	start = clock();
	for (int k = 0; k < gate_set.size(); k++) {
		{
			tdd = apply(dd, tdd, gate_set[k].name, Index_set[k]);
			int node_num_max = dd->size(tdd.e);
			if (node_num_max > nodes[0]) {
				nodes[0] = node_num_max;
			}
			finish = clock();
			double time = (double)(finish - start) / CLOCKS_PER_SEC;
			if (time > 1800) {
				return nodes;
			}
		}
	}
		int node_num_final = dd->size(tdd.e);
		nodes[1] = node_num_final;

		//dd->export2Dot(tdd.e, file_name, false);
		//dd->export2Dot(tdd.e, "tdd", false);
		dd->statistics();
		dd->garbageCollect(true);
		delete dd;
		std::cout << "Done!!!" << std::endl;
		return nodes;
	}