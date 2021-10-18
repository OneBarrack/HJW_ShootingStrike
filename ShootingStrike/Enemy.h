#pragma once
#include "Object.h"

class Enemy : public Object
{
	enum class eMoveState
	{
		READY	,
		MOVING	,
		END		,
	};

	enum class eMoveType
	{
		MOVE_TO						,
		SPIN_LEFT_FOR_LOOP			,
		SPIN_LEFT					,
		SPIN_RIGHT_FOR_LOOP			,
		SPIN_RIGHT					,
		BACK_AND_FORTH_LEFT			,
		BACK_AND_FORTH_RIGHT		,
	};

public:
	typedef Object Super;

private:
	int maxHP;
	int HP;	

	bool bSpawing;
	bool bAttacking;
	bool bTakeDamage;
	bool bDied;

	int hitScore;
	int deathScore;

	Vector3 oldPosition;

	// ** ����������
	bool isSpawing;

	// ** �ֱ��� Bullet �߻� ����
	int fireBulletIntervalTime;

	// ** ������
	Vector3 destPosition;

	// ** Move Info
	queue<pair<eMoveType, Vector3>> moveInfos;
	eMoveState moveState;

public:
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Render(HDC _hdc)override;
	virtual void Release()override;
	virtual void OnCollision(Object* _pObject)override;

	virtual Object* Clone()override { return new Enemy(*this); }

public:
	// ** �̻��� �߻�
	void Fire();

	// ** �������� ����
	void ApplyDamage(Object* _pTarget, int _Damage);

	// ** �������� ����
	void TakeDamage(int _Damage);

	// ** ����
	void Spawn(); 

	// ** ����
	void Die();

	// ** Move Info �ʱ�ȭ
	void InitMoveInfo();

	bool IsDead() { return bDied; }
	bool IsSpawning() { return isSpawing; }
	int GetMaxHP() { return maxHP; }
	int GetHP() { return HP; }
	int GetHitPoint() { return hitScore; }
	int GetDeathPoint() { return deathScore; }
	int GetFireBulletIntervalTime() { return fireBulletIntervalTime; }
	Vector3 GetDestPosition() { return destPosition; }
	bool isMoving();

	void SetMaxHP(int _maxHP) { maxHP = _maxHP; }
	void SetHP(int _HP) { HP = _HP; }
	void SetHitPoint(int _point) { hitScore = _point; }
	void SetDeathPoint(int _point) { deathScore = _point; }
	void SetFireBulletIntervalTime(int _time) { fireBulletIntervalTime = _time; }
	void SetSpawnStatus(bool _isSpawing) { isSpawing = _isSpawing; }

	// ** �������� �̵�
	void MoveTo(Vector3 _destPosition);

	// ** �� ��ġ���� Spin. 
	void SpinLeft(bool _bLoop = false);
	void SpinRight(bool _bLoop = false);

	// ** �¿�� �Դٰ��� �ϸ� ����. destPosition�� ���� �� ���� ����
	void BackAndForthLeft(Vector3 _destPosition = Vector3());
	void BackAndForthRight(Vector3 _destPosition = Vector3());

private:
	// ** Stage ������ ������� üũ
	void CheckPositionInBkgBoundary();

public:
	Enemy();
	Enemy(const Transform& _rTransInfo) : Object(_rTransInfo) { }
	virtual ~Enemy();
};
