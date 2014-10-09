#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "resource.h"
#include "device.h"

VOID setVertices(SimpleVertex* vertices, int size, float x0, float y0, float z0, float d, int mode);

VOID setIndices(WORD* indices, int size, int offset);