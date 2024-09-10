#pragma once
#include "framework.h"

const int WIN_SIZE_X = 800;
const int WIN_SIZE_Y = 600;

extern HWND g_hWnd;

// 윈도우 모드
enum WinMode
{
	FULL, // 풀스크린
	WIN,  // 창모드
};

// 씬 종류를 열거
enum SceneType
{
	SCENE_LOGO,
	SCENE_MENU,
	SCENE_INGAME,
	SCENE_END,
};

// 텍스쳐 타입을 열거
enum TexType
{
	Single,
	Multi,
};

// 객체 종류를 열거
enum ObjType
{
	OBJ_PLAYER,
	OBJ_MONSTER,
	OBJ_END,
};

// 텍스쳐 객체와 이미지 원본 정보를 갖는 구조체
// 이미지 -> 파일 원본
// 텍스쳐 -> 이미지 파일을 읽어서 메모리 상에 올라간 데이터
typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9 texture; // 텍스쳐를 담을 필드
	D3DXIMAGE_INFO imgInfo; // 이미지(원본)의 정보
}TEXINFO;

// 모든 액터가 공통으로 갖는 데이터를 모아놓은 구조체
typedef struct tagInfo
{
	D3DXVECTOR3 pos;	// 위치 (월드 위치)
	D3DXVECTOR3 dir;	// 방향 벡터
	D3DXVECTOR3 look;	// 초기 방향을 나타내는 단위 벡터
	D3DXVECTOR3 center; // 로컬 기준 중심점의 좌표
	D3DXMATRIX world;   // 월드 행렬

	tagInfo() {}
	tagInfo(const tagInfo& info)
		:pos(info.pos), dir(info.dir), look(info.look),
		center(info.center), world(info.world)
	{}
}INFO;

// 애니메이션 처리에 사용되는 데이터를 모아놓은 구조체
typedef struct tagFrame
{
	int animType; // 현재 애니메이션 타입 (몇번째 행?)
	int current; // 현재 애니메이션에서의 열 (몇번째 열?)
	int last; // 현재 애니메이션의 마지막 열의 인덱스
	DWORD time;

	tagFrame() {}
	tagFrame(int _animType, int _current, int _last, DWORD _time)
		: animType(_animType), current(_current), last(_last), time(_time) {}
}FRAME;

// cnt x * cnt y 로 맵의 크기가 결정됨
const int TILE_CNT_X = 30;
const int TILE_CNT_Y = 30;

// 하나의 타일 기본 사이즈
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

// astar 알고리즘 
// -> 대표적인 휴리스틱 알고리즘
// 

typedef struct AstarNode
{
					   // F = G + H 
	float	fCost;     // 비용 = 부모 노드까지의 거리 + 도착 노드까지의 거리
	int		idx;       // 타일 인덱스	
	AstarNode* parent; // 시작점이 되는 노드
}NODE;

// 게임 내 사용될 키들을 아래에 별도의 상수로 정의
const DWORD KEY_UP		= 0b00000001;
const DWORD KEY_DOWN	= 0b00000010;
const DWORD KEY_LEFT	= 0b00000100;
const DWORD KEY_RIGHT	= 0b00001000;
const DWORD KEY_SPACE	= 0b00010000;
const DWORD KEY_LM		= 0b00100000;

// 플레이어 상태 정의
const DWORD STATE_IDLE	= 0b00000001;
const DWORD STATE_WALK	= 0b00000010;
const DWORD STATE_ATK	= 0b00000100;
const DWORD STATE_HIT	= 0b00001000; 
const DWORD STATE_DEAD	= 0b00010000;

typedef struct Ray
{
	D3DXVECTOR3 origin; // 광선의 시작점
	D3DXVECTOR3 dir;    // 광선의 방향
}Ray;

typedef struct CUSTOMVERTEX
{
	D3DXVECTOR3 position;
	DWORD color;
	float tu; // 텍스쳐 UV 좌표 중 U 성분
	float tv; // 텍스쳐 UV 좌표 중 V 성분
} VERTEX, *LPVERTEX;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL)
#define D3DFVF_CUSTOMVERTEX_TEX (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE)
#define D3DFVF_CUSTOMVERTEX_ANIM (D3DFVF_XYZ | D3DFVF_DIFFUSE)

typedef struct ModelData
{
	wchar_t* parent; // 부모 행성의 이름
	wchar_t* name; // 자신의 이름

	ModelData() {}
	ModelData(const wchar_t* parent, const wchar_t* name)
		: parent((wchar_t*)parent), name((wchar_t*)name) { }
}MD;

// 사용할 최대 행성의 개수
const int MaxPlanetCnt = 9;
// 사용할 최대 위성의 개수
const int MaxSatelliteCnt = 1;

// 태양에서 지구까지의 거리가 1이라고 가정했을 때,
// 태양과 각 행성 간의 상대거리 비율
const float relativeDistance[] =
{
	0, .4f, .7f, 1.f, 1.5f, 5.f, 10.f, 20.f, 30.f
};

// 태양, 수금지화목토천해
// 행성 간의 관계를 나타내는 데이터
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

// 행성과 위성 간의 관계를 나타내는 데이터
const MD SatelliteData[] =
{
	MD(L"earth", L"moon"),
};

class Model
{
public:
	MD data; // 자신의 이름과 부모의 이름을 갖는 필드
	Model* parent; // 부모 행성 객체
	D3DXVECTOR3 pos; // 위치
	D3DXMATRIX matWorld; // 월드 행렬
	LPD3DXMESH mesh; // 메시
	D3DMATERIAL9* mat; // 재질
	LPDIRECT3DTEXTURE9* tex; // 텍스쳐
	DWORD matCnt; // 메시에서 사용 중인 재질 개수
};
