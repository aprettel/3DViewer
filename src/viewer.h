#ifndef VIEWER_H
#define VIEWER_H

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct Vertex {
  double x, y, z;
};

struct Surface {
  int v1, v2, v3;
};

struct Model {
  struct Vertex* vertices;   // вершины
  struct Surface* surfaces;  // соединения
  int numVertices, numSurfaces;  // количество вершин и соединений
};

// Парсинг
struct Model* loadModelFromFile(const char* filename);
void addVertex(struct Model* model, struct Vertex vertex);
void addSurface(struct Model* model, struct Surface surface);

// Аффинные преобразования
void translateModel(struct Model* model, double x, double y, double z);
void scaleModel(struct Model* model, double scaleFactor);
void rotateModel_X(struct Model* model, double x_turn);
void rotateModel_Y(struct Model* model, double y_turn);
void rotateModel_Z(struct Model* model, double z_turn);
void normalizeModel(struct Model* model, double minRange, double maxRange);

void collapseModel(struct Model* model);

#endif  // VIEWER_H
