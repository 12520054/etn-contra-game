#include "Tile.h"


Tile::Tile(void)
{
}

Tile::Tile(D3DXVECTOR3 _position, int ID, eSpriteID tileMapID)
{
	m_TypeObject = ETypeObject::TILE_MAP;
	m_Position = _position;
	m_Sprite = new CSpriteDx9(*SpriteManager::getInstance()->getSprite(tileMapID));
	m_Sprite->getAnimation()->setSourceRectAtIndex(ID);
	m_ID = ID;
	/*RECT temp = *m_Sprite->getAnimation()->getSourceRect();
	temp.right = temp.right - 1;
	m_Sprite->getAnimation()->setSourceRect(temp);*/
}

void Tile::Initialize()
{

}
void Tile::UpdateAnimation()
{

}
void Tile::UpdateCollision(Object* checkingObject)
{
	// NOTE: No code
}

void Tile::Update()
{
}

void Tile::Render(SPRITEHANDLE spriteHandle)
{
	//m_Sprite->Render(spriteHandle, D3DXVECTOR2(getPositionVec2().x - (getPositionVec2().x - 32)/64, getPositionVec2().y), ESpriteEffect::None, 0.0f, 1.0f, getPositionVec3().z);
	m_Sprite->Render(spriteHandle, getPositionVec2(), ESpriteEffect::None, 0.0f, 1.0f, getPositionVec3().z);
}

void Tile::Release()
{
}

Tile::~Tile(void)
{
}