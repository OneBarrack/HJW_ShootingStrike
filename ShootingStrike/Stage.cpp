#include "Stage.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "EnemyHole.h"
#include "HammerEffect.h"
#include "ObjectFactory.h"
#include "CollisionManager.h"
#include "StageBackground.h"
#include "BitmapManager.h"

Stage::Stage() : pPlayer(nullptr)
{

}

Stage::~Stage()
{
	Release();
}

void Stage::Initialize()
{
	pPlayer = ObjectManager::GetInstance()->GetPlayer();

	Bridge* pBridge = new StageBackground;
	pBackground = ObjectManager::GetInstance()->TakeObject(eObjectKey::BACKGROUND, pBridge);

	static_cast<Player*>(pPlayer)->SetStatus(eObjectStatus::ACTIVATED);
	static_cast<Player*>(pPlayer)->SpawnPlayer();	
}

void Stage::Update()
{
	ObjectManager::GetInstance()->Update();

	CheckPositionInBkgBoundary(eObjectKey::PLAYER);
	CheckPositionInBkgBoundary(eObjectKey::ENEMY);
	CheckPositionInBkgBoundary(eObjectKey::BULLET);
}

void Stage::Render(HDC _hdc)
{
	ObjectManager::GetInstance()->Render(_hdc);
}

void Stage::Release()
{
	if ( pBackground )
	{
		pBackground->Release();
		ObjectManager::GetInstance()->RecallObject(pBackground);	
	}
}

void Stage::CheckPositionInBkgBoundary(eObjectKey _ObjectKey)
{
	// ** Key�� �ش��ϴ� Object List�� �޾ƿ�
	list<Object*> ObjectList = ObjectManager::GetInstance()->GetObjectList(_ObjectKey);

	// ** List�� ���� Position üũ
	for ( Object* pObject : ObjectList )
	{
		// ** Object�� Position�� Stage�� �ٿ������ �޾ƿ´�
		Vector3 ObjectPosition = pObject->GetPosition();
		RectF ScreenRect(
			pBackground->GetPosition().x - (pBackground->GetScale().x * 0.5f),
			pBackground->GetPosition().y - (pBackground->GetScale().y * 0.5f),
			pBackground->GetPosition().x + (pBackground->GetScale().x * 0.5f),
			pBackground->GetPosition().y + (pBackground->GetScale().y * 0.5f));

		float Offset = 0.0f;
		switch ( pObject->GetKey() )
		{
			case eObjectKey::PLAYER:
				// ** �׵θ� ��輱 ���ؿ� ��ü�� �߸��� �ʰ� �ϱ� ���� Offset ��
				Offset = 22.0f;

				// ** ��
				if ( ObjectPosition.x < ScreenRect.Left + Offset )
					ObjectPosition.x = ScreenRect.Left + Offset;
				// ** ��
				if ( ObjectPosition.x > ScreenRect.Right - Offset )
					ObjectPosition.x = ScreenRect.Right - Offset;
				// ** ��
				if ( ObjectPosition.y < ScreenRect.Top - Offset )
					ObjectPosition.y = ScreenRect.Top - Offset;
				// ** ��
				if ( ObjectPosition.y > ScreenRect.Bottom - Offset )
					ObjectPosition.y = ScreenRect.Bottom - Offset;

				// ** ���� Position�� �ٽ� �÷��̾ ����
				pObject->SetPosition(ObjectPosition);
				break;
			case eObjectKey::ENEMY:
				[[fallthrough]];
			case eObjectKey::BULLET:
				// ** Stage �ٿ������ ���� ������ ������ Offset
				Offset = 0.0f;

				// ** Stage �ٿ������ Offset ��ŭ ������
				ScreenRect.Left -= Offset;
				ScreenRect.Top -= Offset;
				ScreenRect.Right += Offset;
				ScreenRect.Bottom += Offset;

				// ** Stage�� �ٿ���� �� Object Position�� ��ġ���� ������ Destroy
				if ( !CollisionManager::IsPointInRect(ScreenRect, ObjectPosition) )
					pObject->SetStatus(eObjectStatus::DESTROYED);
				break;
			default:
				break;
		}
	}
}