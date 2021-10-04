#pragma once
#include "Object.h"
#include "BulletSpawnPatternScript.h"

class Player : public Object
{
public:
	typedef Object Super;

private:
	BulletSpawnPatternScript bulletScript;

	int HP;
	int damage;
	int level;
	eBulletType bulletType;

	bool bSpawing;
	bool bAttacking;
	bool bTakeDamage;
	bool bDied;

	Vector3 oldPosition;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;	
	virtual void OnCollision(Object* _pObject) override;

	virtual Object* Clone() override { return new Player(*this); }

public:
	// ** Spawn / ReSpawn
	void Spawn() { bSpawing = true; };

	// ** �̻��� �߻�
	void Fire(eBulletType _bulletType, int _power, int _damage);

	// ** �������� ����
	void ApplyDamage(Object* _pTarget, int _damage);

	// ** �������� ����
	void TakeDamage(int _damage);

	// ** ����
	void Die() { bDied = true; };

private:
	// ** Stage ������ ������� üũ
	void CheckPositionInBkgBoundary();

	// ** �÷��̾��� ĳ���Ͱ� �����Ǵ� �ִϸ��̼� ����. 
	// ** �߻����� : ���� ��ŸƮ / ���� �� �ٽ� ��Ƴ��� ��Ȳ
	bool RenderSpawn(HDC _hdc);

	// ** �÷��̾��� �̵����⿡ ���� ��ü �ִϸ��̼� ����. 
	void RenderPlayer(HDC _hdc);

public:
	Player();
	Player(const Transform& _rTransInfo) : Object(_rTransInfo) { }
	virtual ~Player();
};

