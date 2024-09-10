#pragma once
#include "Include.h"

class Shape
{
public:
	int shapeNum; // �ش� �ʵ尪���� � �޽ø� �׸� ������ ����
	D3DXVECTOR3 pos;
	D3DXVECTOR3 angle;
	D3DXVECTOR3 scale;

private:
	LPDIRECT3DTEXTURE9 texture;			// �ؽ��� ����
	LPDIRECT3DTEXTURE9 textureNormal;	// �ؽ��� ��ָ�
	D3DXVECTOR3 light; // ����

	IDirect3DVertexBuffer9* vb; // ���ؽ� ����

	bool isPicking; // ���� ���콺�� ����Ű�� �ִ���
	D3DXVECTOR3* center; // �浹�ڽ��� ���� ������ ��Ƶ� �迭

public:
	void Initialize();
	void Update();
	void Render();
	void Release();
	void CalculateLight();
	DWORD VectorToRGBA(D3DXVECTOR3* v);

	bool OnCollisionEnter(D3DXVECTOR3* ori, D3DXVECTOR3* dir);
};

