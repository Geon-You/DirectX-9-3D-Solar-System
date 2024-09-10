#include "KeyAnimObj.h"
#include "Device.h"

void KeyAnimObj::Initialize()
{
	InitAnimation();
	InitGeometry();
}

void KeyAnimObj::Update()
{
	Animate();
}

void KeyAnimObj::Render()
{
	D3DXMATRIXA16 matWorld;

	// �θ� ��ü ���� ��� ����
	matWorld = matTransParent * matRotParent;
	// �θ� ť�� �׸���
	DrawMesh(&matWorld);

	// �ڽ� ��ü ���� ��� ����
	matWorld = matTransChild * matRotChild * matWorld;
	// �ڽ� ť�� �׸���
	DrawMesh(&matWorld);
}

void KeyAnimObj::Release()
{
	if (vb)
	{
		vb->Release();
		vb = nullptr;
	}

	if (ib)
	{
		ib->Release();
		ib = nullptr;
	}
}

HRESULT KeyAnimObj::InitVB()
{
	// ť�긦 �������ϱ� ���� 8���� ���� ����
	VA vertices[] =
	{
		{ -1, 1, 1,  0xffff0000 },
		{ 1, 1, 1,   0xff00ff00 },
		{ 1, 1, -1,  0xff0000ff },
		{ -1, 1, -1, 0xffffff00 },

		{ -1, -1, 1, 0xff00ffff },
		{ 1, -1, 1,  0xffff00ff },
		{ 1, -1, -1, 0xff000000 },
		{ -1, -1, -1,0xffffffff }
	};

	auto device = GET_SINGLE(Device)->GetDevice();

	// ���� ���� ����
	if (FAILED(device->CreateVertexBuffer(sizeof(VA) * 8,
		NULL, D3DFVF_CUSTOMVERTEX_ANIM, D3DPOOL_DEFAULT, &vb, NULL)))
		return E_FAIL;

	VOID* pVertices;

	// ���� ���� ��
	if (FAILED(vb->Lock(0, sizeof(vertices), (void**)&pVertices, NULL)))
		return E_FAIL;

	// ���ʿ� �̸� �����ص� ���ؽ� �����͵��� ����
	memcpy_s(pVertices, sizeof(vertices), vertices, sizeof(vertices));
	
	// ���� ���� ���
	vb->Unlock();

	return S_OK;
}

HRESULT KeyAnimObj::InitIB()
{
	MYINDEXANIM indices[] =
	{
		{0, 1, 2}, {0, 2, 3}, // ����
		{4, 6, 5}, {4, 7, 6}, // �Ʒ���
		{0, 3, 7}, {0, 7, 4}, // ����
		{1, 5, 6}, {1, 6, 2}, // ������
		{3, 2, 6}, {3, 6, 7}, // �ո�
		{0, 4, 5}, {0, 5, 1}  // �޸�
	};

	auto device = GET_SINGLE(Device)->GetDevice();

	// �ε��� ���� ����
	if (FAILED(device->CreateIndexBuffer(sizeof(MYINDEXANIM) * 12, NULL,
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &ib, NULL)))
		return E_FAIL;

	// �ε��� ���� ��
	VOID* pIndices;
	if (FAILED(ib->Lock(0, sizeof(indices), (void**)&pIndices, NULL)))
		return E_FAIL;

	// �̸� �����ص� �ε��� ���� ����
	memcpy_s(pIndices, sizeof(indices), indices, sizeof(indices));

	// �ε��� ���� ���
	ib->Unlock();

	return S_OK;
}

HRESULT KeyAnimObj::InitGeometry()
{
	if (FAILED(InitVB())) return E_FAIL;
	if (FAILED(InitIB())) return E_FAIL;

	return S_OK;
}

void KeyAnimObj::InitAnimation()
{
	// ���� ��ġ ��
	aniPos[0] = D3DXVECTOR3(0, 0, 0);
	// ���� ��ġ ��
	aniPos[1] = D3DXVECTOR3(5, 5, 5);
	// ���� ȸ�� ��
	D3DXVECTOR3 ypr = D3DXVECTOR3(D3DX_PI * 90.f / 180.f, 0, 0);
	D3DXQuaternionRotationYawPitchRoll(&aniRot[0], ypr.x, ypr.y, ypr.z);
	// ���� ȸ�� ��
	ypr = D3DXVECTOR3(0, D3DX_PI * 90.f / 180.f, 0);
	D3DXQuaternionRotationYawPitchRoll(&aniRot[1], ypr.x, ypr.y, ypr.z);
}

void KeyAnimObj::Animate()
{
	// �ִϸ��̼ǿ� ����� �ð� ��
	static float t = 0;

	// �ð��� 0~1������ ���� ��� (0: �ִϸ��̼� ���� �ð�, 1: �ִϸ��̼� ���� �ð�)
	if (t > 1.f) t = 0;

	// ������ ��ġ ���� ���� ����
	D3DXVECTOR3 pos
		= D3DXVECTOR3(Linear(aniPos[0].x, aniPos[1].x, t),
			Linear(aniPos[0].y, aniPos[1].y, t),
			Linear(aniPos[0].z, aniPos[1].z, t));

	// ���� ���� ���
	// D3DXVec3Lerp();

	// ������ ��ġ�� �θ��� ��ġ ��Ŀ� ����
	D3DXMatrixTranslation(&matTransParent, pos.x, pos.y, pos.z);

	// ������ ȸ�� ���� ���� ����
	D3DXQUATERNION quat;

	// ȸ�� ���� ���� ���� ����
	D3DXQuaternionSlerp(&quat, &aniRot[0], &aniRot[1], t);
	// ������ ȸ�� ���� ���� ���ʹϾ��� �θ��� ȸ�� ��Ŀ� ����
	D3DXMatrixRotationQuaternion(&matRotParent, &quat);

	// �θ� �����ϹǷ�, �θ� �������� 3,3,3��ŭ ������ ��ġ
	D3DXMatrixTranslation(&matTransChild, 3.f, 3.f, 3.f);
	// ���� ȸ���� �������� �ʰ� ����
	D3DXMatrixIdentity(&matRotChild);

	t += 0.003f;
}

void KeyAnimObj::DrawMesh(D3DXMATRIXA16* mat)
{
	auto device = GET_SINGLE(Device)->GetDevice();

	// ���� �����Ƿ� ���� ��
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	device->SetTransform(D3DTS_WORLD, mat);
	device->SetStreamSource(0, vb, 0, sizeof(VA));
	device->SetFVF(D3DFVF_CUSTOMVERTEX_ANIM);
	device->SetIndices(ib);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

float KeyAnimObj::Linear(float a, float b, float t)
{
	// a�� ������ 1���� 0���� ���� ����
	// b�� ������ 0���� 1�� ���� ����
	return a * (1.f - t) + b * t;
}
