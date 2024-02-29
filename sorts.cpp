#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;

void bubble_sort(vector<int> data, int64_t &comparisons, int64_t &swaps, int64_t &time)
{
    swaps = 0;
    comparisons = 0;
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < data.size(); i++)
    {
        for (int j = 0; j < data.size() - i - 1; j++)
        {
            comparisons++;
            if (data[j] > data[j + 1])
            {
                swaps++;
                swap(data[j + 1], data[j]);
            }
        }
    }
    auto stop = chrono::high_resolution_clock::now();
    time = chrono::duration_cast<chrono::microseconds>(stop - start).count();
    return;
}

void selection_sort(vector<int> data, int64_t &comparisons, int64_t &swaps, int64_t &time)
{
    swaps = 0;
    comparisons = 0;
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < data.size() - 1; i++)
    {
        int min = i;
        for (int j = i + 1; j < data.size(); j++)
        {
            comparisons++;
            if (data[j] < data[min])
            {
                min = j;
            }
        }
        if (min != i)
        {
            swap(data[i], data[min]);
            swaps++;
        }
    }
    auto stop = chrono::high_resolution_clock::now();
    time = chrono::duration_cast<chrono::microseconds>(stop - start).count();
    return;
}

int main(int argc, char **argv)
{
    // check the correct amount of arguments
    if (argc != 2)
    {
        cout << "Command takes exactly one parameter (length).";
        return -1;
    }
    int length = atoi(argv[1]); // read length argument

    int64_t c, s, t;
    int nr;

    // sequential tests
    cout << "---Sequential tests---\n";
    // try to open the file
    ifstream file_seq("./data/" + to_string(length) + "_sequential.txt");
    if (!file_seq.good())
    {
        cout << "Sequential file not found. Aborting.\n";
        return -1;
    }
    // read the data
    vector<int> seq_data;
    while (file_seq >> nr)
    {
        seq_data.push_back(nr);
    }
    bubble_sort(seq_data, c, s, t);
    cout << "Bubble sort: " << c << " comparissons, " << s << " swaps, " << t << " microseconds\n";
    selection_sort(seq_data, c, s, t);
    cout << "Selection sort: " << c << " comparissons, " << s << " swaps, " << t << " microseconds\n";

    // random tests
    cout << "\n---Random tests---\n";
    // try to open the file
    ifstream file_rand("./data/" + to_string(length) + "_random.txt");
    if (!file_rand.good())
    {
        cout << "Random file not found. Aborting.\n";
        return -1;
    }
    // read the data
    vector<int> rand_data;
    while (file_rand >> nr)
    {
        rand_data.push_back(nr);
    }
    bubble_sort(rand_data, c, s, t);
    cout << "Bubble sort: " << c << " comparissons, " << s << " swaps, " << t << " microseconds\n";
    selection_sort(rand_data, c, s, t);
    cout << "Selection sort: " << c << " comparissons, " << s << " swaps, " << t << " microseconds\n";

    // reverse sequential tests
    cout << "\n---Reverse sequential tests---\n";
    // try to open the file
    ifstream file_rev_seq("./data/" + to_string(length) + "_reverse_sequential.txt");
    if (!file_rev_seq.good())
    {
        cout << "Sequential file not found. Aborting.\n";
        return -1;
    }
    // read the data
    vector<int> rev_seq_data;
    while (file_rev_seq >> nr)
    {
        rev_seq_data.push_back(nr);
    }
    bubble_sort(rev_seq_data, c, s, t);
    cout << "Bubble sort: " << c << " comparissons, " << s << " swaps, " << t << " microseconds\n";
    selection_sort(rev_seq_data, c, s, t);
    cout << "Selection sort: " << c << " comparissons, " << s << " swaps, " << t << " microseconds\n";

    return 0;
}