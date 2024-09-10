#include "SolarSystem.h"
#include "Device.h"

void SolarSystem::Initialize()
{
	// 행성 데이터 설정 및 파일 로드
	for (int i = 0; i < MaxPlanetCnt; ++i)
	{
		// 행성 데이터 (부모, 자신 이름)을 채운다.
		planets[i].data = PlanetData[i];
		// 행성 거리 설정
		planets[i].pos = D3DXVECTOR3(0, 0, relativeDistance[i] * 10.f);
		// x 파일 로드
		LoadXFile(&planets[i]);
	}

	// 위성 데이터 설정 및 파일 로드
	for (int i = 0; i < MaxSatelliteCnt; ++i)
	{
		// 위성 데이터(부모, 자신 이름)을 채운다.
		satellite[i].data = SatelliteData[i];
		// 위성 거리 설정
		satellite[i].pos = D3DXVECTOR3(0, 0, 2.f);
		// x 파일 로드
		LoadXFile(&satellite[i]);
	}

	// 각 행성/위성 데이터에는 현재 자신의 부모 이름을 데이터로 갖고 있음
	// 해당 이름으로 부모 객체를 찾아, 부모 객체의 주소를 전달해준다.
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

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // 컬링 끄기
	device->SetRenderState(D3DRS_LIGHTING, FALSE); // 텍스쳐 색상 그대로 출력하도록 라이팅을 끔

	// 토성 고리 때문에 알파블렌딩..
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 행성 그리기
	for (int i = 0; i < MaxPlanetCnt; ++i)
	{
		DrawMesh(&planets[i]);
	}

	// 위성 그리기
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
	// 재질을 임시로 보관할 버퍼 선언
	LPD3DXBUFFER matBuffer;

	// x 파일 경로를 채움
	wchar_t xFilePath[256];
	swprintf_s(xFilePath, 256, L"SolarSystem/%s.x", model->data.name);

	// x 파일 로드
	if (FAILED(D3DXLoadMeshFromX(xFilePath, D3DXMESH_SYSTEMMEM,
		device, NULL, &matBuffer, NULL, &model->matCnt, &model->mesh)))
	{
		return E_FAIL;
	}

	D3DXMATERIAL* mat = (D3DXMATERIAL*)matBuffer->GetBufferPointer();

	// 사용하는 재질 개수만큼 재질 배열 생성
	model->mat = new D3DMATERIAL9[model->matCnt];
	// 사용하는 재질 개수만큼 텍스쳐 배열 생성
	model->tex = new LPDIRECT3DTEXTURE9[model->matCnt];

	for (int i = 0; i < model->matCnt; ++i)
	{
		// 재질 정보 복사
		model->mat[i] = mat[i].MatD3D;
		// 텍스쳐 로드
		model->tex[i] = nullptr;

		// 텍스쳐 경로가 유효한지 확인 후, 로드
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

	// 임시로 생성한 버퍼 해제
	matBuffer->Release();

    return S_OK;
}

void SolarSystem::DrawMesh(Model* model)
{
	auto device = GET_SINGLE(Device)->GetDevice();

	// 재질 개수만큼 그리도록
	for (int i = 0; i < model->matCnt; ++i)
	{
		// 재질과 텍스쳐 설정
		device->SetMaterial(&model->mat[i]);
		device->SetTexture(0, model->tex[i]);

		// 행렬 변환을 통해 크기, 회전, 위치 수정
		D3DXMATRIX matScale, matRot, matTrans, matOrbit;

		// 모델 원본 사이즈 그대로
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		// 자전
		D3DXMatrixIdentity(&matRot);
		// 위치
		D3DXMatrixTranslation(&matTrans, model->pos.x, model->pos.y, model->pos.z);
		// 공전
		D3DXMatrixIdentity(&matOrbit);

		// 태양이 아닐 경우만 공전하도록
		if (model->data.name != L"sun")
		{
			// 모델 기저축이 우리가 원하는 방향이 아니므로, 자전을 이용해 모델을 임의로 회전
			D3DXMatrixRotationX(&matRot, 60.f * D3DX_PI / 180.f);

			auto d = GetTickCount64() % ((int)((D3DX_PI * 2.f) * 1000.f));
			D3DXMatrixRotationY(&matOrbit, d / 1000.f);
		}

		// 부모가 존재한다면
		if (model->parent != nullptr)
		{
			model->matWorld = matScale * matRot * matTrans * matOrbit * (model->parent->matWorld);
		}
		else
		{
			model->matWorld = matScale * matRot * matTrans;
		}

		// 월드행렬 적용
		device->SetTransform(D3DTS_WORLD, &model->matWorld);

		// 메시 그리기
		model->mesh->DrawSubset(i);
	}
}

Model* SolarSystem::GetPlanet(const wchar_t* name)
{
	// 행성 데이터를 순회하며, 파라미터로 받은 이름과 동일한 이름이 있다면 반환
	for (int i = 0; i < MaxPlanetCnt; ++i)
	{
		if (lstrcmpW(planets[i].data.name, name) == 0)
		{
			return &planets[i];
		}
	}

    return nullptr;
}
