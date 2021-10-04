#pragma once
#include "SpawnPatternScript.h"

class BulletSpawnPatternScript : public SpawnPatternScript
{
private:
	// ** ��ü Object
	Object* pOwner;

	// ** Spawn Pattern
	eBulletSpawnPattern spawnPattern;

	// ** ������
	int damage;

	// ** Run �� �غ� �Ǿ�����
	bool bReady;
	
public:	
	// ** �ʱ�ȭ
	void Initialize(Object* _pOwner = nullptr);

	// ** Pattern �� ������ ����
	void ReadyToSpawn(eBulletSpawnPattern _SpawnPattern, int _damage);

	// ** ���� ������
	bool IsRunning() { return !bReady; }

	// ** ���� ����
	virtual void Run() override;
	// ������, �𷺼�, �̸� �������������� �޾ƾ��ϴ°�?
public:
	BulletSpawnPatternScript();
	~BulletSpawnPatternScript();
};

