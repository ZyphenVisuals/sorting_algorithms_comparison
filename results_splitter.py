import csv

test_types = {
    "sequential", 
    "random_small", 
    "random_big", 
    "inverse_sequential"
};

algorithms = {
    "Bubble Sort": "bubble_sort",
    "Bubble Sort (Fast exit)": "bubble_sort_fe",
    "Selection Sort": "selection_sort",
    "Insertion Sort": "insertion_sort",
    "Merge Sort": "merge_sort",
    "Quick Sort": "quick_sort",
    "Quick Sort (Median of three)": "quick_sort_mot",
    "Counting Sort": "counting_sort",
    "Counting Sort (Stable)": "counting_sort_s",
    "Radix Sort": "radix_sort"
}



for type in test_types:
    for algo in algorithms:
        lines = [];
        # read correct lines
        with open("results.csv", "r") as in_file:
            r = csv.reader(in_file)
            for row in r:
                if row[0] == type and row[2] == algo:
                    lines.append(row)
        with open(f"results/{algorithms[algo]}_{type}.csv", "w+") as out_file:
            w = csv.writer(out_file)
            w.writerows(lines)