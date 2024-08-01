#include <iostream>
#include <string.h>

#define CAPACITY 10000

enum Color : unsigned char
{
  BLACK,
  RED
};

struct Ht_item
{
  char* key;
  char* value;
};

struct RBTreeNode
{
  Ht_item* item;
  RBTreeNode* parent;
  RBTreeNode* left;
  RBTreeNode* right;
  Color color;
};

struct HashTable
{
  RBTreeNode** buckets;
  size_t size;
  size_t count;
};


// Temporary hash function
int hash(char* str)
{
  int index = 0;
  for(int i = 0; str[i]; i++)
    index += str[i];

  return index % CAPACITY;
}

// Include the table and index just in case we update the root
void left_rotate(HashTable* table, int index, RBTreeNode* node)
{
  RBTreeNode* rightChild = node->right;
  rightChild->parent = node->parent;
  node->right = rightChild->left;
  if(node->right != NULL)
    node->right->parent = node;
  if (node->parent == NULL)
    table->buckets[index] = rightChild;
  else if (node == node->parent->left)
      node->parent->left = rightChild;
  else
      node->parent->right = rightChild;
  rightChild->left = node;
  node->parent = rightChild;
}

// Include the table and index just in case we update the root
void right_rotate(HashTable* table, int index, RBTreeNode* node)
{
  RBTreeNode* leftChild = node->left;
  leftChild->parent = node->parent;
  node->left = leftChild->right;
  if(node->left != NULL)
    node->left->parent = node;
  if(node->parent == NULL)
    table->buckets[index] = leftChild;
  else if(node->parent->left == node)
    node->parent->left = leftChild;
  else
    node->parent->right = leftChild;
  leftChild->right = node;
  node->parent = leftChild;
}

void fixup(HashTable* table, int index, RBTreeNode* node)
{
  if(node->parent == NULL) // Root
    node->color = BLACK;
    return;
  if(node->parent->color == BLACK) // Nothing vioalated
    return;

  RBTreeNode* parent = node->parent;
  RBTreeNode* grandpa = node->parent->parent;
  RBTreeNode* uncle = NULL;
  bool isParentLeft = node->parent == grandpa->left;
  if(isParentLeft)
    uncle = grandpa->right;
  else
    uncle = grandpa->left;
  
  if(parent->color == RED && uncle != NULL && uncle->color == RED) // Uncle and parent are red
  {
    parent->color = BLACK;
    uncle->color = BLACK;
    grandpa->color = RED;
    fixup(table, index, grandpa);
  }
  else // Parent is red and uncle is black or null
  {
    if(parent->right == node && grandpa->left == parent) // The new node is a right child, and the parent is a left child
    {
      left_rotate(table, index, parent);
      node = parent;
      parent = node->parent;
    }
    else if(parent->left == node && grandpa->left == parent) // The new node is a left child, and the parent is a left child
    {
      right_rotate(table, index, grandpa);
      Color grandpaColor = grandpa->color;
      grandpa->color = parent->color;
      parent->color = grandpaColor;
      return;
    }
    else if(parent->left == node && grandpa->right == parent) // The new node is a left child, and the parent is a right child
    {
      right_rotate(table, index, parent);
      node = parent;
      parent = node->parent;
    }
    else if(parent->right == node && grandpa->right == parent) // The new node is a right child, and the parent is a right child
    {
      left_rotate(table, index, grandpa);
      Color grandpaColor = grandpa->color;
      grandpa->color = parent->color;
      parent->color = grandpaColor;
      return;
    }
  }
  fixup(table, index, node);
}

void delete_fixup(HashTable* table, int index, RBTreeNode* node)
{
  while (node != table->buckets[index] && (node == NULL || node->color == BLACK))
  {
    if (node == node->parent->left)
    {
      RBTreeNode* sibling = node->parent->right;
      if (sibling->color == RED)
      {
        sibling->color = BLACK;
        node->parent->color = RED;
        left_rotate(table, index, node->parent);
        sibling = node->parent->right;
      }
      if ((sibling->left == NULL || sibling->left->color == BLACK) &&
        (sibling->right == NULL || sibling->right->color == BLACK))
      {
        sibling->color = RED;
        node = node->parent;
      }
      else
      {
        if (sibling->right == NULL || sibling->right->color == BLACK)
        {
          if (sibling->left != NULL) sibling->left->color = BLACK;
          sibling->color = RED;
          right_rotate(table, index, sibling);
          sibling = node->parent->right;
        }
        sibling->color = node->parent->color;
        node->parent->color = BLACK;
        if (sibling->right != NULL) sibling->right->color = BLACK;
        left_rotate(table, index, node->parent);
        node = table->buckets[index];
      }
    }
    else
    {
      RBTreeNode* sibling = node->parent->left;
      if (sibling->color == RED)
      {
        sibling->color = BLACK;
        node->parent->color = RED;
        right_rotate(table, index, node->parent);
        sibling = node->parent->left;
      }
      if ((sibling->right == NULL || sibling->right->color == BLACK) &&
        (sibling->left == NULL || sibling->left->color == BLACK))
      {
        sibling->color = RED;
        node = node->parent;
      }
      else
      {
        if (sibling->left == NULL || sibling->left->color == BLACK)
        {
          if (sibling->right != NULL) sibling->right->color = BLACK;
          sibling->color = RED;
          left_rotate(table, index, sibling);
          sibling = node->parent->left;
        }
        sibling->color = node->parent->color;
        node->parent->color = BLACK;
        if (sibling->left != NULL) sibling->left->color = BLACK;
        right_rotate(table, index, node->parent);
        node = table->buckets[index];
      }
    }
  }
  if (node != NULL) node->color = BLACK;
}

void transplant(RBTreeNode* u, RBTreeNode* v, HashTable* table)
{
  if (u->parent == NULL)
    table->buckets[hash(u->item->key)] = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;

  if (v != NULL)
    v->parent = u->parent;
}

RBTreeNode* minimum(RBTreeNode* node)
{
  while (node->left != NULL)
    node = node->left;
  
  return node;
}


HashTable* create_table(int size)
{
  HashTable* table = (HashTable*)malloc(sizeof(HashTable));
  table->size = size;
  table->count = 0;
  table->buckets = (RBTreeNode**)calloc(table->size, sizeof(RBTreeNode*));
  for(int i = 0; i < table->size; i++)
    table->buckets[i] = NULL;
  return table;
}

Ht_item* create_item(char* key, char* value)
{
  Ht_item* item = (Ht_item*)malloc(sizeof(Ht_item));
  item->key = (char*)malloc(strlen(key) + 1);
  item->value = (char*)malloc(strlen(value) + 1);
  strcpy(item->key, key);
  strcpy(item->value, value);
  return item;
}

RBTreeNode* create_node(Ht_item* item)
{
  RBTreeNode* node = (RBTreeNode*)malloc(sizeof(RBTreeNode));
  node->item = item;
  node->color = RED;
  node->parent = NULL;
  node->left = NULL;
  node->right = NULL;
  return node;
}

void free_item(Ht_item* item)
{
  free(item->key);
  free(item->value);
  free(item);
}

void free_node(RBTreeNode* node)
{
  free_item(node->item);
  node->parent = NULL;
  node->left = NULL;
  node->right = NULL;
  free(node);
}

void free_tree(RBTreeNode* node)
{
  if(node == NULL)
    return;
  free_tree(node->left);
  free_tree(node->right);
  free_node(node);
}


void free_table(HashTable* table)
{
  for(int i = 0; i < table->size; i++)
    free_tree(table->buckets[i]);
  free(table);
}

void ht_insert(HashTable* table, char* key, char* value)
{
  Ht_item* item = create_item(key, value);
  int index = hash(key);
  RBTreeNode* node = table->buckets[index];
  if(node == NULL)
  {
    std::cout << "Added " << key << " successfully" << std::endl;
    RBTreeNode* newNode = create_node(item);
    newNode->color = BLACK;
    table->buckets[index] = newNode;
    table->count++;
    return;
  }
  while(strcmp(node->item->key, key) != 0)
  {
    if(strcmp(node->item->key, key) > 0 && node->right != NULL)
      node = node->right;
    else if(strcmp(node->item->key, key) < 0 && node->left != NULL)
      node = node->left;
    else
      break;
  }
  if(strcmp(node->item->key, key) == 0)
  {
    strcpy(node->item->value, value);
    std::cout << "Updated " << key << " successfully" << std::endl;
  }
  else if(strcmp(node->item->key, key) > 0)
  {
    std::cout << "Added " << key << " successfully" << std::endl;
    RBTreeNode* newNode = create_node(item);
    node->right = newNode;
    newNode->parent = node;
    fixup(table, index, newNode);
  }
  else
  {
    std::cout << "Added " << key << " successfully" << std::endl;
    RBTreeNode* newNode = create_node(item);
    node->left = newNode;
    newNode->parent = node;
    fixup(table, index, newNode);
  }
  table->count++;
}

char* ht_find(HashTable* table, char* key)
{
  int index = hash(key);
  RBTreeNode* node = table->buckets[index];
  while(node != NULL && strcmp(node->item->key, key) != 0)
  {
    if(strcmp(node->item->key, key) > 0)
      node = node->right;
    else
      node = node->left;
  }
  if(node != NULL)
  {
    std::cout << "Found key " << key << " with value: " << node->item->value << std::endl;
    return node->item->value;
  }
  std::cerr << "Key Error: " << key << std::endl;
  std::cout << "Element " << key << " could not be found" << std::endl;
  return NULL;
}

void ht_delete(HashTable* table, char* key)
{
  if(table->count == 0)
  {
    std::cerr << "Deletion Error: Table is empty!" << std::endl;
    return;
  }
  int index = hash(key);
  RBTreeNode* node = table->buckets[index];
  while(node != NULL && strcmp(node->item->key, key) != 0)
  {
    if(strcmp(node->item->key, key) > 0)
      node = node->right;
    else
      node = node->left;
  }
  if(node == NULL)
  {
    std::cerr << "Deletion Error: Key " << key << " could not found" << std::endl;
    return;
  }
    RBTreeNode* y = node; // Node to be deleted
    RBTreeNode* x; // Node that replaces y
    Color yOriginalColor = y->color;

    if (node->left == NULL)
    {
      x = node->right;
      transplant(node, node->right, table);
    }
    else if (node->right == NULL)
    {
      x = node->left;
      transplant(node, node->left, table);
    }
    else
    {
      y = minimum(node->right);
      yOriginalColor = y->color;
      x = y->right;
      if (y->parent == node)
      {
        if (x != NULL)
          x->parent = y;
      }
      else
      {
        transplant(y, y->right, table);
        y->right = node->right;
        y->right->parent = y;
      }
      transplant(node, y, table);
      y->left = node->left;
      y->left->parent = y;
      y->color = node->color;
    }

    free_node(node);
    table->count--;

    if (yOriginalColor == BLACK)
        delete_fixup(table, index, x);

    std::cout << "Deleted " << key << " successfully" << std::endl;
}

void print_recursively(RBTreeNode* node)
{
  if(node == NULL)
    return;
  Ht_item* item = node->item;
  std::cout << "  Key: " << item->key << ", " << " Value: " << item->value << std::endl;
  print_recursively(node->left);
  print_recursively(node->right);
}

void print_table(HashTable* table)
{
  std::cout << "--------Hashtable-------" << std::endl;
  if(table->count != 0)
  {
    for(int i = 0; i < table->size; i++)
    {
      RBTreeNode* node = table->buckets[i];
      if(node != NULL)
      {
        std::cout << "Item(" << i << "):" << std::endl;
        print_recursively(node);
      }
    }
  }
  else
  {
    std::cout << "\n|  The table is empty  |\n" << std::endl;
  }
  std::cout << "------------------------" << std::endl;
}

int main()
{
  HashTable* table = create_table(CAPACITY);
  std::cout << "Use one of the following commands to interact with the set: \n"
            << "A: adding a new element to the set\n"
            << "D: deleting an element from the set\n"
            << "F: finding whether an element exists or not in the set\n"
            << "P: printing the entire set\n"
            << "Q: to quit\n"
            << "--------------------------------------------------------------"
            << std::endl;
  char cmd;
  char key[256];
  while(true)
  {
    std::cout << "Please enter (Q/A/D/P/F): ";
    std::cin >> cmd;
    if(cmd == 'Q' || cmd == 'q')
    {
      std::cout << "Quitting.." << std::endl;
      break;
    }
    else if(cmd == 'A' || cmd == 'a')
    {
      // Add
      std::cout << "Enter a key to be added (or B to go back): ";
      std::cin >> key;
      if(strcmp(key, "B") == 0)
        std::cout << "Going back..";

      char value[256];
      std::cout << "Enter the associated value with the previous key entered (or B to go back): ";
      std::cin >> value;
      if(strcmp(value, "B") == 0)
        std::cout << "Going back..";
      else
        ht_insert(table, key, value);
    }
    else if(cmd == 'D' || cmd == 'd')
    {
      // Delete
      std::cout << "Enter an element to be deleted (or B to go back): ";
      std::cin >> key;
      if(strcmp(key, "B") == 0)
        std::cout << "Going back..";
      else
        ht_delete(table, key);
      
    }
    else if(cmd == 'F' || cmd == 'f')
    {
      // Find
      std::cout << "Enter an element to be found (or B to go back): ";
      std::cin >> key;
      if(strcmp(key, "B") == 0)
        std::cout << "Going back..";
      else
        ht_find(table, key);
    }
    else if(cmd == 'P' || cmd == 'p')
    {
      std::cout << "Priting the entire table.." << std::endl;
      print_table(table);
    }
    else
      std::cout << "Unrecognized command, please try again" << std::endl;
    
  }
  return EXIT_SUCCESS;
}