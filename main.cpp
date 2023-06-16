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

using namespace std;

// global variables
atomic<int> counter(2);
vector<vector<int>> primesPerThread;
int maxNumber = 0;
int numThreads = 0;

// function declarations
void findPrimes(int threadId);
void printPrimes(const vector<int>& primes);
void printTime(chrono::high_resolution_clock::time_point start, chrono::high_resolution_clock::time_point stop);

int main(int argc, char *argv[])
{
    // get user input
    maxNumber = pow(10, 6);
    //Obtener el numero de hilos desde los argumentos de la linea de comandos
    numThreads = atoi(argv[1]);

    //Imprimir el numero de hilos y el numero maximo a calcular
    cout << "Threads: " << numThreads << endl;
    cout << "Max Number: " << maxNumber << endl;

    // start timer
    auto start = chrono::high_resolution_clock::now();

    // create threads
    vector<thread> threads;
    primesPerThread.resize(numThreads);
    for (int i = 0; i < numThreads; i++)
    {
        threads.push_back(thread([i](){ findPrimes(i); }));
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
    int limit = 0;

    while (true)
    {
        // get next number to test
        i = counter.fetch_add(1);
        if (i > maxNumber)
        {
            break;
        }

        // check if number is prime
        if (i == 1)
        {
            // 1 is not prime
            continue;
        }
        if (i == 2)
        {
            // 2 is prime
            primesPerThread[threadId].push_back(i);
            continue;
        }
        if (i % 2 == 0)
        {
            // even numbers are not prime
            continue;
        }
        prime = 1;
        limit = sqrt(i);
        for (j = 3; j <= limit; j += 2)
        {
            if (i % j == 0)
            {
                prime = 0;
                break;
            }
        }

        // add prime to vector
        if (prime == 1)
        {
            primesPerThread[threadId].push_back(i);
        }
    }
}

void printPrimes(const vector<int>& primes)
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

