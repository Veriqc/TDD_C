//
// Created by stefan on 04.12.19.
//

#ifndef TDD_PACKAGE_UTIL_H
#define TDD_PACKAGE_UTIL_H

#include "TDDpackage.h"
#include <string>
#include <cassert>

// X gate matrix
constexpr dd::Matrix2x2 Xmat = {{{ 0, 0 }, { 1, 0 } }, {{ 1, 0 }, { 0, 0 } }};
// Hadamard gate matrix
constexpr dd::Matrix2x2 Hmat = {{{ dd::SQRT_2, 0 }, { dd::SQRT_2,  0 }},
                                {{ dd::SQRT_2, 0 }, { -dd::SQRT_2, 0 }}};


constexpr dd::Matrix2x2 Imat = { {{ 1, 0 }, { 0, 0 } }, {{ 0, 0 }, { 1, 0 } } };

#endif //DD_PACKAGE_UTIL_H
