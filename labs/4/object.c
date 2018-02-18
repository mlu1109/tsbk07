#include "object.h"
#include "algebra.h"

mat4 objectGetModelMatrix(const Object *o)
{
    return mult3(
        T(o->position.x, o->position.y, o->position.z),
        S(o->scale.x, o->scale.y, o->scale.z),
        mult3(Rx(o->rotation.x), Ry(o->rotation.y), Rz(o->rotation.z)));
}

Object objectGet(const char *model, vec3 *position, vec3 *scale, vec3 *rotation, vec3 *direction)
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
    if (direction != NULL)
        o.direction = *direction;
    else
        o.direction = (vec3){0.0, 0.0, 0.0};

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

void objectMoveInDirection(Object *o, float amount)
{
    o->position.x += o->direction.x * amount;
    o->position.y += o->direction.y * amount;
    o->position.z += o->direction.z * amount;
}

void objectHandleCollision(Object *a, Object *b)
{
    // Assumes model width/height == 2 and uniform scaling... getting lazy
    const float radius = 1;
    float r_a = a->scale.x * radius;
    float r_b = b->scale.x * radius;
    float r = r_a + r_b;
    vec3 v = VectorSub(a->position, b->position);
    float d = Norm(v);
    if (d <= r)
    {
        if (DotProduct(v, b->direction) < 0)
        {
            a->direction = ScalarMult(Normalize(v), -1);
            b->direction = Normalize(v);
        }
        else
        {
            b->direction = ScalarMult(Normalize(v), -1);
            a->direction = Normalize(v);
        }
        objectMoveInDirection(b, r - d);
        objectMoveInDirection(a, 1);
    }
}