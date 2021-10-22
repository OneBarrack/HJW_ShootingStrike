#pragma once
#include "Object.h"
#include "BulletSpawnPatternScript.h"

class Player : public Object
{
public:
	typedef Object Super;

private:
	BulletSpawnPatternScript bulletScript;

	int damage;
	int life;
	int maxLife;
	int power;
	int maxPower;

	// ** ����������
	bool isSpawing;

	// ** ����������
	bool bReSpawn; 

	// ** �׾�����
	bool isDied;

	// ** ��������
	bool isInvicible;

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
	// ** ���� ��������
	bool IsInvicible() { return isInvicible; }

	// ** Life ��ȯ
	int GetLife() { return life; }

	// ** Power,Life Up / Down
	void PowerUp(bool _bMax = false);
	void PowerDown(bool _bMin = false);
	void LifeUp(bool _bMax = false);
	void LifeDown(bool _bMin = false);

	// ** �̻��� �߻�
	void Fire(int _power, int _damage);

	// ** �������� ����
	void ApplyDamage(Object* _pTarget, int _damage);

	// ** �������� ����
	void TakeDamage(int _damage);

	// ** Spawn
	void Spawn();

	// ** ReSpawn
	void ReSpawn();

	// ** ����
	void Die();

private:
	// ** ���� �÷��̾� ���� üũ
	void CheckStatus();

	// ** Stage ������ ������� üũ
	void CheckPositionInBkgBoundary();

	// ** �÷��̾��� ĳ���Ͱ� �����Ǵ� �ִϸ��̼� ����. 
	// ** �߻����� : ���� ��ŸƮ / ���� �� �ٽ� ��Ƴ��� ��Ȳ
	void RenderSpawn(HDC _hdc);

	// ** �÷��̾��� �̵����⿡ ���� ��ü �ִϸ��̼� ����. 
	void RenderPlayer(HDC _hdc);

public:
	Player();
	Player(const Transform& _rTransInfo) : Object(_rTransInfo) { }
	virtual ~Player();
};

