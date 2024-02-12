#include "grid.h"
#include <stdio.h>
#include <stdlib.h>

char grid1[GRID_SIZE][GRID_SIZE];
char grid2[GRID_SIZE][GRID_SIZE];
char grid3[GRID_SIZE][GRID_SIZE];

void loadGridFromFile(const char *filename, char grid[GRID_SIZE][GRID_SIZE]) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    exit(1);
  }

  // Ler os caracteres do arquivo e preencher a matriz
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      fscanf(file, " %c", &grid[i][j]);
    }
  }

  fclose(file);
}
