#include "DDpackage.h"
#include <iostream>
#include <cmath>
#include <string> 
#include "Cir_import.h"

//namespace dd = dd_package;
int main2();

int main() {


    std::string path = "Benchmarks/";

	std::string file_name = "test.qasm";
    int* nodes;
    int n = get_qubits_num(path + file_name);
    clock_t   start_t, finish_t;
    double time_t;
    std::cout << "File name:" << file_name << std::endl;
    start_t = clock();
    nodes = Simulate_with_tdd(path, file_name);
    finish_t = clock();
    time_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;

    std::cout << "Time:" << time_t << std::endl;
    std::cout << "Nodes max:" << *nodes << std::endl;
    std::cout << "Nodes Final:" << *(nodes + 1) << std::endl;
	std::cout << "==================================="<< std::endl;

	//std::cout << "File name:" << file_name << std::endl;
	//start_t = clock();
	//nodes = Simulate_with_partition1(path, file_name);
	//finish_t = clock();
	//time_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;

	//std::cout << "Time:" << time_t << std::endl;
	//std::cout << "Nodes max:" << *nodes << std::endl;
	//std::cout << "Nodes Final:" << *(nodes + 1) << std::endl;
	//std::cout << "===================================" << std::endl;

	//std::cout << "File name:" << file_name << std::endl;
	//start_t = clock();
	//nodes = Simulate_with_partition2(path, file_name);
	//finish_t = clock();
	//time_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;

	//std::cout << "Time:" << time_t << std::endl;
	//std::cout << "Nodes max:" << *nodes << std::endl;
	//std::cout << "Nodes Final:" << *(nodes + 1) << std::endl;
	//std::cout << "===================================" << std::endl;

	//main2();
    system("pause");

}

int main2() {

	std::ofstream  ofile;

	string path2 = "Benchmarks2/";
	std::ifstream  file_list2;
	std::string line2;
	clock_t   start2, finish2;
	double time2;
	int* nodes2;


	//ofile.open("data.csv", ios::app);
	//ofile << "Simulate_with_tdd" << endl;
	//ofile << "benchmarks" << "," << "time" << "," << "node max" << "," << "node final" << endl;
	//file_list2.open("test.txt");
	//while (std::getline(file_list2, line2)) {
	//	std::cout << "file name:" << line2 << std::endl;
	//	start2 = clock();
	//	nodes2 = Simulate_with_tdd(path2, line2);
	//	finish2 = clock();
	//	time2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
	//	std::cout << "time:" << time2 << std::endl;
	//	std::cout << "nodes max:" << *nodes2 << std::endl;
	//	std::cout << "nodes final:" << *(nodes2 + 1) << std::endl;
	//	ofile << line2 << "," << time2 << "," << *nodes2 << "," << *(nodes2 + 1) << endl;
	//}
	//file_list2.close();
	//ofile.close();


	ofile.open("data.csv", ios::app);
	ofile << "Simulate_with_partition1" << endl;
	ofile << "benchmarks" << "," << "time" << "," << "node max" << "," << "node final" << endl;
	file_list2.open("test.txt");
	while (std::getline(file_list2, line2)) {
		std::cout << "file name:" << line2 << std::endl;
		start2 = clock();
		nodes2 = Simulate_with_partition1(path2, line2);
		finish2 = clock();
		time2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
		std::cout << "time:" << time2 << std::endl;
		std::cout << "nodes max:" << *nodes2 << std::endl;
		std::cout << "nodes final:" << *(nodes2 + 1) << std::endl;
		ofile << line2 << "," << time2 << "," << *nodes2 << "," << *(nodes2 + 1) << endl;
	}
	file_list2.close();
	ofile.close();

	//ofile.open("data.csv", ios::app);
	//ofile << "Simulate_with_partition2" << endl;
	//ofile << "benchmarks" << "," << "time" << "," << "node max" << "," << "node final" << endl;
	//file_list2.open("test.txt");
	//while (std::getline(file_list2, line2)) {
	//	std::cout << "file name:" << line2 << std::endl;
	//	start2 = clock();
	//	nodes2 = Simulate_with_partition2(path2, line2);
	//	finish2 = clock();
	//	time2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
	//	std::cout << "time:" << time2 << std::endl;
	//	std::cout << "nodes max:" << *nodes2 << std::endl;
	//	std::cout << "nodes final:" << *(nodes2 + 1) << std::endl;
	//	ofile << line2 << "," << time2 << "," << *nodes2 << "," << *(nodes2 + 1) << endl;
	//}
	//file_list2.close();
	//ofile.close();

	system("pause");
	return 0;
}