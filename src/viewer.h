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
  struct Vertex *vertices;
  struct Surface *surfaces;
  int numVertices, numSurfaces;
};

struct Model *loadModelFromFile(const char *filename);
void addVertex(struct Model *model, struct Vertex vertex);
void addSurface(struct Model *model, struct Surface surface);

void translateModel(struct Model *model, double x, double y, double z);
void rotateModel(struct Model *model, double x_turn, double y_turn,
                 double z_turn);
void scaleModel(struct Model *model, double scaleFactor);
void normalizeModel(struct Model *model, double minRange, double maxRange);

void collapseModel(struct Model *model);

#endif  // VIEWER_H
