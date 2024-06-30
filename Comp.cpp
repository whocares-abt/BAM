#include "Comp.h"

void Comp::createAccount(std::string id, int count){
    Account new_account;
    new_account.balance = count; new_account.id = id;
    int hash_val = hash(id);
    int hash_offset = hash2(id);
    int probe_val, offset = 0;
    while (true){
        probe_val = (hash_val + offset)%capacity;
        if (!element_array[probe_val]){
            bankStorage1d[probe_val] = new_account;
            element_array[probe_val] = true;
            tombstone_array[probe_val] = true;
            break;
        }
        offset = offset + hash_offset;
    }
    num_of_accounts++;
    num_of_tombstones++;

    if (num_of_tombstones >= 0.3*capacity){
        if (prime_index == 7){resize(prime_num[prime_index]); return;}
        prime_index++;
        resize(prime_num[prime_index]);
    }
}

std::vector<int> Comp::getTopK(int k) {
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

int Comp::getBalance(std::string id) {
    int hash_val = hash(id);
    int hash_offset = hash2(id);
    int probe_val = hash_val, offset = 0;
    if (!doesExist(id)){return -1;}

    while (true){
        probe_val = (hash_val + offset)%capacity;
        if (element_array[probe_val]){
            Account &curr_account = bankStorage1d[probe_val];
            if (curr_account.id == id){
                return curr_account.balance;
            }
        }
        offset += hash_offset;
    }

    return -1;
}

void Comp::addTransaction(std::string id, int count) {
    int hash_val = hash(id);
    int hash_offset = hash2(id);
    int probe_val = hash_val, offset = 0;

    if (!doesExist(id)){createAccount(id, count); return;}

    while (true){
        probe_val = (hash_val + offset)%capacity;
        if (element_array[probe_val]){
            Account &curr_account = bankStorage1d[probe_val];
            if (curr_account.id == id){
                curr_account.balance += count;
                return;
            }
        }
        offset += hash_offset;
    }
}

bool Comp::doesExist(std::string id) {
    int hash_val = hash(id);
    int hash_offset = hash2(id);
    int probe_val, offset = 0;

    while (true){
        probe_val = (hash_val + offset)%capacity;
        if (element_array[probe_val]){
            Account &curr_account = bankStorage1d[probe_val];
            if (curr_account.id == id){
                return true;
            }
        }
        if (!tombstone_array[probe_val]){return false;}
        offset += hash_offset;
    }

    return false; 
}

bool Comp::deleteAccount(std::string id) {
    int hash_val = hash(id);
    int hash_offset = hash2(id);
    int probe_val = hash_val, offset = 0;

    if (!doesExist(id)){return false;}

    while (true){
        probe_val = (hash_val + offset)%capacity;
        if (element_array[probe_val]){
            Account &curr_account = bankStorage1d[probe_val];
            if (curr_account.id == id){
                num_of_accounts--; element_array[probe_val] = false;
                return true;
            }
        }
        offset += hash_offset;
    }

    return false; 
}

int Comp::databaseSize() {
    return num_of_accounts;
}

int Comp::hash(std::string id) {
    int size = id.size();
    int hash = 0;
    for (int i = 0; i < size; i++){
        hash += (int)(id[i]);
        hash = (hash+addn_factor)*8092967;
    }
    if (hash < 0){hash = hash * -1;}
    hash = hash%8092967;
    return hash%capacity;
}

int Comp::hash2(std::string id) {
    int size = id.size();
    int hash2 = 0;
    for (int i = 0; i < size; i++){
        hash2 += (int)(id[i]);
        hash2 = (hash2+addn_factor2)*8092967;
    }
    if (hash2 < 0){hash2 = hash2 * -1;}
    hash2 = hash2%8092967;
    return (hash2%hash_floor) + 1;
}

Comp::Comp(){
    prime_num = {131, 383, 1151, 3457, 10369, 31121, 119771, 403391};
    prime_num2 = {37, 113, 281, 953, 2927, 9241, 29033, 62981};
    Account account;
    num_of_accounts = 0;
    num_of_tombstones = 0;
    prime_index = 0;
    capacity = 131;
    for (int i = 0; i < capacity; i++){
        bankStorage1d.push_back(account);
        element_array.push_back(false);
        tombstone_array.push_back(false);
    }
    addn_factor = rand()%8092967;
    addn_factor2 = rand()%8092967;
    hash_floor = 37;
}

Comp::~Comp(){}

void Comp::resize(int new_capacity){
    std::vector<bool> old_element_array(element_array);
    std::vector<Account> old_bankStorage1d(bankStorage1d);
    int old_capacity = capacity;

    Account curr_account;
    capacity = new_capacity;
    num_of_accounts = 0;
    num_of_tombstones = 0;
    hash_floor = prime_num2[prime_index];
    bankStorage1d.clear(); tombstone_array.clear(), element_array.clear();
    addn_factor = rand()%8092967;
    addn_factor2 = rand()%8092967;


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