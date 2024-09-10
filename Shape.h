#pragma once
#include "Include.h"

class Shape
{
public:
	int shapeNum; // 해당 필드값으로 어떤 메시를 그릴 것인지 결정
	D3DXVECTOR3 pos;
	D3DXVECTOR3 angle;
	D3DXVECTOR3 scale;

private:
	LPDIRECT3DTEXTURE9 texture;			// 텍스쳐 벽면
	LPDIRECT3DTEXTURE9 textureNormal;	// 텍스쳐 노멀맵
	D3DXVECTOR3 light; // 광원

	IDirect3DVertexBuffer9* vb; // 버텍스 버퍼

	bool isPicking; // 현재 마우스로 가리키고 있는지
	D3DXVECTOR3* center; // 충돌박스로 사용될 점들을 담아둘 배열

public:
	void Initialize();
	void Update();
	void Render();
	void Release();
	void CalculateLight();
	DWORD VectorToRGBA(D3DXVECTOR3* v);

	bool OnCollisionEnter(D3DXVECTOR3* ori, D3DXVECTOR3* dir);
};

