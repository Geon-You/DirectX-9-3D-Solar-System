#include "SolarSystem.h"
#include "Device.h"

void SolarSystem::Initialize()
{
	// �༺ ������ ���� �� ���� �ε�
	for (int i = 0; i < MaxPlanetCnt; ++i)
	{
		// �༺ ������ (�θ�, �ڽ� �̸�)�� ä���.
		planets[i].data = PlanetData[i];
		// �༺ �Ÿ� ����
		planets[i].pos = D3DXVECTOR3(0, 0, relativeDistance[i] * 10.f);
		// x ���� �ε�
		LoadXFile(&planets[i]);
	}

	// ���� ������ ���� �� ���� �ε�
	for (int i = 0; i < MaxSatelliteCnt; ++i)
	{
		// ���� ������(�θ�, �ڽ� �̸�)�� ä���.
		satellite[i].data = SatelliteData[i];
		// ���� �Ÿ� ����
		satellite[i].pos = D3DXVECTOR3(0, 0, 2.f);
		// x ���� �ε�
		LoadXFile(&satellite[i]);
	}

	// �� �༺/���� �����Ϳ��� ���� �ڽ��� �θ� �̸��� �����ͷ� ���� ����
	// �ش� �̸����� �θ� ��ü�� ã��, �θ� ��ü�� �ּҸ� �������ش�.
	for (int i = 0; i < MaxPlanetCnt; ++i)
	{
		planets[i].parent = GetPlanet(planets[i].data.parent);
	}

	for (int i = 0; i < MaxSatelliteCnt; ++i)
	{
		satellite[i].parent = GetPlanet(satellite[i].data.parent);
	}
}

void SolarSystem::Update()
{
}

void SolarSystem::Render()
{
	auto device = GET_SINGLE(Device)->GetDevice();

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // �ø� ����
	device->SetRenderState(D3DRS_LIGHTING, FALSE); // �ؽ��� ���� �״�� ����ϵ��� �������� ��

	// �伺 �� ������ ���ĺ���..
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �༺ �׸���
	for (int i = 0; i < MaxPlanetCnt; ++i)
	{
		DrawMesh(&planets[i]);
	}

	// ���� �׸���
	for (int i = 0; i < MaxSatelliteCnt; ++i)
	{
		DrawMesh(&satellite[i]);
	}
}

void SolarSystem::Release()
{
	for (int i = 0; i < MaxPlanetCnt; ++i)
	{
		if (planets[i].mesh)
		{
			planets[i].mesh->Release();
			planets[i].mesh = nullptr;
		}

		if (planets[i].tex)
		{
			delete[] planets[i].tex;
			planets[i].tex = nullptr;
		}

		if (planets[i].mat)
		{
			delete[] planets[i].mat;
			planets[i].mat = nullptr;
		}
	}

	for (int i = 0; i < MaxSatelliteCnt; ++i)
	{
		if (satellite[i].mesh)
		{
			satellite[i].mesh->Release();
			satellite[i].mesh = nullptr;
		}

		if (satellite[i].tex)
		{
			delete[] satellite[i].tex;
			satellite[i].tex = nullptr;
		}

		if (satellite[i].mat)
		{
			delete[] satellite[i].mat;
			satellite[i].mat = nullptr;
		}
	}
}

HRESULT SolarSystem::LoadXFile(Model* model)
{
	auto device = GET_SINGLE(Device)->GetDevice();
	// ������ �ӽ÷� ������ ���� ����
	LPD3DXBUFFER matBuffer;

	// x ���� ��θ� ä��
	wchar_t xFilePath[256];
	swprintf_s(xFilePath, 256, L"SolarSystem/%s.x", model->data.name);

	// x ���� �ε�
	if (FAILED(D3DXLoadMeshFromX(xFilePath, D3DXMESH_SYSTEMMEM,
		device, NULL, &matBuffer, NULL, &model->matCnt, &model->mesh)))
	{
		return E_FAIL;
	}

	D3DXMATERIAL* mat = (D3DXMATERIAL*)matBuffer->GetBufferPointer();

	// ����ϴ� ���� ������ŭ ���� �迭 ����
	model->mat = new D3DMATERIAL9[model->matCnt];
	// ����ϴ� ���� ������ŭ �ؽ��� �迭 ����
	model->tex = new LPDIRECT3DTEXTURE9[model->matCnt];

	for (int i = 0; i < model->matCnt; ++i)
	{
		// ���� ���� ����
		model->mat[i] = mat[i].MatD3D;
		// �ؽ��� �ε�
		model->tex[i] = nullptr;

		// �ؽ��� ��ΰ� ��ȿ���� Ȯ�� ��, �ε�
		if (mat[i].pTextureFilename != NULL &&
			strlen(mat[i].pTextureFilename) > 0)
		{
			char texPath[256];
			sprintf_s(texPath, "SolarSystem/%s", mat[i].pTextureFilename);

			if (FAILED(D3DXCreateTextureFromFileA(device, texPath, &model->tex[i])))
			{
				return E_FAIL;
			}
		}
	}

	// �ӽ÷� ������ ���� ����
	matBuffer->Release();

    return S_OK;
}

void SolarSystem::DrawMesh(Model* model)
{
	auto device = GET_SINGLE(Device)->GetDevice();

	// ���� ������ŭ �׸�����
	for (int i = 0; i < model->matCnt; ++i)
	{
		// ������ �ؽ��� ����
		device->SetMaterial(&model->mat[i]);
		device->SetTexture(0, model->tex[i]);

		// ��� ��ȯ�� ���� ũ��, ȸ��, ��ġ ����
		D3DXMATRIX matScale, matRot, matTrans, matOrbit;

		// �� ���� ������ �״��
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		// ����
		D3DXMatrixIdentity(&matRot);
		// ��ġ
		D3DXMatrixTranslation(&matTrans, model->pos.x, model->pos.y, model->pos.z);
		// ����
		D3DXMatrixIdentity(&matOrbit);

		// �¾��� �ƴ� ��츸 �����ϵ���
		if (model->data.name != L"sun")
		{
			// �� �������� �츮�� ���ϴ� ������ �ƴϹǷ�, ������ �̿��� ���� ���Ƿ� ȸ��
			D3DXMatrixRotationX(&matRot, 60.f * D3DX_PI / 180.f);

			auto d = GetTickCount64() % ((int)((D3DX_PI * 2.f) * 1000.f));
			D3DXMatrixRotationY(&matOrbit, d / 1000.f);
		}

		// �θ� �����Ѵٸ�
		if (model->parent != nullptr)
		{
			model->matWorld = matScale * matRot * matTrans * matOrbit * (model->parent->matWorld);
		}
		else
		{
			model->matWorld = matScale * matRot * matTrans;
		}

		// ������� ����
		device->SetTransform(D3DTS_WORLD, &model->matWorld);

		// �޽� �׸���
		model->mesh->DrawSubset(i);
	}
}

Model* SolarSystem::GetPlanet(const wchar_t* name)
{
	// �༺ �����͸� ��ȸ�ϸ�, �Ķ���ͷ� ���� �̸��� ������ �̸��� �ִٸ� ��ȯ
	for (int i = 0; i < MaxPlanetCnt; ++i)
	{
		if (lstrcmpW(planets[i].data.name, name) == 0)
		{
			return &planets[i];
		}
	}

    return nullptr;
}
