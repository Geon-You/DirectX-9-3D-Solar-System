#pragma once
#include "Include.h"

class SolarSystem
{
private:
	// �༺ ������ �迭
	Model planets[MaxPlanetCnt];
	// ���� ������ �迭
	Model satellite[MaxSatelliteCnt];

public:
	void Initialize();
	void Update();
	void Render();
	void Release();

private:
	HRESULT LoadXFile(Model* model);
	void DrawMesh(Model* model);
	// �༺ �Ǵ� ������ �θ� ã�� �� ���
	Model* GetPlanet(const wchar_t* name);
};

