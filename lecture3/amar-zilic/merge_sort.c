#include <paroot.h>
#include <stdio.h>
void merge(int arr[], int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = 0;

    int temp[right - left + 1]; 


    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }


    while (i <= mid) {
        temp[k++] = arr[i++];
    }


    while (j <= right) {
        temp[k++] = arr[j++];
    }


    for (i = left, k = 0; i <= right; i++, k++) {
        arr[i] = temp[k];
    }
}


void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;

        mergeSort(arr, left, mid);      
        mergeSort(arr, mid + 1, right);  

        merge(arr, left, mid, right); 
    }
}

int main() {
    int arr[] = {38, 27, 43, 3, 9, 82, 10};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Before sort: ");
    print_array_int(arr, n);

    mergeSort(arr, 0, n - 1);

    printf("After sort: ");
    print_array_int(arr, n);

    return 0;
}