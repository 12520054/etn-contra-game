#include "SpriteManager.h"

#define INCLUDE_MAP1 LoadSpriteContent(_lpDirectDevice, "resources\\Map\\1\\1.bmp", eSpriteID::SPRITE_MAP_1, 45, 1, 45)

SpriteManager* SpriteManager::s_Instance = 0;

SpriteManager::SpriteManager(void)
{
	
}

SpriteManager* SpriteManager::getInstance()
{
	if(s_Instance == 0)
	{
		s_Instance = new SpriteManager();
	}
	return s_Instance;
}

void SpriteManager::LoadSpriteContent(LPDIRECT3DDEVICE9 _lpDirectDevice, const char* fileName, eSpriteID eID, int Column, int Row, int TotalFrame)
{
	CSpriteDx9* sprite = new CSpriteDx9();
	sprite->LoadContent(_lpDirectDevice, fileName, Column, Row, TotalFrame);
	m_ListSprite[eID] = sprite;
}

CSpriteDx9* SpriteManager::getSprite(eSpriteID eID)
{
	map<eSpriteID, CSpriteDx9*>::iterator i;

	i = m_ListSprite.find(eID);

	if(i == m_ListSprite.end())
		return 0;

	return i->second;
}

void SpriteManager::InitializeListSprite(LPDIRECT3DDEVICE9 _lpDirectDevice)
{
	
	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Rambo\\rambo_idle.png", eSpriteID::SPRITE_RAMBO_IDLE, 2, 1, 2);
	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Rambo\\rambo_jump.png", eSpriteID::SPRITE_RAMBO_JUMP, 4, 1, 4);
	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Rambo\\rambo_lie.png", eSpriteID::SPRITE_RAMBO_LIE, 1, 1, 1);
	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Rambo\\rambo_run.png", eSpriteID::SPRITE_RAMBO_RUN, 6, 1, 6);
	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Rambo\\rambo_shoot_bottom_right.png", eSpriteID::SPRITE_RAMBO_SHOOT_BOTTOM_RIGHT, 3, 1, 3);
	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Rambo\\rambo_shoot_run.png", eSpriteID::SPRITE_RAMBO_SHOOT_RUN, 3, 1, 3);
	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Rambo\\rambo_shoot_top_right.png", eSpriteID::SPRITE_RAMBO_SHOOT_TOP_RIGHT, 3, 1, 3);
	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Rambo\\rambo_shoot_up.png", eSpriteID::SPRITE_RAMBO_SHOOT_UP, 2, 1, 2);

	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Rambo\\bullet.png", eSpriteID::SPRITE_BULLET_BIG, 1, 1, 1);
	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Rambo\\FBullet.png", eSpriteID::SPRITE_F_BULLET, 4, 1, 4);

	INCLUDE_MAP1;

	LoadSpriteContent(_lpDirectDevice, "resources\\Map\\MenuGame\\MenuBG.png", eSpriteID::SPRITE_MENUBG, 1, 1, 1);
	LoadSpriteContent(_lpDirectDevice, "resources\\Map\\MenuGame\\icon.png", eSpriteID::SPRITE_ICON, 1, 1, 1);

	LoadSpriteContent(_lpDirectDevice, "resources\\grid.png", eSpriteID::SPRITE_GRID, 1, 1, 1);

	// Start load EnemyContent
	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Exploision.png", eSpriteID::SPRITE_EXPLOISION, 6, 1, 6);

	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Enemy\\Sniper\\SniperStanding\\SniperStanding_bot.png", eSpriteID::SPRITE_SNIPER_STANDING_BOT, 1, 1, 1);
	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Enemy\\Sniper\\SniperStanding\\SniperStanding_top.png", eSpriteID::SPRITE_SNIPER_STANDING_TOP, 2, 1, 2);
	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Enemy\\Sniper\\SniperStanding\\SniperStanding_mid.png", eSpriteID::SPRITE_SNIPER_STANDING_MID, 2, 1, 2);
	
	// End load EnemyContent

	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Gun\\Gun_1.png", eSpriteID::SPRITE_GUN_1, 7, 1, 7);
	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Gun\\Gun_2.png", eSpriteID::SPRITE_GUN_2, 14, 1, 14);
	LoadSpriteContent(_lpDirectDevice, "resources\\Character\\Flyer_Sin\\Flyersin.png", eSpriteID::SPRITE_FLYER_SIN, 1, 1, 1);
}

void SpriteManager::Release()
{
	map<eSpriteID, CSpriteDx9*>::iterator i;
	for(i = m_ListSprite.begin(); i != m_ListSprite.end(); i++)
	{
		i->second->Release();
		SAFE_DELETE(i->second);
	}
	m_ListSprite.clear();
}

SpriteManager::~SpriteManager(void)
{
}