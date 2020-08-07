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
uint64_t test_speed_function(uint32_t nr_trials, uint32_t nr_times, vector<uint64_t>& numbers) {
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

void test_speed(uint32_t nr_trials, uint32_t nr_times) {
    ofstream output("dist/speed.out");

    polytwo_64 rng;
    rng.init();

    vector<uint64_t> numbers;
    for(int i = 0; i < nr_trials; ++i) {
        numbers.push_back(rng(i));
    }

    output << endl << "64-bit keys to 64-bit output:" << endl;
    output << "PolyTwo: " << test_speed_function<polytwo_64>(nr_trials, nr_times, numbers) << endl;
    output << "CarrylessTwo: " << test_speed_function<carrylesstwo_64>(nr_trials, nr_times, numbers) << endl;
    output << "PolyTwo_exact: " << test_speed_function<polytwo_64_exact>(nr_trials, nr_times, numbers) << endl;

    output << "PolyFour: " << test_speed_function<polyfour_64>(nr_trials, nr_times, numbers) << endl;
    output << "CarrylessFour: " << test_speed_function<carrylessfour_64>(nr_trials, nr_times, numbers) << endl;
    output << "PolyFour_exact: " << test_speed_function<polyfour_64_exact>(nr_trials, nr_times, numbers) << endl;

    output.close();
}

int main() {
    init_randomness();


    test_speed(10000000, 50);

#ifdef DEBUG
    cout << "Random bytes used: " << usedBytes << endl;
#endif

    return 0;
}
