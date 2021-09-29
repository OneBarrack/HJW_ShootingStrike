#pragma once
#include "Headers.h"

class Bridge;
class Object
{
protected:
	// ** Bitmap Image
	Bitmap* pImage;

	// ** 분할 Bitmap에서 현재 적용할 이미지의 Offset
	Point ImageOffsetOrder;

	// ** Bridge Object
	Bridge* pBridge;

	// ** 오브젝트의 기본 Transform
	Transform TransInfo;

	// ** 충돌체의  기본 Transform
	Transform Collider;
	Vector3 Offset;

	eObjectKey Key;
	eObjectStatus Status;
	eCollisionType CollisionType;
	float Speed;

	// ** 모든 오브젝트에 대한 충돌 체크가 필요한 오브젝트인지
	bool bGenerateCollisionEvent;
public:
	virtual void Initialize();
	virtual void Update();
	virtual void Render(HDC _hdc);
	virtual void Release();

	virtual Object* Clone()PURE;

	virtual void OnCollision(Object* _pObject)PURE;

public:
	// ** 이미지를 반환
	Bitmap* GetImage() { return pImage; }

	// ** 현재 적용할 이미지의 Offset 반환
	Point GetImageOffsetOrder() { return ImageOffsetOrder; }

	// ** BridgeObject를 반환
	Bridge* GetBridgeObject() { return pBridge; }

	// ** 오브젝트의 Key 값을 out
	eObjectKey GetKey() const { return Key; }

	// ** 오브젝트의 활성화 상태 반환
	eObjectStatus GetStatus() const { return Status; }	

	// ** 오브젝트의 충돌체 타입 반환
	eCollisionType GetCollisionType() const { return CollisionType; }

	// ** Transform 정보를 out
	Transform GetTransInfo() { return TransInfo; }

	// ** 좌표를 Vector3로 out
	Vector3 GetPosition() { return TransInfo.Position; }

	// ** 크기를 Vector3로 out
	Vector3 GetScale() { return TransInfo.Scale; }

	// ** 방향를 Vector3로 out
	Vector3 GetDirection() { return TransInfo.Direction; }

	// ** 충돌체를 out.
	RECT GetColliderL();
	RectF GetColliderF();
	Transform GetCollider() { return Collider; }
	Vector3 GetColliderPosition() { return Collider.Position; }
	Vector3 GetColliderScale() { return Collider.Scale; }

	// ** 오브젝트의 속도 반환
	float GetSpeed() { return Speed; }

	// ** 오든 오브젝트에 대한 충돌 체크가 필요한지 확인
	bool IsGeneratedCollisionEvent() { return bGenerateCollisionEvent; }

	// ** 키 입력 상태 반환
	bool CheckKeyInputStatus(eInputKey _InputKey, eKeyInputState _Status);

	// ** 이미지 적재
	void SetImage(eImageKey _ImageKey);

	// ** 현재 적용할 이미지의 Offset 설정
	void SetImageOffsetOrder(Point _Order) { ImageOffsetOrder = _Order; }

	// ** Bridge를 Setting
	void SetBridge(Bridge* _pBridge);

	// ** Transform 정보를 Setting (Transform)
	void SetTransInfo(Transform _TransInfo) { TransInfo = _TransInfo; }

	// ** 좌표를 Setting (Vector3)
	void SetPosition(Vector3 _Position) { TransInfo.Position = _Position; }	
	void SetPosition(float _x, float _y) { TransInfo.Position.x = _x; TransInfo.Position.y = _y; }

	// ** Scale을 Setting (Vector3)
	void SetScale(Vector3 _Scale) { TransInfo.Scale = _Scale; }
	void SetScale(float _x, float _y) { TransInfo.Scale.x = _x; TransInfo.Scale.y = _y; }

	// ** 좌표를 Setting (Vector3)
	void SetDirection(Vector3 _Direction) { TransInfo.Direction = _Direction; }
	void SetDirection(float _x, float _y) { TransInfo.Direction.x = _x; TransInfo.Direction.y = _y; }

	void SetCollider(Transform _Collider) { Collider = _Collider; }

	// ** 충돌체 Position Setting
	void SetColliderPosition(Vector3 _position) { Collider.Position = _position; }
	void SetColliderPosition(float _x, float _y) { Collider.Position.x = _x; Collider.Position.y = _y; }

	// ** 충돌체 Scale Setting
	void SetColliderScale(Vector3 _Scale) { Collider.Scale = _Scale; }
	void SetColliderScale(float _x, float _y) { Collider.Scale.x = _x; Collider.Scale.y = _y; }

	// ** 오브젝트의 활성화 상태 Setting 
	void SetStatus(const eObjectStatus& _Status);

	// ** 오브젝트의 속도 Setting
	void SetSpeed(float _Speed) { Speed = _Speed; }

	// ** 모든 오브젝트에 대한 충돌 체크 필요여부 Setting
	void SetGenerateCollisionEvent(bool _GenerateCollisionEvent) { bGenerateCollisionEvent = _GenerateCollisionEvent; }

public:
	Object();
	Object(const Transform& _rTransInfo) : TransInfo(_rTransInfo) { }
	virtual ~Object();
};