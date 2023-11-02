#ifndef xgate
#define xgate
#include "Complex.hpp"
#include <xtensor/xarray.hpp>

namespace xgate{
	double SQRT2_2 = 0.707106781186547524400844362104849039284835937688474036588L;
	dd::ComplexValue complex_one = {1., 0.};
	dd::ComplexValue complex_mone = {-1., 0.};
	dd::ComplexValue complex_zero = {0., 0.};
	dd::ComplexValue complex_i = {0., 1.};
	dd::ComplexValue complex_mi = {0., -1.};
	dd::ComplexValue complex_SQRT2_2 = {SQRT2_2, 0.};
	dd::ComplexValue complex_mSQRT2_2 = {-SQRT2_2, 0.};
	dd::ComplexValue complex_iSQRT2_2 = {0., SQRT2_2};
	dd::ComplexValue complex_miSQRT2_2 = {0., -SQRT2_2};
	dd::ComplexValue complex_1plusi = {SQRT2_2, SQRT2_2};
	dd::ComplexValue complex_1minusi = {SQRT2_2, -SQRT2_2};
	dd::ComplexValue complex_1plusi_2 = {0.5, 0.5};
	dd::ComplexValue complex_1minusi_2 = {0.5, -0.5};

	xt::xarray<dd::ComplexValue> Imat{
		{complex_one, complex_zero},
		{complex_zero, complex_one}
		};
	xt::xarray<dd::ComplexValue> Hmat{
		{complex_SQRT2_2, complex_SQRT2_2},
		{complex_SQRT2_2, complex_mSQRT2_2}
		};
	xt::xarray<dd::ComplexValue> Xmat{
		{complex_zero, complex_one},
		{complex_one, complex_zero}
		};
	xt::xarray<dd::ComplexValue> Ymat{
		{complex_zero, complex_mi},
		{complex_i, complex_zero}
		};
	xt::xarray<dd::ComplexValue> Zmat{
		{complex_one, complex_zero},
		{complex_zero,complex_mone}
		};
	xt::xarray<dd::ComplexValue> Smat{
		{complex_one, complex_zero},
		{complex_zero, complex_i}
		};
	xt::xarray<dd::ComplexValue> Sdagmat{
		{complex_one, complex_zero},
		{complex_zero,complex_mi}
		};
	xt::xarray<dd::ComplexValue> Tmat{
		{complex_one, complex_zero},
		{complex_zero,complex_1plusi}
		};
	xt::xarray<dd::ComplexValue> Tdagmat{
		{complex_one, complex_zero},
		{complex_zero,complex_1minusi}
		};
	xt::xarray<dd::ComplexValue> SXmat{
        {complex_1plusi_2, complex_1minusi_2},
        {complex_1minusi_2, complex_1plusi_2}
        };
	xt::xarray<dd::ComplexValue> SXdagmat{
        {complex_1minusi_2, complex_1plusi_2},
        {complex_1plusi_2, complex_1minusi_2}
        };
	xt::xarray<dd::ComplexValue> Vmat{
        {complex_SQRT2_2, complex_miSQRT2_2},
        {complex_miSQRT2_2,complex_SQRT2_2}
        };
	xt::xarray<dd::ComplexValue> Vdagmat{
        {complex_SQRT2_2, complex_iSQRT2_2},
        {complex_iSQRT2_2, complex_SQRT2_2}
        };
	xt::xarray<dd::ComplexValue> CXmat{
        {
            {
                {complex_one, complex_zero},
                {complex_zero, complex_one}
            },
            {
                {complex_zero, complex_zero},
                {complex_zero, complex_zero}
            }
        },
        {
            {
                {complex_zero, complex_zero},
                {complex_zero, complex_zero}
            },
            {
                {complex_zero, complex_one},
                {complex_one, complex_zero}
            }
        }
    };

	xt::xarray<dd::ComplexValue> U3mat(double lambda, double phi, double theta) {
	return xt::xarray<dd::ComplexValue>{
            {
                {std::cos(theta / 2.),
                0.},
                {-std::cos(lambda) * std::sin(theta / 2.),
                -std::sin(lambda) * std::sin(theta / 2.)}
            },
            {
                {std::cos(phi) * std::sin(theta / 2.),
                std::sin(phi) * std::sin(theta / 2.)},
                {std::cos(lambda + phi) * std::cos(theta / 2.),
                std::sin(lambda + phi) * std::cos(theta / 2.)}
            }
        };
	}

	xt::xarray<dd::ComplexValue> Phasemat(double lambda) {
	return xt::xarray<dd::ComplexValue>{
            {complex_one,complex_zero},
            {
                complex_zero,
                {std::cos(lambda),
                std::sin(lambda)
                }
            }
        };
	}
}

#endif
