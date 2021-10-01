#pragma once
#include "Object.h"

class CollisionManager
{
public:
	static bool IsCollision(Object* _pObj1, Object* _pObj2);

	// ** �� �浹
	static bool EllipseCollision(Object* _pObj1, Object* _pObj2);
	static bool EllipseCollision(Transform _TransInfo1, Transform _TransInfo2);
	
	// ** �簢�� �浹
	static bool RectCollision(Object* _pObj1, Object* _pObj2);
	static bool RectCollision(RECT _rect1, RECT _rect2);
	static bool RectCollision(RectF _rect1, RectF _rect2);

	// ** ���� �簢�� �浹
	static bool EllipseRectCollision(Object* _pEllipseObj, Object* _pRectObj);

	// ** ���� �� �ȿ� �ִ���
	static bool IsPointInCircle(float _Cx, float _Cy, float _Cr, float _Px, float _Py);
	static bool IsPointInCircle(Vector3 _circlePos, float _Cr, Vector3 _pointPos);

	// ** ���� �簢�� �ȿ� �ִ���
	static bool IsPointInRect(RECT _rect, Vector3 _pointPos);
	static bool IsPointInRect(RectF _rect, Vector3 _pointPos);
};