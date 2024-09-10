#pragma once
#include "Include.h"

class SolarSystem
{
private:
	// 행성 데이터 배열
	Model planets[MaxPlanetCnt];
	// 위성 데이터 배열
	Model satellite[MaxSatelliteCnt];

public:
	void Initialize();
	void Update();
	void Render();
	void Release();

private:
	HRESULT LoadXFile(Model* model);
	void DrawMesh(Model* model);
	// 행성 또는 위성의 부모를 찾을 때 사용
	Model* GetPlanet(const wchar_t* name);
};

