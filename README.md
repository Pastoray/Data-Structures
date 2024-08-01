# Data Structures Implementation in C++ (Hashtable + Heap)

This project provides basic implementations of a hash table and a heap in C++. The hash table supports basic operations such as insertion, deletion, and searching for key-value pairs, and uses red-black trees to handle collisions. The heap implementation includes max-heap and min-heap operations, as well as heap-based sorting and merging.

**_Note:_** This project used a C-style hash table in C++ on purpose. Although classes could have been used, a C-style approach was preferred for its simplicity. This may be revisited in the future to use C++ classes. The heap implementation includes both max-heap and min-heap functionalities, including operations like heap insertion, heap pop, and heap sort.

## Features

### Hash Table

- **Insert:** Add a key-value pair to the hash table.
- **Delete:** Remove a key-value pair from the hash table.
- **Search:** Find a value associated with a given key.

### Heap

- **Insert:** Add an element to the heap.
- **Pop:** Remove and return the root element of the heap.
- **Sort:** Sort elements using heap sort.
- **Merge:** Merge two heaps into one.
- **Find N Largest:** Find the top N largest elements from a min-heap.

## Time Complexity

### Hash Table

- **Insert:** Best case O(1), Average case O(log k), Worst case O(log n).
- **Delete:** Best case O(1), Average case O(log k), Worst case O(log n).
- **Search:** Best case O(1), Average case O(log k), Worst case O(log n).
- **_Note:_** K denotes the number of nodes in a single bucket (usually small).

### Heap

- **Insert:** O(log n)
- **Pop:** O(log n)
- **Sort:** O(n log n)
- **Merge:** O(n + m) where n and m are the sizes of the two heaps.

## Usage

1. **Clone the repository:**

   ```bash
   git clone https://github.com/yourusername/data-structures.git
   cd data-structures

   ```

2. **Build the project:**

- **hashtable:**

```bash
g++ -o hashtable hashtable.cpp
./hashtable
```

2. **Build the heap:**

```bash
g++ -o heap heap.cpp
./heap
```

## Contributions

Contributions are welcome! Please open an issue or submit a pull request.

## License

MIT License.
