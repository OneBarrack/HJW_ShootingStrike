#pragma once
#include "Object.h"

class CollisionManager
{
public:
	static bool IsCollision(Object* _pObj1, Object* _pObj2);

	// ** �� �浹
	static bool EllipseCollision(Object* _pObj1, Object* _pObj2);
	static bool EllipseCollision(const Transform& _Temp, const Transform& _Dest);
	
	// ** �簢�� �浹
	static bool RectCollision(Object* _pObj1, Object* _pObj2);
	static bool RectCollision(RECT _R1, RECT _R2);

	// ** ���� �簢�� �浹
	static bool EllipseRectCollision(Object* _pEllipseObj, Object* _pRectObj);

	// ** ���� �� �ȿ� �ִ���
	static bool IsPointInCircle(float _Cx, float _Cy, float _Cr, float _Px, float _Py);	
};