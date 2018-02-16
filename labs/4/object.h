#ifndef OBJECT_H
#define OBJECT_H

#include "VectorUtils3.h"
#include "loadobj.h"

typedef struct {
    Model *model;
    vec3 position;
    vec3 scale;
    vec3 rotation;  
} Object;

mat4 objectGetModelMatrix(const Object *o);
Object objectGet(const char *model, vec3 *translate, vec3 *scale, vec3 *rotation);
void objectMoveX(Object *o, float amount);
void objectMoveZ(Object *o, float amount);
#endif