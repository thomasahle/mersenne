#include <vector>
#include <chrono>
#include <iostream>
#include <x86intrin.h>

#include "framework/randomgen.h"
#include "framework/fast_hashing.h"

using namespace std;


void print128_num(__m128i var) {
    int64_t v64val[2];
    memcpy(v64val, &var, sizeof(v64val));
    printf("%ld %ld\n", v64val[1], v64val[0]);
}


template<typename T>
uint64_t test_speed_function64(uint32_t nr_trials, uint32_t nr_times, vector<uint64_t>& numbers) {
    volatile __uint128_t x;

    vector<uint64_t> times;

    for (uint32_t i = 0; i < nr_times; ++i) {
        T hash;
        hash.init();

        auto start = chrono::high_resolution_clock::now();
        for (uint32_t j = 0; j < nr_trials; ++j) {
            x = hash(numbers[j]);
        }
        auto end = chrono::high_resolution_clock::now();
        times.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count());
    }

    long double mean = 0;
    for (uint32_t i = 0; i < nr_times; ++i) {
        mean += times[i];
    }
    mean = mean/nr_times;

    long double MSE = 0;
    for (uint32_t i = 0; i < nr_times; ++i) {
        MSE += (times[i] - mean)*(times[i] - mean);
    }
    MSE = MSE/nr_times;

    cout << "Mean: " << mean << " MSE: " << MSE << endl;

    return (uint64_t)mean; //chrono::duration_cast<chrono::nanoseconds>(end - start).count();
}

void test_speed64(uint32_t nr_trials, uint32_t nr_times) {
    polytwo_64 rng;
    rng.init();

    vector<uint64_t> numbers;
    for(int i = 0; i < nr_trials; ++i) {
        numbers.push_back(rng(i));
    }

    cout << "64-bit keys to 64-bit output:" << endl;
    cout << "PolyTwo: ";
    test_speed_function64<polytwo_64>(nr_trials, nr_times, numbers);
    cout << "CarrylessTwo: ";
    test_speed_function64<carrylesstwo_64>(nr_trials, nr_times, numbers);
    cout << "MultShift: ";
    test_speed_function64<multishift_64>(nr_trials, nr_times, numbers);
    cout << endl;

    cout << "PolyFour: ";
    test_speed_function64<polyfour_64>(nr_trials, nr_times, numbers);
    cout << "CarrylessFour: ";
    test_speed_function64<carrylessfour_64>(nr_trials, nr_times, numbers);
    cout << endl;

    cout << "PolyEight: ";
    test_speed_function64<polyeight_64>(nr_trials, nr_times, numbers);
    cout << "CarrylessEight: ";
    test_speed_function64<carrylesseight_64>(nr_trials, nr_times, numbers);
}

template<typename T>
uint64_t test_speed_function32(uint32_t nr_trials, uint32_t nr_times, vector<uint32_t>& numbers) {
    volatile __uint128_t x;

    vector<uint64_t> times;

    for (uint32_t i = 0; i < nr_times; ++i) {
        T hash;
        hash.init();

        auto start = chrono::high_resolution_clock::now();
        for (uint32_t j = 0; j < nr_trials; ++j) {
            x = hash(numbers[j]);
        }
        auto end = chrono::high_resolution_clock::now();
        times.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count());
    }

    long double mean = 0;
    for (uint32_t i = 0; i < nr_times; ++i) {
        mean += times[i];
    }
    mean = mean/nr_times;

    long double MSE = 0;
    for (uint32_t i = 0; i < nr_times; ++i) {
        MSE += (times[i] - mean)*(times[i] - mean);
    }
    MSE = MSE/nr_times;

    cout << "Mean: " << mean << " MSE: " << MSE << endl;

    return (uint64_t)mean; //chrono::duration_cast<chrono::nanoseconds>(end - start).count();
}

void test_speed32(uint32_t nr_trials, uint32_t nr_times) {
    polytwo_32 rng;
    rng.init();

    vector<uint32_t> numbers;
    for(int i = 0; i < nr_trials; ++i) {
        numbers.push_back(rng(i));
    }

    cout << "32-bit keys to 32-bit output:" << endl;
    cout << "PolyTwo: ";
    test_speed_function32<polytwo_32>(nr_trials, nr_times, numbers);
    cout << "CarrylessTwo: ";
    test_speed_function32<carrylesstwo_32>(nr_trials, nr_times, numbers);
    cout << "MultShift: ";
    test_speed_function32<multishift_32>(nr_trials, nr_times, numbers);
    cout << endl;

    cout << "PolyFour: ";
    test_speed_function32<polyfour_32>(nr_trials, nr_times, numbers);
    cout << "CarrylessFour: ";
    test_speed_function32<carrylessfour_32>(nr_trials, nr_times, numbers);
    cout << endl;

    cout << "PolyEight: ";
    test_speed_function32<polyeight_32>(nr_trials, nr_times, numbers);
    cout << "CarrylessEight: ";
    test_speed_function32<carrylesseight_32>(nr_trials, nr_times, numbers);
}


int main() {
    init_randomness();

    // cout << "32 bits:" << endl;
    test_speed32(1e7, 50);
    cout << endl << endl;
    // cout << endl << "64 bits:" << endl;
    test_speed64(1e7, 50);

#ifdef DEBUG
    cout << "Random bytes used: " << usedBytes << endl;
#endif

    return 0;
}
