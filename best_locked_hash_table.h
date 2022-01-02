#ifndef _BEST_LOCKED_HASH_TABLE_H_
#define _BEST_LOCKED_HASH_TABLE_H_

#define TABLE_SIZE 1000
#define THRESHOLD 0.35
#define NULL_HT INT_MIN
#define EMPTY_HT INT_MAX

#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <climits>
#include "hash_table.h"

class best_locked_hash_table : public hash_table {

    // TODO
    int* table;
    int size;
    int mutex_size;
    int threshold;
    bool ready;

    std::atomic<int> items;
    std::mutex* mutexes;
    // TODO 
    //
    // Important!!
    // INT_MIN and INT_MAX will be used to control the hash table
    // Therefore, this implementation doesn't correctly accept INT_MIN and INT_MAX

    public:
        best_locked_hash_table(){
            // TODO
            this -> size = 256; // 2 to the n for simplicity
            this -> mutex_size = 4; // mutex_length will be constant even if the size gets large
            this -> items = 0;
            this -> table = new int[size];
            this -> mutexes = new std::mutex[mutex_size]();

            for (int i = 0; i < size; i++) {
                this -> table[i] = NULL_HT;
            }

            this -> threshold = (int) (size * THRESHOLD); // predefined as 0.35
            this -> ready = true;
            // TODO
        }

        bool contains(int key){
            // TODO

            return this -> contains_(key);
            // TODO
        }

        bool insert(int key) {
            // TODO
            // First check if items < threshold
            if (items >= threshold) {
                this -> expand();
            }

            return this -> insert_(key);
            // TODO
        }

        bool remove(int key) {
            // TODO
            return this -> remove_(key);
            // TODO
        }

    private:
        // Hash function - can be changed in the future!
        int h(int k) {
            return k % size;
        }
        // Probing function - also can be changed
        int p(int x) {
            // Linear probing
            return x;
        }

        void expand() {

            const int original_size = size;
            int* original_table = table;

            this -> size *= 2;
            this -> threshold = (int) (size * THRESHOLD);

            this -> table = new int[size];
            for (int i = 0; i < size; i++) {
                this -> table[i] = NULL_HT;
            }

            for (int i = 0; i < original_size; ++i) {
                if (original_table[i] == EMPTY_HT || original_table[i] == NULL_HT) {
                    break;
                }
                this -> insert_(original_table[i]);
            }

            delete[] original_table;
        }

        bool contains_(int key) {
            int hash_value = h(key);
            const int init_hash_value = hash_value;

            int x = 0;
            while (table[hash_value] != key) {

                if (table[hash_value] == NULL_HT || init_hash_value == hash_value) {
                    return false;
                }
                ++x;
                hash_value = (h(key) + p(x)) % size;
            }

            return true;
        }

        bool insert_(int key) {
            // Insert the key to the first NULL slot
            int hash_value = h(key);
            const int init_hash_value = hash_value;

            int x = 0;
            while (table[hash_value] != NULL_HT) {
                ++x;
                hash_value = (h(key) + p(x)) % size;
                if (init_hash_value == hash_value) {
                    return false;
                // preventing an infinite loop
                }
            }

            table[hash_value] = key;
            items++;
            return true;
        }


        bool remove_(int key) {
            int hash_value = h(key);
            const int init_hash_value = hash_value;

            int x = 0;
            while (table[hash_value] != key) {
                ++x;
                hash_value = (h(key) + p(x)) % size;
                if (init_hash_value == hash_value) {
                    return false;
                    // preventing an infinite loop
                }
            }

            table[hash_value] = EMPTY_HT; // for its performance and safety
            items--;
            return true;
        }
};

#endif
