#include "Headers.h"

class GameDebugManager
{
private:
	enum class eDebugMode
	{
		NONE,
		PRINT_TEXTINFO,
		DRAW_COLLISION,
		ALL
	};

private:
	static GameDebugManager* pInstance;
public:
	static GameDebugManager* GetInstance()
	{
		if ( pInstance == nullptr )
			pInstance = new GameDebugManager;

		return pInstance;
	}

private:
	eDebugMode bDebugMode;

public:
	void Initialize();
	void Update();
	void Render(HDC _hdc);
	void Release();

private:
	// ** Game Info�� ���� Text ���
	void PrintTextForGameInfo(HDC _hdc);

	// ** Collision Box �ٿ���� ������
	void DrawCollisionBoundary(HDC _hdc);

public:
	GameDebugManager();
	~GameDebugManager();
};