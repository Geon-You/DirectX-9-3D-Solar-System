#pragma once
#include "Include.h"

typedef struct CubeVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 tex;
};

typedef struct MYINDEX
{
	WORD _1, _2, _3;
};

#define D3DFVF_CUBETEX (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0))

class SkyBox
{
private:
	LPDIRECT3DVERTEXBUFFER9 vb;
	LPDIRECT3DINDEXBUFFER9 ib;
	LPDIRECT3DCUBETEXTURE9 cubeTex;
	D3DXMATRIX matWorld;

public:
	void Initialize();
	void Update();
	void Render();
	void Release();

private:
	void InitVB();
	void InitIB();
};

