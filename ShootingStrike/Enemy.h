#pragma once
#include "Object.h"

class Enemy : public Object
{
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

	// ** �������� ����Ǿ�����
	bool bChangedDestPos;

	// ** ���������� ���߾�� �ϴ���
	bool bStopAtDest;

	// ** �������� ���� �ߴ���
	bool bArrivedToDest;

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

	bool IsDead() { return bDied; }
	bool IsSpawning() { return isSpawing; }
	int GetMaxHP() { return maxHP; }
	int GetHP() { return HP; }
	int GetHitPoint() { return hitScore; }
	int GetDeathPoint() { return deathScore; }
	int GetFireBulletIntervalTime() { return fireBulletIntervalTime; }
	Vector3 GetDestPosition() { return destPosition; }
	bool GetChangedDestPos() { return bChangedDestPos; }
	bool GetStopAtDest() { return bStopAtDest; }
	bool GetArrivedToDest() { return bArrivedToDest; }

	void SetMaxHP(int _maxHP) { maxHP = _maxHP; }
	void SetHP(int _HP) { HP = _HP; }
	void SetHitPoint(int _point) { hitScore = _point; }
	void SetDeathPoint(int _point) { deathScore = _point; }
	void SetFireBulletIntervalTime(int _time) { fireBulletIntervalTime = _time; }
	void SetDestPosition(Vector3 _pos) { destPosition = _pos; bChangedDestPos = true; }
	void SetChangedDestPos(bool _bChanged) { bChangedDestPos = _bChanged; }
	void SetStopAtDest(bool _stopAtDest) { bStopAtDest = _stopAtDest; }
	void SetArrivedToDest(bool _arrivedToDest) { bArrivedToDest = _arrivedToDest; }
	void SetSpawnStatus(bool _isSpawing) { isSpawing = _isSpawing; }

private:
	// ** Stage ������ ������� üũ
	void CheckPositionInBkgBoundary();

public:
	Enemy();
	Enemy(const Transform& _rTransInfo) : Object(_rTransInfo) { }
	virtual ~Enemy();
};
