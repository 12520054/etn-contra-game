#ifndef __BULLETPOOLMANAGER_H__
#define __BULLETPOOLMANAGER_H__

#include "Bullet.h"
#include "BulletPool.h"
#include <list>

class BulletPoolManager
{
private:
	//std::list<Bullet* > m_ListBulletInGame;

	BulletPoolManager();
	~BulletPoolManager();
public:
	std::list<Bullet* > m_ListBulletInGame;
	static BulletPoolManager* m_Instance;
	static BulletPoolManager* getInstance();

	void Initialize();
	void addBulletIntoList(eIDTypeBullet, D3DXVECTOR3, D3DXVECTOR2, float);
	void popBulletFromList();
	void Update();
	void UpdateAnimation();
	void UpdateMovement();
	void UpdateCollision(Object* checkingObject);
	void Render(SPRITEHANDLE spriteHandler);
	void Release();

	// Danh code
	std::list<Bullet*> getListBullet() { return m_ListBulletInGame; }
};

#endif

