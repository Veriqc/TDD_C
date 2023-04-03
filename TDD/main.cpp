#include "TDDpackage.h"
#include "Cir_import.h"
#include <iostream>
#include <fstream>
#include <array>
#include <string> 
#include <regex>


int main() {

	//std::map<std::string, int> var;
	//var["x0"] = 1;
	//var["x1"] = 2;
	//var["x2"] = 3;
	//var["y0"] = 4;
	//var["y1"] = 5;
	//var["y2"] = 6;
	//var["y3"] = 7;
	//var["-1"] = 100;
 //   auto* dd = new dd::Package(var);

 //  dd::TDD tdd = dd->Matrix2TDD(Hmat, { { "y0",0 }, { "y1",0 } });

 //  dd->export2Dot(tdd.e, "H.dot", false);


 //  dd::TDD tdd2 = dd->Matrix2TDD(Hmat, { { "y1",0 }, { "y2",0 } });

 //  dd::TDD tdd3=dd->cont(tdd, tdd2);

 //  std::map<int, std::string>::iterator pos2;
 //  std::cout << "-------------------" << std::endl;
 //  for (pos2 = tdd3.key_2_index.begin(); pos2 != tdd3.key_2_index.end(); pos2++) {
	//   std::cout << pos2->first << pos2->second << std::endl;
 //  }
 //  std::map<std::string,int>::iterator pos;
 //  std::cout << "-------------------" << std::endl;
 //  for (pos = tdd3.index_2_key.begin(); pos != tdd3.index_2_key.end(); pos++) {
	//   std::cout << pos->first << pos->second << std::endl;
 //  }

 //  std::cout << "-------------------" << std::endl;

 //  dd->export2Dot(tdd3.e, "H_X_cont.dot", false);


 //  dd->garbageCollect(true);
 //  delete dd;



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

	//start = clock();
	//nodes = Simulate_with_partition1(path, file_name);
	//finish = clock();
	//time = (double)(finish - start) / CLOCKS_PER_SEC;
	//std::cout << "Time:" << time << std::endl;
	//std::cout << "Nodes max:" << *nodes << std::endl;
	//std::cout << "Nodes Final:" << *(nodes + 1) << std::endl;

	//start = clock();
	//nodes = Simulate_with_partition2(path, file_name);
	//finish = clock();
	//time = (double)(finish - start) / CLOCKS_PER_SEC;
	//std::cout << "Time:" << time << std::endl;
	//std::cout << "Nodes max:" << *nodes << std::endl;
	//std::cout << "Nodes Final:" << *(nodes + 1) << std::endl;


	return 0;

}
