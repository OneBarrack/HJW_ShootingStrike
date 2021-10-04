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

	// ** ����������
	bool bSpawing;

	// ** �׾�����
	bool bDied;

	// ** ��������
	bool bInvicible;

	// ** Ű �Է��� ��������
	bool bCantAccessInput;

	Vector3 oldPosition;

	// ** ���� �� ������ Timer
	ULONGLONG cantAccessInputTimer;
	int cantAccessInputDurationTime;

	// ** ���� Timer
	ULONGLONG invincibleTimer;
	int invicibleDurationTime;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;	
	virtual void OnCollision(Object* _pObject) override;

	virtual Object* Clone() override { return new Player(*this); }

public:
	// ** �̻��� �߻�
	void Fire(eBulletType _bulletType, int _power, int _damage);

	// ** �������� ����
	void ApplyDamage(Object* _pTarget, int _damage);

	// ** �������� ����
	void TakeDamage(int _damage);

	// ** Spawn / ReSpawn
	void Spawn();

	// ** ����
	void Die();

private:
	// ** ���� �÷��̾� ���� üũ
	void CheckStatus();

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

