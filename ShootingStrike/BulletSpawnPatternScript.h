#pragma once
#include "SpawnPatternScript.h"

class BulletSpawnPatternScript : public SpawnPatternScript
{
private:
	// ** ��ü Object
	Object* pOwner;

	// ** Spawn Pattern
	eBulletSpawnPattern spawnPattern;

	// ** Spawn Position
	Vector3 spawnPosition;

	// ** ������
	int damage;

	// ** Run �� �غ� �Ǿ�����
	bool bReady;
	
public:	
	// ** �ʱ�ȭ
	void Initialize(Object* _pOwner = nullptr);

	// ** Pattern �� ������ ����
	void ReadyToSpawn(eBulletSpawnPattern _spawnPattern, Vector3 _spawnPosision, int _damage);

	// ** ���� ������
	bool IsRunning() { return !bReady; }

	// ** ���� ����
	virtual void Run() override;
	// ������, �𷺼�, �̸� �������������� �޾ƾ��ϴ°�?
public:
	BulletSpawnPatternScript();
	~BulletSpawnPatternScript();
};

