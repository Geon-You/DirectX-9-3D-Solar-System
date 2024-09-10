#pragma once
#include "Define.h"

typedef struct VERTEXANIM
{
	float x, y, z;
	DWORD color;
} VA;

struct MYINDEXANIM
{
	WORD _0, _1, _2;
};

class KeyAnimObj
{
private:
	LPDIRECT3DVERTEXBUFFER9 vb; // ���� ����
	LPDIRECT3DINDEXBUFFER9 ib;  // �ε��� ����

	D3DXMATRIXA16 matTransParent; // �θ��� ��ġ ���
	D3DXMATRIXA16 matRotParent; // �θ��� ȸ�� ���

	D3DXMATRIXA16 matTransChild; // �ڽ��� ��ġ ���
	D3DXMATRIXA16 matRotChild; // �ڽ��� ȸ�� ���

	// �ִϸ��̼� Ű ���� �����ϱ� ���� �迭
	D3DXVECTOR3 aniPos[2];    // ��ġ Ű ��
	D3DXQUATERNION aniRot[2]; // ȸ�� Ű ��

public:
	void Initialize();
	void Update();
	void Render();
	void Release();

	HRESULT InitVB(); // ���� ���� �ʱ�ȭ
	HRESULT InitIB(); // �ε��� ���� �ʱ�ȭ
	HRESULT InitGeometry(); // ���� ���� �ʱ�ȭ

	void InitAnimation(); // �ִϸ��̼� �ʱ�ȭ
	void Animate();  // �ִϸ��̼� ����
	void DrawMesh(D3DXMATRIXA16* mat); // �޽� �׸���

	// ���� ���� ���
	float Linear(float a, float b, float t);
};

