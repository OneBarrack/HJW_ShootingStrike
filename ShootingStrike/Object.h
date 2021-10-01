#pragma once
#include "Headers.h"

class Bridge;
class Object
{
protected:
	// ** Bitmap Image
	Bitmap* pImage;

	// ** Tag Name
	eTagName tagName;

	// ** ���� Bitmap���� ���� ������ �̹����� Offset
	Point imageOffsetOrder;

	// ** Bridge Object
	Bridge* pBridge;

	// ** ������Ʈ�� �⺻ Transform
	Transform transInfo;

	// ** �浹ü��  �⺻ Transform
	Transform collider;

	eObjectKey key;
	eObjectStatus status;
	eCollisionType collisionType;

	// ** ��� ������Ʈ�� ���� �浹 üũ�� �ʿ��� ������Ʈ����
	bool bGenerateCollisionEvent;

	// ** Object Speed
	float speed;

public:
	virtual void Initialize();
	virtual void Update();
	virtual void Render(HDC _hdc);
	virtual void Release();

	virtual Object* Clone()PURE;

	virtual void OnCollision(Object* _pObject)PURE;

public:
	// ** �̹����� ��ȯ
	Bitmap* GetImage() { return pImage; }

	// ** �±׸� ��ȯ
	eTagName GetTagName() { return tagName; }

	// ** ���� ������ �̹����� Offset ��ȯ
	Point GetImageOffsetOrder() { return imageOffsetOrder; }

	// ** BridgeObject�� ��ȯ
	Bridge* GetBridgeObject() { return pBridge; }

	// ** ������Ʈ�� Key ���� out
	eObjectKey GetKey() const { return key; }

	// ** ������Ʈ�� Ȱ��ȭ ���� ��ȯ
	eObjectStatus GetStatus() const { return status; }	

	// ** ������Ʈ�� �浹ü Ÿ�� ��ȯ
	eCollisionType GetCollisionType() const { return collisionType; }

	// ** Transform ������ out
	Transform GetTransInfo() { return transInfo; }

	// ** ��ǥ�� Vector3�� out
	Vector3 GetPosition() { return transInfo.Position; }

	// ** ũ�⸦ Vector3�� out
	Vector3 GetScale() { return transInfo.Scale; }

	// ** ���⸦ Vector3�� out
	Vector3 GetDirection() { return transInfo.Direction; }

	// ** �浹ü�� out.
	RECT GetColliderL();
	RectF GetColliderF();
	Transform GetCollider() { return collider; }
	Vector3 GetColliderPosition() { return collider.Position; }
	Vector3 GetColliderScale() { return collider.Scale; }

	// ** ������Ʈ�� �ӵ� ��ȯ
	float GetSpeed() { return speed; }

	// ** ���� ������Ʈ�� ���� �浹 üũ�� �ʿ����� Ȯ��
	bool IsGeneratedCollisionEvent() { return bGenerateCollisionEvent; }

	// ** Ű �Է� ���� ��ȯ
	bool CheckKeyInputStatus(eInputKey _inputKey, eKeyInputState _status);

	// ** �̹��� ����
	void SetImage(eImageKey _imageKey);

	// ** �±׸� ����
	void SetTagName(eTagName _tagName) { tagName = _tagName; }

	// ** ���� ������ �̹����� Offset ����
	void SetImageOffsetOrder(Point _order) { imageOffsetOrder = _order; }

	// ** Bridge�� Setting
	void SetBridge(Bridge* _pBridge);

	// ** Transform ������ Setting (Transform)
	void SetTransInfo(Transform _transInfo) { transInfo = _transInfo; }

	// ** ��ǥ�� Setting (Vector3)
	void SetPosition(Vector3 _position) { transInfo.Position = _position; }	
	void SetPosition(float _x, float _y) { transInfo.Position.x = _x; transInfo.Position.y = _y; }

	// ** Scale�� Setting (Vector3)
	void SetScale(Vector3 _Scale) { transInfo.Scale = _Scale; }
	void SetScale(float _x, float _y) { transInfo.Scale.x = _x; transInfo.Scale.y = _y; }

	// ** ��ǥ�� Setting (Vector3)
	void SetDirection(Vector3 _Direction) { transInfo.Direction = _Direction; }
	void SetDirection(float _x, float _y) { transInfo.Direction.x = _x; transInfo.Direction.y = _y; }

	// ** �浹ü Transform Setting
	void SetCollider(Transform _collider) { collider = _collider; }
	void SetCollider(Vector3 _position, Vector3 _scale) { collider.Position = _position; collider.Scale = _scale; }

	// ** �浹ü Position Setting
	void SetColliderPosition(Vector3 _position) { collider.Position = _position; }
	void SetColliderPosition(float _x, float _y) { collider.Position.x = _x; collider.Position.y = _y; }

	// ** �浹ü Scale Setting
	void SetColliderScale(Vector3 _scale) { collider.Scale = _scale; }
	void SetColliderScale(float _x, float _y) { collider.Scale.x = _x; collider.Scale.y = _y; }

	// ** ������Ʈ�� Ȱ��ȭ ���� Setting 
	void SetStatus(const eObjectStatus& _status);

	// ** ������Ʈ�� �ӵ� Setting
	void SetSpeed(float _speed) { speed = _speed; }

	// ** ��� ������Ʈ�� ���� �浹 üũ �ʿ俩�� Setting
	void SetGenerateCollisionEvent(bool _generateCollisionEvent) { bGenerateCollisionEvent = _generateCollisionEvent; }

public:
	Object();
	Object(const Transform& _rTransInfo) : transInfo(_rTransInfo) { }
	virtual ~Object();
};