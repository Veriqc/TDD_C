#include <iostream>
#include "Cir_tn.h"
extern "C"{
    int gate_num(std::string path){
        return get_gates_num(path);
    }
    void infor(){
        std::cout <<"hello" ;
    }
    void error(){
        throw std::runtime_error("test") ;
    }
}