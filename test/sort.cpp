#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

struct Index {
    std::string key;
    short idx;
};

// Custom comparator function for sorting Index objects
struct IndexComparator {
    const std::map<std::string, int>& varOrder;

    IndexComparator(const std::map<std::string, int>& varOrder) : varOrder(varOrder) { }

    bool operator()(const Index& a, const Index& b) const {
        // If a key is not found in varOrder, assign it the maximum value
        int a_val = varOrder.at(a.key);
        int b_val = varOrder.at(b.key);

        return a_val < b_val;
    }
};

int main() {
    std::map<std::string, int> varOrder = {
        {"key1", 3},
        {"key2", 1},
        {"key3", 2}
    };

    std::vector<Index> indexVector = {
        {"key1", 10},
        {"key2", 20},
        {"key3", 30},
        {"key4", 40}
    };
    std::cout << "varOrder:" << std::endl;
    for (const auto& entry : varOrder) {
        std::cout << "Key: " << entry.first << ", Value: " << entry.second << std::endl;
    }

    // Update varOrder with missing keys
    for (const Index& index : indexVector) {
        if (varOrder.find(index.key) == varOrder.end()) {
            varOrder[index.key] = varOrder.empty() ? 0 : std::max_element(varOrder.begin(), varOrder.end(),
                [](const auto& a, const auto& b) { return a.second < b.second; })->second + 1;
        }
    }

    // Sort the vector using the custom comparator
    std::sort(indexVector.begin(), indexVector.end(), IndexComparator(varOrder));

    // Print the sorted vector and updated varOrder
    std::cout << "Sorted vector" << std::endl;
    for (const Index& index : indexVector) {
        std::cout << "Key: " << index.key << ", Index: " << index.idx << std::endl;
    }

    std::cout << "Updated varOrder:" << std::endl;
    for (const auto& entry : varOrder) {
        std::cout << "Key: " << entry.first << ", Value: " << entry.second << std::endl;
    }

    return 0;
}