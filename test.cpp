#include "Chaining.h"
#include "LinearProbing.h"
#include "QuadraticProbing.h"
#include "CubicProbing.h"
#include "Comp.h"
#include <cassert>
#include <string>

void testDatabase(BaseClass *db) {
    db->createAccount("Alice", 1000);
    db->createAccount("Bob", 1500);
    db->createAccount("Charlie", 2000);

    assert(db->getBalance("Alice") == 1000);
    assert(db->getBalance("Bob") == 1500);
    assert(db->getBalance("Charlie") == 2000);

    db->addTransaction("Alice", -200);
    db->addTransaction("Bob", 300);
    db->addTransaction("Charlie", -400);
    db->addTransaction("Delta", 1234);

    int vctr_size = db->bankStorage2d.size();
    int vctr_size2 = db->bankStorage1d.size();
    assert(vctr_size < 100000 && vctr_size2 < 100000);
    assert(db->getBalance("Alice") == 800);
    assert(db->getBalance("Bob") == 1800);
    assert(db->getBalance("Charlie") == 1600);
    assert(db->getBalance("Delta") == 1234);
    // std::cout << "WREG" << std::flush;

    std::vector<int> topBalances = db->getTopK(2);
    assert(topBalances.size() == 2);
    assert(topBalances[0] == 1800);
    assert(topBalances[1] == 1600);
    assert(db->databaseSize() == 4);

    assert(db->doesExist("Alice"));
    assert(!db->doesExist("Eve"));

    assert(!db->deleteAccount("EFNET"));
    assert(db->deleteAccount("Alice"));
    assert(!db->doesExist("Alice"));
    assert(db->databaseSize() == 3);

    std::cout << "WREG" << std::flush;
    for (int i = 0; i < 50000; i++){
        db->createAccount(std::to_string(i), 123);
    }

    std::cout << "WREG" << std::flush;

    for (int i = 50000; i < 100000; i++){
        db->addTransaction(std::to_string(i), 123);
    } 

    std::cout << "WREG" << std::flush;
    for (int i = 0; i < 100000; i++){
        assert(db->doesExist(std::to_string(i)));
        assert(db->getBalance(std::to_string(i)) == 123);
        db->deleteAccount(std::to_string(i));
        assert(!db->doesExist(std::to_string(i)));
    }

    for (int i = 0; i < 100000; i++){
        assert (!db->deleteAccount(std::to_string(i)));
    }

    assert(db->databaseSize() == 3);
}


int main() {
    Chaining chainingDB;
    LinearProbing linearProbingDB;
    QuadraticProbing quadraticProbingDB;
    CubicProbing cubicProbingDB;
    Comp compDB;

    std::cout << "Testing Chaining:" << std::endl;
    testDatabase(&chainingDB);

    std::cout << "\nTesting Linear Probing:" << std::endl;
    testDatabase(&linearProbingDB);
    // std::cout << linearProbingDB.num_of_collisions << std::endl;

    std::cout << "\nTesting Quadratic Probing:" << std::endl;
    testDatabase(&quadraticProbingDB);
    // std::cout << quadraticProbingDB.num_of_collisions << std::endl;

    std::cout << "\nTesting Cubic Probing:" << std::endl;
    testDatabase(&cubicProbingDB);

    std::cout << "\nTesting Comp:" << std::endl;
    testDatabase(&compDB);
    return 0;
}
