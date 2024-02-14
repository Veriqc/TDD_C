#ifndef Cir_tn_HPP
#define Cir_tn_HPP

#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include <map>
#include <stdexcept>

#include "dd/GateXarray.hpp"

#include "dd/Tensor.hpp"
#include "dd/Package.hpp"
#include "QuantumComputation.hpp"

#include <xtensor/xarray.hpp>


#ifndef gate
	struct gate {
		std::string name;
		short int qubits[2];
		//std::vector<dd::Index> index_set;
	};
#endif // !gate
#ifndef circuitResult
	struct circuitReslut
	{
		std::map<int, gate> gate_set;
		int qubits_num;
		int gates_num;
	};
#endif // !circuitResult
#ifndef release
bool release = true;
bool get_max_node = true;
constexpr long double PI = 3.14159265358979323846264338327950288419716939937510L;
#endif // !release

#ifndef print_index_set
void print_index_set(std::vector<dd::Index> index_set) {
	for (int k = 0; k < index_set.size(); k++) {
		std::cout << "(" << index_set[k].key << ", " << index_set[k].idx << ") ,";

	}
	std::cout << std::endl;
}
#endif
//是为了从qasm文件的行条目中提取出门的name和qubit
std::vector<std::string> split(const std::string& s, const std::string& seperator) {
	std::vector<std::string> result;
	typedef std::string::size_type string_size;
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


float match_a_string(std::string s) {
	std::smatch result;
	std::regex pattern1("(-?\\d+.\\d+)");
	std::regex pattern2("(-?\\d+.\\d+)\\*?pi/(\\d+)");
	std::regex pattern3("(-?\\d+.\\d+)\\*?pi");
	std::regex pattern4("pi/(\\d+)");
	std::regex pattern5("(\\d+)");
	std::regex pattern6("-pi/(\\d+)");
	if (regex_match(s, result, pattern1)) {
		//cout << result[1] << endl;
		return stof(result[1]);
	}
	else if (regex_match(s, result, pattern2)) {
		//cout << result[1] << ',' << result[2] << endl;
		return stof(result[1]) * PI / stof(result[2]);
	}
	else if (regex_match(s, result, pattern3)) {
		//cout << result[1] << endl;
		return stof(result[1]) * PI;
	}
	else if (regex_match(s, result, pattern4)) {
		//cout << result[1] << endl;
		return PI / stof(result[1]);
	}
	else if (regex_match(s, result, pattern5)) {
		//cout << result[1] << endl;
		return stof(result[1]);
	}
	else if (regex_match(s, result, pattern6)) {
		//cout << result[1] << endl;
		return -PI / stof(result[1]);
	}
	std::cout << s << std::endl;
	std::cout << "Not Macth" << std::endl;
	return 0.0;
}

//导入一个qasm文件
circuitReslut import_circuit(std::string file_name) {

	circuitReslut res;
	res.gates_num = 0;
	res.qubits_num = 0;

	std::ifstream infile;
	infile.open(file_name);
	if (!infile.is_open()) {
		throw std::runtime_error("File not found: " + file_name);
	}

	std::string line;
	std::getline(infile, line);
	std::getline(infile, line);
	std::getline(infile, line);
	std::getline(infile, line);
	while (std::getline(infile, line))
	{
		gate temp_gate;

		std::vector<std::string> g = split(line, " ");
		std::smatch result;

		temp_gate.name = g[0];

		if (xgate::twoGate.find(g[0]) != xgate::twoGate.end()||g[0].substr(0,3)=="cu1") {
			std::regex pattern("q\\[(\\d+)\\], ?q\\[(\\d+)\\];\r?");
			if (std::regex_match(g[1], result, pattern))
			{
				if (stoi(result[1]) > res.qubits_num) {
					res.qubits_num = stoi(result[1]);
				}
				if (stoi(result[2]) > res.qubits_num) {
					res.qubits_num = stoi(result[2]);
				}
				temp_gate.qubits[0] = stoi(result[1]);
				temp_gate.qubits[1] = stoi(result[2]);
			}

		}
		else {
			std::regex pattern("q\\[(\\d+)\\];\r?");
			if (regex_match(g[1], result, pattern))
			{
				if (stoi(result[1]) > res.qubits_num) {
					res.qubits_num = stoi(result[1]);
				}
				temp_gate.qubits[0] = stoi(result[1]);
			}
		}

		res.gate_set[res.gates_num] = temp_gate;
		res.gates_num++;
	}
	infile.close();
	res.qubits_num += 1;
	return res;
}

std::map<int, std::vector<dd::Index>> get_circuit_index(const circuitReslut& cir, std::map<std::string, int> var) {

	std::map<int, std::vector<dd::Index>> Index_set;

	std::map<std::string, short> hyper_idx;


	for (const auto& pair : var) {
		hyper_idx[pair.first] = 0;
	}



	int* qubit_idx = new   int[cir.qubits_num]();


	for (int k = 0; k < cir.gates_num; k++)
	{
		//std::cout << k << std::endl;
		gate gateObj = cir.gate_set.find(k)->second;
		std::string nam = gateObj.name;
		//std::cout << nam << std::endl;
		//std::cout << gate_set[k].qubits[0]<<"    "<<gate_set[k].qubits[1] << endl;
		if (xgate::twoGate.find(nam) != xgate::twoGate.end()||nam.substr(0,3)=="cu1") {
			int con_q = gateObj.qubits[0];
			int tar_q = gateObj.qubits[1];
						std::string cont_idx = "x";
			cont_idx += std::to_string(con_q);
			cont_idx += "_";
			cont_idx += std::to_string(qubit_idx[con_q]);
			std::string targ_idx1 = "x";
			targ_idx1 += std::to_string(tar_q);
			targ_idx1 += "_";
			targ_idx1 += std::to_string(qubit_idx[tar_q]);
			qubit_idx[tar_q] += 1;
			std::string targ_idx2 = "x";
			targ_idx2 += std::to_string(tar_q);
			targ_idx2 += "_";
			targ_idx2 += std::to_string(qubit_idx[tar_q]);
			Index_set[k] = {
				{cont_idx,hyper_idx[cont_idx]},
				{cont_idx,static_cast<short>(hyper_idx[cont_idx] + 1)},
				{targ_idx1,hyper_idx[targ_idx1]},
				{targ_idx2,hyper_idx[targ_idx2]} };
			//std::cout << cont_idx<<" " << hyper_idx[cont_idx] << " " << cont_idx << " " << hyper_idx[cont_idx] + 1 << " " << cont_idx << " " << hyper_idx[cont_idx] + 2 << " " << targ_idx1 << " " << hyper_idx[targ_idx1] << " " << targ_idx2 << " " <<hyper_idx[targ_idx2] << " " << std::endl;
			hyper_idx[cont_idx] += 1;

		}
		else {
			int tar_q = gateObj.qubits[0];
			std::string targ_idx1 = "x";
			std::string targ_idx2 = "x";

			targ_idx1 += std::to_string(tar_q);
			targ_idx1 += "_";
			targ_idx1 += std::to_string(qubit_idx[tar_q]);
			qubit_idx[tar_q] += 1;
			targ_idx2 += std::to_string(tar_q);
			targ_idx2 += "_";
			targ_idx2 += std::to_string(qubit_idx[tar_q]);
			Index_set[k] = { {targ_idx1,hyper_idx[targ_idx1]},{targ_idx2,hyper_idx[targ_idx2]} };
			// TODO:bug here
			// if (nam == "z" || nam == "s" || nam == "sdg" || nam == "t" || nam == "tdg" || (nam[0] == 'u' && nam[1] == '1') || (nam[0] == 'r' && nam[1] == 'z')) {
			// 	Index_set[k] = {
			// 		{targ_idx1,hyper_idx[targ_idx1]},
			// 		{targ_idx1,short(hyper_idx[targ_idx1] + 1)} };
			// 	qubit_idx[tar_q] -= 1;
			// 	hyper_idx[targ_idx1] += 1;
			// }
			// else {
			// 	Index_set[k] = { {targ_idx1,hyper_idx[targ_idx1]},{targ_idx2,hyper_idx[targ_idx2]} };
			// }
		}
		//std::cout << k << " ";
		//print_index_set(Index_set[k]);
	}
	return Index_set;
}


std::map<int, std::map<int, std::vector<int>>>  cir_partition1(const circuitReslut& cir, int cx_cut_max) {
	std::map<int, std::map<int, std::vector<int>>>   par;
	int cx_cut = 0;
	int block = 0;
	for (int k = 0; k < cir.gates_num; k++)
	{
		gate gateObj = cir.gate_set.find(k)->second;
		std::string nam = gateObj.name;
		if (nam != "cx") {
			if (gateObj.qubits[0] <= cir.qubits_num / 2) {
				par[block][0].push_back(k);
			}
			else {
				par[block][1].push_back(k);
			}
		}
		else {
			if (gateObj.qubits[0] <= cir.qubits_num / 2 && gateObj.qubits[1] <= cir.qubits_num / 2) {
				par[block][0].push_back(k);
			}
			else if (gateObj.qubits[0] > cir.qubits_num / 2 && gateObj.qubits[1] > cir.qubits_num / 2) {
				par[block][1].push_back(k);
			}
			else {
				if (cx_cut <= cx_cut_max) {
					if (gateObj.qubits[1] > cir.qubits_num / 2) {
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
					if (gateObj.qubits[1] > cir.qubits_num / 2) {
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

std::map<int, std::map<int, std::vector<int>>>  cir_partition2(const circuitReslut& cir, int cx_cut_max, int c_part_width) {
	std::map<int, std::map<int, std::vector<int>>>   par;
	int cx_cut = 0;
	int block = 0;
	int c_part_min = cir.qubits_num / 2;
	int c_part_max = cir.qubits_num / 2;
	for (int k = 0; k < cir.gates_num; k++)
	{
		gate gateObj = cir.gate_set.find(k)->second;
		std::string nam = gateObj.name;

		if (cx_cut <= cx_cut_max) {

			if (nam != "cx") {
				if (gateObj.qubits[0] <= cir.qubits_num / 2) {
					par[block][0].push_back(k);
				}
				else {
					par[block][1].push_back(k);
				}
			}
			else {
				if (gateObj.qubits[0] <= cir.qubits_num / 2 && gateObj.qubits[1] <= cir.qubits_num / 2) {
					par[block][0].push_back(k);
				}
				else if (gateObj.qubits[0] > cir.qubits_num / 2 && gateObj.qubits[1] > cir.qubits_num / 2) {
					par[block][1].push_back(k);
				}
				else {
					if (gateObj.qubits[1] > cir.qubits_num / 2) {
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
				if (gateObj.qubits[0] < c_part_min) {
					par[block][0].push_back(k);
				}
				else if (gateObj.qubits[0] > c_part_max) {
					par[block][1].push_back(k);
				}
				else {
					par[block][2].push_back(k);
				}
			}
			else if (gateObj.qubits[0] >= c_part_min && gateObj.qubits[0] <= c_part_max && gateObj.qubits[1] >= c_part_min && gateObj.qubits[1] <= c_part_max) {
				par[block][2].push_back(k);
			}
			else if (gateObj.qubits[0] < c_part_min && gateObj.qubits[1] < c_part_min)
			{
				par[block][0].push_back(k);
			}
			else if (gateObj.qubits[0] > c_part_max && gateObj.qubits[1] > c_part_max)
			{
				par[block][1].push_back(k);
			}
			else {
				int temp_c_min = min(c_part_min, min(gateObj.qubits[0], gateObj.qubits[1]));
				int temp_c_max = max(c_part_max, max(gateObj.qubits[0], gateObj.qubits[1]));
				if ((temp_c_max - temp_c_min) > c_part_width) {
					block += 1;
					cx_cut = 0;
					c_part_min = cir.qubits_num / 2;
					c_part_max = cir.qubits_num / 2;
					if (gateObj.qubits[0] <= cir.qubits_num / 2 && gateObj.qubits[1] <= cir.qubits_num / 2) {
						par[block][0].push_back(k);
					}
					else if (gateObj.qubits[0] > cir.qubits_num / 2 && gateObj.qubits[1] > cir.qubits_num / 2) {
						par[block][1].push_back(k);
					}
					else {
						if (gateObj.qubits[1] > cir.qubits_num / 2) {
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
					c_part_max = temp_c_max;
				}
			}
		}
	}

	//for (int k = 0; k < 1; k++) {
	//for (int k1 = 0; k1 < par[k][0].size(); k1++) {
	//	cout << par[k][0][k1] << "  ";
	//}
	//cout << endl;
	//for (int k1 = 0; k1 < par[k][1].size(); k1++) {
	//	cout << par[k][1][k1] << "  ";
	//}
	//cout << endl;
	//for (int k1 = 0; k1 < par[k][2].size(); k1++) {
	//	cout << par[k][2][k1] << "  ";
	//}
	//cout << endl;
	//cout << "--------" << endl;
	//}
	return par;
}


std::map<std::string, int> get_var_order(const int qubits_num,const int gates_num) {

	std::map<std::string, int> var;


	//设置变量顺序
	int order_num = 1000000;

	for (int k = qubits_num; k >= 0; k--) {
		std::string idx_nam;
		idx_nam = "y";
		idx_nam += std::to_string(k);
		var[idx_nam] = order_num;
		order_num -= 1;
		for (int k2 = gates_num; k2 >= 0; k2--) {
			idx_nam = "x";
			idx_nam += std::to_string(k);
			idx_nam += "_";
			idx_nam += std::to_string(k2);
			var[idx_nam] = order_num;
			order_num -= 1;
			//cout << idx_nam << endl;
		}
		idx_nam = "x";
		idx_nam += std::to_string(k);
		var[idx_nam] = order_num;
		order_num -= 1;
	}
	return var;
}


//计算qubit_num
int get_qubits_num(std::string  file_name) {

	int qubits_num = 0;

	std::ifstream  infile;

	infile.open(file_name);
	if (!infile.is_open()) {
        throw std::runtime_error("null file"); // Return an error code
    }

	std::string line;
	std::getline(infile, line);
	std::getline(infile, line);
	std::getline(infile, line);
	std::getline(infile, line);
	while (std::getline(infile, line))
	{

		std::vector<std::string> g = split(line, " ");

		std::smatch result;

		if (xgate::twoGate.find(g[0]) != xgate::twoGate.end()||g[0].substr(0,3)=="cu1") {

			std::regex pattern("q\\[(\\d+)\\],q\\[(\\d+)\\];\r?");
			if (std::regex_match(g[1], result, pattern))
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
			std::regex pattern("q\\[(\\d+)\\];\r?");
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
	if (!infile.is_open()) {
        throw std::runtime_error("null file"); // Return an error code
    }

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


dd::Tensor gate_2_tensor(std::string name, std::vector<dd::Index> index_set) {

	static const std::map<std::string, xt::xarray<dd::ComplexValue>> gate_type = {
		{"x", xgate::Xmat}, {"y", xgate::Ymat}, {"z", xgate::Zmat}, {"h", xgate::Hmat},
		{"s", xgate::Smat}, {"sdg", xgate::Sdagmat}, {"t", xgate::Tmat}, {"tdg", xgate::Tdagmat},
		{"cx",xgate::CXmat},{"swap", xgate::SWAPmat},
	};

	auto it = gate_type.find(name);
	if (it != gate_type.end()) {
		return dd::Tensor(it->second, index_set, name);
	}
	if (name.rfind("rz(", 0) == 0) {
		std::smatch result;
		if (std::regex_search(name, result, std::regex("rz\\((-?\\d+\\.\\d+)\\)"))) {
			float theta = stof(result[1]);
			return dd::Tensor(xgate::Phasemat(theta), index_set, "Rz");
		}
	}
	if (name.rfind("u1(", 0) == 0) {
		std::smatch result;
		if (std::regex_search(name, result, std::regex("u1\\((-?\\d+\\.\\d+)\\)"))) {
			float theta = stof(result[1]);
			return dd::Tensor(xgate::Phasemat(theta), index_set, "Rz");
		}
	}
	if (name.rfind("cu1(", 0) == 0) {
		std::smatch result;
		if (std::regex_search(name, result, std::regex("cu1\\(pi/(\\d+)(?:\\))"))) {
			int Fraction = stof(result[1]);
			return dd::Tensor(xgate::CU1mat(PI/Fraction), index_set, "cu1");
		}
	}
	if (name.rfind("u3(", 0) == 0) {
		std::smatch result;
		if (std::regex_search(name, result, std::regex("u3\\((-?\\d+\\.\\d+),\\s*(-?\\d+\\.\\d+),\\s*(-?\\d+\\.\\d+)\\)"))) {
			float theta = stof(result[1]);
			float phi = stof(result[2]);
			float lambda = stof(result[3]);
			return dd::Tensor(xgate::U3mat(lambda, phi, theta), index_set, "U3");
		}
	}
	throw std::invalid_argument("Unsupported gate: " + name);
}

dd::TensorNetwork cir_2_tn(std::string path, std::string  file_name, std::shared_ptr<dd::Package<>>& ddpack) {
	// TODO: optimize code structure

	circuitReslut cir = import_circuit(path + file_name);
	std::cout << "Start!!!" << std::endl;

	ddpack->varOrder = get_var_order(cir.qubits_num, cir.gates_num);
	std::map<int, std::vector<dd::Index>> Index_set = get_circuit_index(cir, ddpack->varOrder);

	dd::TensorNetwork tn;

	for (int k = 0; k < cir.gates_num; k++) {
		tn.add_ts(gate_2_tensor(cir.gate_set[k].name, Index_set[k]));
	}

	std::cout << "done" << std::endl;
	return tn;
}

void op_2_tensor(const std::unique_ptr<qc::Operation>& op) {
	std::cout << "----------------------" <<std::endl;
    std::cout<< op->getName() << std::endl;
    std::cout << "is control: "<<op->isControlled() << std::endl;
	std::cout << "is stanard: "<<op->isStandardOperation() << std::endl;
    std::cout << "qubits: " ;
    for(auto& q: op->getUsedQubits()){
      std::cout << q <<" ";
    }
    std::cout << std::endl;
    if(op->isControlled()){
      std::cout << "controls:";
      auto c = op->getControls();
      for(auto i: c){
        std::cout << i.qubit << " ";
      }
      std::cout << std::endl;
	  std::cout << "targets:";
      auto t = op->getTargets();
      for(auto i: t){
        std::cout << i << " ";
      }
      std::cout << std::endl;
    }
    auto p = op->getParameter();
    if(!p.empty()){
      std::cout << "para" << std::endl;
      for(auto& i: p){
        std::cout << i << " ";
      }
	std::cout << std::endl;
	}
}

std::vector<dd::Index> getOpIndex(const std::unique_ptr<qc::Operation>& op,std::vector<int>& existIndexs, std::map<std::string, short>& hyperIndexs){
	size_t n = op->getUsedQubits().size();

	auto buildIndex = [](int q, int idx) {
		std::ostringstream oss;
		oss << 'x' << q << '_' << idx;
		return oss.str();
	};
	std::vector<dd::Index> indexSet;

	if (op->isControlled() && n == 2) {
		auto it = op->getControls();
		const int con_q = it.begin()->qubit;
		const int tar_q = op->getTargets()[0];

		const std::string cont_idx = buildIndex(con_q, existIndexs[con_q]);
		const std::string targ_idx1 = buildIndex(tar_q, existIndexs[tar_q]);
		existIndexs[tar_q] += 1;
		const std::string targ_idx2 = buildIndex(tar_q, existIndexs[tar_q]);

		indexSet = {
			{cont_idx, hyperIndexs[cont_idx]},
			{cont_idx, static_cast<short>(hyperIndexs[cont_idx] + 1)},
			{targ_idx1, hyperIndexs[targ_idx1]},
			{targ_idx2, hyperIndexs[targ_idx2]}
		};
		hyperIndexs[cont_idx] += 1;
		return indexSet;
	}
	else if (n == 1){
		const int tar_q = *op->getUsedQubits().begin();
		const std::string targ_idx1 = buildIndex(tar_q,existIndexs[tar_q]);
		existIndexs[tar_q] += 1;
		const std::string targ_idx2 = buildIndex(tar_q,existIndexs[tar_q]);
		indexSet = {
			{targ_idx1,hyperIndexs[targ_idx1]},
			{targ_idx2,hyperIndexs[targ_idx2]}
		};
		return indexSet;
	}
	throw std::invalid_argument("throw from getOpIndex. unknown gate: "+ op->getName());
}

xt::xarray<dd::ComplexValue> getOpData(const std::unique_ptr<qc::Operation>& op) {
    static const std::map<std::string, xt::xarray<dd::ComplexValue>> supportGate = {
        {"x", xgate::Xmat}, {"y", xgate::Ymat}, {"z", xgate::Zmat}, {"h", xgate::Hmat},
        {"s", xgate::Smat}, {"sdg", xgate::Sdagmat}, {"t", xgate::Tmat}, {"tdg", xgate::Tdagmat},
        {"swap", xgate::SWAPmat},
    };

    const std::string& gateName = op->getName(); // Use reference to avoid copying
    const auto& parameters = op->getParameter(); // Avoid multiple calls
    size_t Npara = parameters.size();
    bool isControlled = op->isControlled();

    // Check for non-controlled operations with no parameters first
    if (!isControlled && Npara == 0) {
        auto it = supportGate.find(gateName);
        if (it != supportGate.end()) {
            return it->second;
        }
    } 
    else if (!isControlled && Npara == 1 && gateName == "p") {
        return xgate::Phasemat(parameters[0]);
    } 
    else if (isControlled && Npara == 0 && gateName == "x") {
        return xgate::CXmat;
    }

    // If no condition is met, throw an exception
    std::string prefix(op->getControls().size(), 'c'); // Create a string of 'c' characters
	throw std::invalid_argument("Unsupported gate: " + prefix + gateName);
}


dd::TensorNetwork cir_2_tn(std::shared_ptr<qc::QuantumComputation>& QC, std::shared_ptr<dd::Package<>>& ddPack){
	auto var = get_var_order(QC->getNqubits(), QC->getNops());
	ddPack->varOrder = var;

	std::vector<int> existIndexs(QC->getNqubits(), 0);
	std::map<std::string, short> hyperIndexs;
	for (const auto& pair : var) {
		hyperIndexs[pair.first] = 0;
	}

	dd::TensorNetwork tn;
	for(const auto& op: *QC){
		// op_2_tensor(op);
		if(!op->isStandardOperation()){
			throw std::invalid_argument("unstandard gate, which is not support");
		}
		// if it contains any dynamic circuit primitives, it certainly is dynamic
		if (op->isClassicControlledOperation() || op->getType() == qc::Reset) {
			// isDynamicCircuit = true;
			throw std::invalid_argument("a dynamic circuit, which is not suppory");
		}

		// once a measurement is encountered we store the corresponding mapping
		// (qubit -> bit)
		if (const auto* measure = dynamic_cast<qc::NonUnitaryOperation*>(op.get());
			measure != nullptr && measure->getType() == qc::Measure) {
			// hasMeasurements = true;
			throw std::invalid_argument("have measurement, which is not suppory");
		}

		auto indexSet = getOpIndex(op,existIndexs,hyperIndexs);
		auto data = getOpData(op);

		std::string prefix(op->getControls().size(), 'c');
		std::string gateName = prefix + op->getName();
		tn.add_ts(dd::Tensor(data,indexSet,gateName));
	}
	return tn;

}
#endif