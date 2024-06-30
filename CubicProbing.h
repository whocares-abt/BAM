#ifndef CUBICPROBING_H
#define CUBICPROBING_H

#include "BaseClass.h"
#include <iostream>

class CubicProbing : public BaseClass {
public:
    void createAccount(std::string id, int count) override;
    std::vector<int> getTopK(int k) override;
    int getBalance(std::string id) override;
    void addTransaction(std::string id, int count) override;
    bool doesExist(std::string id) override;
    bool deleteAccount(std::string id) override;
    int databaseSize() override;
    int hash(std::string id) override;
    CubicProbing();
    ~CubicProbing();
    
private:
    int capacity;
    int num_of_accounts;
    int num_of_tombstones;
    int addn_factor;
    std::vector<bool> element_array; // Tells if an element is cuurently present
    std::vector<bool> tombstone_array; // Tells if an element has been present at any time
    std::vector<int> prime_num;
    int prime_index;
    void resize(int new_size);
    // Other data members and functions specific to Quadratic Probing
};

#endif // CUBICPROBING_H
