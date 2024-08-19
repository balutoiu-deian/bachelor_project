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
	return start;
    }
    else
        return stop - start;

}

void sort(int arr[], int n){
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
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

    sort(arr, n);                                    // sort the array    
                                  
    stop = rdtscp();                                 // get the clock count from after the sort

    printf("%ld", handleOverflow(start, stop));

    writeArrayToFile(outputFilename, arr, n);        // write the sorted array to the output file

    return 0;               

}
