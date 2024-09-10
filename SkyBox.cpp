#include "SkyBox.h"
#include "Device.h"

void SkyBox::Initialize()
{
	auto device = GET_SINGLE(Device)->GetDevice();

	// 큐브 텍스쳐 로드
	if (FAILED(D3DXCreateCubeTextureFromFile(device, L"SolarSystem/Skybox.dds", &cubeTex)))
		return;

	InitVB();
	InitIB();
}

void SkyBox::Update()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, 1000.f, 1000.f, 1000.f);
	// 스카이박스가 카메라를 따라다니도록, 카메라의 위치로 설정한다면
	// 카메라가 어디로 이동하든, 항상 배경이 스카이 박스로 보이게 됨
	D3DXMatrixTranslation(&matTrans, 0,0,0);

	matWorld = matScale * matTrans;
}

void SkyBox::Render()
{
	auto device = GET_SINGLE(Device)->GetDevice();

	// 큐브 내의 텍스쳐를 카메라로 봐야하기 때문에
	// 스카이 박스를 그릴 때, 컬링이 꺼져 있어야 함
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	device->SetTexture(0, cubeTex);
	device->SetStreamSource(0, vb, 0, sizeof(CubeVertex));
	device->SetFVF(D3DFVF_CUBETEX);

	device->SetIndices(ib);
	
	device->SetTransform(D3DTS_WORLD, &matWorld);

	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

void SkyBox::Release()
{
	if (cubeTex)
	{
		cubeTex->Release();
		cubeTex = nullptr;
	}

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

void SkyBox::InitVB()
{
	auto device = GET_SINGLE(Device)->GetDevice();

	// 정점 버퍼 생성
	if (FAILED(device->CreateVertexBuffer(
		sizeof(CubeVertex) * 8, NULL, D3DFVF_CUBETEX, D3DPOOL_DEFAULT, &vb, NULL)))
		return;

	CubeVertex* pVertex = nullptr;
	// 정점 버퍼 락
	vb->Lock(0, 0, (void**)&pVertex, NULL);

	// 앞쪽
	pVertex[0].pos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVertex[0].tex = pVertex[0].pos;

	pVertex[1].pos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVertex[1].tex = pVertex[1].pos;

	pVertex[2].pos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVertex[2].tex = pVertex[2].pos;

	pVertex[3].pos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVertex[3].tex = pVertex[3].pos;

	// 뒤쪽
	pVertex[4].pos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVertex[4].tex = pVertex[4].pos;

	pVertex[5].pos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVertex[5].tex = pVertex[1].pos;

	pVertex[6].pos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVertex[6].tex = pVertex[6].pos;

	pVertex[7].pos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVertex[7].tex = pVertex[7].pos;

	vb->Unlock();
}

void SkyBox::InitIB()
{
	auto device = GET_SINGLE(Device)->GetDevice();
	// 인덱스 버퍼 생성
	if (FAILED(device->CreateIndexBuffer(sizeof(MYINDEX) * 12, NULL,
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &ib, NULL)))
		return;

	MYINDEX* pIndex = nullptr;
	int triCnt = 0;
	// 인덱스 버퍼 락
	ib->Lock(0, 0, (void**)&pIndex, NULL);

	// +x
	pIndex[triCnt]._1 = 1; pIndex[triCnt]._2 = 5; pIndex[triCnt]._3 = 6;
	++triCnt;
	pIndex[triCnt]._1 = 1; pIndex[triCnt]._2 = 6; pIndex[triCnt]._3 = 2;
	++triCnt;

	// -x
	pIndex[triCnt]._1 = 4; pIndex[triCnt]._2 = 0; pIndex[triCnt]._3 = 3;
	++triCnt;
	pIndex[triCnt]._1 = 4; pIndex[triCnt]._2 = 3; pIndex[triCnt]._3 = 7;
	++triCnt;

	// +y
	pIndex[triCnt]._1 = 4; pIndex[triCnt]._2 = 5; pIndex[triCnt]._3 = 1;
	++triCnt;
	pIndex[triCnt]._1 = 4; pIndex[triCnt]._2 = 1; pIndex[triCnt]._3 = 0;
	++triCnt;

	// -y
	pIndex[triCnt]._1 = 7; pIndex[triCnt]._2 = 6; pIndex[triCnt]._3 = 2;
	++triCnt;
	pIndex[triCnt]._1 = 7; pIndex[triCnt]._2 = 2; pIndex[triCnt]._3 = 3;
	++triCnt;

	// +z
	pIndex[triCnt]._1 = 4; pIndex[triCnt]._2 = 5; pIndex[triCnt]._3 = 6;
	++triCnt;
	pIndex[triCnt]._1 = 4; pIndex[triCnt]._2 = 6; pIndex[triCnt]._3 = 7;
	++triCnt;

	// -z
	pIndex[triCnt]._1 = 0; pIndex[triCnt]._2 = 1; pIndex[triCnt]._3 = 2;
	++triCnt;
	pIndex[triCnt]._1 = 0; pIndex[triCnt]._2 = 2; pIndex[triCnt]._3 = 3;
	++triCnt;

	ib->Unlock();
}
