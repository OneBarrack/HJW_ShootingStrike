#include "ScoreUI.h"
#include "GameDataManager.h"
#include "CollisionManager.h"
#include "BitmapManager.h"

ScoreUI::ScoreUI()
	: pScoreImage(nullptr)
	, pNumberImage(nullptr)
	, numberList(queue<int>())
{

}

ScoreUI::~ScoreUI()
{	
}


void ScoreUI::Initialize()
{
	pScoreImage = BitmapManager::GetInstance()->GetImage(eImageKey::SCORE);
	pNumberImage = BitmapManager::GetInstance()->GetImage(eImageKey::NUMBER);

	transInfo.Position = Vector3(0.0f, 0.0f);
	transInfo.Scale = Vector3(0.0f, 0.0f);
	transInfo.Direction = Vector3(0.0f, 0.0f);

	collider.Position = transInfo.Position;
	collider.Scale = transInfo.Scale;

	key = eObjectKey::UI_SCORE;
	status = eObjectStatus::ACTIVATED;
	collisionType = eCollisionType::NONE;

	bGenerateCollisionEvent = false;
}

void ScoreUI::Update()
{
	MakeScoreNumberList();

	// ** �浹ü ����
	collider = transInfo;
}

void ScoreUI::Render(HDC _hdc)
{
	if ( !pScoreImage || !pNumberImage )
		return;

	// ** Score
	TransparentBlt(_hdc, // ** ���� ��� ��ġ
		int(transInfo.Position.x - transInfo.Scale.x),
		int(transInfo.Position.y - transInfo.Scale.y),
		int(transInfo.Scale.x),
		int(transInfo.Scale.y),
		pScoreImage->GetMemDC(),
		0,
		0,
		int(pScoreImage->GetScale().x),
		int(pScoreImage->GetScale().y),
		RGB(255, 0, 255));

	// ** Number
	// ** �ڿ������� �׸��� ���� Offset�� �������� ����
	for ( int offset = 9; offset >= 0; --offset )
	{
		int Num = 0; 

		// ** NumberList�� ���ڸ� �̾� ���ʴ�� �ڿ������� �׸���, 
		// ** ���ڰ� �� �̻� ���ٸ� 0���� ä���
		if ( !numberList.empty() )
		{
			Num = numberList.front();
			numberList.pop();
		}
		
		TransparentBlt(_hdc, // ** ���� ��� ��ġ
			int((transInfo.Position.x - transInfo.Scale.x) + ((pNumberImage->GetSegmentationScale().x + 15) * offset)),
			int(transInfo.Position.y),
			int(pNumberImage->GetSegmentationScale().x + 13),
			int(transInfo.Scale.y),
			pNumberImage->GetMemDC(),
			int(pNumberImage->GetSegmentationScale().x * Num),
			0,
			int(pNumberImage->GetSegmentationScale().x),
			int(pNumberImage->GetScale().y),
			RGB(255, 0, 255));
	}
}

void ScoreUI::Release()
{

}

void ScoreUI::OnCollision(Object* _pObject)
{

}

void ScoreUI::MakeScoreNumberList()
{
	int score = GameDataManager::GetInstance()->GetScore();
	while ( score > 0 )
	{
		int iTemp = int(score % 10);
		score /= 10;

		numberList.push(iTemp);
	}
}