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
	Super::Initialize();

	pScoreImage = BitmapManager::GetInstance()->GetImage(eImageKey::SCORE);
	pNumberImage = BitmapManager::GetInstance()->GetImage(eImageKey::NUMBER);

	key = eBridgeKey::UI_SCORE;
}

void ScoreUI::Update()
{
	Super::Update();

	MakeScoreNumberList();
}

void ScoreUI::Render(HDC _hdc)
{
	Super::Render(_hdc);

	if ( !pScoreImage || !pNumberImage )
		return;

	// ** Score
	TransparentBlt(_hdc, // ** ���� ��� ��ġ
		int(pOwner->GetPosition().x - pOwner->GetScale().x),
		int(pOwner->GetPosition().y - pOwner->GetScale().y),
		int(pOwner->GetScale().x),
		int(pOwner->GetScale().y),
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
			int((pOwner->GetPosition().x - pOwner->GetScale().x) + ((pNumberImage->GetSegmentationScale().x + 15) * offset)),
			int(pOwner->GetPosition().y),
			int(pNumberImage->GetSegmentationScale().x + 13),
			int(pOwner->GetScale().y),
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
	Super::Release();
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