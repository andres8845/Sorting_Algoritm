#pragma once
#include <functional>
#include <vector>
using ProgressCallback = std::function<void(int)>;
void insertionSort(std::vector<int>&, const ProgressCallback&);
void selectionSort(std::vector<int>&, const ProgressCallback&);
void bubbleSort(std::vector<int>&, const ProgressCallback&);
void mergeSort(std::vector<int>&, const ProgressCallback&);
void heapSort(std::vector<int>&, const ProgressCallback&);
void quickSort(std::vector<int>&, const ProgressCallback&);
