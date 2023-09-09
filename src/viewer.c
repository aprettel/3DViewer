#include "viewer.h"

void findCenter(struct Vertex *center, struct Model *model);
void rotateModel_X(struct Model *model, double x_turn);
void rotateModel_Y(struct Model *model, double y_turn);
void rotateModel_Z(struct Model *model, double z_turn);

struct Model *loadModelFromFile(const char *filename) {
  FILE *file = fopen(filename, "r");
  char line_file[256];
  struct Model *model = (struct Model *)malloc(sizeof(struct Model));
  model->vertices = NULL;
  model->numVertices = 0;
  model->surfaces = NULL;
  model->numSurfaces = 0;
  while (fgets(line_file, sizeof(line_file), file)) {
    if (line_file[0] == 'v') {
      if (line_file[1] == ' ') {
        double x, y, z;
        sscanf(line_file, "v %lf %lf %lf", &x, &y, &z);
        struct Vertex vertex = {x, y, z};
        addVertex(model, vertex);
      } else if (line_file[1] == 't' || line_file[1] == 'n') {
        continue;
      }
    } else if (line_file[0] == 'f') {
      int vertices[4], texCoords[4], normals[4];
      if (sscanf(line_file, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                 &vertices[0], &texCoords[0], &normals[0], &vertices[1],
                 &texCoords[1], &normals[1], &vertices[2], &texCoords[2],
                 &normals[2], &vertices[3], &texCoords[3], &normals[3]) == 12 ||
          sscanf(line_file, "f %d//%d//%d %d//%d//%d %d//%d//%d %d//%d//%d",
                 &vertices[0], &texCoords[0], &normals[0], &vertices[1],
                 &texCoords[1], &normals[1], &vertices[2], &texCoords[2],
                 &normals[2], &vertices[3], &texCoords[3], &normals[3]) == 12 ||
          sscanf(line_file, "f %d/%d %d/%d %d/%d %d/%d", &vertices[0],
                 &texCoords[0], &vertices[1], &texCoords[1], &vertices[2],
                 &texCoords[2], &vertices[3], &texCoords[3]) == 8 ||
          sscanf(line_file, "f %d %d %d %d", &vertices[0], &vertices[1],
                 &vertices[2], &vertices[3]) == 4) {
        addSurface(model,
                   (struct Surface){vertices[0], vertices[1], vertices[2]});
        addSurface(model,
                   (struct Surface){vertices[0], vertices[2], vertices[3]});
      } else if (sscanf(line_file, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertices[0],
                        &texCoords[0], &normals[0], &vertices[1], &texCoords[1],
                        &normals[1], &vertices[2], &texCoords[2],
                        &normals[2]) == 9 ||
                 sscanf(line_file, "f %d/%d %d/%d %d/%d", &vertices[0],
                        &texCoords[0], &vertices[1], &texCoords[1],
                        &vertices[2], &texCoords[2]) == 6 ||
                 sscanf(line_file, "f %d//%d %d//%d %d//%d", &vertices[0],
                        &texCoords[0], &vertices[1], &texCoords[1],
                        &vertices[2], &texCoords[2]) == 6 ||
                 sscanf(line_file, "f %d %d %d ", &vertices[0], &vertices[1],
                        &vertices[2]) == 3) {
        addSurface(model,
                   (struct Surface){vertices[0], vertices[1], vertices[2]});
      } else if (sscanf(line_file, "f %d//%d %d//%d %d//%d %d//%d",
                        &vertices[0], &texCoords[0], &vertices[1],
                        &texCoords[1], &vertices[2], &texCoords[2],
                        &vertices[3], &texCoords[3]) == 8) {
        addSurface(model,
                   (struct Surface){vertices[0], vertices[1], vertices[2]});
      } else if (sscanf(line_file, "f %d %d ", &vertices[0], &vertices[1]) ==
                 2) {
        vertices[2] = vertices[0];
        addSurface(model,
                   (struct Surface){vertices[0], vertices[1], vertices[2]});
      }
    }
  }
  fclose(file);
  return model;
}

void addVertex(struct Model *model, struct Vertex vertex) {
  model->vertices = (struct Vertex *)realloc(
      model->vertices, (model->numVertices + 1) * sizeof(struct Vertex));
  model->vertices[model->numVertices] = vertex;
  model->numVertices++;
}

void addSurface(struct Model *model, struct Surface surface) {
  model->surfaces = (struct Surface *)realloc(
      model->surfaces, (model->numSurfaces + 1) * sizeof(struct Surface));
  model->surfaces[model->numSurfaces] = surface;
  model->numSurfaces++;
}

void translateModel(struct Model *model, double x, double y, double z) {
  for (int i = 0; i < model->numVertices; i++) {
    model->vertices[i].x += x;
    model->vertices[i].y += y;
    model->vertices[i].z += z;
  }
}

void scaleModel(struct Model *model, double scaleFactor) {
  struct Vertex center = {0, 0, 0};
  findCenter(&center, model);

  for (int i = 0; i < model->numVertices; i++) {
    double deltaX = model->vertices[i].x - center.x;
    double deltaY = model->vertices[i].y - center.y;
    double deltaZ = model->vertices[i].z - center.z;

    model->vertices[i].x = center.x + (deltaX * scaleFactor);
    model->vertices[i].y = center.y + (deltaY * scaleFactor);
    model->vertices[i].z = center.z + (deltaZ * scaleFactor);
  }
  //}
}

void rotateModel(struct Model *model, double x_turn, double y_turn,
                 double z_turn) {
  struct Vertex center = {0, 0, 0};
  findCenter(&center, model);

  for (int i = 0; i < model->numVertices; i++) {
    model->vertices[i].x -= center.x;
    model->vertices[i].y -= center.y;
    model->vertices[i].z -= center.z;
  }

  rotateModel_X(model, x_turn * M_PI / 180.0);

  rotateModel_Z(model, z_turn * M_PI / 180.0);
  rotateModel_Y(model, y_turn * M_PI / 180.0);

  for (int i = 0; i < model->numVertices; i++) {
    model->vertices[i].x += center.x;
    model->vertices[i].y += center.y;
    model->vertices[i].z += center.z;
  }
}

void rotateModel_X(struct Model *model, double x_turn) {
  for (int i = 0; i < model->numVertices; i++) {
    double y = model->vertices[i].y;
    double z = model->vertices[i].z;
    double newY = y * cos(x_turn) - z * sin(x_turn);
    double newZ = y * sin(x_turn) + z * cos(x_turn);
    model->vertices[i].y = newY;
    model->vertices[i].z = newZ;
  }
}

void rotateModel_Y(struct Model *model, double y_turn) {
  for (int i = 0; i < model->numVertices; i++) {
    double x = model->vertices[i].x;
    double z = model->vertices[i].z;
    double newX = x * cos(y_turn) + z * sin(y_turn);
    double newZ = -x * sin(y_turn) + z * cos(y_turn);
    model->vertices[i].x = newX;
    model->vertices[i].z = newZ;
  }
}

void rotateModel_Z(struct Model *model, double z_turn) {
  for (int i = 0; i < model->numVertices; i++) {
    double x = model->vertices[i].x;
    double y = model->vertices[i].y;
    double newX = x * cos(z_turn) - y * sin(z_turn);
    double newY = x * sin(z_turn) + y * cos(z_turn);
    model->vertices[i].x = newX;
    model->vertices[i].y = newY;
  }
}

void normalizeModel(struct Model *model, double minRange, double maxRange) {
  double minX = DBL_MAX, minY = DBL_MAX, minZ = DBL_MAX;
  double maxX = -DBL_MAX, maxY = -DBL_MAX, maxZ = -DBL_MAX;

  for (int i = 0; i < model->numVertices; i++) {
    double x = model->vertices[i].x;
    double y = model->vertices[i].y;
    double z = model->vertices[i].z;

    if (x < minX) minX = x;
    if (x > maxX) maxX = x;
    if (y < minY) minY = y;
    if (y > maxY) maxY = y;
    if (z < minZ) minZ = z;
    if (z > maxZ) maxZ = z;
  }

  double maxDistance = fmax(fmax(maxX - minX, maxY - minY), maxZ - minZ);
  double scaleFactor =
      (maxDistance != 0.0f) ? (maxRange - minRange) / maxDistance : 1.0f;

  double centerX = (minX + maxX) / 2.0f;
  double centerY = (minY + maxY) / 2.0f;
  double centerZ = (minZ + maxZ) / 2.0f;

  for (int i = 0; i < model->numVertices; i++) {
    model->vertices[i].x = (model->vertices[i].x - centerX) * scaleFactor;
    model->vertices[i].y = (model->vertices[i].y - centerY) * scaleFactor;
    model->vertices[i].z = (model->vertices[i].z - centerZ) * scaleFactor;
  }
}

void collapseModel(struct Model *model) {
  free(model->surfaces);
  free(model->vertices);
  free(model);
}

void findCenter(struct Vertex *center, struct Model *model) {
  for (int i = 0; i < model->numVertices; i++) {
    center->x += model->vertices[i].x;
    center->y += model->vertices[i].y;
    center->z += model->vertices[i].z;
  }
  center->x /= model->numVertices;
  center->y /= model->numVertices;
  center->z /= model->numVertices;
}
