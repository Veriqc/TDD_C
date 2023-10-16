#ifndef C8FAF390_DFC3_4832_9CED_297E251943C5
#define C8FAF390_DFC3_4832_9CED_297E251943C5


#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>
#include <xtensor/xarray.hpp>
#include "Tdd.hpp"

namespace dd {

    struct Tensor {

        xt::xarray<ComplexValue> data;

        std::vector<Index> index_set;

        std::string name=NULL;

    };


    struct TensorNetwork {
        std::vector<Tensor> tensors;
    };


}


#endif /* C8FAF390_DFC3_4832_9CED_297E251943C5 */
