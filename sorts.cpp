#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <format>

using namespace std;

ofstream fout("results.csv");

int step = 0;
const char *test_types[] = {"sequential", "random_small", "random_big", "inverse_sequential"};
const int lengths[] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
const int nr_algos = 7;

void print_array(vector<int> &data)
{
    cout << "\n";
    for (int i = 0; i < data.size(); i++)
    {
        cout << data[i] << " ";
    }
    cout << "\n";
    return;
}

void increment_process()
{
    step++;
    // cout.flush();
    cout << format("\r{}/{}", step, size(test_types) * size(lengths) * nr_algos);
    return;
}

void check(vector<int> &data, char *type)
{
    for (int i = 1; i < data.size(); i++)
    {
        if (data[i] < data[i - 1])
        {
            cout << "\nError detected at algorithm " << type << "\n";
            exit(-1);
        }
    }
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
    fout << format("{},{},{},{},{},{}\n", type, to_string(length), name, to_string(comparisons), to_string(swaps), to_string(time));
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
    check(data, "Bubble sort");
    return;
}

void bubble_sort_optimised(vector<int> data, int64_t &comparisons, int64_t &swaps, int64_t &time)
{
    swaps = 0;
    comparisons = 0;
    bool sorted;
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < data.size(); i++)
    {
        sorted = true;
        for (int j = 0; j < data.size() - i - 1; j++)
        {
            comparisons++;
            if (data[j] > data[j + 1])
            {
                sorted = false;
                swaps++;
                swap(data[j + 1], data[j]);
            }
        }
        if (sorted)
            break;
    }
    auto stop = chrono::high_resolution_clock::now();
    time = chrono::duration_cast<chrono::microseconds>(stop - start).count();
    check(data, "Bubble sort (Optimised)");
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
    check(data, "Selection sort");
    return;
}

void insertion_sort(vector<int> data, int64_t &comparisons, int64_t &swaps, int64_t &time)
{
    swaps = 0;
    comparisons = 0;
    int aux, j;
    auto start = chrono::high_resolution_clock::now();
    for (int i = 1; i < data.size(); i++)
    {
        aux = data[i];
        j = i - 1;
        comparisons++;
        while (j >= 0 && data[j] > aux)
        {
            comparisons++;
            // TODO: these ain't swaps
            swaps++;
            data[j + 1] = data[j];
            j--;
        }
        swaps++;
        data[j + 1] = aux;
    }
    auto stop = chrono::high_resolution_clock::now();
    time = chrono::duration_cast<chrono::microseconds>(stop - start).count();
    check(data, "Insertion sort");
    return;
}

void merge(vector<int> &data, int64_t &comparisons, int64_t &swaps, int start, int mid, int end)
{
    vector<int> vl(mid - start + 1);
    vector<int> vr(end - mid);

    // populatng vectors
    for (int i = 0; i < vl.size(); i++)
    {
        swaps++;
        vl[i] = data[start + i];
    }
    for (int i = 0; i < vr.size(); i++)
    {
        swaps++;
        vr[i] = data[mid + i + 1];
    }

    // merge the vectors back into the data vector
    int li = 0, ri = 0, i = start;
    while (li < vl.size() && ri < vr.size())
    {
        comparisons++;
        if (vl[li] <= vr[ri])
        {
            swaps++;
            data[i] = vl[li];
            li++;
        }
        else
        {
            swaps++;
            data[i] = vr[ri];
            ri++;
        }
        i++;
    }

    // finish consuming temporary vectors
    for (int x = li; x < vl.size(); x++)
    {
        swaps++;
        data[i] = vl[x];
        i++;
    }
    for (int x = ri; x < vr.size(); x++)
    {
        swaps++;
        data[i] = vr[x];
        i++;
    }

    return;
}

void merge_sort(vector<int> &data, int64_t &comparisons, int64_t &swaps, int start, int end)
{
    // base case
    comparisons++;
    if (start >= end)
        return;

    int mid = (start + end) / 2;

    merge_sort(data, comparisons, swaps, start, mid);
    merge_sort(data, comparisons, swaps, mid + 1, end);
    merge(data, comparisons, swaps, start, mid, end);
}

void merge_sort_wrapper(vector<int> data, int64_t &comparisons, int64_t &swaps, int64_t &time)
{
    swaps = 0;
    comparisons = 0;
    auto start = chrono::high_resolution_clock::now();
    int begin = 0;
    int end = data.size() - 1;
    merge_sort(data, comparisons, swaps, begin, end);
    auto stop = chrono::high_resolution_clock::now();
    time = chrono::duration_cast<chrono::microseconds>(stop - start).count();
    check(data, "Merge sort");
    return;
}

void quick_sort(vector<int> &data, int64_t &comparisons, int64_t &swaps, int start, int end)
{
    if (end <= start)
        return;
    int pivot = end; // using final element as pivot
    // place pivot in the correct spot
    int i = start - 1;
    for (int j = start; j < end; j++)
    {
        comparisons++;
        if (data[j] < data[pivot])
        {
            i++;
            swaps++;
            swap(data[i], data[j]);
        }
    }
    i++;
    swaps++;
    swap(data[i], data[pivot]);
    // recursively sort the left and right arrays
    quick_sort(data, comparisons, swaps, start, i - 1);
    quick_sort(data, comparisons, swaps, i + 1, end);
    return;
}

void quick_sort_wrapper(vector<int> data, int64_t &comparisons, int64_t &swaps, int64_t &time)
{
    swaps = 0;
    comparisons = 0;
    auto start = chrono::high_resolution_clock::now();
    int begin = 0;
    int end = data.size() - 1;
    quick_sort(data, comparisons, swaps, begin, end);
    auto stop = chrono::high_resolution_clock::now();
    time = chrono::duration_cast<chrono::microseconds>(stop - start).count();
    check(data, "Quick sort");
    return;
}

void quick_sort_optimised(vector<int> &data, int64_t &comparisons, int64_t &swaps, int start, int end)
{
    if (end <= start)
        return;
    // median of three
    int mid = (start + end) / 2;
    if ((data[mid] > data[start]) ^ (data[mid] > data[end]))
    {
        swaps++;
        swap(data[mid], data[end]);
    }
    else if ((data[start] > data[mid]) ^ (data[start] > data[end]))
    {
        swaps++;
        swap(data[start], data[end]);
    }
    int pivot = end; // using final element as pivot
    // place pivot in the correct spot
    int i = start - 1;
    for (int j = start; j < end; j++)
    {
        comparisons++;
        if (data[j] < data[pivot])
        {
            i++;
            swaps++;
            swap(data[i], data[j]);
        }
    }
    i++;
    swaps++;
    swap(data[i], data[pivot]);
    // recursively sort the left and right arrays
    quick_sort_optimised(data, comparisons, swaps, start, i - 1);
    quick_sort_optimised(data, comparisons, swaps, i + 1, end);
    return;
}

void quick_sort_optimised_wrapper(vector<int> data, int64_t &comparisons, int64_t &swaps, int64_t &time)
{
    swaps = 0;
    comparisons = 0;
    auto start = chrono::high_resolution_clock::now();
    int begin = 0;
    int end = data.size() - 1;
    quick_sort_optimised(data, comparisons, swaps, begin, end);
    auto stop = chrono::high_resolution_clock::now();
    time = chrono::duration_cast<chrono::microseconds>(stop - start).count();
    check(data, "Quick sort");
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
            bubble_sort(data, comp, swap, time);
            write_data(test_types[t], lengths[l], "Bubble Sort", comp, swap, time);

            increment_process();
            bubble_sort_optimised(data, comp, swap, time);
            write_data(test_types[t], lengths[l], "Bubble Sort (Fast exit)", comp, swap, time);

            increment_process();
            selection_sort(data, comp, swap, time);
            write_data(test_types[t], lengths[l], "Selection Sort", comp, swap, time);

            increment_process();
            insertion_sort(data, comp, swap, time);
            write_data(test_types[t], lengths[l], "Insertion Sort", comp, swap, time);

            increment_process();
            merge_sort_wrapper(data, comp, swap, time);
            write_data(test_types[t], lengths[l], "Merge Sort", comp, swap, time);

            increment_process();
            quick_sort_wrapper(data, comp, swap, time);
            write_data(test_types[t], lengths[l], "Quick Sort", comp, swap, time);

            increment_process();
            quick_sort_optimised_wrapper(data, comp, swap, time);
            write_data(test_types[t], lengths[l], "Quick Sort (Median of three)", comp, swap, time);
        }
    }

    return 0;
}