#pragma once
#include "Scene.h"
#include "SolarSystem.h"
#include "SkyBox.h"

class Ingame :
    public Scene
{
private:
	D3DXVECTOR3 eye, lookAt, up; // ��ġ, ���� ����, ���� ���⺤��
	D3DXMATRIX view, projection; // ��, �������� ���
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

