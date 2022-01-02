#ifndef _BETTER_LOCKED_HASH_TABLE_H_
#define _BETTER_LOCKED_HASH_TABLE_H_

#define TABLE_SIZE 1000

#include <iostream>
#include <mutex>
#include <thread>
#include "hash_table.h"
#include "linked_list.h"

class better_locked_hash_table : public hash_table {

    // TODO
    linked_list* table;
    std::mutex* mutexes;
    const int factor = 4;
    // TODO

public:
    better_locked_hash_table(){
        // TODO
        this -> table = new linked_list[TABLE_SIZE]();
        this -> mutexes = new std::mutex[TABLE_SIZE / factor]();
        // TODO
    }

    bool contains(int key){
        // TODO
        int hash_value = key % TABLE_SIZE;
        int lock_value = hash_value / factor;
        std::lock_guard<std::mutex> guard(mutexes[lock_value]);
        return this -> table[hash_value].contains(key);
        // TODO
    }


    bool insert(int key) {
        // TODO
        int hash_value = key % TABLE_SIZE;
        int lock_value = hash_value / factor;
        std::lock_guard<std::mutex> guard(mutexes[lock_value]);
        return this -> table[hash_value].insert(key);
        // TODO
    }

    bool remove(int key) {
        // TODO
        int hash_value = key % TABLE_SIZE;
        int lock_value = hash_value / factor;
        std::lock_guard<std::mutex> guard(mutexes[lock_value]);
        return this -> table[hash_value].remove(key);
        // TODO
    }
};