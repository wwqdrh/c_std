#include "unitest.h"
#include "sort.h"


UNITEST(intarr_quick_sort) {
    int arr[] = {64, 25, 12, 22, 11};
    quick_sort(arr, 0, sizeof(arr) / sizeof(arr[0]) - 1, sizeof(int), intCmp);
    unitest_assert(arr[0] == 11, "check sort");
    unitest_assert(arr[4] == 64, "check sort");
}

UNITEST(intarr_merge_sort) {
    int arr[] = {64, 25, 12, 22, 11};
    merge_sort(arr, 0, sizeof(arr) / sizeof(arr[0]) - 1, sizeof(int), intCmp);
    unitest_assert(arr[0] == 11, "check sort");
    unitest_assert(arr[4] == 64, "check sort");
}
