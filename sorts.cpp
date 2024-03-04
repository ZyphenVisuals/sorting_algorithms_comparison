#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <format>

using namespace std;

ofstream fout("results.csv");

int step = 0;
const char *test_types[] = {"sequential", "random", "inverse_sequential"};
const int lengths[] = {1000, 10000};

void increment_process()
{
    step++;
    cout.flush();
    cout << format("{}/{}\r", step, size(test_types) * size(lengths) * 2);
    return;
}

vector<int> get_data(const char *type, int length)
{
    ifstream file("./data/" + to_string(length) + "_" + type + ".txt");
    if (!file.good())
    {
        cout << "No file found for type " << type << " and length " << length << ".\n";
        exit(-1);
    }
    // read the data
    int nr;
    vector<int> data;
    while (file >> nr)
    {
        data.push_back(nr);
    }
    return data;
}

void write_data(const char *type, int length, char *name, int64_t comparisons, int64_t swaps, int64_t time)
{
    fout << format("{},{},{},{},{},{}\n", name, to_string(length), name, to_string(comparisons), to_string(swaps), to_string(time));
}

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
    // write header
    fout << "Type,Length,Name,Comparisons,Swaps,Time\n";

    int64_t comp,
        swap, time;

    for (int t = 0; t < size(test_types); t++)
    {
        for (int l = 0; l < size(lengths); l++)
        {
            vector<int> data = get_data(test_types[t], lengths[l]);

            increment_process();
            comp = 0;
            swap = 0;
            time = 0;
            bubble_sort(data, comp, swap, time);
            write_data(test_types[t], lengths[l], "Bubble Sort", comp, swap, time);

            increment_process();
            comp = 0;
            swap = 0;
            time = 0;
            selection_sort(data, comp, swap, time);
            write_data(test_types[t], lengths[l], "Selection Sort", comp, swap, time);
        }
    }

    return 0;
}