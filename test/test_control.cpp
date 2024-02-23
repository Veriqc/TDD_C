#include <xtensor/xarray.hpp>
#include <xtensor/xview.hpp>
#include <xtensor/xio.hpp>
#include <cmath>
#include <vector>

// Function to simulate splitting and reshaping
xt::xarray<int> reshapeViaSplit(const xt::xarray<int>& arr, int n) {
    // Base case: If n is 1, the array is already at the desired shape
    if (n == 1) {
        return arr;
    }

    // Calculate the size of the sub-arrays after splitting
    size_t newSize = 1<< (n - 1);

    // Placeholder for the result of the splits, initialized for demonstration
    std::vector<xt::xarray<int>> splits;

    // Simulate horizontal and vertical splits
    // For demonstration purposes, we'll just use views to represent splits
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            auto split = xt::view(arr, xt::range(i * newSize, (i + 1) * newSize), xt::range(j * newSize, (j + 1) * newSize));
            splits.push_back(split);
        }
    }

    // Recursively apply the function to each split and combine them
    xt::xarray<int> combined = xt::xarray<int>::from_shape({2, 2, newSize, newSize});
    int index = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            auto reshapedSplit = reshapeViaSplit(splits[index++], n - 1);
            // Assign reshapedSplit to the correct position in combined
            // This requires proper indexing or reshaping to fit into combined
            // For demonstration, assuming reshapedSplit can be directly assigned
            xt::view(combined, i, j) = reshapedSplit;
        }
    }

    return combined;
}

// Example usage
int main() {
    // Example: Initialize a 4x4 array to demonstrate the concept
    xt::xarray<int> original = xt::arange(64).reshape({8, 8});
    int n = 2; // For a 4x4 array, n=2 since 2^2 * 2^2 = 4 * 4

    auto reshaped = reshapeViaSplit(original, n);
    std::cout << reshaped << std::endl;

    return 0;
}
