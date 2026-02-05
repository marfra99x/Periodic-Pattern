#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <chrono>

using namespace std;

struct Pattern
{
    int s;        // start index (0-based)
    int e;        // end index (0-based, inclusive)
    long long dP; // reference period
};

// Placeholder purity check (replace with your real one)
static bool isPure(const vector<long long> &L,
                   int s, int e,
                   long long dP,
                   int /*k*/, long long /*Delta*/)
{
    long long left = L[s] - dP;
    long long right = L[e] + dP;

    // check left extension
    if (binary_search(L.begin(), L.end(), left))
        return false;

    // check right extension
    if (binary_search(L.begin(), L.end(), right))
        return false;

    return true;
}

vector<Pattern> detectApproxPureConsecutiveMaximal(const vector<long long> &L,
                                                   int k, long long Delta)
{
    int n = (int)L.size();
    vector<Pattern> out;
    if (n < 3)
        return out;

    int s = 0;
    vector<int> errors;
    long long dP = L[1] - L[0];

    auto emitIfValidAndPure = [&](int sIdx, int eIdx, long long period)
    {
        if (eIdx - sIdx >= 2)
        {
            if (isPure(L, sIdx, eIdx, period, k, Delta))
            {
                out.push_back({sIdx, eIdx, period});
            }
        }
    };

    for (int i = 1; i < n; ++i)
    {
        long long g = L[i] - L[i - 1];

        if (llabs(g - dP) <= Delta)
        {
            if (g != dP)
            {
                errors.push_back(i);

                if ((int)errors.size() > k)
                {
                    emitIfValidAndPure(s, i - 1, dP);

                    s = errors.front();
                    if (s + 1 < n)
                        dP = L[s + 1] - L[s];

                    errors.clear();
                }
            }
        }
        else
        {
            emitIfValidAndPure(s, i - 1, dP);

            if (!errors.empty())
            {
                s = errors.front();
                if (s + 1 < n)
                    dP = L[s + 1] - L[s];
            }
            else
            {
                s = i - 1;
                dP = g;
            }

            errors.clear();
        }
    }

    emitIfValidAndPure(s, n - 1, dP);
    return out;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " input_file\n";
        return 1;
    }

    ifstream fin(argv[1]);
    if (!fin)
    {
        cerr << "Error: cannot open file " << argv[1] << endl;
        return 1;
    }

    vector<long long> L;
    long long x;
    while (fin >> x)
    {
        L.push_back(x);
    }
    fin.close();

    if (L.size() < 3)
    {
        cerr << "Input sequence must contain at least 3 elements\n";
        return 1;
    }

    int k = 1;
    long long Delta = 1;

    using clock = std::chrono::steady_clock;

    auto t_start = clock::now();

    auto patterns = detectApproxPureConsecutiveMaximal(L, k, Delta);

    auto t_end = clock::now();

    auto elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count();

    cout << "Running time: " << elapsed_ms << " ms" << endl;

    cout << "Detected " << patterns.size() << " patterns\n";

    /*
    for (const auto &p : patterns)
    {
        cout << "Pattern { ";
        for (int i = p.s; i <= p.e; ++i)
        {
            cout << L[i] << " ";
        }
        cout << "}  dP=" << p.dP << endl;
    }
    */

    return 0;
}