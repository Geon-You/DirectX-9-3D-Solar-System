#include "BillBoard.h"
#include "Device.h"

void BillBoard::Initialize()
{
	auto device = GET_SINGLE(Device)->GetDevice();

	// �ؽ��� �ε�
	if (FAILED(D3DXCreateTextureFromFile(device, L"hp_bar.bmp", &tex)))
		return;

	// �ؽ��� ���� ����
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// ���ؽ� ���� ����
	if (FAILED(device->CreateVertexBuffer(sizeof(VERTEX) * 4, 0,
		D3DFVF_CUSTOMVERTEX_TEX, D3DPOOL_DEFAULT, &vb, NULL)))
		return;

	// ���ؽ� ���� ������ �����Ͽ� ���� ����
	VERTEX* vertice;

	// ���ؽ� ���� ���
	if (FAILED(vb->Lock(0, sizeof(VERTEX) * 4, (void**)&vertice, 0)))
		return;

	vertice[0].position = D3DXVECTOR3(-5, 1, -5);
	vertice[0].color = 0xffffffff;
	vertice[0].tu = 0;
	vertice[0].tv = 0;

	vertice[1].position = D3DXVECTOR3(5, 1, -5);
	vertice[1].color = 0xffffffff;
	vertice[1].tu = 1.f;
	vertice[1].tv = 0;

	vertice[2].position = D3DXVECTOR3(-5, -1, -5);
	vertice[2].color = 0xffffffff;
	vertice[2].tu = 0;
	vertice[2].tv = 1.f;

	vertice[3].position = D3DXVECTOR3(5, -1, -5);
	vertice[3].color = 0xffffffff;
	vertice[3].tu = 1.f;
	vertice[3].tv = 1.f;

	vb->Unlock();
}

void BillBoard::Update()
{
}

void BillBoard::Render()
{
	auto device = GET_SINGLE(Device)->GetDevice();
	
	D3DXMATRIX matTrans, matRot, matScale, matWorld;
	D3DXQUATERNION quat;

	// ��ġ ��� ��ȯ
	D3DXMatrixTranslation(&matTrans, 0, 160.f, 0);

	// ȸ�� ��� ��ȯ
	D3DXQuaternionRotationYawPitchRoll(&quat, 0, 0, 0);
	D3DXMatrixRotationQuaternion(&matRot, &quat);

	// ������ ���� �κ�
	// ī�޶� ���� Y�� ȸ���� �ϹǷ�, ī�޶��� y�� ȸ������� ������
	D3DXMATRIX matView;
	device->GetTransform(D3DTS_VIEW, &matView);

	matRot._11 = matView._11;
	matRot._13 = matView._13;
	matRot._31 = matView._31;
	matRot._33 = matView._33;

	// ��ġ�� ������� ����ǰ� �����Ƿ�, 
	// �� ���¿��� ��ü�� ���� ����� ���� ��Ű��
	// �ڵ����� ���� ��� * �� ��� ������ �̷����

	// Y�� ȸ�� ����� ������� ���ؼ�, ����� Y�� ȸ���� ��ȿȭ ��Ŵ
	D3DXMatrixInverse(&matRot, NULL, &matRot);

	// ũ�� ���
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	// ���� ���
	matWorld = matScale * matRot * matTrans;
	device->SetTransform(D3DTS_WORLD, &matWorld);

	// �ؽ��� ����
	device->SetTexture(0, tex);

	// ����� ���ؽ� ���� ����
	device->SetStreamSource(0, vb, 0, sizeof(VERTEX));

	// FVF ����
	device->SetFVF(D3DFVF_CUSTOMVERTEX_TEX);

	// �޽� �׸���
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void BillBoard::Release()
{
	if (vb)
	{
		vb->Release();
		vb = nullptr;
	}

	if (tex)
	{
		tex->Release();
		tex = nullptr;
	}
}
