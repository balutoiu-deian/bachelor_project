#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void readNumbersFromFile(const char *filename, int arr[], int n) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < n; i++) {
        if (fscanf(file, "%d", &arr[i]) != 1) {
            fprintf(stderr, "Error reading number from file\n");
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
}

void writeArrayToFile(const char *filename, int arr[], int n) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        fprintf(file, "%d", arr[i]);
        if (i != n - 1) {
            fprintf(file, ",");
        }
    }

    fclose(file);
}

static inline uint64_t rdtscp(){
    uint64_t lo, hi;

    asm volatile ("rdtscp" : "=a"(lo), "=d"(hi) :: "rcx");     // read the current processor clock count
    return ((uint64_t)hi << 32) | lo;
}

uint64_t handleOverflow(uint64_t start, uint64_t stop){
    uint64_t elapsed;
    if (stop < start)
    {
	stop += UINT64_MAX - start;
    }
    else
        return stop - start;

}

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void sort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        sort(arr, low, pi - 1);
        sort(arr, pi + 1, high);
    }
}

int main(int argc, char** argv) {
    const char *inputFilename = "../input_array.txt";
    const char *outputFilename = "../output_array.txt";
    uint64_t start = 0, stop = 0;
    const int n = atoi(argv[1]); 
    int arr[n];

    readNumbersFromFile(inputFilename, arr, n);      // read n numbers from the input file to the array

    start = rdtscp();                                // get the clock count form before sort

    sort(arr, 0, n - 1);                             // sort the array    
                                  
    stop = rdtscp();                                 // get the clock count from after the sort
    
    printf("%ld", handleOverflow(start, stop));

    writeArrayToFile(outputFilename, arr, n);        // write the sorted array to the output file

    return 0;               

}
