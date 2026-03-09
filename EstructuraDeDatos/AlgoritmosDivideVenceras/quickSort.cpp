#include <iostream>
#include <stdlib.h>
using namespace std;

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high, int& comparisons, int& swaps) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        comparisons += (high - low); // Count comparisons in partition
        
        quickSort(arr, low, pivotIndex - 1, comparisons, swaps);
        quickSort(arr, pivotIndex + 1, high, comparisons, swaps);
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    cout << "=== BEST CASE SCENARIO ===" << endl;
   
    // Best case: Already sorted array with good pivot choice
    // For demonstration, using an array that will create balanced partitions
    int bestCase[] = {4, 2, 6, 1, 3, 5, 7};
    int n = sizeof(bestCase) / sizeof(bestCase[0]);
    
    cout << "Original array: ";
    printArray(bestCase, n);
    
    int comparisons = 0, swaps = 0;
    quickSort(bestCase, 0, n - 1, comparisons, swaps);
    
    cout << "Sorted array: ";
    printArray(bestCase, n);
    cout << "Comparisons: " << comparisons << endl;
    system("pause");
    return 0;
}
