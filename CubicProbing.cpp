#include "CubicProbing.h"

void CubicProbing::createAccount(std::string id, int count) {
    Account new_account;
    new_account.balance = count; new_account.id = id;
    int hash_val = hash(id);
    int probe_val, offset = 0;
    bool flag;

    for (int i = 0; i < capacity; i++){
        probe_val = (hash_val + (offset*offset*offset))%capacity;
        if (!element_array[probe_val]){
            bankStorage1d[probe_val] = new_account;
            element_array[probe_val] = true;
            tombstone_array[probe_val] = true;
            flag = true;
            break;
        }
        offset++;
    }

    if (not flag){
        offset = 0;
        for (int i = 0; i < capacity; i++){
            probe_val = (hash_val + offset)%capacity;
            if (!element_array[probe_val]){
                bankStorage1d[probe_val] = new_account;
                element_array[probe_val] = true;
                tombstone_array[probe_val] = true;
                break;
            }
            offset++;        
        }
    }

    num_of_accounts++;
    num_of_tombstones++;

    if (num_of_tombstones > 0.4*capacity){
        if (prime_index == 7){resize(prime_num[prime_index]); return;}
        prime_index++;
        resize(prime_num[prime_index]);
    }
}

std::vector<int> CubicProbing::getTopK(int k) {
    std::vector<int> balances_rev, balances;
    int balance;
    for (int i = 0; i < k; i++){balances_rev.push_back(-1);}
    for (int j = 0; j < capacity; j++){
        if (element_array[j]){
            Account &account = bankStorage1d[j];
            balance = account.balance;
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

int CubicProbing::getBalance(std::string id) {
    int hash_val = hash(id);
    int probe_val = hash_val, offset = 0;
    if (!doesExist(id)){return -1;}

    for (int i = 0; i < capacity; i++){
        probe_val = (hash_val + (offset*offset*offset))%capacity;
        if (element_array[probe_val]){
            Account &curr_account = bankStorage1d[probe_val];
            if (curr_account.id == id){
                return curr_account.balance;
            }
        }
        offset++;
    }

    for (int i = 0; i < capacity; i++){
        probe_val = (hash_val + (offset))%capacity;
        if (element_array[probe_val]){
            Account &curr_account = bankStorage1d[probe_val];
            if (curr_account.id == id){
                return curr_account.balance;
            }
        }
        offset++;
    }    

    return -1;
}

void CubicProbing::addTransaction(std::string id, int count) {
    int hash_val = hash(id);
    int probe_val = hash_val, offset = 0;

    if (!doesExist(id)){createAccount(id, count); return;}

    for (int i = 0; i < capacity; i++){
        probe_val = (hash_val + (offset*offset*offset))%capacity;
        if (element_array[probe_val]){
            Account &curr_account = bankStorage1d[probe_val];
            if (curr_account.id == id){
                curr_account.balance += count;
                return;
            }
        }
        offset++;
    }

    for (int i = 0; i < capacity; i++){
        probe_val = (hash_val + offset)%capacity;
        if (element_array[probe_val]){
            Account &curr_account = bankStorage1d[probe_val];
            if (curr_account.id == id){
                curr_account.balance += count;
                return;
            }
        }
        offset++;
    }
}

bool CubicProbing::doesExist(std::string id) {
    int hash_val = hash(id);
    int probe_val, offset = 0;

    for (int i = 0; i < capacity; i++){
        probe_val = (hash_val + (offset*offset*offset))%capacity;
        if (element_array[probe_val]){
            Account &curr_account = bankStorage1d[probe_val];
            if (curr_account.id == id){
                return true;
            }
        }
        if (!tombstone_array[probe_val]){return false;}
        offset++;
    }

    offset = 0;
    for (int i = 0; i < capacity; i++){
        probe_val = (hash_val + offset)%capacity;
        if (element_array[probe_val]){
            Account &curr_account = bankStorage1d[probe_val];
            if (curr_account.id == id){
                return true;
            }
        }
        if (!tombstone_array[probe_val]){return false;}
        offset++;        
    }

    return false;
}

bool CubicProbing::deleteAccount(std::string id) {
    int hash_val = hash(id);
    int probe_val = hash_val, offset = 0;

    if (!doesExist(id)){return false;}

    for (int i = 0; i < capacity; i++){
        probe_val = (hash_val + (offset*offset*offset))%capacity;
        if (element_array[probe_val]){
            Account &curr_account = bankStorage1d[probe_val];
            if (curr_account.id == id){
                num_of_accounts--; element_array[probe_val] = false;
                return true;
            }
        }
        offset++;
    }

    for (int i = 0; i < capacity; i++){
        probe_val = (hash_val + (offset))%capacity;
        if (element_array[probe_val]){
            Account &curr_account = bankStorage1d[probe_val];
            if (curr_account.id == id){
                num_of_accounts--; element_array[probe_val] = false;
                return true;
            }
        }
        offset++;
    }


    return false; 
}

int CubicProbing::databaseSize() {
    return num_of_accounts;
}

int CubicProbing::hash(std::string id) {
    int size = id.size();
    int hash = 0;
    for (int i = 0; i < size; i++){
        hash += (int)(id[i]);
        hash = (hash + addn_factor)*8092967;
    }
    if (hash < 0){hash = hash * -1;}
    hash = (hash)%8092967;
    return hash%capacity;
}

CubicProbing::CubicProbing(){
    Account account;
    num_of_accounts = 0;
    num_of_tombstones = 0;
    capacity = 131;
    prime_num = {131, 383, 1151, 3457, 10369, 31121, 119771, 403391};
    prime_index = 0;
    for (int i = 0; i < capacity; i++){
        bankStorage1d.push_back(account);
        element_array.push_back(false);
        tombstone_array.push_back(false);
    }
    addn_factor = rand()%8092967;
}

CubicProbing::~CubicProbing(){}

void CubicProbing::resize(int new_capacity){
    std::vector<bool> old_element_array(element_array);
    std::vector<Account> old_bankStorage1d(bankStorage1d);
    int old_capacity = capacity;

    Account curr_account;
    capacity = new_capacity;
    num_of_accounts = 0;
    num_of_tombstones = 0;
    bankStorage1d.clear(); tombstone_array.clear(), element_array.clear();
    addn_factor = rand()%8092967;


    for (int i = 0; i < capacity; i++){
        bankStorage1d.push_back(curr_account);
        element_array.push_back(false);
        tombstone_array.push_back(false);
    }

    for (int i = 0; i < old_capacity; i++){
        if (old_element_array[i]){
            curr_account = old_bankStorage1d[i];
            createAccount(curr_account.id, curr_account.balance);
        }
    }
}