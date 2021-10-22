#pragma once
#include "Scene.h"
#include "EnemySpawnPatternScript.h"

class Object;
class Stage : public Scene
{
private:
	int frame;
	ULONGLONG startTime;
	ULONGLONG currentTime;

	Object* pPlayer;
	Object* pStageDummyEnemy;
	Object* pBackground;
	Object* pLeftSideBackground;
	Object* pRightSideBackground;
	Object* pPlayTimeTextUI;
	Object* pScoreTextUI;
	Object* pScoreUI;
	Object* pLifeTextUI;
	Object* pLifeUI;
	Object* pGameOverUI;
	Object* pGameClearUI;
	Object* pBombTextUI;
	Object* pBombUI;
	Object* pBossAngelEnemy;
	Object* pBossEnemyProgressBar;
	Object* pMapProgress;	

	// ** ������ ��ȯ �Ǿ�����
	bool bSpawnedBossEnemy;

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

public:
	ULONGLONG GetPlayTime() { return currentTime - startTime; }

private:
	// ** ����Ÿ��� ���� Enemy Spawn Script List �ʱ� ���尪 ����
	void InitEnemySpawnPatternTimings();

	// ** Stage Start
	void Start();

public:
	Stage();
	virtual ~Stage();
};