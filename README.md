# B-Tree Implementation in C++

## Overview

This C++ project implements a B-Tree, a self-balancing tree data structure that maintains sorted data in a way that allows searches, sequential access, insertions, and deletions in logarithmic time. The branching factor of the B-Tree can be adjusted according to the requirements. This implementation provides a simple console interface to interact with the B-Tree, including operations such as adding and deleting nodes, and viewing the tree in different ways.

## Features

- **Dynamic Branching Factor**: The B-Tree branching factor (`t`) is adjustable at runtime.
- **Node Operations**: Insertion and deletion of nodes. Deletion is marked as TODO and needs to be implemented.
- **Search**: Find the minimum and maximum values, find predecessors and successors (marked as TODO).
- **Traversal**: Show tree content using inorder traversal and display the tree structure in an indented format.
- **Exit**: Safely exit the application.

## How to Use

1. Compile the C++ files in your preferred development environment.
2. Run the executable. You will be prompted to enter the desired branching factor `t` for the B-Tree.
3. Follow the on-screen instructions to interact with the B-Tree using the numbered options.

## Operations Menu

- `1. Add nodes`: Insert multiple nodes into the B-Tree by entering their keys.
- `2. Delete node (TODO)`: Delete a node by its key. This feature needs implementation.
- `3. Get minimum node`: Find and display the minimum key in the B-Tree.
- `4. Get maximum node`: Find and display the maximum key in the B-Tree.
- `5. Get predecessor (TODO)`: Find the predecessor of a given key. This feature needs implementation.
- `6. Get successor (TODO)`: Find the successor of a given key. This feature needs implementation.
- `7. Show tree`: Display the B-Tree structure in an indented format.
- `8. Show tree content (inorder traversal)`: Perform an inorder traversal of the B-Tree and display the content.
- `9. Exit`: Exit the application.

## Building the Project

Ensure you have a C++ compiler installed and configured. Use the following command to compile the project (example given for g++ compiler):

```bash
g++ -o btree main.cpp BTree.h
