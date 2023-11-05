#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

#include <dd/Tensor.hpp>
#include <dd/ComplexValue.hpp>

#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xadapt.hpp>

dd::ComplexValue string_2complex(const std::string s){
    std::cout << "---------------" << std::endl;
    std::cout << "s:" << s << std::endl;
    std::regex pattern(R"((-?\d+\.\d+)([+-])(\d+\.\d+))");
    std::smatch matches;

    if (std::regex_search(s, matches, pattern)) {
        double real = std::stod(matches[1]);
        double imag = std::stod(matches[3]);
        char sign = matches[2].str()[0];
        if (sign == '-') {
                imag = -imag;
            }
        std::cout << "real: " << real << std::endl;
        std::cout << "imag: " << imag << std::endl;
        return dd::ComplexValue(real,imag);
    }
    else{
        std::cout << "faile" << std::endl;
    }
}

int main(int argc, char *argv[]) {
    if(argc < 2 or argc > 3){
        std::cerr << "invaild input." << std::endl;
        return 1;
    }
    std::ifstream inputFile;
    inputFile.open(argv[1]);

    if (!inputFile.is_open()) {
        std::cerr << "Unable to open the file." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::vector<dd::ComplexValue> data;
        std::vector<int> shape;
        std::string temp;
        std::cout << line << std::endl;

        std::istringstream ss(line);
        std::string token;

        std::getline(ss, token, ',');
        std::cout << token << std::endl;
        std::istringstream datass(token);
        while( std::getline(datass, temp, ' ')){
            if(temp.empty()){ continue;}
            std::cout << string_2complex(temp) << std::endl;
            data.push_back(string_2complex(temp));
        }
         

        std::getline(ss, token, ',');
        std::cout << token << std::endl;
        std::istringstream shapess(token);
        while( std::getline(shapess, temp, ' ')){
            if(temp.empty()){ continue;}
            try {
                int value = std::stoi(token);
                shape.push_back(value);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid input format." << std::endl;
                return 1;
            }
            std::cout << temp << std::endl;
        }

        xt::xarray<dd::ComplexValue> res = xt::adapt(data,shape);
        std::cout << res  << std::endl;
    }

    inputFile.close();

    return 0;
}
