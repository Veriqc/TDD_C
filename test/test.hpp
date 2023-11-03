#ifndef TEST_HPP
#define TEST_HPP
#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <ctime>


#include "../include/dd/Export.hpp"
#include <xtensor/xio.hpp>
#include <xtensor/xarray.hpp>
#include <Cir_tn.hpp>
namespace test {
	void test_tdd_2_tensor();

	void test_circuit();

	void test_TensorNetwork_cont();

}
#endif