#pragma once
#include "Headers.h"

class Bridge;
class Object
{
protected:
	// ** Bridge Object
	Bridge* pBridgeObject;

	// ** ������Ʈ�� �⺻ Transform
	Transform TransInfo;

	// ** �浹ü��  �⺻ Transform
	Transform Collider;
	Vector3 Offset;

	eObjectKey Key;
	eObjectStatus Status;
	eCollisionType CollisionType;
	float Speed;

	// ** ��� ������Ʈ�� ���� �浹 üũ�� �ʿ��� ������Ʈ����
	bool bGenerateCollisionEvent;
public:
	virtual void Initialize()PURE;
	virtual void Update()PURE;
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

	// ** ���� ������Ʈ�� ���� �浹 üũ�� �ʿ����� Ȯ��
	bool IsGeneratedCollisionEvent() { return bGenerateCollisionEvent; }

	// ** Ű �Է� ���� ��ȯ
	bool CheckKeyInputStatus(eInputKey _InputKey, eKeyInputStatus _Status);

	// ** BridgeObject�� Setting
	void SetBridgeObject(Bridge* _pBridge) { pBridgeObject = _pBridge; }

	// ** ��ǥ�� Setting (Vector3)
	void SetPosition(Vector3 _position) { TransInfo.Position = _position; }	
	void SetPosition(float _x, float _y) { TransInfo.Position.x = _x; TransInfo.Position.y = _y; }

	// ** Scale�� Setting (Vector3)
	void SetScale(Vector3 _Scale) { TransInfo.Scale = _Scale; }
	void SetScale(float _x, float _y) { TransInfo.Scale.x = _x; TransInfo.Scale.y = _y; }

	// ** �浹ü Position Setting
	void SetColliderPosition(Vector3 _position) { Collider.Position = _position; }
	void SetColliderPosition(float _x, float _y) { Collider.Position.x = _x; Collider.Position.y = _y; }

	// ** �浹ü Scale Setting
	void SetColliderScale(Vector3 _Scale) { Collider.Scale = _Scale; }
	void SetColliderScale(float _x, float _y) { Collider.Scale.x = _x; Collider.Scale.y = _y; }

	// ** ������Ʈ�� Ȱ��ȭ ���� Setting 
	void SetStatus(const eObjectStatus& _Status);

	// ** ��� ������Ʈ�� ���� �浹 üũ �ʿ俩�� Setting
	void SetGenerateCollisionEvent(bool _GenerateCollisionEvent) { bGenerateCollisionEvent = _GenerateCollisionEvent; }

public:
	Object();
	Object(const Transform& _rTransInfo) : TransInfo(_rTransInfo) { }
	virtual ~Object();
};