#pragma once
#include "Headers.h"

class BitmapManager
{
private:
	static BitmapManager* pInstance;
public:
	static BitmapManager* GetInstance()
	{
		if ( pInstance == nullptr )
			pInstance = new BitmapManager;

		return pInstance;
	}

public:
	void Initialize();
	Bitmap* GetImage(const eImageKey _key);

private:
	// imageKey enum���� Key�� ������ Bitmap List
	map<eImageKey, Bitmap*> imageList;

public:
	const map<eImageKey, Bitmap*> GetImageList() { return imageList; }
};