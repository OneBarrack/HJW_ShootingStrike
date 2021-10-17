#pragma once
#include "Scene.h"
#include "EnemySpawnPatternScript.h"

class Object;
class Stage : public Scene
{
private:
	int frame;

private:
	Object* pPlayer;
	Object* pBackground;
	Object* pLeftSideBackground;
	Object* pRightSideBackground;
	Object* pScoreTextUI;
	Object* pScoreUI;
	Object* pLifeTextUI;
	Object* pLifeUI;
	Object* pBossAngelEnemy;
	Object* pBossEnemyProgressBar;
	Object* pMapProgress;

	// ** Enemy Spawn Script
	EnemySpawnPatternScript enemyScript;

	// ** Enemy Spawn Pattern �ߵ� Ÿ�̹� ����
	// ** First : Map Progress Percentage, Second : eEnemySpawnPattern
	priority_queue<	
		pair<float, eEnemySpawnPattern>, 
		vector<pair<float, eEnemySpawnPattern>>,
		greater<pair<float, eEnemySpawnPattern>> > enemySpawnTimings;

public:
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Render(HDC _hdc)override;
	virtual void Release()override;

private:
	// ** ����Ÿ��� ���� Enemy Spawn Script List �ʱ� ���尪 ����
	void InitEnemySpawnPatternTimings();

private:
	// ** Stage Start
	void Start();

public:
	Stage();
	virtual ~Stage();
};