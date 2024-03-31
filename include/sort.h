#ifndef SORT_H
#define SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// quick sort
typedef int (*CompareFunc)(void*, void*);
int intCmp(void* a, void* b) {
	return *(int*)a - *(int*)b;
}

// !only for base type and struct
static void swap(void* a, void* b, size_t size) {
	char temp[size];
	memcpy(temp, a, size);
	memcpy(a, b, size);
	memcpy(b, temp, size);
}

static int partition(void* arr, int low, int high, size_t size, CompareFunc cmp) {
	void* pivot = (char*)arr + high * size;
	int i = low - 1;
	for (int j = low; j < high; j++) {
		if (cmp((char*)arr + j * size, pivot) < 0) {
			i++;
			swap((char*)arr + i * size, (char*)arr + j * size, size);
		}
	}
	swap((char*)arr + (i + 1) * size, pivot, size);
	return i + 1;
}

// merge sort
static void merge(void* arr, int left, int mid, int right, size_t size, CompareFunc cmp) {
	int i, j, k;
	int n1 = mid - left + 1; // 左边部分个数
	int n2 = right - mid; // 右边部分个数
    // 创建临时数组并拷贝数据过来
    char* L = (char*)malloc(n1 * size);
	char* R = (char*)malloc(n2 * size);
	for (i = 0; i < n1; i++) {
		memcpy(L + i * size, (char*)arr + (left + i) * size, size);
	}
	for (j = 0; j < n2; j++) {
		memcpy(R + j * size, (char*)arr + (mid + 1 + j) * size, size); 
	}

	// 合并数组
	i = 0;
	j = 0;
	k = left;
	while (i < n1 && j < n2) {
		if (cmp(L + i * size, R + j * size) <= 0) {
			memcpy((char*)arr + k * size, L + i * size, size);
			i++;
		} else {
			memcpy((char*)arr + k * size, R + j * size, size);
			j++;
		}
		k++;
	}
	while (i < n1) {
		memcpy((char*)arr + k * size, L + i * size, size);
		i++;
		k++;
	}
	while (j < n2) {
		memcpy((char*)arr + k * size, R + j * size, size);
		j++;
		k++;
	}
	free(L);
	free(R);
}

void quick_sort(void* arr, int low, int high, size_t size, CompareFunc cmp) {
	if (low < high) {
		int pi = partition(arr, low, high, size, cmp);
		quick_sort(arr, low, pi - 1, size, cmp);
		quick_sort(arr, pi + 1, high, size, cmp);
	}
}

void merge_sort(void* arr, int low, int high, size_t size, CompareFunc cmp) {
	if (low < high) {
		int mid = low + (high - low) / 2;
		merge_sort(arr, low, mid, size, cmp);
		merge_sort(arr, mid + 1, high, size, cmp);
		merge(arr, low, mid, high, size, cmp);
	}
}

#endif
