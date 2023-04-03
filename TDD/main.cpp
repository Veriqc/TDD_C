#include "TDDpackage.h"
#include "Cir_import.h"
#include <iostream>
#include <fstream>
#include <array>
#include <string> 
#include <regex>


int main() {


	//std::ofstream  ofile;

	//string path = "benchmarks/";
	//std::ifstream  file_list;
	//std::string line;
	//clock_t   start, finish;
	//double time;
	//int* nodes;


	//ofile.open("data4.csv", ios::app);
	//ofile << "Simulate_with_tdd" << endl;
	//ofile << "benchmarks" << "," << "time" << "," << "node max" << "," << "node final" << endl;
	//file_list.open("test.txt");
 //   while (std::getline(file_list, line)) {
	//	std::cout << "file name:" << line << std::endl;
	//	start = clock();
	//	nodes = Simulate_with_tdd(path, line);
	//	finish = clock();
	//	time = (double)(finish - start) / CLOCKS_PER_SEC;
	//	std::cout << "time:" << time << std::endl;
	//	std::cout << "nodes max:" << *nodes << std::endl;
	//	std::cout << "nodes final:" << *(nodes + 1) << std::endl;
	//	ofile << line << "," << time << "," << *nodes << "," << *(nodes + 1) << endl;
 //   }
	//file_list.close();
	//ofile.close();


	//ofile.open("data4.csv", ios::app);
	//ofile << "Simulate_with_partition1" << endl;
	//ofile << "benchmarks" << "," << "time" << "," << "node max" << "," << "node final" << endl;
	//file_list.open("test.txt");
	//while (std::getline(file_list, line)) {
	//	std::cout << "file name:" << line << std::endl;
	//	start = clock();
	//	nodes = Simulate_with_partition1(path, line);
	//	finish = clock();
	//	time = (double)(finish - start) / CLOCKS_PER_SEC;
	//	std::cout << "time:" << time << std::endl;
	//	std::cout << "nodes max:" << *nodes << std::endl;
	//	std::cout << "nodes final:" << *(nodes + 1) << std::endl;
	//	ofile << line << "," << time << "," << *nodes << "," << *(nodes + 1) << endl;
	//}
	//file_list.close();
	//ofile.close();

	//ofile.open("data4.csv", ios::app);
	//ofile << "Simulate_with_partition2" << endl;
	//ofile << "benchmarks" << "," << "time" << "," << "node max" << "," << "node final" << endl;
	//file_list.open("test.txt");
	//while (std::getline(file_list, line)) {
	//	std::cout << "file name:" << line << std::endl;
	//	start = clock();
	//	nodes = Simulate_with_partition2(path, line);
	//	finish = clock();
	//	time = (double)(finish - start) / CLOCKS_PER_SEC;
	//	std::cout << "time:" << time << std::endl;
	//	std::cout << "nodes max:" << *nodes << std::endl;
	//	std::cout << "nodes final:" << *(nodes + 1) << std::endl;
	//	ofile << line << "," << time << "," << *nodes << "," << *(nodes + 1) << endl;
	//}
	//file_list.close();
	//ofile.close();

	string path = "Benchmarks/";
	string file_name = "3_17_13.qasm";

	int* nodes ;
	clock_t   start, finish;
	double time;
	std::cout << "File name:" << file_name<< std::endl;


	start = clock();
	nodes=Simulate_with_tdd(path, file_name);
	finish = clock();
	time = (double)(finish - start) / CLOCKS_PER_SEC;
	std::cout << "Time:" << time << std::endl;
	std::cout << "Nodes max:" << *nodes << std::endl;
	std::cout << "Nodes Final:" << *(nodes + 1) << std::endl;

	start = clock();
	nodes = Simulate_with_partition1(path, file_name);
	finish = clock();
	time = (double)(finish - start) / CLOCKS_PER_SEC;
	std::cout << "Time:" << time << std::endl;
	std::cout << "Nodes max:" << *nodes << std::endl;
	std::cout << "Nodes Final:" << *(nodes + 1) << std::endl;

	start = clock();
	nodes = Simulate_with_partition2(path, file_name);
	finish = clock();
	time = (double)(finish - start) / CLOCKS_PER_SEC;
	std::cout << "Time:" << time << std::endl;
	std::cout << "Nodes max:" << *nodes << std::endl;
	std::cout << "Nodes Final:" << *(nodes + 1) << std::endl;



	//std::map<std::string, int> var;
	//var["x0"] = 1;
	//var["x1"] = 2;
	//var["x2"] = 3;
	//var["y0"] = 4;
	//var["y1"] = 5;
	//var["y2"] = 6;
	//var["y3"] = 7;
	//var["-1"] = 100;
 //    auto* dd = new dd::Package(var);
	 //dd::TDD low = dd->diag_matrix_2_TDD({ {{ 1, 0 }, { 0, 0 } }, {{ 0, 0 }, { 0, 0 } } }, { { "y0",0 }, { "y0",0 } });
	 //dd->export2Dot(low.e, "low.dot", false);
	 //dd::TDD high = dd->diag_matrix_2_TDD({ {{ 0, 0 }, { 0, 0 } }, {{ 0, 0 }, { 1, 0 } } }, { { "y0",0 }, { "y0",0 } });
	 //dd->export2Dot(high.e, "high.dot", false);
	 //dd::Edge res = dd->T_add(low.e, high.e);
	 //dd->export2Dot(res, "res.dot", false);

	//dd::TDD tdd = dd->Matrix2TDD(Hmat, { { "y0",0 }, { "y1",0 } });

	////dd->export2Dot(tdd.e, "H_tdd.dot", false);


	//dd::TDD tdd2 = dd->Matrix2TDD(Hmat, { { "y2",0 }, { "y3",0 } });
	////

	//dd::TDD tdd3=dd->cont(tdd, tdd2);
	//dd->export2Dot(tdd3.e, "H_X_cont.dot", false);


	//dd::TDD cnot_tdd = dd->cnot_2_TDD({ {"x0",0},{"x0",1},{"x0",2},{"y1",0},{"y2",0} }, 3);
	//dd->export2Dot(cnot_tdd.e, "cnot_tdd.dot", false);


	//tdd= dd->cont(tdd, cnot_tdd);
	//tdd = dd->cont(tdd, tdd2);
	//dd->export2Dot(tdd.e, "cont_tdd", false);

	
	//dd->garbageCollect(true);
 //   delete dd;

	

	return 0;

}
