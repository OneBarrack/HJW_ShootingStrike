#pragma once
#include "Object.h"

class Player : public Object
{
private:
	Bitmap* pPlayerImage;	

	int HP;
	int Damage;
	int Power;
	eBulletFireType FireType;

	bool bSpawing;
	bool bAttacking;
	bool bTakeDamage;
	bool bDied;

	int Frame;

	Vector3 OldPosition;
public:
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Render(HDC _hdc)override;
	virtual void Release()override;	
	virtual void OnCollision(Object* _pObject)override;

	virtual Object* Clone()override { return new Player(*this); }

public:
	// ** Spawn / ReSpawn
	void SpawnPlayer() { bSpawing = true; };

	// ** �̻��� �߻�
	void Fire(eBulletFireType _FireType, int _Power, int _Damage);

	// ** �������� ����
	void ApplyDamage(Object* _pTarget, int _Damage);

	// ** �������� ����
	void TakeDamage(int _Damage);

	// ** ����
	void Die() { bDied = true; };

private:
	// ** �÷��̾ ȭ�� ������ ������ �ʵ��� ó�� (�켱 �������� ��� ũ�� ��ġ �Է�)
	void CheckPositionInsideScreen();

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

