#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

#include <dd/Tensor.hpp>
#include <dd/Tdd.hpp>
#include <dd/ComplexValue.hpp>

#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xadapt.hpp>

dd::ComplexValue string_2complex(const std::string s){
    std::regex pattern(R"((-?\d+\.\d+)([+-])(\d+\.\d+))");
    std::smatch matches;

    if (std::regex_search(s, matches, pattern)) {
        double real = std::stod(matches[1]);
        double imag = std::stod(matches[3]);
        char sign = matches[2].str()[0];
        if (sign == '-') {
                imag = -imag;
            }
        return dd::ComplexValue(real,imag);
    }
    else{
        std::cerr << "faile" << std::endl;
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
    std::getline(inputFile, line);
    while (std::getline(inputFile, line)) {
        std::vector<dd::ComplexValue> data;
        std::vector<int> shape;
        std::vector<dd::Index> index_set;
        std::string temp;

        std::istringstream ss(line);
        std::string token;

        std::getline(ss, token, ',');
        std::istringstream datass(token);
        while( std::getline(datass, temp, ' ')){
            if(temp.compare(" ")==0 or temp.empty()){ continue;}
            data.push_back(string_2complex(temp));
        }
         

        std::getline(ss, token, ',');
        std::istringstream shapess(token);
        while( std::getline(shapess, temp, ' ')){
            if(temp.compare(" ")==0 or temp.empty()){ continue;}
            try {
                int value = std::stoi(token);
                shape.push_back(value);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid input format." << std::endl;
                return 1;
            }
        }

        xt::xarray<dd::ComplexValue> res = xt::adapt(data,shape);
        std::cout << res  << std::endl;

        std::getline(ss, token, ',');
        std::istringstream index_setss(token);
        while(std::getline(index_setss, temp, ' ')){
            if(temp.compare(" ")==0 or temp.empty()){ continue;}

            std::istringstream index_partss(temp);
            std::vector<std::string> parts;
            std::string part;
            while (std::getline(index_partss, part, ':')) {
                parts.push_back(part);
            }
            if (parts.size() == 2) {
                std::string firstPart = parts[0];
                int secondPart;
                
                // Convert the second part to an integer
                try {
                    secondPart = std::stoi(parts[1]);
                    index_set.push_back(dd::Index(firstPart,secondPart));
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid integer in the second part: " << e.what() << std::endl;
                    return 1;
                }
            } else {
                continue;
            }
        }
        for(auto index_temp: index_set){
            std::cout << "index key: " << index_temp.key << " index idx: " << index_temp.idx << std::endl;
        }
        dd::Tensor tn = dd::Tensor(res,index_set);
    }

    inputFile.close();

    return 0;
}
