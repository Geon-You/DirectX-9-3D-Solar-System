#pragma once
#include "framework.h"

const int WIN_SIZE_X = 800;
const int WIN_SIZE_Y = 600;

extern HWND g_hWnd;

// ������ ���
enum WinMode
{
	FULL, // Ǯ��ũ��
	WIN,  // â���
};

// �� ������ ����
enum SceneType
{
	SCENE_LOGO,
	SCENE_MENU,
	SCENE_INGAME,
	SCENE_END,
};

// �ؽ��� Ÿ���� ����
enum TexType
{
	Single,
	Multi,
};

// ��ü ������ ����
enum ObjType
{
	OBJ_PLAYER,
	OBJ_MONSTER,
	OBJ_END,
};

// �ؽ��� ��ü�� �̹��� ���� ������ ���� ����ü
// �̹��� -> ���� ����
// �ؽ��� -> �̹��� ������ �о �޸� �� �ö� ������
typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9 texture; // �ؽ��ĸ� ���� �ʵ�
	D3DXIMAGE_INFO imgInfo; // �̹���(����)�� ����
}TEXINFO;

// ��� ���Ͱ� �������� ���� �����͸� ��Ƴ��� ����ü
typedef struct tagInfo
{
	D3DXVECTOR3 pos;	// ��ġ (���� ��ġ)
	D3DXVECTOR3 dir;	// ���� ����
	D3DXVECTOR3 look;	// �ʱ� ������ ��Ÿ���� ���� ����
	D3DXVECTOR3 center; // ���� ���� �߽����� ��ǥ
	D3DXMATRIX world;   // ���� ���

	tagInfo() {}
	tagInfo(const tagInfo& info)
		:pos(info.pos), dir(info.dir), look(info.look),
		center(info.center), world(info.world)
	{}
}INFO;

// �ִϸ��̼� ó���� ���Ǵ� �����͸� ��Ƴ��� ����ü
typedef struct tagFrame
{
	int animType; // ���� �ִϸ��̼� Ÿ�� (���° ��?)
	int current; // ���� �ִϸ��̼ǿ����� �� (���° ��?)
	int last; // ���� �ִϸ��̼��� ������ ���� �ε���
	DWORD time;

	tagFrame() {}
	tagFrame(int _animType, int _current, int _last, DWORD _time)
		: animType(_animType), current(_current), last(_last), time(_time) {}
}FRAME;

// cnt x * cnt y �� ���� ũ�Ⱑ ������
const int TILE_CNT_X = 30;
const int TILE_CNT_Y = 30;

// �ϳ��� Ÿ�� �⺻ ������
const int TILE_SIZE_X = 64;
const int TILE_SIZE_Y = 64;

typedef struct tagTile
{
	float posX;
	float posY;
	float sizeX;
	float sizeY;
	int drawId;
	bool isColl;
}TILE;

// astar �˰��� 
// -> ��ǥ���� �޸���ƽ �˰���
// 

typedef struct AstarNode
{
					   // F = G + H 
	float	fCost;     // ��� = �θ� �������� �Ÿ� + ���� �������� �Ÿ�
	int		idx;       // Ÿ�� �ε���	
	AstarNode* parent; // �������� �Ǵ� ���
}NODE;

// ���� �� ���� Ű���� �Ʒ��� ������ ����� ����
const DWORD KEY_UP		= 0b00000001;
const DWORD KEY_DOWN	= 0b00000010;
const DWORD KEY_LEFT	= 0b00000100;
const DWORD KEY_RIGHT	= 0b00001000;
const DWORD KEY_SPACE	= 0b00010000;
const DWORD KEY_LM		= 0b00100000;

// �÷��̾� ���� ����
const DWORD STATE_IDLE	= 0b00000001;
const DWORD STATE_WALK	= 0b00000010;
const DWORD STATE_ATK	= 0b00000100;
const DWORD STATE_HIT	= 0b00001000; 
const DWORD STATE_DEAD	= 0b00010000;

typedef struct Ray
{
	D3DXVECTOR3 origin; // ������ ������
	D3DXVECTOR3 dir;    // ������ ����
}Ray;

typedef struct CUSTOMVERTEX
{
	D3DXVECTOR3 position;
	DWORD color;
	float tu; // �ؽ��� UV ��ǥ �� U ����
	float tv; // �ؽ��� UV ��ǥ �� V ����
} VERTEX, *LPVERTEX;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL)
#define D3DFVF_CUSTOMVERTEX_TEX (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE)
#define D3DFVF_CUSTOMVERTEX_ANIM (D3DFVF_XYZ | D3DFVF_DIFFUSE)

typedef struct ModelData
{
	wchar_t* parent; // �θ� �༺�� �̸�
	wchar_t* name; // �ڽ��� �̸�

	ModelData() {}
	ModelData(const wchar_t* parent, const wchar_t* name)
		: parent((wchar_t*)parent), name((wchar_t*)name) { }
}MD;

// ����� �ִ� �༺�� ����
const int MaxPlanetCnt = 9;
// ����� �ִ� ������ ����
const int MaxSatelliteCnt = 1;

// �¾翡�� ���������� �Ÿ��� 1�̶�� �������� ��,
// �¾�� �� �༺ ���� ���Ÿ� ����
const float relativeDistance[] =
{
	0, .4f, .7f, 1.f, 1.5f, 5.f, 10.f, 20.f, 30.f
};

// �¾�, ������ȭ����õ��
// �༺ ���� ���踦 ��Ÿ���� ������
const MD PlanetData[] =
{
	MD(L"", L"sun"),
	MD(L"sun", L"mercury"),
	MD(L"sun", L"venus"),
	MD(L"sun", L"earth"),
	MD(L"sun", L"mars"),
	MD(L"sun", L"jupiter"),
	MD(L"sun", L"saturn"),
	MD(L"sun", L"uranus"),
	MD(L"sun", L"neptune"),
};

// �༺�� ���� ���� ���踦 ��Ÿ���� ������
const MD SatelliteData[] =
{
	MD(L"earth", L"moon"),
};

class Model
{
public:
	MD data; // �ڽ��� �̸��� �θ��� �̸��� ���� �ʵ�
	Model* parent; // �θ� �༺ ��ü
	D3DXVECTOR3 pos; // ��ġ
	D3DXMATRIX matWorld; // ���� ���
	LPD3DXMESH mesh; // �޽�
	D3DMATERIAL9* mat; // ����
	LPDIRECT3DTEXTURE9* tex; // �ؽ���
	DWORD matCnt; // �޽ÿ��� ��� ���� ���� ����
};
