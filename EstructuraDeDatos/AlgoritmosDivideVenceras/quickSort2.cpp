#include <iostream>
using namespace std;

// Modified partition that always chooses worst pivot (for demonstration)
int partitionWorstCase(int arr[], int low, int high) {
    // INTENTIONALLY choosing the smallest element as pivot
    // by finding min and moving it to the end
    int minIndex = low;
    for (int i = low + 1; i <= high; i++) {
        if (arr[i] < arr[minIndex]) {
            minIndex = i;
        }
    }
    swap(arr[minIndex], arr[high]); // Move smallest to pivot position
    
    // Now proceed with normal partition
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

void quickSortWorstCase(int arr[], int low, int high, int& comparisons, int& swaps) {
    if (low < high) {
        int pivotIndex = partitionWorstCase(arr, low, high);
        comparisons += (high - low);
        
        quickSortWorstCase(arr, low, pivotIndex - 1, comparisons, swaps);
        quickSortWorstCase(arr, pivotIndex + 1, high, comparisons, swaps);
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    cout << "\n=== WORST CASE SCENARIO ===" << endl;
    
    
    // Worst case: Already sorted array with bad pivot choice
    int worstCase[] = {1, 2, 3, 4, 5, 6, 7}; // Already sorted
    int n = sizeof(worstCase) / sizeof(worstCase[0]);
    
    cout << "Original array: ";
    printArray(worstCase, n);
    
    int comparisons = 0, swaps = 0;
    quickSortWorstCase(worstCase, 0, n - 1, comparisons, swaps);
    
    cout << "Sorted array: ";
    printArray(worstCase, n);
    cout << "Comparisons: " << comparisons << endl;
    
    return 0;
}
