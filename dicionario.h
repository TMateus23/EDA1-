#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

#define WORD_SIZE 16

typedef struct Node {
  char word[WORD_SIZE];
  struct Node *left;
  struct Node *right;
} Node;

Node *createNode(const char *word);
Node *insertWord(Node *root, const char *word);
bool isWordValid(Node *root, const char *word);
void freeTree(Node *root);

#endif // DICTIONARY_H
