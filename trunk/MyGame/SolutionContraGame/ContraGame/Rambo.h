#ifndef __RAMBO_H__
#define __RAMBO_H__
#include "CObjectDx9.h"
#include "CInputDx9.h"

class Rambo : public Object
{
public:
	Rambo();
	Rambo(D3DXVECTOR3 _position, eDirection _direction);
	void Initialize();
	void HandleInput();
	void UpdateAnimation();
	void UpdateCollision(Object* checkingObject);
	void UpdateMovement();
	void Render(SPRITEHANDLE spriteHandle);
	void Release();
	virtual ~Rambo();
};

#endif