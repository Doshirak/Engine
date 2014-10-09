#include "setCube.h"

VOID setVertices(SimpleVertex* vertices, int size, float x0, float y0, float z0, float d, int mode)
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			int x, y, z;
			if (mode == XY) {
				x = x0 + d * j;
				y = y0 + i * d;
				z = z0;
			}
			else if (mode == XZ) {
				 x = x0 + d * j;
				 y = y0;
				 z = z0 + d * i;
			}
			else if (mode == YZ) {
				 x = x0;
				 y = y0 + i * d;
				 z = z0 + j * d;
			}
			vertices[i * size + j] = { XMFLOAT3(x, y, z), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) };
		}
	}
}

VOID setIndices(WORD* indices, int size, int offset)
{
	for (int h = 0; h < size - 1; ++h)
	{
		int j = size * (h + 1) + offset;
		for (int i = 0; i < size - 1; ++i)
		{
			indices[(h * (size - 1) + i) * 6] = j;
			indices[(h * (size - 1) + i) * 6 + 1] = (j - size + 1);
			indices[(h * (size - 1) + i) * 6 + 2] = j - size;
			indices[(h * (size - 1) + i) * 6 + 3] = (j - size + 1);
			indices[(h * (size - 1) + i) * 6 + 4] = j;
			indices[(h * (size - 1) + i) * 6 + 5] = (j + 1);
			++j;
		}
	}
}