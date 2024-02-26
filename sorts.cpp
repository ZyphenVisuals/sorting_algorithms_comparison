#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;

int64_t selection_sort(vector<int> data)
{
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < data.size() - 1; i++)
    {
        for (int j = i; j < data.size(); j++)
        {
            if (data[j] < data[i])
            {
                swap(data[i], data[j]);
            }
        }
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    return duration.count();
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

    int64_t time;
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
    time = selection_sort(seq_data);
    cout << "Selection sort: " << time << "\n";

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
    time = selection_sort(rand_data);
    cout << "Selection sort: " << time << "\n";

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
    time = selection_sort(rev_seq_data);
    cout << "Selection sort: " << time << "\n";

    return 0;
}