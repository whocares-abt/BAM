#ifndef COMP_H
#define COMP_H

#include "BaseClass.h"
#include <iostream>
#include <vector>

class Comp : public BaseClass {
public:
    void createAccount(std::string id, int count) override;
    std::vector<int> getTopK(int k) override;
    int getBalance(std::string id) override;
    void addTransaction(std::string id, int count) override;
    bool doesExist(std::string id) override;
    bool deleteAccount(std::string id) override;
    int databaseSize() override;
    int hash(std::string id) override;
    int hash2(std::string id);
    Comp();
    ~Comp();
    
private:
    // Using double hashing
    int num_of_accounts;
    int num_of_tombstones;
    int capacity;
    int addn_factor;
    int addn_factor2;
    int hash_floor;
    std::vector<bool> element_array; // Tells if an element is cuurently present
    std::vector<bool> tombstone_array; // Tells if an element has been present at any time
    std::vector<int> prime_num; // Used for capacity
    std::vector<int> prime_num2; // Used for second hash function
    int prime_index;
    void resize(int new_size);
    // Other data members and functions specific to Your implementation
};

#endif // COMP_H
