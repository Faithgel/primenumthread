//parallel prime number calculator using sieve of eratosthenes and threads and mutex
// Name: Angelo Orellana
// Date: 8/06/2023
// Sources: http://www.cplusplus.com/reference/atomic/atomic/atomic/
//          http://www.cplusplus.com/reference/thread/thread/
//          http://www.cplusplus.com/reference/vector/vector/
//          http://www.cplusplus.com/reference/chrono/high_resolution_clock/now/
//          http://www.cplusplus.com/reference/chrono/duration/duration_cast/
//          http://www.cplusplus.com/reference/chrono/microseconds/

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <math.h>
#include <algorithm>
#include <bitset>

using namespace std;

// global variables
atomic<int> counter(2);
vector<vector<int>> primesPerThread;
int maxNumber = 0;
int numThreads = 0;

// function declarations
void findPrimes(int threadId);
void printPrimes(const vector<int> &primes);
void printTime(chrono::high_resolution_clock::time_point start, chrono::high_resolution_clock::time_point stop);

int main(int argc, char *argv[])
{
    // get user input
    cout << "Ingresa el número hasta el cual quieres encontrar primos: ";
    cin >> maxNumber;
    cout << "Ingresa el número de hilos: ";
    cin >> numThreads;

    // start timer
    auto start = chrono::high_resolution_clock::now();

    // create threads
    vector<thread> threads;
    primesPerThread.resize(numThreads);
    for (int i = 0; i < numThreads; i++)
    {
        threads.push_back(thread([i] { findPrimes(i); }));
    }

    // join threads
    for (auto &th : threads)
    {
        th.join();
    }

    // combine prime vectors
    vector<int> primes;
    for (auto &primeThread : primesPerThread)
    {
        primes.insert(primes.end(), primeThread.begin(), primeThread.end());
    }

    // stop timer
    auto stop = chrono::high_resolution_clock::now();

    // print results
    printPrimes(primes);
    printTime(start, stop);

    return 0;
}

// function definitions
void findPrimes(int threadId)
{
    int i = 0;
    int j = 0;
    int prime = 0;
    int limit = sqrt(maxNumber);

    // create bitset to mark composite numbers
    bitset<1000000> isComposite;
    isComposite.reset();

    while (true)
    {
        // get next number to test
        i = counter.fetch_add(1);
        if (i > limit)
        {
            break;
        }

        // check if number is prime
        if (isComposite[i])
        {
            continue;
        }

        prime = 1;
        for (j = i * i; j <= maxNumber; j += i)
        {
            isComposite[j] = 1;
        }

        // add prime to vector
        primesPerThread[threadId].push_back(i);
    }
}

void printPrimes(const vector<int> &primes)
{
    cout << "Primes: ";
    for (auto &prime : primes)
    {
        cout << prime << " ";
    }
    cout << endl;
}

void printTime(chrono::high_resolution_clock::time_point start, chrono::high_resolution_clock::time_point stop)
{
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time: " << duration.count() << " microseconds" << endl;
}

