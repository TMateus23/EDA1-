#include "dicionario.h"
#include "grid.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID_SIZE 4
#define WORD_SIZE 16

// Estrutura para representar as coordenadas de uma célula
typedef struct Cell {
  int row;
  int col;
} Cell;

// Estrutura para representar um nó na lista encadeada
typedef struct LinkedListNode {
  char word[WORD_SIZE];
  Cell path[WORD_SIZE];
  int length;
  struct LinkedListNode *next;
} LinkedListNode;

// Função para verificar se a palavra é válida
bool isValidWord(char grid[GRID_SIZE][GRID_SIZE],
                 bool visited[GRID_SIZE][GRID_SIZE], const char *word, int row,
                 int col, int index, Node *dictionary) {
  if (grid[row][col] != word[index]) {
    return false;
  }

  // Marca a célula atual como verificada
  visited[row][col] = true;

  // Verifica se a palavra foi encontrada
  if (index == strlen(word) - 1) {
    visited[row][col] = false;            // Faz reset da verificação
    return isWordValid(dictionary, word); // Verifica se a palavra é válida
  }

  // Verifica as células vizinhas
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int newRow = row + i;
      int newCol = col + j;

      // Confirma as fronteiras e se a célula já foi verificada
      if (newRow >= 0 && newRow < GRID_SIZE && newCol >= 0 &&
          newCol < GRID_SIZE && !visited[newRow][newCol]) {
        if (isValidWord(grid, visited, word, newRow, newCol, index + 1,
                        dictionary)) {
          visited[row][col] = false; // Faz reset da verificação
          return true;
        }
      }
    }
  }

  visited[row][col] = false; // Faz reset da verificação
  return false;
}

void findWordsHelper(char grid[GRID_SIZE][GRID_SIZE],
                     bool visited[GRID_SIZE][GRID_SIZE], Node *dictionary,
                     char word[WORD_SIZE], int row, int col, int index,
                     Cell path[WORD_SIZE], LinkedListNode **wordList);

void findValidWords(char grid[GRID_SIZE][GRID_SIZE], Node *dictionary,
                    LinkedListNode **wordList, int *totalSolutions) {
  bool visited[GRID_SIZE][GRID_SIZE] = {{false}};
  char word[WORD_SIZE];
  Cell path[WORD_SIZE];

  for (int row = 0; row < GRID_SIZE; row++) {
    for (int col = 0; col < GRID_SIZE; col++) {
      findWordsHelper(grid, visited, dictionary, word, row, col, 0, path,
                      wordList);
    }
  }

  // Contar o número de soluções encontradas
  LinkedListNode *current = *wordList;
  while (current != NULL) {
    (*totalSolutions)++;
    current = current->next;
  }
}

void findWordsHelper(char grid[GRID_SIZE][GRID_SIZE],
                     bool visited[GRID_SIZE][GRID_SIZE], Node *dictionary,
                     char word[WORD_SIZE], int row, int col, int index,
                     Cell path[WORD_SIZE], LinkedListNode **wordList) {
  // Ignora palavras com menos de 3 letras
  if (index >= 3 && isWordValid(dictionary, word)) {
    printf("Palavra encontrada: %s\n", word); // Imprime a palavra encontrada
    LinkedListNode *newNode = (LinkedListNode *)malloc(sizeof(LinkedListNode));
    strcpy(newNode->word, word);
    memcpy(newNode->path, path, sizeof(Cell) * index);
    newNode->length = index;
    newNode->next = *wordList;
    *wordList = newNode;
  }

  if (index >= WORD_SIZE - 1) {
    return;
  }

  visited[row][col] = true;
  word[index] = grid[row][col];
  word[index + 1] = '\0';
  path[index].row = row;
  path[index].col = col;

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int newRow = row + i;
      int newCol = col + j;

      if (newRow >= 0 && newRow < GRID_SIZE && newCol >= 0 &&
          newCol < GRID_SIZE && !visited[newRow][newCol]) {
        findWordsHelper(grid, visited, dictionary, word, newRow, newCol,
                        index + 1, path, wordList);
      }
    }
  }

  visited[row][col] = false;
}

void freeLinkedList(LinkedListNode *head) {
  LinkedListNode *current = head;
  while (current != NULL) {
    LinkedListNode *temp = current;
    current = current->next;
    free(temp);
  }
}

void printLinkedList(LinkedListNode *head) {
  if (head == NULL) {
    return;
  }

  printLinkedList(head->next);

  printf("%8s: ", head->word);
  for (int i = 0; i < head->length - 1; i++) {
    printf("%c(%d,%d) -> ", head->word[i], head->path[i].row,
           head->path[i].col);
  }
  printf("%c(%d,%d)\n", head->word[head->length - 1],
         head->path[head->length - 1].col, head->path[head->length - 1].row);
}

void printGrid(char grid[GRID_SIZE][GRID_SIZE]) {
  printf("*********************\n");
  printf("*   B O G G L E     *\n");
  printf("*********************\n");
  printf("*    0   1   2   3  *\n");
  for (int i = 0; i < GRID_SIZE; i++) {
    printf("* %d ", i);
    for (int j = 0; j < GRID_SIZE; j++) {
      printf(" %c  ", grid[i][j]);
    }
    printf("*\n");
  }
  printf("*********************\n");
}

int main() {

  int totalSolutions = 0;
  int gridChoice;
  printf("Introduza o tabuleiro de jogo (0, 1, 2): ");
  scanf("%d", &gridChoice);

  char filename[20];
  char grid[GRID_SIZE][GRID_SIZE];

  if (gridChoice == 0) {

    strcpy(filename, "boggle0.txt");
  } else if (gridChoice == 1) {
    strcpy(filename, "boggle1.txt");
  } else if (gridChoice == 2) {
    strcpy(filename, "boggle2.txt");
  } else {
    printf("Escolha de tabuleiro inválida.\n");
    return 1;
  }

  loadGridFromFile(filename, grid);
  printGrid(grid);

  // Cria a árvore de palavras do dicionário
  Node *dictionary = NULL;

  // Carrega as palavras do arquivo do dicionário
  FILE *file = fopen("corncob_caps_2023.txt", "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo do dicionário.\n");
    return 1;
  }

  char word[WORD_SIZE];
  while (fgets(word, sizeof(word), file)) {
    word[strcspn(word, "\r\n")] = '\0'; // Remove a quebra de linha

    // Verifica se a palavra não é vazia antes de inserir no dicionário
    if (strlen(word) > 0) {
      dictionary = insertWord(dictionary, word);
    }
  }

  fclose(file);

  // Encontra e guarda todas as palavras válidas em uma lista encadeada
  LinkedListNode *wordList = NULL;
  findValidWords(grid, dictionary, &wordList, &totalSolutions);

  printf("Total de soluções encontradas: %d\n", totalSolutions);

  printLinkedList(wordList);

  // Libera a memória da árvore e da lista encadeada
  freeTree(dictionary);
  freeLinkedList(wordList);

  return 0;
}
