#ifndef __BIG_GUN_ROTATING_H__
#define __BIG_GUN_ROTATING_H__
#include "DynamicObject.h"
#include "CInputDx9.h"

class BigGunRotating : public DynamicObject
{
	CSpriteDx9* sprite_dead;
	CSpriteDx9* sprite_alive;
	int _distance_X;
	int _distance_Y;
public:
	BigGunRotating();
	//Physic*			getPhysic() const { return m_Physic; }
	BigGunRotating(D3DXVECTOR3 _position, eDirection _direction, eObjectID _objectID);
	void Initialize();
	void UpdateAnimation();
	void UpdateCollision(Object* checkingObject);
	void UpdateMovement();
	void Update();
	void Render(SPRITEHANDLE spriteHandle);
	void Release();
	virtual ~BigGunRotating();
};
#endif