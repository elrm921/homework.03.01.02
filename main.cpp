#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <vector>
#include <random>

using namespace std;

void sum_func(int split, vector<int> &v1, vector<int> &v2, vector<int> &v3)
{
    int max_size = max(v1.size(), v2.size());
    int slice = (max_size / split);
    int stop = (slice * split);
    int start = (stop - slice);
    
    auto it1 = v1.begin() + start;
    auto end1 = v1.begin() + stop;
    auto it2 = v2.begin() + start;
    auto end2 = v2.begin() + stop;
    auto it3 = v3.begin() + start;

    while (it1 != end1 && it2 != end2)
    {
        int sum = 0;
        if (it1 != v1.end())
        {
            sum += *(it1);
            it1++;
        }
        if (it2 != v2.end())
        {
            sum += *(it2);
            it2++;
        }
        *(it3) = sum;
        it3++;
    }
}

void reset_vectors(int size, vector<int> &v1, vector<int> &v2, vector<int> &v3)
{
    v1.clear();
    v2.clear();
    v3.clear();
    v1 = vector<int>(size);
    v2 = vector<int>(size);
    v3 = vector<int>(max(v1.size(),v2.size()));
    mt19937 gen;
    uniform_int_distribution<int> dis(0, 1000);
    auto rand_num([=]() mutable { return dis(gen); });
    generate(begin(v1), end(v1), rand_num);
    generate(begin(v2), end(v2), rand_num);
}

void test_threads(vector<thread> &poll, int num, vector<int> &v1, vector<int> &v2, vector<int> &v3)
{
    poll.clear();
    auto start = chrono::steady_clock::now();
    for (int i=0; i<num; i++)
    {
        poll.push_back(thread(sum_func, num, ref(v1), ref(v2), ref(v3)));
    }
    for (auto &t : poll)
    {
        t.join();
    }
    auto end = chrono::steady_clock::now();
    chrono::duration<double, milli> time = end - start;
    cout << fixed << setprecision(4) << " " << time.count() << "ms" << " ";
}

int main()
{
    vector<int> v1, v2, v3;

    // hardware_concurrency
    int hardware = thread::hardware_concurrency();
    cout << "Количество аппаратных ядер: " << hardware << endl;

    // test threads
    vector<thread> poll;

    string header = (std::string(11, ' ') + "1'000" +
        std::string(5, ' ') + "10'000" +
        std::string(4, ' ') + "100'000" +
        std::string(3, ' ') + "1'000'000");
    cout << header << endl;

    // 1-threads
    cout << "1 потоков ";
    reset_vectors(1000,v1,v2,v3);
    test_threads(poll,1,v1,v2,v3);
    reset_vectors(10000,v1,v2,v3);
    test_threads(poll,1,v1,v2,v3);
    reset_vectors(100000,v1,v2,v3);
    test_threads(poll,1,v1,v2,v3);
    reset_vectors(1000000,v1,v2,v3);
    test_threads(poll,1,v1,v2,v3);
    cout << endl;

    // 2-threads
    cout << "2 потоков ";
    reset_vectors(1000,v1,v2,v3);
    test_threads(poll,2,v1,v2,v3);
    reset_vectors(10000,v1,v2,v3);
    test_threads(poll,2,v1,v2,v3);
    reset_vectors(100000,v1,v2,v3);
    test_threads(poll,2,v1,v2,v3);
    reset_vectors(1000000,v1,v2,v3);
    test_threads(poll,2,v1,v2,v3);
    cout << endl;

    // 4-threads
    cout << "4 потоков ";
    reset_vectors(1000,v1,v2,v3);
    test_threads(poll,4,v1,v2,v3);
    reset_vectors(10000,v1,v2,v3);
    test_threads(poll,4,v1,v2,v3);
    reset_vectors(100000,v1,v2,v3);
    test_threads(poll,4,v1,v2,v3);
    reset_vectors(1000000,v1,v2,v3);
    test_threads(poll,4,v1,v2,v3);
    cout << endl;

    // 8-threads
    cout << "8 потоков ";
    reset_vectors(1000,v1,v2,v3);
    test_threads(poll,8,v1,v2,v3);
    reset_vectors(10000,v1,v2,v3);
    test_threads(poll,8,v1,v2,v3);
    reset_vectors(100000,v1,v2,v3);
    test_threads(poll,8,v1,v2,v3);
    reset_vectors(1000000,v1,v2,v3);
    test_threads(poll,8,v1,v2,v3);
    cout << endl;

    // 16-threads
    cout << "16 потоков";
    reset_vectors(1000,v1,v2,v3);
    test_threads(poll,16,v1,v2,v3);
    reset_vectors(10000,v1,v2,v3);
    test_threads(poll,16,v1,v2,v3);
    reset_vectors(100000,v1,v2,v3);
    test_threads(poll,16,v1,v2,v3);
    reset_vectors(1000000,v1,v2,v3);
    test_threads(poll,16,v1,v2,v3);
    cout << endl;

    return 0;
}