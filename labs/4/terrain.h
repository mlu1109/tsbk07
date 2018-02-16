#ifndef TERRAIN_H
#define TERRAIN_H

#include "GL_utilities.h"
#include "loadobj.h" // Model
#include "LoadTGA.h" // TextureData

Model *generateTerrain(TextureData *tex, int quadWidth, int quadHeight);
float calcY(GLfloat *arr, int p_x, int p_z, int qPxWidth, int qPxHeight, int width, int height);

#endif