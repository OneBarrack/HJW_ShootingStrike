#pragma once
#include "SpawnPatternScript.h"

class EnemySpawnPatternScript : public SpawnPatternScript
{
private:
	// ** �غ�� ���ϵ��� ��Ƴ��� List
	list<pair<Object*, eEnemySpawnPattern>> readyPatternList;

public:
	// ** �ʱ�ȭ
	void Initialize();

	// ** Pattern �� ������ ����
	void ReadyToSpawn(Object* _pOwner, eEnemySpawnPattern _spawnPattern);

	// ** ���� ����
	virtual void Run() override;

private:
	// ** ������Ʈ ���� ���� 
	virtual void Spawn() override;

	// ** ���� ���鿡 ���� ������Ʈ
	virtual void Update() override;

public:
	EnemySpawnPatternScript();
	~EnemySpawnPatternScript();
};

