#include "dicionario.h"
#include <stdlib.h>
#include <string.h>

// Função que cria nós para a árvore
Node *createNode(const char *word) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  strcpy(newNode->word, word);
  newNode->left = NULL;
  newNode->right = NULL;
  return newNode;
}

Node *insertWord(Node *root, const char *word) {
  if (root == NULL) {
    return createNode(word);
  }

  int cmp = strcmp(word, root->word);
  if (cmp < 0) {
    root->left = insertWord(root->left, word);
  } else if (cmp > 0) {
    root->right = insertWord(root->right, word);
  }

  return root;
}

// Função que verifica se a palavra é válida
bool isWordValid(Node *root, const char *word) {
  if (root == NULL) {
    return false;
  }

  int cmp = strcmp(word, root->word);
  if (cmp == 0) {
    return true; // palavra válida
  } else if (cmp < 0) {
    return isWordValid(root->left, word);
  } else {
    return isWordValid(root->right, word);
  }
}

void freeTree(Node *root) {
  if (root == NULL) {
    return;
  }

  freeTree(root->left);
  freeTree(root->right);
  free(root);
}