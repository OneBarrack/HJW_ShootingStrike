#include "Headers.h"

class GameDataManager
{
private:
	static GameDataManager* pInstance;

public:
	static GameDataManager* GetInstance()
	{
		if ( pInstance == nullptr )
			pInstance = new GameDataManager;

		return pInstance;
	}

private:
	Vector3 playerPos;
	int playerLife;
	int score;
	int FPS;
	float mapProgressRatio;

public:
	void Initialize();
	void Update();

	Vector3 GetPlayerPos() { return playerPos; }
	int GetPlayerLife() { return playerLife; }
	int GetScore() { return score; }
	int GetFPS() { return FPS; }
	float GetMapProgressRatio() { return mapProgressRatio; }

	void AddScore(int _score);

private:
	// ** FPS ���
	void CalcFPS();

public:
	GameDataManager();
	~GameDataManager();
};