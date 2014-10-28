#include "SniperHiding.h"


SniperHiding::SniperHiding()
{

}

SniperHiding::SniperHiding(D3DXVECTOR3 _position, eDirection _direction, eObjectID _objectID) 
	: DynamicObject(_position, _direction, _objectID)
{
	
}

void SniperHiding::Initialize()
{
	m_ObjectState = eObjectState::STATE_HIDING;
	sprite_alive = new CSpriteDx9(*SpriteManager::getInstance()->getSprite(eSpriteID::SPRITE_BIG_GUN_ROATING));
	sprite_dead = new CSpriteDx9(*SpriteManager::getInstance()->getSprite(eSpriteID::SPRITE_EXPLOISION));
	m_Sprite = sprite_alive;
}

void SniperHiding::UpdateAnimation()
{	
	switch (m_ObjectState)
	{
	case STATE_ALIVE_IDLE: // cant be attack by rambo bullet
		m_Sprite = sprite_alive;
		_distance_X = (int)(abs(CGlobal::Rambo_X - this->getPositionVec2().x));
		_distance_Y = (int)(CGlobal::Rambo_Y - this->getPositionVec2().y);
		
		if(_distance_X > 350)
		{
			this->getSprite()->getAnimation()->setCurrentFrame(0);
		}
		else
		{
			if(_distance_X > 350 && _distance_X >= 100)
			{
				if( _distance_Y == 0 )
				{
					this->getSprite()->getAnimation()->setIndexStart(0);
					this->getSprite()->getAnimation()->setIndexEnd(2);
				}
				if( _distance_Y > 0 && _distance_Y < 50)
						{
							this->getSprite()->getAnimation()->setIndexStart(3);
							this->getSprite()->getAnimation()->setIndexEnd(5);
						}
				
			}
			else if( _distance_X <= 100)
			{
			if( _distance_Y >= 50)
			{
				this->getSprite()->getAnimation()->setIndexStart(6);
				this->getSprite()->getAnimation()->setIndexEnd(8);			
			}			
			}
			
			
		}
	
		m_Sprite->UpdateAnimation(1000);
		break;
	case STATE_BEFORE_DEATH:
		m_Sprite = sprite_dead;
		m_Sprite->UpdateAnimation(120);
		break;
	case STATE_DEATH:
		break;
	default:
		break;
	}	
}


void SniperHiding::UpdateCollision(Object* checkingObject)
{
	switch (checkingObject->getID())
	{
	case eObjectID::RAMBO:

		break;
	case eObjectID::BULLET_RAMBO:

		break;
	default:
		break;
	}
}

void SniperHiding:: UpdateMovement()
{}
void SniperHiding::Update()
{
	switch (m_ObjectState)
	{
	case STATE_ALIVE_IDLE:
		break;
	case STATE_BEFORE_DEATH:
		m_TimeChangeState += (int)CGameTimeDx9::getInstance()->getElapsedGameTime().getMilliseconds();
		if(m_TimeChangeState > 1000)
		{
			m_ObjectState = eObjectState::STATE_DEATH;
			m_TimeChangeState = 0;
		}
		break;
	case STATE_DEATH:
		this->Release();
		break;
	default:
		break;
	}
}
void SniperHiding::Render(SPRITEHANDLE spriteHandle)
{
	if(m_Sprite != 0)
	{
		m_Sprite->Render(spriteHandle, 
						getPositionVec2(), 
						m_Sprite->getSpriteEffect(), 
						m_Sprite->getRotate(), 
						m_Sprite->getScale(), 
						m_Position.z);
	}
}


void SniperHiding::Release()
{
	m_Sprite = 0;
	sprite_alive->Release();
	sprite_dead->Release();
}

SniperHiding::~SniperHiding()
{

}