#include "ButtonUI.h"
#include "InputManager.h"
#include "CollisionManager.h"

ButtonUI::ButtonUI()
	: bExistHoverImage(false)
	, bExistPressedImage(false)
	, buttonState(eButtonState::NORMAL)
	, bOnClick(false)
{

}

ButtonUI::~ButtonUI()
{

}


void ButtonUI::Initialize()
{
	transInfo.Position = Vector3(0.0f, 0.0f);
	transInfo.Scale = Vector3(0.0f, 0.0f);
	transInfo.Direction = Vector3(0.0f, 0.0f);
		
	collider.Position = transInfo.Position;
	collider.Scale = transInfo.Scale;

	key = eObjectKey::UI_BUTTON;
	status = eObjectStatus::ACTIVATED;
	collisionType = eCollisionType::RECT;
	
	bGenerateCollisionEvent = false;
	
	buttonState = eButtonState::NORMAL;
	bOnClick = false;
}

void ButtonUI::Update()
{
	Vector3 mousePos = InputManager::GetInstance()->GetMousePosition();

	// ** ���콺�� ��ư �̹��� ���� ��ġ �ϴ���
	if ( CollisionManager::IsPointInRect(GetColliderL(), mousePos))
	{
		// ** ������ ��
		if ( CheckKeyInputStatus(eInputKey::KEY_LBUTTON, eKeyInputState::PRESSED) )
		{
			buttonState = eButtonState::PRESSED;
		}
		// ** ������ �� ����(Ŭ�� �Ϸ�� ����)
		else if ( CheckKeyInputStatus(eInputKey::KEY_LBUTTON, eKeyInputState::UP) )
		{
			buttonState = eButtonState::NORMAL;
			bOnClick = true;
		}
		// ** ���콺�� ��ư �̹��� ���� ���� (������ �ʴ� ����)
		else
		{
			buttonState = eButtonState::HOVER;
		}
	}
	else
	{
		buttonState = eButtonState::NORMAL;
	}

	// ** �浹ü ����
	collider = transInfo;
}

void ButtonUI::Render(HDC _hdc)
{
	if ( !pImage )
		return;

	TransparentBlt(_hdc, // ** ���� ��� ��ġ
		int(transInfo.Position.x - (transInfo.Scale.x * 0.5f)),
		int(transInfo.Position.y - (transInfo.Scale.y * 0.5f)),
		int(transInfo.Scale.x),
		int(transInfo.Scale.y),
		pImage->GetMemDC(),
		int(transInfo.Scale.x * static_cast<int>(buttonState)),
		0,
		int(transInfo.Scale.x),
		int(transInfo.Scale.y),
		RGB(255, 0, 255));
}

void ButtonUI::Release()
{

}

void ButtonUI::OnCollision(Object* _pObject)
{

}
