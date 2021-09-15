#pragma once
#include "Headers.h"

class Bitmap;
class Object
{
protected:
	// ** ������Ʈ�� �⺻ Transform
	Transform TransInfo;

	// ** �浹ü��  �⺻ Transform
	Transform Collider;
	Vector3 Offset;

	eObjectKey Key;
	eObjectStatus Status;
	eCollisionType CollisionType;
	float Speed;

	//Object* Target;
public:
	virtual void Initialize()PURE;
	virtual int Update()PURE;
	virtual void Render(HDC _hdc)PURE;
	virtual void Release()PURE;
	virtual void OnCollision(Object* _pObject)PURE;

	virtual Object* Clone()PURE;

public:
	// ** �̹����� ����ϴµ� ����ϴ� Key ���� out
	eObjectKey GetKey() const { return Key; }

	// ** ������Ʈ�� Ȱ��ȭ ���� ��ȯ
	eObjectStatus GetStatus() const { return Status; }	

	// ** ������Ʈ�� �浹ü Ÿ�� ��ȯ
	eCollisionType GetCollisionType() const { return CollisionType; }

	// ** ��ǥ�� Vector3�� out
	Vector3 GetPosition() { return TransInfo.Position; }

	// ** ũ�⸦ Vector3�� out
	Vector3 GetScale() { return TransInfo.Scale; }

	// ** �浹ü�� out.
	RECT GetCollider();
	
	Vector3 GetColliderPosition() { return Collider.Position; }
	Transform GetColliderTransform() { return Collider; }

	// ** ��ǥ�� Setting (Vector3)
	void SetPosition(Vector3 _position) { TransInfo.Position = _position; }

	// ** ��ǥ�� Setting (_x, _y)
	void SetPosition(float _x, float _y) { TransInfo.Position.x = _x; TransInfo.Position.y = _y; }

	// ** �浹ü�� Setting
	void SetColliderPosition(float _x, float _y) { Collider.Position.x = _x; Collider.Position.y = _y; }

	// ** ������Ʈ�� Ȱ��ȭ ���� Setting 
	void SetStatus(const eObjectStatus& _Status) { Status = _Status; }
public:
	Object();
	Object(const Transform& _rTransInfo) : TransInfo(_rTransInfo) { }
	virtual ~Object();
};