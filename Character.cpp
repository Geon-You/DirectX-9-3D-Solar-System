#include "Character.h"
#include "Device.h"
void Character::Initialize()
{
	pos = D3DXVECTOR3(0, 100.f, 0);

	// ������ �ӽ÷� ������ ���� ����
	LPD3DXBUFFER matBuffer;

	auto device = GET_SINGLE(Device)->GetDevice();

	// x������ �ҷ��ɴϴ�.
	if (FAILED(D3DXLoadMeshFromX(L"Ezreal/Iz.x", D3DXMESH_SYSTEMMEM,
		device, NULL, &matBuffer, NULL, &matNum, &mesh)))
	{
		return;
	}

	// ���� ������ �ؽ��� ������ �и��ؼ� �����ɴϴ�.
	D3DXMATERIAL* mat = (D3DXMATERIAL*)matBuffer->GetBufferPointer();

	// ���� ������ŭ ���� �迭�� ����
	meshMat = new D3DMATERIAL9[matNum];
	// ���� ������ŭ �ؽ��� �迭�� ����
	meshTex = new LPDIRECT3DTEXTURE9[matNum];

	// ���� ������ŭ �ݺ��ϸ�, ���� ���� ���� ��������� �ؽ��� �ε带 ����
	for (int i = 0; i < matNum; ++i)
	{
		// ���� ���� ��������
		meshMat[i] = mat[i].MatD3D;
		// �ֺ� ���� �����͸� ������ �ִ� �ݻ籤���� ����
		meshMat[i].Ambient = meshMat[i].Diffuse;

		meshTex[i] = NULL;

		// ������ ���Ǵ� �����ؽ��� �̸��� ����, �ؽ��ĸ� �ε��Ѵ�.
		if (mat[i].pTextureFilename != NULL &&
			strlen(mat[i].pTextureFilename) > 0)
		{
			// �� ������ ���Դٴ� ����, ����ϴ� �����ؽ��İ� �����Ѵٴ� ��

			char filePath[256];
			sprintf_s(filePath, "Ezreal/%s", mat[i].pTextureFilename);

			// �ؽ��� �ε�
			if (FAILED(D3DXCreateTextureFromFileA(device, filePath, &meshTex[i])))
			{
				return;
			}
		}
	}

	// �ӽ÷� ������ ���� ���� ����
	matBuffer->Release();
}

void Character::Update()
{
}

void Character::Render()
{
	auto device = GET_SINGLE(Device)->GetDevice();
	D3DXMATRIX matWorld, matTrans, matScale;

	// ���� ������ŭ �ݺ��ؼ� �׸�
	for (int i = 0; i < matNum; ++i)
	{
		// ������ �ؽ��ĸ� ����
		device->SetMaterial(&meshMat[i]);
		device->SetTexture(0, meshTex[i]);

		// ��� ����
		D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);
		D3DXMatrixScaling(&matScale, .5f, .5f, .5f);

		matWorld = matScale * matTrans;
		device->SetTransform(D3DTS_WORLD, &matWorld);

		// �޽� �׸���
		mesh->DrawSubset(i);
	}
}

void Character::Release()
{
	if (mesh)
	{
		mesh->Release();
		mesh = nullptr;
	}

	if (meshMat)
	{
		delete[] meshMat;
		meshMat = nullptr;
	}

	if (meshTex)
	{
		delete[] meshTex;
		meshTex = nullptr;
	}
}
