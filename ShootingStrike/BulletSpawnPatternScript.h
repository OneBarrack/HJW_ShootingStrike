#pragma once
#include "SpawnPatternScript.h"

class BulletSpawnPatternScript : public SpawnPatternScript
{
private:
	// ** ��ü Object
	Object* pOwner;

	// ** Spawn Pattern
	eBulletSpawnPattern spawnPattern;

	// ** Spawn Transform
	Transform spawnTransInfo;

	// ** ������
	int damage;

	// ** Run �� �غ� �Ǿ�����
	bool bReady;

	// ** N���� Polygon pattern���� ���� ���ؽ� ��
	int polygonVtxCount;

public:	
	// ** �ʱ�ȭ
	void Initialize();

	// ** Pattern �� ������ ����
	void ReadyToSpawn(Object* _pOwner, eBulletSpawnPattern _spawnPattern, Transform _spawnTransInfo, int _damage, int _polygonVtxCount = 0);

	// ** ���� ������
	bool IsRunning() { return !bReady; }

	// ** ���� ����
	virtual void Run() override;

private:
	// ** ������Ʈ ���� ���� 
	virtual void Spawn() override;

	// ** ���� ���鿡 ���� ������Ʈ
	virtual void Update() override;

public:
	BulletSpawnPatternScript();
	~BulletSpawnPatternScript();
};

