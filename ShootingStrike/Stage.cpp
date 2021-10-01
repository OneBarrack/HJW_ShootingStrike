#include "Stage.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyHole.h"
#include "HammerEffect.h"
#include "ObjectFactory.h"
#include "CollisionManager.h"
#include "Background.h"
#include "BasicBkg.h"
#include "ScrollVerticalBkg.h"
#include "BitmapManager.h"
#include "SpawnManager.h"

Stage::Stage() 
	: pPlayer(nullptr)
	, pBackground(nullptr)
	, pLeftSideBackground(nullptr)
	, pRightSideBackground(nullptr)
{

}

Stage::~Stage()
{
	Release();
}

void Stage::Initialize()
{
	Bridge* pBridge = nullptr;

	// ** Player
	pPlayer = ObjectManager::GetInstance()->GetPlayer();

	// ** Main Scrolling Background
	pBridge = ObjectManager::GetInstance()->NewBridge(eBridgeKey::BACKGROUND_SCROLL_VERTICAL);
	pBackground = ObjectManager::GetInstance()->NewObject(eObjectKey::BACKGROUND);
	pBackground->SetBridge(pBridge);
	pBackground->SetImage(eImageKey::STAGEBACK);
	pBackground->SetTagName(eTagName::STAGE_MAIN_BKG);
	pBackground->SetPosition(WindowsWidth * 0.5f, WindowsHeight * 0.5f);
	pBackground->SetScale(600, WindowsHeight);
	pBackground->SetSpeed(0.5f);
	static_cast<ScrollVerticalBkg*>(pBridge)->StartBottom();
	static_cast<ScrollVerticalBkg*>(pBridge)->ScrollUp();
	
	// ** Left Side Background
	pBridge = ObjectManager::GetInstance()->NewBridge(eBridgeKey::BACKGROUND_BASIC);
	pLeftSideBackground = ObjectManager::GetInstance()->NewObject(eObjectKey::FOREGROUND);
	pLeftSideBackground->SetBridge(pBridge);
	pLeftSideBackground->SetImage(eImageKey::STAGESIDEBACK);
	pLeftSideBackground->SetPosition(((WindowsWidth - pBackground->GetScale().x) * 0.5f) * 0.5f, WindowsHeight * 0.5f);
	pLeftSideBackground->SetScale((WindowsWidth - pBackground->GetScale().x) * 0.5f, WindowsHeight);

	// ** Right Side Background
	pBridge = ObjectManager::GetInstance()->NewBridge(eBridgeKey::BACKGROUND_BASIC); 
	pRightSideBackground = ObjectManager::GetInstance()->NewObject(eObjectKey::FOREGROUND);
	pRightSideBackground->SetBridge(pBridge);
	pRightSideBackground->SetImage(eImageKey::STAGESIDEBACK);
	pRightSideBackground->SetImageOffsetOrder(Point(1,0));
	pRightSideBackground->SetPosition(WindowsWidth - pLeftSideBackground->GetPosition().x, WindowsHeight * 0.5f);
	pRightSideBackground->SetScale(pLeftSideBackground->GetScale());

	// ** EnemyBoss
	pBridge = ObjectManager::GetInstance()->NewBridge(eBridgeKey::ENEMY_BOSS);
	pBossAngelEnemy = ObjectManager::GetInstance()->NewObject(eObjectKey::ENEMY);
	pBossAngelEnemy->SetBridge(pBridge);
	pBossAngelEnemy->SetImage(eImageKey::ANGEL);
	pBossAngelEnemy->SetTagName(eTagName::ENEMY_BOSS);
	pBossAngelEnemy->SetPosition(pBackground->GetPosition().x, pBackground->GetScale().y * 0.3f);
	pBossAngelEnemy->SetScale(224.0f, 320.0f);
	static_cast<Enemy*>(pBossAngelEnemy)->SetHitPoint(10);
	static_cast<Enemy*>(pBossAngelEnemy)->SetDeathPoint(5000);

	// ** Score UI
	pScoreUI = ObjectManager::GetInstance()->NewObject(eObjectKey::UI_SCORE);
	pScoreUI->SetPosition(WindowsWidth - 200.0f, 45.0f);
	pScoreUI->SetScale(125.0f, 25.0f);

	// ** Spawn Player
	SpawnManager::SpawnPlayer();
}

void Stage::Update()
{
	ObjectManager::GetInstance()->Update();

	// ** �ش� ������Ʈ���� ���� Background ���θ� ������� üũ
	//CheckPositionInBkgBoundary(eObjectKey::PLAYER);
	//CheckPositionInBkgBoundary(eObjectKey::ENEMY);
	//CheckPositionInBkgBoundary(eObjectKey::BULLET);
}

void Stage::Render(HDC _hdc)
{
	ObjectManager::GetInstance()->Render(_hdc);
}

void Stage::Release()
{
	if ( pBackground )			ObjectManager::GetInstance()->RecallObject(pBackground);
	if ( pLeftSideBackground )	ObjectManager::GetInstance()->RecallObject(pLeftSideBackground);
	if ( pRightSideBackground ) ObjectManager::GetInstance()->RecallObject(pRightSideBackground);
	if ( pScoreUI )				ObjectManager::GetInstance()->RecallObject(pScoreUI);
	if ( pBossAngelEnemy )		ObjectManager::GetInstance()->RecallObject(pBossAngelEnemy);
}

void Stage::CheckPositionInBkgBoundary(eObjectKey _ObjectKey)
{
	// ** Stage�� �ٿ����
	RectF ScreenRect = pBackground->GetColliderF();

	// ** Key�� �ش��ϴ� Object List�� �޾ƿ�
	list<Object*> ObjectList = ObjectManager::GetInstance()->GetObjectList(_ObjectKey);

	// ** List�� ���� Position üũ
	for ( Object* pObject : ObjectList )
	{
		// ** Object�� Position�� 
		Vector3 ObjectPosition = pObject->GetPosition();		

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
				if ( ObjectPosition.y < ScreenRect.Top + Offset )
					ObjectPosition.y = ScreenRect.Top + Offset;
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
				ScreenRect.Left   -= Offset;
				ScreenRect.Top    -= Offset;
				ScreenRect.Right  += Offset;
				ScreenRect.Bottom += Offset;

				// ** Stage�� �ٿ���� �� Object Position�� ��ġ���� ������ Destroy
				if ( !CollisionManager::IsPointInRect(ScreenRect, ObjectPosition) )
				{
					pObject->SetStatus(eObjectStatus::DESTROYED);
				}
				break;
			default:
				break;
		}
	}
}