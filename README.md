# Hash Table Implementation in C++

## Overview

This project provides a basic implementation of a hash table in C++. The hash table supports basic operations such as insertion, deletion, and searching for key-value pairs. The hash table uses red-black trees to handle collisions.

## Features

- **Insert:** Add a key-value pair to the hash table.
- **Delete:** Remove a key-value pair from the hash table.
- **Search:** Find a value associated with a given key.

## Time complexity

- **Insert:** Best case O(1), Average case O(log k), Worst case O(log n).
- **Delete:** Best case O(1), Average case O(log k), Worst case O(log n).
- **Search:** Best case O(1), Average case O(log k), Worst case O(log n).
- ***Note:*** K denotes the number of nodes in a single bucket (usually small).

## Usage

1. **Clone the repository:**

   ```bash
   git clone https://github.com/yourusername/hashtable-cpp.git
   cd hashtable-cpp
   ```

2. **Build the project:**

  ```bash
  g++ -o hashtable main.cpp hashtable.cpp
  ./hashtable
  ```

## Contributions
Contributions are welcome! Please open an issue or submit a pull request.

## License
MIT License.