#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <time.h>

#include "dd/Tensor.hpp"
#include "dd/Tdd.hpp"
#include "dd/ComplexValue.hpp"
#include "dd/DDpackageConfig.hpp"
#include "dd/Export.hpp"

#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xadapt.hpp>


dd::ComplexValue string_2_complex(const std::string& s){
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
        std::cerr << "false here " << s << std::endl;
        throw std::runtime_error("invalid format during string_2_complex");
    }
}

std::vector<dd::ComplexValue> string_2_complex_vector(const std::string& s){
    std::istringstream ss(s);
    std::string temp;
    std::vector<dd::ComplexValue> res;
    while(std::getline(ss, temp, ' ')){
        if(temp.compare(" ")==0 or temp.empty()){ continue;}
        try{
            res.push_back(string_2_complex(temp));
        }
        catch(std::runtime_error){
            continue;
        }
    }
    return res;
}

std::vector<int> string_2_shape (const std::string& s){
    std::istringstream ss(s);
    std::string temp;
    std::vector<int> shape ;

    while(std::getline(ss, temp, ' ')){
        if(temp.compare(" ")==0 or temp.empty()){ continue;}
        try {
            int value = std::stoi(temp);
            shape.push_back(value);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid input format." << std::endl;
            throw std::invalid_argument("Invalid integer in shape");
        }
    }
    return shape;
} 

dd::Index string_2_index (const std::string& s){
    std::istringstream ss(s);
    std::vector<std::string> parts;
    std::string part;

    while (std::getline(ss, part, ':')) {
        parts.push_back(part);
    }
    if (parts.size() == 2) {
        std::string firstPart = parts[0];
        int secondPart;
        
        // Convert the second part to an integer
        try {
            secondPart = std::stoi(parts[1]);
            return dd::Index(firstPart,secondPart);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid integer in the second part: " << e.what() << std::endl;
            throw std::invalid_argument("Invalid integer in index set");
        }
    }
    else{
        std::cerr << "parts size < 2" << std::endl;
        throw std::runtime_error("invalid format");
    }
}

std::vector<dd::Index> string_2_index_set(const std::string& s){
    std::istringstream ss(s);
    std::string temp;

    std::vector<dd::Index> index_set ;
    while(std::getline(ss, temp, ' ')){
        try{
            index_set.push_back(string_2_index(temp));
        }
        catch (const std::runtime_error){
            std::cerr << "catch error" << std::endl;
            continue;
        }
    }
    return index_set;
}

dd::Tensor string_2_tensor(const std::string& s){
    std::vector<dd::ComplexValue> array;
    std::vector<int> shape;
    std::vector<dd::Index> index_set;

    std::string temp;

    std::istringstream ss(s);
    std::string token;

    std::getline(ss, token, ',');
    array = string_2_complex_vector(token);

    std::getline(ss, token, ',');
    shape = string_2_shape(token);

    std::getline(ss, token, ',');
    index_set = string_2_index_set(token);

    xt::xarray<dd::ComplexValue> data = xt::adapt(array,shape);
    return dd::Tensor(data,index_set);
}

void image(dd::TensorNetwork tn, std::unique_ptr<dd::Package<>>& ddpackage){
    tn.cont(ddpackage.get());
    std::cout << "finish image function" << std::endl; 
}



int main(int argc, char** argv){
    // todo: function choose
    if(argc != 3){
        std::cerr << "wrong argument" << std::endl;
        return 1;
    }
    std::string line;
    dd::TensorNetwork tn;

    std::ifstream tensorsfile(argv[1]);
    if (!tensorsfile.is_open()) {
        std::cerr << "Error: Unable to open tensorsfile " << argv[1] << std::endl;
        return 1; // Return an error code
    }
    std::getline(tensorsfile, line);
    while (std::getline(tensorsfile, line)) {
        dd::Tensor ts = string_2_tensor(line);
        tn.add_ts(ts);
    }
    tn.infor();
    
    
    std::ifstream varOrderfile(argv[2]);
    if (!tensorsfile.is_open()) {
        std::cerr << "Error: Unable to open varOrderfile " << argv[1] << std::endl;
        return 1; // Return an error code
    }

    int qubit_num;
    std::getline(varOrderfile,line);
    try{
        qubit_num = std::stoi(line);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << argv[2] << std::endl;
        std::cerr << line << std::endl;
        std::cerr << "Invalid input format." << std::endl;
        throw std::invalid_argument("Invalid integer about qubit_num");
        return 1;
    }
    auto dd = std::make_unique<dd::Package<>>(qubit_num);
    int order = 0;
    while (std::getline(varOrderfile,line)){
        if(line.empty() or line.compare(" ")==0) {continue;}
        dd->varOrder[line] = order ;
        order ++;
    }
    // std::cerr << argv[3] << std::endl;
    // std::string fun_name(argv[3]);
    // if(fun_name.compare("image") == 0){
    //     std::cerr << "hi" << std::endl;
    //     return 1;
    // }
    std::cout << "begin:" << std::endl;
    std::clock_t start = clock();
    dd::TDD tdd = tn.cont(dd.get());
    std::cout << "time: " << double(clock()-start)/CLOCKS_PER_SEC << "s" << std::endl;
    return 0;
}