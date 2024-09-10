#pragma once
#include "Scene.h"
#include "SolarSystem.h"
#include "SkyBox.h"

class Ingame :
    public Scene
{
private:
	D3DXVECTOR3 eye, lookAt, up; // 위치, 보는 방향, 월드 상향벡터
	D3DXMATRIX view, projection; // 뷰, 프로젝션 행렬
	SolarSystem solarSystem;
	SkyBox skyBox;

public:
    void Initialize() override;
    int Update() override;
    void Render() override;
    void Release() override;
	virtual void OnMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

public:
    Ingame();
    virtual ~Ingame();
};

