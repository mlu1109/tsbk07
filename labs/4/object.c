#include "object.h"
#include "algebra.h"

mat4 objectGetModelMatrix(const Object *o)
{
    return mult3(
        T(o->position.x, o->position.y, o->position.z),
        S(o->scale.x, o->scale.y, o->scale.z),
        mult3(Rx(o->rotation.x), Ry(o->rotation.y), Rz(o->rotation.z)));
}

Object objectGet(const char *model, vec3 *position, vec3 *scale, vec3 *rotation)
{
    Object o;
    o.model = LoadModelPlus(model);

    if (position != NULL)
        o.position = *position;
    else
        o.position = (vec3){0.0, 0.0, 0.0};
    if (rotation != NULL)
        o.rotation = *rotation;
    else
        o.rotation = (vec3){0.0, 0.0, 0.0};
    if (scale != NULL)
        o.scale = *scale;
    else
        o.scale = (vec3){1.0, 1.0, 1.0};

    return o;
}

void objectMoveX(Object *o, float amount)
{
    o->position.x += amount;
}

void objectMoveZ(Object *o, float amount)
{
    o->position.z += amount;
}