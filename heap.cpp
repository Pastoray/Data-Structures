#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <cmath>

void max_heapify(std::vector<int>& heap, int i, size_t heap_size)
{
  int l = i * 2;
  int r = i * 2 + 1;
  int largest = i;

  if(l < heap_size && heap[l] > heap[largest])
    largest = l;

  if(r < heap_size && heap[r] > heap[largest])
    largest = r;
  
  if(largest != i)
  {
    std::swap(heap[i], heap[largest]);
    max_heapify(heap, largest, heap_size);
  }
}

void min_heapify(std::vector<int>& heap, int i, size_t heap_size)
{
  int l = i * 2;
  int r = i * 2 + 1;
  int smallest = i;

  if(l < heap_size && heap[l] < heap[smallest])
    smallest = l;

  if(r < heap_size && heap[r] < heap[smallest])
    smallest = r;
  
  if(smallest != i)
  {
    std::swap(heap[i], heap[smallest]);
    min_heapify(heap, smallest, heap_size);
  }
}

void build_max_heap(std::vector<int>& arr)
{
  size_t heap_size = arr.size();
  for(int i = heap_size / 2 - 1; i > 0; i--)
    max_heapify(arr, i, heap_size);
}

void build_min_heap(std::vector<int>& arr)
{
  size_t heap_size = arr.size();
  for(int i = heap_size / 2 - 1; i > 0; i--)
    min_heapify(arr, i, heap_size);
}

void heap_sort(std::vector<int>& arr, bool reverse = false)
{
  build_max_heap(arr);
  size_t heap_size = arr.size();
  for (int i = heap_size - 1; i > 0; i--)
  {
    std::swap(arr[0], arr[i]);
    heap_size--;
    max_heapify(arr, 0, heap_size);
  }
  if(reverse)
    std::reverse(arr.begin(), arr.end());
}

void heap_insert(std::vector<int>& arr, int val)
{
  arr.push_back(val);
  int i = arr.size() - 1;
  while(i > 0)
  {
    int parent = (i - 1) / 2;
    if(arr[i] > arr[parent])
    {
      std::swap(arr[i], arr[parent]);
      i = parent;
    }
    else
      break;
  }
}

int max_heap_pop(std::vector<int>& arr)
{
  if(arr.empty())
    throw std::runtime_error("the heap is empty");
  int root = arr[0];
  arr[0] = arr.back();
  arr.pop_back();
  if(!arr.empty())
    max_heapify(arr, 0, arr.size());
  return root;
}

int min_heap_pop(std::vector<int>& arr)
{
  if(arr.empty())
    throw std::runtime_error("the heap is empty");
  int root = arr[0];
  arr[0] = arr.back();
  arr.pop_back();
  if(!arr.empty())
    min_heapify(arr, 0, arr.size());
  return root;
}

int heap_nlargest(std::vector<int>& arr, size_t n)
{
    if (n <= 0 || n > arr.size())
        throw std::invalid_argument("Invalid value for n");

    int result;
    std::vector<int> maxHeap = arr;
    build_max_heap(maxHeap);

    for (int i = 0; i < n; i++) {
      int largest = max_heap_pop(maxHeap);
      if(i == n - 1)
        result = largest;
    }
    return result;
}

int heap_nsmallest(std::vector<int>& arr, size_t n)
{
  if (n <= 0 || n > arr.size())
    throw std::invalid_argument("Invalid value for n");

  int result;
  std::vector<int> minHeap = arr;
  build_min_heap(minHeap);

  for (int i = 0; i < n; i++) {
    int smallest = max_heap_pop(minHeap);
    if(i == n - 1)
      result = smallest;
  }
  return result;
}

std::vector<int> max_heap_merge(std::vector<int>& arr1, std::vector<int>& arr2)
{
  std::vector<int> result = arr1;
  result.insert(result.end(), arr2.begin(), arr2.end());
  build_max_heap(result);
  return result;
}

std::vector<int> min_heap_merge(std::vector<int>& arr1, std::vector<int>& arr2)
{
  std::vector<int> result = arr1;
  result.insert(result.end(), arr2.begin(), arr2.end());
  build_min_heap(result);
  return result;
}
