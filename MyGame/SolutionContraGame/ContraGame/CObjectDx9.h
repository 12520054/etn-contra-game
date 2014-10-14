#ifndef __OBJECT_H__
#define __OBJECT_H__
#include "CGlobal.h"
#include "CSpriteDx9.h"
#include "CPhysicDx9.h"
#include "SpriteManager.h"
#include "SoundMangerDx9.h"
#include <d3dx9.h>
#include <d3d9.h>

class Physic;

class Object
{	
protected:
	eObjectID		m_eObjectID;
	eDirection		m_Direction;
	CSpriteDx9*		m_Sprite;
	//Physic*			m_Physic;
	eObjectState	m_ObjectState;

	int				m_TimeChangeState;

	D3DXVECTOR3		m_Position;
	//bit 0 -> Update Sprite
	//bit 1 -> Update Animation
	//bit 2 -> Update Movement
	//bit 3 -> Available
	//bit 4 -> Available
	//bit 5 -> Available
	//bit 6 -> Available
	//bit 7 -> Available
	char			m_UpdateFlag;
public:
	Object();
	Object(D3DXVECTOR3 _position, eDirection _direction, eObjectID _objectID);
	CSpriteDx9*		getSprite() const { return m_Sprite; }

	void			setPosition(D3DXVECTOR3 _position){ m_Position = _position; }
	D3DXVECTOR3		getPositionVec3(){ return m_Position; }
	D3DXVECTOR2		getPositionVec2(){ return D3DXVECTOR2(m_Position.x, m_Position.y); }

	//Physic*			getPhysic() const { return m_Physic; }
	eObjectID		getID() const { return m_eObjectID; }
	virtual void Initialize() = 0;
	virtual void UpdateAnimation() = 0;
	
	virtual void UpdateCollision(Object* checkingObject) = 0;
	
	virtual void Update() = 0;
	virtual void Render(SPRITEHANDLE spriteHandle) = 0;
	virtual void Release() = 0;
	virtual ~Object();
};

#endif