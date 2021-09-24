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

	/*
	// ** ������Ʈ �Ŵ������� �Ѿ� ����Ʈ�� �޾ƿ�. (�����ͷ�...)
	BulletList = ObjectManager::GetInstance()->GetBulletList();

	// ** ������Ʈ �Ŵ������� ���� ����Ʈ�� �޾ƿ�. (�����ͷ�...)
	EnemyList = ObjectManager::GetInstance()->GetEnemyList();
	*/

	Bridge* pBridge = new StageBackground;
	pBackground = ObjectManager::GetInstance()->TakeObject(eObjectKey::BACKGROUND, pBridge);

	static_cast<Player*>(pPlayer)->SetStatus(eObjectStatus::ACTIVATED);
	static_cast<Player*>(pPlayer)->SpawnPlayer();	
}

void Stage::Update()
{
	// ** ��� Ȱ��ȭ ������Ʈ �� �浹 �˻�
	CheckCollisionForAllObjects();
	UpdateForAllObjects();
}

void Stage::Render(HDC _hdc)
{
	RenderForAllObjects(_hdc);
}

void Stage::Release()
{
	if ( pBackground )
	{
		pBackground->Release();
		ObjectManager::GetInstance()->RecallObject(pBackground);	
	}
}

void Stage::CheckCollisionForAllObjects()
{
	auto EnableList = ObjectManager::GetInstance()->GetEnableList();
	for ( auto ListIter1 = EnableList->begin(); ListIter1 != EnableList->end(); ++ListIter1 )
	{
		for ( auto ObjIter1 = ListIter1->second.begin(); ObjIter1 != ListIter1->second.end(); ++ObjIter1 )
		{
			if ( (*ObjIter1)->IsGeneratedCollisionEvent() == false )
				continue;

			// ** ��ü Object�� �浹üũ
			auto ListIter2 = ListIter1;
			for ( ++ListIter2; ListIter2 != EnableList->end(); ++ListIter2 )
			{
				for ( auto ObjIter2 = ListIter2->second.begin(); ObjIter2 != ListIter2->second.end(); ++ObjIter2 )
				{
					if ( (*ObjIter2)->IsGeneratedCollisionEvent() == false )
						continue;

					if ( CollisionManager::IsCollision(*ObjIter1, *ObjIter2) )
					{
						// �浹 Ʈ���� �ߵ�
						(*ObjIter1)->OnCollision(*ObjIter2);
						(*ObjIter2)->OnCollision(*ObjIter1);
					}
				}
			}
		}
	}
}

void Stage::CheckPositionInsideStage(Object* _pObject)
{
	// ** Object�� Position�� Stage�� �ٿ������ �޾ƿ´�
	Vector3 ObjectPosition = _pObject->GetPosition();
	RectF ScreenRect(
		pBackground->GetPosition().x - (pBackground->GetScale().x * 0.5f),
		pBackground->GetPosition().y - (pBackground->GetScale().y * 0.5f),
		pBackground->GetPosition().x + (pBackground->GetScale().x * 0.5f),
		pBackground->GetPosition().y + (pBackground->GetScale().y * 0.5f));	

	float Offset = 0.0f;
	switch ( _pObject->GetKey() )
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
			_pObject->SetPosition(ObjectPosition);
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
				_pObject->SetStatus(eObjectStatus::DESTROYED);
			break;
		default:
			break;
	}	
}

void Stage::UpdateForAllObjects()
{
	auto enableList = ObjectManager::GetInstance()->GetEnableList();
	for ( auto ListIter1 = enableList->begin(); ListIter1 != enableList->end(); ++ListIter1 )
	{
		for ( auto ObjIter1 = ListIter1->second.begin(); ObjIter1 != ListIter1->second.end(); )
		{
			// ** Status�� üũ�Ͽ� Update �Ǵ� Recall ���ش�
			switch ( (*ObjIter1)->GetStatus() )
			{
				case eObjectStatus::DESTROYED:
					ObjectManager::GetInstance()->RecallObject(*ObjIter1++);
					break;
				case eObjectStatus::ACTIVATED:					
					(*ObjIter1)->Update(); // ** Update			
					CheckPositionInsideStage(*ObjIter1); // ** ������Ʈ�� Stage ���θ� ������� üũ
					[[fallthrough]];
				default:
					++ObjIter1;
					break;
			}
		}
	}

	#ifdef GAME_DEBUG_MODE
	static ULONGLONG Time = GetTickCount64();
	if ( Time + 1000 < GetTickCount64() )
	{
		auto temp = ObjectManager::GetInstance()->GetEnableList();
		auto temp2 = temp->find(eObjectKey::BULLET);
		if ( temp2 != temp->end() )
		{
			cout << temp2->second.size() << endl;
		}
		Time = GetTickCount64();
	}
	#endif // GAME_DEBUG_MODE
}

void Stage::RenderForAllObjects(HDC _hdc)
{
	// ** Buffer MemDC�� ��� �׸� �� memDC�� hdc�� ���ҽ��� �׸���
	map<eObjectKey, list<Object*>>* enableList = ObjectManager::GetInstance()->GetEnableList();
	for ( map<eObjectKey, list<Object*>>::iterator iter = enableList->begin();
		iter != enableList->end(); ++iter )
	{
		for ( list<Object*>::iterator iter2 = iter->second.begin();
			iter2 != iter->second.end(); ++iter2 )
		{
			if ( (*iter2)->GetStatus() == eObjectStatus::ACTIVATED )
			{
				(*iter2)->Render(_hdc);
			}
		}
	}
}
