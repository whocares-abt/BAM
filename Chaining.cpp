#include "Chaining.h"

void Chaining::createAccount(std::string id, int count) {
    Account new_account;
    new_account.balance = count; new_account.id = id;
    int hash_val = hash(id);
    bankStorage2d[hash_val].push_back(new_account);
    num_of_accounts++;
    if (num_of_accounts > 0.4*capacity){resize(3*capacity);}
}

std::vector<int> Chaining::getTopK(int k) {
    std::vector<int> balances_rev, balances;
    int balance;
    for (int i = 0; i < k; i++){balances_rev.push_back(-1);}
    for (int j = 0; j < capacity; j++){
        std::vector<Account> &Accounts = bankStorage2d[j];
        int length = Accounts.size();
        for (int account_num = 0; account_num < length; account_num++){
            balance = Accounts[account_num].balance;
            for (int bal_num = 0; bal_num < k; bal_num++){
                if (balances_rev[bal_num] < balance){
                    if (bal_num == k - 1){balances_rev[bal_num] = balance; break;}
                    if (balances_rev[bal_num+1] < balance){
                        balances_rev[bal_num] = balances_rev[bal_num+1];
                    }
                    else{
                        balances_rev[bal_num] = balance; break;
                    }
                }
            }
        }
    }

    int limit = (k > num_of_accounts) ? num_of_accounts: k;
    for (int i = 0; i < limit; i++){
        balances.push_back(balances_rev[k-i-1]);
    }

    return balances;
}

int Chaining::getBalance(std::string id) {
    int hash_val = hash(id);
    std::vector<Account> &Accounts = bankStorage2d[hash_val];
    int num_accounts = Accounts.size();

    for (int i = 0; i < num_accounts; i++){
        if (Accounts[i].id == id){
            return Accounts[i].balance;
        }
    }
    return -1;
}

void Chaining::addTransaction(std::string id, int count) {
    int hash_val = hash(id);
    std::vector<Account> &Accounts = bankStorage2d[hash_val];
    int num_accounts = Accounts.size();

    for (int i = 0; i < num_accounts; i++){
        if (Accounts[i].id == id){
            Accounts[i].balance += count;
            return;
        }
    }
    createAccount(id, count);
}

bool Chaining::doesExist(std::string id) {
    int hash_val = hash(id);
    std::vector<Account> &Accounts = bankStorage2d[hash_val];
    int num_accounts = Accounts.size();

    for (int i = 0; i < num_accounts; i++){
        if (Accounts[i].id == id){return true;}
    }
    return false;
}

bool Chaining::deleteAccount(std::string id) {
    int hash_val = hash(id);
    std::vector<Account> &Accounts = bankStorage2d[hash_val];
    int num_accounts = Accounts.size();

    for (int i = 0; i < num_accounts; i++){
        if (Accounts[i].id == id){
            Accounts.erase(Accounts.begin() + i);
            num_of_accounts--;
            return true;
        }
    }
    return false;
}

int Chaining::databaseSize() {
    return num_of_accounts;
}

int Chaining::hash(std::string id) {
    int size = id.size();
    int hash = 0;
    for (int i = 0; i < size; i++){
        hash += (int)(id[i]);
        hash = (hash + addn_factor)*8092967;
    }
    if (hash < 0){hash = hash * -1;}
    hash = hash%8092967;
    return hash%capacity;
}

Chaining::Chaining(){
    srand(time(0));
    num_of_accounts = 0;
    capacity = 128;
    for (int i = 0; i < capacity; i++){
        std::vector<Account> vctr;
        bankStorage2d.push_back(vctr);
    }
    addn_factor = rand()%8092967;
}

Chaining::~Chaining(){}

void Chaining::resize(int new_size){
    int old_capacity = capacity;
    std::vector<std::vector<Account>> oldbankStorage2d(bankStorage2d);
    Account curr_account;

    std::vector<Account> vctr;
    capacity = new_size;
    num_of_accounts = 0;
    bankStorage2d.clear();
    addn_factor = rand()%8092967;

    for (int i = 0; i < capacity; i++){
        bankStorage2d.push_back(vctr);
    }

    for (int i = 0; i < old_capacity; i++){
        std::vector<Account> &Accounts = oldbankStorage2d[i];
        int length = Accounts.size();
        for (int j = 0; j < length; j++){
            curr_account = Accounts[j];
            createAccount(curr_account.id, curr_account.balance);
        }
    }
}