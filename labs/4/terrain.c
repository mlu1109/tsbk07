#include "terrain.h"
#include "VectorUtils3.h"

vec3 fromVertexArray(GLfloat *arr, int x, int z, int width)
{
    vec3 res = {arr[(x + z * width) * 3 + 0],
                arr[(x + z * width) * 3 + 1],
                arr[(x + z * width) * 3 + 2]};
    return res;
}

vec3 calcVertexNormalSimple(GLfloat *arr, int x, int z, int width, int height)
{
    vec3 v1 = {-1, -1, -1};
    vec3 v2 = {0, 0, 0};
    vec3 v3 = {0, 0, 0};
    // Edge cases (no further thought gone into handling these)
    if (x == 0)
    {
        v1 = fromVertexArray(arr, x, z, width);
        if (z == 0)
        {
            v2 = fromVertexArray(arr, x + 0, z + 1, width);
            v3 = fromVertexArray(arr, x + 1, z + 0, width);
        }
        else
        {
            v2 = fromVertexArray(arr, x + 0, z - 1, width);
            v3 = fromVertexArray(arr, x + 1, z - 0, width);
        }
    }
    else if (x == width - 1)
    {
        v1 = fromVertexArray(arr, x, z, width);
        if (z == 0)
        {
            v2 = fromVertexArray(arr, x - 0, z + 1, width);
            v3 = fromVertexArray(arr, x - 1, z + 0, width);
        }
        else
        {
            v2 = fromVertexArray(arr, x - 0, z - 1, width);
            v3 = fromVertexArray(arr, x - 1, z - 0, width);
        }
    }
    else if (z == 0)
    {
        v1 = fromVertexArray(arr, x + 1, z + 0, width);
        v2 = fromVertexArray(arr, x - 1, z + 1, width);
        v3 = fromVertexArray(arr, x + 0, z + 1, width);
    }
    else if (z == height - 1)
    {
        v1 = fromVertexArray(arr, x + 1, z + 0, width);
        v2 = fromVertexArray(arr, x - 1, z - 1, width);
        v3 = fromVertexArray(arr, x + 0, z - 1, width);
    }
    // Not edge case
    else
    {
        v1 = fromVertexArray(arr, x + 1, z - 1, width);
        v2 = fromVertexArray(arr, x - 1, z + 1, width);
        v3 = fromVertexArray(arr, x + 1, z + 1, width);
    }
    // The vertex normal is the normal of the triangle formed by its surrounding neighbours
    vec3 e1 = VectorSub(v1, v3);
    vec3 e2 = VectorSub(v1, v2);
    vec3 n = CrossProduct(e2, e1);
    if (n.y < 0) // Flip the normal if it points downwards
        n = ScalarMult(n, -1.f);

    return Normalize(n);
}

float calcY(GLfloat *arr, int p_x, int p_z, int qPxWidth, int qPxHeight, int width, int height)
{
    // What quad the point falls into
    int q_x = p_x / qPxWidth;
    int q_z = p_z / qPxHeight;
    if (q_x >= width - 1 || q_z >= height - 1 || p_x < 0 || p_z < 0)
        return 0;

    vec3 v1 = fromVertexArray(arr, q_x + 0, q_z + 0, width);
    vec3 v2 = fromVertexArray(arr, q_x + 1, q_z + 1, width);

    // Pick triangle
    int q_px = p_x % qPxWidth;
    int q_pz = p_z % qPxHeight;
    int i = q_px > q_pz ? 1 : 0;
    vec3 v3;
    if (i == 1)
        v3 = fromVertexArray(arr, q_x + 1, q_z, width);
    else
        v3 = fromVertexArray(arr, q_x, q_z + 1, width);
    
    // Calculate normal
    vec3 n = Normalize(CrossProduct(VectorSub(v1, v2), VectorSub(v1, v3)));
    if (n.y < 0) // Flip the normal if it points downwards
        ScalarMult(n, -1);

    // n = [a,b,c]
    // r = [x,y,z]
    // looking for r_p = [p_x, p_y, p_z]
    // a(x - p_x) + b(y - p_y) + c(y - p_z) = 0 =>
    // p_y = (a(x - p_x) + b*y + c(z - p_z))/b

    return (n.x * (v1.x - p_x) + n.y * v1.y + n.z * (v1.z - p_z)) / n.y;
}

Model *generateTerrain(TextureData *tex, int quadWidth, int quadHeight)
{
	int vertexCount = tex->width * tex->height;
    int triangleCount = (tex->width - 1) * (tex->height - 1) * 2;
	int x, z;

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount * 3);
	
    printf("bpp %d\n", tex->bpp);

    // Vertices and tex coords
	for (x = 0; x < tex->width; x++)
	{
		for (z = 0; z < tex->height; z++)
		{
			// Vertices, scalable
			vertexArray[(x + z * tex->width) * 3 + 0] = quadWidth * x / 1.0;
			vertexArray[(x + z * tex->width) * 3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp / 8)] / 7.0;
			vertexArray[(x + z * tex->width) * 3 + 2] = quadHeight * z / 1.0;

			// Tex coords, scalable
			texCoordArray[(x + z * tex->width) * 2 + 0] = (float)x / tex->width;
			texCoordArray[(x + z * tex->width) * 2 + 1] = (float)z / tex->height;
		}
	}
    // Normals
	for (x = 0; x < tex->width; x++)
	{
		for (z = 0; z < tex->height; z++)
		{
			vec3 n = calcVertexNormalSimple(vertexArray, x, z, tex->width, tex->height);
			normalArray[(x + z * tex->width) * 3 + 0] = n.x;
			normalArray[(x + z * tex->width) * 3 + 1] = n.y;
			normalArray[(x + z * tex->width) * 3 + 2] = n.z;
		}
	}
    // Indices
	for (x = 0; x < tex->width - 1; x++)
	{
		for (z = 0; z < tex->height - 1; z++)
		{
			// Triangle 1
			indexArray[(x + z * (tex->width - 1)) * 6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 1] = x + (z + 1) * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 2] = x + 1 + z * tex->width;
			// Triangle 2
			indexArray[(x + z * (tex->width - 1)) * 6 + 3] = x + 1 + z * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 4] = x + (z + 1) * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 5] = x + 1 + (z + 1) * tex->width;
		}
	}

	Model *terrain = LoadDataToModel(
		vertexArray,
		normalArray,
		texCoordArray,
		NULL,
		indexArray,
		vertexCount,
		triangleCount * 3);

	return terrain;
}