#pragma once
#include "Include.h"
#include "BillBoard.h"
class Character
{
private:
	LPD3DXMESH mesh; // �޽ø� ���� �ʵ�
	// �����̶�? ���� ����ϴµ� �ʿ��� �����͸� ����
	D3DMATERIAL9* meshMat; // �޽ÿ��� ����� ����
	LPDIRECT3DTEXTURE9* meshTex; // �޽ÿ��� ����� �ؽ���
	DWORD matNum; // �޽ÿ��� ��� ���� ���� ����
	D3DXVECTOR3 pos;

public:
	void Initialize();
	void Update();
	void Render();
	void Release();
};

