#pragma once
#include "test/test.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <test_function> (A, B, C)" << std::endl;
        test::test_TensorNetwork_cont();
        return 1;
    }

    std::map<std::string, std::function<void()>> testFunctions = {
        {"tdd",test::test_tdd_2_tensor},
        //{"circuit",test::test_circuit},
        {"tn",test::test_TensorNetwork_cont}
    };
    std::string input = argv[1];
    if (testFunctions.find(input) != testFunctions.end()) {
        testFunctions[input](); // Run the selected test function
    } else {
        std::cout << "Invalid input. Only support test:" << std::endl;
		for(auto fun_pair: testFunctions){
			std::cout << fun_pair.first << " ";
		}
		std::cout << std::endl;
        return 1;
    }
    return 0;
}