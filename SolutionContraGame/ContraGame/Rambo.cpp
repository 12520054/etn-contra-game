#include "Rambo.h"
#include "RoShan.h"
#include "BigCapsuleBoss.h"

#define VELOCITY_Y_JUMP				4.8f
#define VELOCITY_X_MOVE_TO_RIGHT	1.1f
#define VELOCITY_Y_MOVE_TO_LEFT		-1.1f

Rambo::Rambo()
{

}

Rambo::Rambo(D3DXVECTOR3 _position, eDirection _direction, eObjectID _objectID)
	: DynamicObject(_position, _direction, _objectID)
{
	m_ObjectState				= eObjectState::STATE_RAMBO_JUMP;
	m_RamboSprite				= new RamboSprite();
	m_Position.z				= 1.0f;
	isJump						= false;
	isFall						= false;
	isLieDown					= false;
	m_timeClimb					= 0;
	m_timeWaterBomb				= 0;
	m_timeAddBullet				= 1000;
	m_DirectAttack				= eDirectAttack::AD_RIGHT;
	m_timeDelayRunAndShootRun	= 0;
	m_SkillBullet				= eIDSkillBullet::S_SKILL_BULLET;
	isSetVelocityDeathState		= false;
	m_life						= 10;
	m_timeDeath					= 0;
	this->m_Physic->setAccelerate(D3DXVECTOR2(0.0f, -0.1f));
	isInvulnerable = true;
	m_colorAlpha = 255;
	m_inverseColorAlpha = 1;
	m_timeInvulnerable = 0;
	m_timeBeforeDeadBottom = 0;
	prePosX = 0;
	finalPosX = 0;
}

int Rambo::CheckOutBottomCamera()
{
	if (m_Position.y < Camera::getInstance()->getBound().bottom)
	{
		if (m_ObjectState != eObjectState::STATE_RAMBO_BEFORE_DEAD && m_ObjectState != eObjectState::STATE_RAMBO_DEAD)
		{
			m_ObjectState = eObjectState::STATE_RAMBO_BEFORE_DEAD;
		}
		isFall = false;
		m_timeBeforeDeadBottom += CGameTimeDx9::getInstance()->getElapsedGameTime().getMilliseconds();
		if (m_timeBeforeDeadBottom > 3000)
		{
			m_timeBeforeDeadBottom = 0;
			m_ObjectState = eObjectState::STATE_RAMBO_DEAD;
			m_timeDeath = 3000;
		}
	}
	return 0;
}

RECT Rambo::getBound()
{
	RECT tempBound = RECT();

	tempBound.left		= (long)(m_Position.x - m_RamboSprite->GetFrameSize().x / 2);
	tempBound.right		= (long)(tempBound.left + m_RamboSprite->GetFrameSize().x);
	tempBound.top		= (long)(m_Position.y + m_RamboSprite->GetFrameSize().y / 2);
	tempBound.bottom	= (long)(tempBound.top - m_RamboSprite->GetFrameSize().y);

	return tempBound;
}

void Rambo::Initialize()
{

}

int Rambo::HandleInputDeadState()
{
	if (m_ObjectState == eObjectState::STATE_RAMBO_DEAD)
	{
		m_timeDeath += CGameTimeDx9::getInstance()->getElapsedGameTime().getMilliseconds();
		if (m_timeDeath > 2000)
		{
			m_timeDeath				= 0;
			isSetVelocityDeathState	= false;
			m_ObjectState			= eObjectState::STATE_RAMBO_JUMP;
			m_Position.x			= (float)(Camera::getInstance()->getBound().left + 128);
			m_Position.y			= (float)(Camera::getInstance()->getBound().top - 32);
			this->m_Physic->setAccelerate(D3DXVECTOR2(0.0f, -0.1f));
			m_Physic->setVelocityX(0.0f);
			m_Physic->setVelocityY(0.0f);
			isInvulnerable = true;
			m_SkillBullet = eIDSkillBullet::DEFAULT_SKILL_BULLET;
			--m_life;
			Camera::getInstance()->SetTempLockHeight(true);
		}
	}
	return 0;
}

void Rambo::HandleInput()
{
	//if(CInputDx9::getInstance()->IsKeyDown(DIK_SPACE))
	//{
	//	m_Position.y = 450;
	//	isSetVelocityDeathState = false;
	//	m_ObjectState = eObjectState::STATE_RAMBO_FALL;
	//}
	CheckOutBottomCamera();
	HandleInputDeadState();

	if (CInputDx9::getInstance()->IsKeyPress(DIK_X))
	{
		//m_ObjectState = eObjectState::STATE_RAMBO_BEFORE_DEAD;
		Camera::getInstance()->SetTempLockHeight(false);
	}

	switch (m_ObjectState)
	{
	case STATE_RAMBO_IDLE:
	{
		HandleInputIdleState();
	}
		break;
	case STATE_RAMBO_JUMP:
	{
		HandleInputJumpState();
	}
		break;
	case STATE_RAMBO_LIE:
	{
		HandleInputLieState();
	}
		break;
	case STATE_RAMBO_RUN:
	{
		HandleInputRunState();
	}
		break;
	case STATE_RAMBO_SHOOT:
		break;
	case STATE_RAMBO_AIM_BOTTOM_RIGHT:
	{
		HandleInputAimBottomRightState();
	}
		break;
	case STATE_RAMBO_SHOOT_RUN:
	{
		HandleInputShootRunState();
	}
		break;
	case STATE_RAMBO_AIM_TOP_RIGHT:
	{
		HandleInputAimTopRightState();
	}
		break;
	case STATE_RAMBO_AIM_UP:
	{
		HandleInputAimUpState();
	}
		break;
	case STATE_SHOOTING:
	{

	}
		break;
	case STATE_RAMBO_WATER_BOMB:
	{
		HandleInputWaterBombState();
	}
		break;
	case STATE_RAMBO_DIVE:
	{
		HandleInputDiveState();
	}
		break;
	case STATE_RAMBO_SWIM:
	{
		HandleInputSwimState();
	}
		break;
	case STATE_RAMBO_CLIMB:
	{
		HandleInputClimbState();
	}
		break;
	case STATE_RAMBO_SWIM_SHOOT:
	{
		HandleInputSwimShootState();
	}
		break;
	case STATE_RAMBO_SWIM_SHOOT_UP:
	{
		HandleInputSwimShootUpState();
	}
		break;
	case STATE_RAMBO_SWIM_SHOOT_TOP_RIGHT:
	{
		HandleInputSwimShootTopRightState();
	}
		break;
	case STATE_RAMBO_DEAD:
	{

	}
		break;
	case STATE_RAMBO_BEFORE_DEAD:
	{
		HandleInputBeforeDieState();
	}
		break;
	default:
		break;
	}

	if (isFall)
	{
		m_ObjectState = eObjectState::STATE_RAMBO_FALL;
	}
	if (m_ObjectState != eObjectState::STATE_RAMBO_CLIMB)
	{
		m_timeClimb = 0;
	}
	if (m_ObjectState != eObjectState::STATE_RAMBO_WATER_BOMB)
	{
		m_timeWaterBomb = 0;
	}

	if (m_ObjectState != eObjectState::STATE_RAMBO_DEAD)
	{
		if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown())
		{
			m_Direction = eDirection::RIGHT;
		}
		else
		{
			if (CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
			{
				m_Direction = eDirection::LEFT;
			}
		}
	}
}

int Rambo::HandleInputBeforeDieState()
{
	if (!isSetVelocityDeathState)
	{
		m_Physic->setVelocityY(3.5f);
		if (m_Direction == eDirection::RIGHT)
		{
			m_Physic->setVelocityX(-0.5f);
		}
		if (m_Direction == eDirection::LEFT)
		{
			m_Physic->setVelocityX(0.5f);
		}
		isSetVelocityDeathState = true;
	}
	return 0;
}

int Rambo::HandleInputSwimState()
{
	m_Physic->setVelocityX(0.0f);
	if (m_Direction == eDirection::RIGHT)
	{
		m_DirectAttack = eDirectAttack::AD_RIGHT;
	}
	if (m_Direction == eDirection::LEFT)
	{
		m_DirectAttack = eDirectAttack::AD_LEFT;
	}
	if (CInputDx9::getInstance()->IsKeyUpUpAndKeyDownDown())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_DIVE;
	}
	if (HandleInputShooting())
	{
		if (CInputDx9::getInstance()->IsKeyUpDownAndKeyDownUp())
		{
			m_ObjectState = eObjectState::STATE_RAMBO_SWIM_SHOOT_UP;
			m_DirectAttack = eDirectAttack::AD_TOP;
			if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown() || CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
			{
				m_ObjectState = eObjectState::STATE_RAMBO_SWIM_SHOOT_TOP_RIGHT;
				if (m_Direction == eDirection::RIGHT)
				{
					m_DirectAttack = eDirectAttack::AD_TOP_RIGHT;
				}
				if (m_Direction == eDirection::LEFT)
				{
					m_DirectAttack = eDirectAttack::AD_TOP_LEFT;
				}
			}
		}
		else
		{
			m_ObjectState = eObjectState::STATE_RAMBO_SWIM_SHOOT;
		}
		Shoot();
	}
	if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown())
	{
		m_Physic->setVelocityX(VELOCITY_X_MOVE_TO_RIGHT);
		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
	{
		m_Physic->setVelocityX(VELOCITY_Y_MOVE_TO_LEFT);
		return 0;
	}
	return 0;
}

int Rambo::HandleInputIdleState()
{
	m_Physic->setVelocityX(0.0f);
	if (m_Direction == eDirection::LEFT)
	{
		m_DirectAttack = eDirectAttack::AD_LEFT;
	}
	if (m_Direction == eDirection::RIGHT)
	{
		m_DirectAttack = eDirectAttack::AD_RIGHT;
	}
	if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown() || CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_RUN;
		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyUpUpAndKeyDownDown())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_LIE;
		m_Position.y -= 20;
		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyUpDownAndKeyDownUp())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_AIM_UP;
		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyPress(DIK_X))
	{
		m_ObjectState = eObjectState::STATE_RAMBO_JUMP;
		m_Physic->setVelocityY(VELOCITY_Y_JUMP);
		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyUpDownAndKeyDownUp())
	{
		if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown() || CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
		{
			m_ObjectState = eObjectState::STATE_RAMBO_AIM_TOP_RIGHT;
			if (m_Direction == eDirection::RIGHT)
			{
				m_DirectAttack = eDirectAttack::AD_TOP_RIGHT;
			}
			if (m_Direction == eDirection::LEFT)
			{
				m_DirectAttack = eDirectAttack::AD_TOP_LEFT;
			}
		}
	}

	if (CInputDx9::getInstance()->IsKeyUpUpAndKeyDownDown())
	{
		if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown() || CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
		{
			m_ObjectState = eObjectState::STATE_RAMBO_AIM_BOTTOM_RIGHT;
			if (m_Direction == eDirection::RIGHT)
			{
				m_DirectAttack = eDirectAttack::AD_BOTTOM_RIGHT;
			}
			if (m_Direction == eDirection::LEFT)
			{
				m_DirectAttack = eDirectAttack::AD_BOTTOM_LEFT;
			}
		}
	}

	if (HandleInputShooting())
	{
		Shoot();
	}
	return 1;
}

bool Rambo::HandleInputShooting()
{
	switch (m_SkillBullet)
	{
	case eIDSkillBullet::DEFAULT_SKILL_BULLET:
		if (CInputDx9::getInstance()->IsKeyPress(DIK_Z))
		{
			if (BulletPoolManager::getInstance()->GetAmountBulletOfType(eIDTypeBullet::DEFAULT_BULLET_OF_RAMBO) < 6)
			{
				m_RamboSprite->IncreaseTimesShake(2);
				SoundBuffer* sound = SoundManagerDx9::getInstance()->getSoundBuffer(eSoundID::bullet_default_sfx);
				sound->Play();
				return true;
			}
			return false;
		}
		break;
	case eIDSkillBullet::M_SKILL_BULLET:
	case eIDSkillBullet::R_SKILL_BULLET:
	case eIDSkillBullet::F_SKILL_BULLET:
	case eIDSkillBullet::S_SKILL_BULLET:
		if (CInputDx9::getInstance()->IsKeyDown(DIK_Z))
		{
			if (BulletPoolManager::getInstance()->GetAmountBulletOfType(eIDTypeBullet::DEFAULT_BULLET_OF_RAMBO) < 2 && isAddBullet())
			{
				m_RamboSprite->IncreaseTimesShake(2);
				SoundBuffer* sound = SoundManagerDx9::getInstance()->getSoundBuffer(eSoundID::bullet_default_sfx);
				sound->Play();
				return true;
			}
			return false;
		}
		break;
		//if (CInputDx9::getInstance()->IsKeyPress(DIK_Z))
		//{
		//	BulletPoolManager::getInstance()->KillBullet(eIDTypeBullet::LAZER_BULLET_OF_RAMBO);
		//	m_RamboSprite->setShakeTime(0);
		//	m_RamboSprite->IncreaseTimesShake(2);
		//	SoundBuffer* sound = SoundManagerDx9::getInstance()->getSoundBuffer(eSoundID::bullet_default_sfx);
		//	sound->Play();
		//	return true;
		//}
		//if(CInputDx9::getInstance()->IsKeyDown(DIK_Z))
		//{
		//	if (BulletPoolManager::getInstance()->GetAmountBulletOfType(eIDTypeBullet::LAZER_BULLET_OF_RAMBO) < 1)
		//	{
		//		m_RamboSprite->IncreaseTimesShake(2);
		//		SoundBuffer* sound = SoundManagerDx9::getInstance()->getSoundBuffer(eSoundID::bullet_default_sfx);
		//		sound->Play();
		//		return true;
		//	}
		//	return false;
		//}
	case eIDSkillBullet::L_SKILL_BULLET:
		if (CInputDx9::getInstance()->IsKeyPress(DIK_Z))
		{
			BulletPoolManager::getInstance()->KillBullet(eIDTypeBullet::LAZER_BULLET_OF_RAMBO);
			m_RamboSprite->setShakeTime(0);
			m_RamboSprite->IncreaseTimesShake(2);
			SoundBuffer* sound = SoundManagerDx9::getInstance()->getSoundBuffer(eSoundID::bullet_default_sfx);
			sound->Play();
			return true;
		}
		if (CInputDx9::getInstance()->IsKeyDown(DIK_Z))
		{
			if (BulletPoolManager::getInstance()->GetAmountBulletOfType(eIDTypeBullet::LAZER_BULLET_OF_RAMBO) < 1)
			{
				m_RamboSprite->IncreaseTimesShake(2);
				SoundBuffer* sound = SoundManagerDx9::getInstance()->getSoundBuffer(eSoundID::bullet_default_sfx);
				sound->Play();
				return true;
			}
			return false;
		}
		break;
	default:
		break;
	}
	return false;
}

int Rambo::HandleInputLieState()
{
	m_Physic->setVelocityX(0.0f);
	if (m_Direction == eDirection::RIGHT)
	{
		m_DirectAttack = eDirectAttack::AD_RIGHT;
	}
	if (m_Direction == eDirection::LEFT)
	{
		m_DirectAttack = eDirectAttack::AD_LEFT;
	}

	if (HandleInputShooting())
	{
		Shoot();
	}

	if (!CInputDx9::getInstance()->IsKeyDown(DIK_DOWN) || CInputDx9::getInstance()->IsKeyUpDownAndKeyDownDown())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_IDLE;
		m_Position.y += 20;
		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown() || CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_AIM_BOTTOM_RIGHT;
		m_Position.y += 20;
		return 0;
	}
	return 0;
}

int Rambo::HandleInputRunState()
{
	if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown())
	{
		m_Physic->setVelocityX(VELOCITY_X_MOVE_TO_RIGHT);
		m_DirectAttack = eDirectAttack::AD_RIGHT;
	}
	if (CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
	{
		m_Physic->setVelocityX(VELOCITY_Y_MOVE_TO_LEFT);
		m_DirectAttack = eDirectAttack::AD_LEFT;
	}
	if (HandleInputShooting())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_SHOOT_RUN;
		Shoot();

	}
	if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightUp() || CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightDown())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_IDLE;
		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyUpDownAndKeyDownUp())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_AIM_TOP_RIGHT;
		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyUpUpAndKeyDownDown())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_AIM_BOTTOM_RIGHT;
		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyPress(DIK_X))
	{
		m_ObjectState = eObjectState::STATE_RAMBO_JUMP;
		m_Physic->setVelocityY(VELOCITY_Y_JUMP);
		return 0;
	}
	return 0;
}

bool Rambo::isAddBullet()
{
	m_timeAddBullet += CGameTimeDx9::getInstance()->getElapsedGameTime().getMilliseconds();
	if (m_timeAddBullet > 500)
	{
		m_timeAddBullet = 0;
		return true;
	}
	return false;
}

int Rambo::HandleInputAimBottomRightState()
{

	if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown())
	{
		m_Physic->setVelocityX(VELOCITY_X_MOVE_TO_RIGHT);
		m_DirectAttack = eDirectAttack::AD_BOTTOM_RIGHT;
	}
	if (CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
	{
		m_Physic->setVelocityX(VELOCITY_Y_MOVE_TO_LEFT);
		m_DirectAttack = eDirectAttack::AD_BOTTOM_LEFT;
	}
	if (HandleInputShooting())
	{
		Shoot();
	}
	if (!CInputDx9::getInstance()->IsKeyDown(DIK_DOWN))
	{
		m_ObjectState = eObjectState::STATE_RAMBO_RUN;
		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightUp() || CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightDown())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_LIE;
		m_Position.y -= 20;
		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyPress(DIK_X))
	{
		m_ObjectState = eObjectState::STATE_RAMBO_JUMP;
		m_Physic->setVelocityY(VELOCITY_Y_JUMP);
		return 0;
	}
	return 0;
}

int Rambo::HandleInputAimTopRightState()
{
	if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown())
	{
		m_Physic->setVelocityX(VELOCITY_X_MOVE_TO_RIGHT);
		m_DirectAttack = eDirectAttack::AD_TOP_RIGHT;
	}
	if (CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
	{
		m_Physic->setVelocityX(VELOCITY_Y_MOVE_TO_LEFT);
		m_DirectAttack = eDirectAttack::AD_TOP_LEFT;
	}
	if (HandleInputShooting())
	{
		Shoot();
	}
	if (!CInputDx9::getInstance()->IsKeyDown(DIK_UP))
	{
		m_ObjectState = eObjectState::STATE_RAMBO_RUN;
		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightUp() || CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightDown())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_AIM_UP;
		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyPress(DIK_X))
	{
		m_ObjectState = eObjectState::STATE_RAMBO_JUMP;
		m_Physic->setVelocityY(VELOCITY_Y_JUMP);
		return 0;
	}
	return 0;
}

int Rambo::HandleInputAimUpState()
{
	m_DirectAttack = eDirectAttack::AD_TOP;
	if(!CInputDx9::getInstance()->IsKeyDown(DIK_UP) || CInputDx9::getInstance()->IsKeyUpDownAndKeyDownDown())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_IDLE;
		return 0;
	}
	if(CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown() || CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_AIM_TOP_RIGHT;
		return 0;
	}
	if (HandleInputShooting())
	{
		Shoot();	
	}
	if(CInputDx9::getInstance()->IsKeyPress(DIK_X))
	{
		m_ObjectState = eObjectState::STATE_RAMBO_JUMP;
		m_Physic->setVelocityY(VELOCITY_Y_JUMP);
		return 0;
	}
	return 0;
}

int Rambo::HandleInputWaterBombState()
{
	m_timeWaterBomb += CGameTimeDx9::getInstance()->getElapsedGameTime().getMilliseconds();
	if (m_timeWaterBomb > 400)
	{
		m_ObjectState = eObjectState::STATE_RAMBO_SWIM;
	}
	return 0;
}

int Rambo::HandleInputClimbState()
{
	m_timeClimb += CGameTimeDx9::getInstance()->getElapsedGameTime().getMilliseconds();
	if (m_timeClimb > 400)
	{
		m_ObjectState = eObjectState::STATE_RAMBO_IDLE;
		m_Position.y = m_Position.y + 20;
	}
	return 0;
}

int Rambo::HandleInputShootRunState()
{
	m_Physic->setVelocityX(0.0f);
	if(CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown())
	{
		m_Physic->setVelocityX(VELOCITY_X_MOVE_TO_RIGHT);
		m_DirectAttack = eDirectAttack::AD_RIGHT;
	}
	if(CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
	{
		m_Physic->setVelocityX(VELOCITY_Y_MOVE_TO_LEFT);
		m_DirectAttack = eDirectAttack::AD_LEFT;
	}

	if (HandleInputShooting())
	{
		Shoot();
		m_timeDelayRunAndShootRun = 0;
	}
	if(m_RamboSprite->IsCompleteAnimation())
	{
		m_timeDelayRunAndShootRun += CGameTimeDx9::getInstance()->getElapsedGameTime().getMilliseconds();
		if (m_timeDelayRunAndShootRun > 800)
		{
			m_ObjectState = eObjectState::STATE_RAMBO_RUN;
			m_timeDelayRunAndShootRun = 0;
		}
		return 0;
	}

	
	if(CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightUp() || CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightDown())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_IDLE;
		return 0;
	}
	if(CInputDx9::getInstance()->IsKeyDown(DIK_UP))
	{
		m_ObjectState = eObjectState::STATE_RAMBO_AIM_TOP_RIGHT;
		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyDown(DIK_DOWN))
	{
		m_ObjectState = eObjectState::STATE_RAMBO_AIM_BOTTOM_RIGHT;
		return 0;	
	}
	if(CInputDx9::getInstance()->IsKeyPress(DIK_X))
	{
		m_ObjectState = eObjectState::STATE_RAMBO_JUMP;
		m_Physic->setVelocityY(VELOCITY_Y_JUMP);
		return 0;
	}
	return 0;
}

void Rambo::Shoot()
{
	switch (m_DirectAttack)
	{
	case AD_TOP:
	{
		if (m_SkillBullet == eIDSkillBullet::DEFAULT_SKILL_BULLET)
		{
			if(m_SkillBullet == eIDSkillBullet::DEFAULT_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::DEFAULT_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, 2.0f), 100);
			}
			else if(m_SkillBullet == eIDSkillBullet::M_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, 2.0f), 100);
			}
			else if(m_SkillBullet == eIDSkillBullet::R_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, 3.0f), 100);
			}
			else if(m_SkillBullet == eIDSkillBullet::F_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::FIRE_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, 2.0f), 100);
			}
			else if(m_SkillBullet == eIDSkillBullet::L_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::LAZER_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, 2.0f), 100, 90.0f);
			}
			else if(m_SkillBullet == eIDSkillBullet::S_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-0.25f, 2.0f), -11.43f);
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.25f, 2.0f), 11.43f);
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-0.75f, 2.0f), -3.73f);
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.75f, 2.0f), 3.73f);
				//BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, 2.0f), 1);
			}
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::DEFAULT_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, 2.0f), 100);
		}
		else if (m_SkillBullet == eIDSkillBullet::M_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, 2.0f), 100);
		}
		else if (m_SkillBullet == eIDSkillBullet::R_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, 3.0f), 100);
		}
		else if (m_SkillBullet == eIDSkillBullet::F_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::FIRE_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, 2.0f), 100);
		}
		else if (m_SkillBullet == eIDSkillBullet::L_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::LAZER_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, 2.0f), 100, 90.0f);
		}
		else if (m_SkillBullet == eIDSkillBullet::S_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-1.3f, 2.0f), -1.73f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-0.5f, 2.0f), -5.67f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.5f, 2.0f), 5.67f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(1.3f, 2.0f), 1.73f);
			//BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, 2.0f), 1);
		}
	}
		break;
	case AD_BOTTOM:
	{
		if (m_SkillBullet == eIDSkillBullet::DEFAULT_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::DEFAULT_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, -2.0f), 100);
		}
		else if (m_SkillBullet == eIDSkillBullet::M_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, -2.0f), 100);
		}
		else if (m_SkillBullet == eIDSkillBullet::R_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, -3.0f), 100);
		}
		else if (m_SkillBullet == eIDSkillBullet::F_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::FIRE_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, -2.0f), 100);
		}
		else if (m_SkillBullet == eIDSkillBullet::L_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::LAZER_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, -2.0f), 100, -90.0f);
		}
		else if (m_SkillBullet == eIDSkillBullet::S_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-1.3f, -2.0f), 1.73f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-0.5f, -2.0f), 5.67f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.5f, -2.0f), -5.67f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(1.3f, -2.0f), -1.73f);
			//BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(0.0f, -2.0f), 3.7);
		}
	}
		break;
	case AD_LEFT:
	{
		if (m_SkillBullet == eIDSkillBullet::DEFAULT_SKILL_BULLET)
		{
			if(m_SkillBullet == eIDSkillBullet::DEFAULT_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::DEFAULT_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), 0);
			}
			else if(m_SkillBullet == eIDSkillBullet::M_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), 0);
			}
			else if(m_SkillBullet == eIDSkillBullet::R_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), 0);
			}
			else if(m_SkillBullet == eIDSkillBullet::F_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::FIRE_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), 0);
			}
			else if(m_SkillBullet == eIDSkillBullet::L_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::LAZER_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), 0);	
			}
			else if(m_SkillBullet == eIDSkillBullet::S_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), 0.087);
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), -0.087);
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), -0.267);
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), 0.267);
			}
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::DEFAULT_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), 0);
		}
		else if (m_SkillBullet == eIDSkillBullet::M_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), 0);
		}
		else if (m_SkillBullet == eIDSkillBullet::R_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), 0);
		}
		else if (m_SkillBullet == eIDSkillBullet::F_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::FIRE_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), 0);
		}
		else if (m_SkillBullet == eIDSkillBullet::L_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::LAZER_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), 0);
		}
		else if (m_SkillBullet == eIDSkillBullet::S_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), -0.131f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), 0.131f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), 0.414f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), -0.414f);
		}
	}
		break;
	case AD_RIGHT:
	{
		if (m_SkillBullet == eIDSkillBullet::DEFAULT_SKILL_BULLET)
		{
			if(m_SkillBullet == eIDSkillBullet::DEFAULT_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::DEFAULT_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 0);
			}
			else if(m_SkillBullet == eIDSkillBullet::M_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 0);
			}
			else if(m_SkillBullet == eIDSkillBullet::R_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 0);
			}
			else if(m_SkillBullet == eIDSkillBullet::F_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::FIRE_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 0);
			}
			else if(m_SkillBullet == eIDSkillBullet::L_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::LAZER_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 0);
			}
			else if(m_SkillBullet == eIDSkillBullet::S_SKILL_BULLET)
			{
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 0.087);
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), -0.087);
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), -0.267);
				BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 0.267);
			}
			
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::DEFAULT_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 0);
		}
		else if (m_SkillBullet == eIDSkillBullet::M_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 0);
		}
		else if (m_SkillBullet == eIDSkillBullet::R_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 0);
		}
		else if (m_SkillBullet == eIDSkillBullet::F_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::FIRE_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 0);
		}
		else if (m_SkillBullet == eIDSkillBullet::L_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::LAZER_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 0);
		}
		else if (m_SkillBullet == eIDSkillBullet::S_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 0.414);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 0.131);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), -0.414);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), -0.131);
		}
	}
		break;
	case AD_TOP_RIGHT:
	{
		if (m_SkillBullet == eIDSkillBullet::DEFAULT_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::DEFAULT_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(2.0f, 0.0f), 1);
		}
		else if (m_SkillBullet == eIDSkillBullet::M_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(2.0f, 0.0f), 1);
		}
		else if (m_SkillBullet == eIDSkillBullet::R_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 1);
		}
		else if (m_SkillBullet == eIDSkillBullet::F_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::FIRE_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(2.0f, 0.0f), 1);
		}
		else if (m_SkillBullet == eIDSkillBullet::L_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::LAZER_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(2.0f, 0.0f), 1, 45.0f);
		}
		else if (m_SkillBullet == eIDSkillBullet::S_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.2f), 1.73f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.5f), 0.57f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.7f), 1.0f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 1.0f), 0.26f);
			//BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), -0.36);
		}
	}
		break;
	case AD_TOP_LEFT:
	{
		if (m_SkillBullet == eIDSkillBullet::DEFAULT_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::DEFAULT_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), -1);
		}
		else if (m_SkillBullet == eIDSkillBullet::M_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), -1);
		}
		else if (m_SkillBullet == eIDSkillBullet::R_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), -1);
		}
		else if (m_SkillBullet == eIDSkillBullet::F_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::FIRE_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), -1);
		}
		else if (m_SkillBullet == eIDSkillBullet::L_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::LAZER_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), -1, 135.0f);
		}
		else if (m_SkillBullet == eIDSkillBullet::S_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 4.0f), -1.73f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 3.5f), -0.57f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 3.0f), -1.0f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 2.0f), -0.26f);
			//BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), 0.36);
		}
	}
		break;
	case AD_BOTTOM_RIGHT:
	{
		if (m_SkillBullet == eIDSkillBullet::DEFAULT_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::DEFAULT_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(2.0f, 0.0f), -1);
		}
		else if (m_SkillBullet == eIDSkillBullet::M_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(2.0f, 0.0f), -1);
		}
		else if (m_SkillBullet == eIDSkillBullet::R_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), -1);
		}
		else if (m_SkillBullet == eIDSkillBullet::F_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::FIRE_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(2.0f, 0.0f), -1);
		}
		else if (m_SkillBullet == eIDSkillBullet::L_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::LAZER_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(2.0f, 0.0f), -1, -45.0f);
		}
		else if (m_SkillBullet == eIDSkillBullet::S_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.2f), 0.087f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 1.5f), -0.46f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 3.0f), -1.0f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 4.5f), -2.14f);
			//BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(3.0f, 0.0f), 5.67);
		}
	}
		break;
	case AD_BOTTOM_LEFT:
	{
		if (m_SkillBullet == eIDSkillBullet::DEFAULT_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::DEFAULT_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), 1);
		}
		else if (m_SkillBullet == eIDSkillBullet::M_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), 1);
		}
		else if (m_SkillBullet == eIDSkillBullet::R_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), 1);
		}
		else if (m_SkillBullet == eIDSkillBullet::F_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::FIRE_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), 1);
		}
		else if (m_SkillBullet == eIDSkillBullet::L_SKILL_BULLET)
		{
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::LAZER_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-2.0f, 0.0f), 1, -135.0f);
		}
		else if (m_SkillBullet == eIDSkillBullet::S_SKILL_BULLET)
		{
			//BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), -5.67);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), 0.267f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), 0.7f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), 1.0f);
			BulletPoolManager::getInstance()->addBulletIntoList(eIDTypeBullet::RED_BULLET_OF_RAMBO, GetStartPositionOfBullet(), D3DXVECTOR2(-3.0f, 0.0f), 2.14f);
		}
	}
	default:
		break;
	}
}

D3DXVECTOR3	Rambo::GetStartPositionOfBullet()
{
	switch (m_ObjectState)
	{
	case STATE_RAMBO_IDLE:
	{
		if (this->m_SkillBullet != eIDSkillBullet::F_SKILL_BULLET)
		{
			if (m_Direction == eDirection::LEFT)
			{
				return D3DXVECTOR3(m_Position.x - 30, m_Position.y + 8, 0);
			}
			if (m_Direction == eDirection::RIGHT)
			{
				return D3DXVECTOR3(m_Position.x + 30, m_Position.y + 8, 0);
			}
			return D3DXVECTOR3(m_Position.x, m_Position.y, 0);
		}
		else
		{
			if (m_Direction == eDirection::LEFT)
			{
				return D3DXVECTOR3(m_Position.x - 60, m_Position.y + 8, 0);
			}
			if (m_Direction == eDirection::RIGHT)
			{
				return D3DXVECTOR3(m_Position.x + 60, m_Position.y + 8, 0);
			}
			return D3DXVECTOR3(m_Position.x, m_Position.y, 0);
		}
	}
		break;
	case STATE_RAMBO_JUMP:
	{
		if (m_Direction == eDirection::LEFT)
		{
			return D3DXVECTOR3(m_Position.x - 30, m_Position.y, 0);
		}
		if (m_Direction == eDirection::RIGHT)
		{
			return D3DXVECTOR3(m_Position.x + 30, m_Position.y, 0);
		}
		return D3DXVECTOR3(m_Position.x, m_Position.y, 0);
	}
		break;
	case STATE_RAMBO_LIE:
	{
		if (m_Direction == eDirection::LEFT)
		{
			return D3DXVECTOR3(m_Position.x - 30, m_Position.y, 0);
		}
		else if (m_Direction == eDirection::RIGHT)
		{
			return D3DXVECTOR3(m_Position.x + 30, m_Position.y, 0);
		}
	}
		break;
	case STATE_RAMBO_AIM_BOTTOM_RIGHT:
	{
		if (m_Direction == eDirection::LEFT)
		{
			return D3DXVECTOR3(m_Position.x - 24, m_Position.y - 10, 0);
		}
		if (m_Direction == eDirection::RIGHT)
		{
			return D3DXVECTOR3(m_Position.x + 22, m_Position.y - 10, 0);
		}
		return D3DXVECTOR3(m_Position.x, m_Position.y + 22, 0);
	}
		break;
	case STATE_RAMBO_SHOOT_RUN:
	{
		if (m_SkillBullet != eIDSkillBullet::F_SKILL_BULLET)
		{
			if (m_Direction == eDirection::LEFT)
			{
				return D3DXVECTOR3(m_Position.x - 30, m_Position.y + 9, 0);
			}
			else if (m_Direction == eDirection::RIGHT)
			{
				return D3DXVECTOR3(m_Position.x + 30, m_Position.y + 9, 0);
			}
		}
		else
		{
			if (m_Direction == eDirection::LEFT)
			{
				return D3DXVECTOR3(m_Position.x - 60, m_Position.y + 9, 0);
			}
			else if (m_Direction == eDirection::RIGHT)
			{
				return D3DXVECTOR3(m_Position.x + 60, m_Position.y + 9, 0);
			}
		}
	}
		break;
	case STATE_RAMBO_AIM_TOP_RIGHT:
	{
		if (m_Direction == eDirection::LEFT)
		{
			return D3DXVECTOR3(m_Position.x - 12, m_Position.y + 10, 0);
		}
		if (m_Direction == eDirection::RIGHT)
		{
			return D3DXVECTOR3(m_Position.x + 10, m_Position.y + 10, 0);
		}
		return D3DXVECTOR3(m_Position.x, m_Position.y, 0);
	}
		break;
	case STATE_RAMBO_AIM_UP:
	{
		if (m_Direction == eDirection::LEFT)
		{
			return D3DXVECTOR3(m_Position.x - 7, m_Position.y + 50, 0);
		}
		if (m_Direction == eDirection::RIGHT)
		{
			return D3DXVECTOR3(m_Position.x + 7, m_Position.y + 50, 0);
		}
	}
		break;
	case STATE_RAMBO_SWIM_SHOOT:
	{
		return D3DXVECTOR3(m_Position.x, m_Position.y - 1, 0);
	}
		break;
	case STATE_RAMBO_SWIM_SHOOT_UP:
	{
		if (m_Direction == eDirection::LEFT)
		{
			return D3DXVECTOR3(m_Position.x - 9, m_Position.y + 8, 0);
		}
		if (m_Direction == eDirection::RIGHT)
		{
			return D3DXVECTOR3(m_Position.x + 5, m_Position.y + 8, 0);
		}
		return D3DXVECTOR3(m_Position.x, m_Position.y, 0);
	}
		break;
	case STATE_RAMBO_SWIM_SHOOT_TOP_RIGHT:
	{
		if (m_Direction == eDirection::LEFT)
		{
			return D3DXVECTOR3(m_Position.x - 16, m_Position.y + 10, 0);
		}
		if (m_Direction == eDirection::RIGHT)
		{
			return D3DXVECTOR3(m_Position.x + 14, m_Position.y + 10, 0);
		}
		return D3DXVECTOR3(m_Position.x, m_Position.y, 0);
	}
		break;
	default:
	{
		return D3DXVECTOR3(m_Position.x, m_Position.y, 0);
	}
		break;
	}
	return D3DXVECTOR3(0, 0, 0);
}

int Rambo::HandleInputJumpState()
{
	if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown())
	{
		m_Physic->setVelocityX(VELOCITY_X_MOVE_TO_RIGHT);
		m_DirectAttack = eDirectAttack::AD_RIGHT;
		if (CInputDx9::getInstance()->IsKeyUpDownAndKeyDownUp())
		{
			m_DirectAttack = eDirectAttack::AD_TOP_RIGHT;
		}
		if (CInputDx9::getInstance()->IsKeyUpUpAndKeyDownDown())
		{
			m_DirectAttack = eDirectAttack::AD_BOTTOM_RIGHT;
		}
	}
	if (CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
	{
		m_Physic->setVelocityX(VELOCITY_Y_MOVE_TO_LEFT);
		m_DirectAttack = eDirectAttack::AD_LEFT;
		if (CInputDx9::getInstance()->IsKeyUpDownAndKeyDownUp())
		{
			m_DirectAttack = eDirectAttack::AD_TOP_LEFT;
		}
		if (CInputDx9::getInstance()->IsKeyUpUpAndKeyDownDown())
		{
			m_DirectAttack = eDirectAttack::AD_BOTTOM_LEFT;
		}
	}
	if (CInputDx9::getInstance()->IsKeyUpDownAndKeyDownUp())
	{
		m_DirectAttack = eDirectAttack::AD_TOP;
	}
	if (CInputDx9::getInstance()->IsKeyUpUpAndKeyDownDown())
	{
		m_DirectAttack = eDirectAttack::AD_BOTTOM;
	}
	if (HandleInputShooting())
	{
		Shoot();
	}
	return 0;
}

int Rambo::HandleInputDiveState()
{
	if (CInputDx9::getInstance()->IsKeyUpDownAndKeyDownDown() || CInputDx9::getInstance()->IsKeyUp(DIK_DOWN))
	{
		m_ObjectState = STATE_RAMBO_SWIM;
	}
	return 1;
}

int Rambo::HandleInputSwimShootState()
{
	m_Physic->setVelocityX(0.0f);
	if (m_Direction == eDirection::RIGHT)
	{
		m_DirectAttack = eDirectAttack::AD_RIGHT;
	}
	if (m_Direction == eDirection::LEFT)
	{
		m_DirectAttack = eDirectAttack::AD_LEFT;
	}


	if (CInputDx9::getInstance()->IsKeyUpDownAndKeyDownUp())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_SWIM_SHOOT_UP;
	}
	if (HandleInputShooting())
	{
		Shoot();
		m_timeDelayRunAndShootRun = 0;
	}
	if (m_RamboSprite->IsCompleteAnimation())
	{
		m_timeDelayRunAndShootRun += CGameTimeDx9::getInstance()->getElapsedGameTime().getMilliseconds();
		if (m_timeDelayRunAndShootRun > 400)
		{
			m_ObjectState = eObjectState::STATE_RAMBO_SWIM;
			m_timeDelayRunAndShootRun = 0;
		}
	}
	if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown())
	{
		m_Physic->setVelocityX(VELOCITY_X_MOVE_TO_RIGHT);

		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
	{
		m_Physic->setVelocityX(VELOCITY_Y_MOVE_TO_LEFT);

		return 0;
	}
	return 1;
}

int Rambo::HandleInputSwimShootUpState()
{
	m_DirectAttack = eDirectAttack::AD_TOP;
	if (
		CInputDx9::getInstance()->IsKeyUp(DIK_UP) || CInputDx9::getInstance()->IsKeyUpDownAndKeyDownDown())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_SWIM;
		return 0;
	}
	if (HandleInputShooting())
	{
		if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown() || CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
		{
			m_ObjectState = eObjectState::STATE_RAMBO_SWIM_SHOOT_TOP_RIGHT;
			if (m_Direction == eDirection::RIGHT)
			{
				m_DirectAttack = eDirectAttack::AD_TOP_RIGHT;
			}
			if (m_Direction == eDirection::LEFT)
			{
				m_DirectAttack = eDirectAttack::AD_TOP_LEFT;
			}
		}
		Shoot();
		m_timeDelayRunAndShootRun = 0;
	}
	if (m_RamboSprite->IsCompleteAnimation())
	{
		m_timeDelayRunAndShootRun += CGameTimeDx9::getInstance()->getElapsedGameTime().getMilliseconds();
		if (m_timeDelayRunAndShootRun > 800)
		{
			m_ObjectState = eObjectState::STATE_RAMBO_SWIM;
			m_timeDelayRunAndShootRun = 0;
		}
		return 0;
	}
	return 1;
}

int Rambo::HandleInputSwimShootTopRightState()
{
	if (CInputDx9::getInstance()->IsKeyUp(DIK_UP))
	{
		m_ObjectState = eObjectState::STATE_RAMBO_SWIM;
		return 0;
	}

	if (CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightDown() || CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightUp())
	{
		m_ObjectState = eObjectState::STATE_RAMBO_SWIM_SHOOT_UP;
		return 0;
	}
	if (CInputDx9::getInstance()->IsKeyLeftUpAndKeyRightDown())
	{
		m_Physic->setVelocityX(VELOCITY_X_MOVE_TO_RIGHT);
		m_DirectAttack = eDirectAttack::AD_TOP_RIGHT;
	}
	if (CInputDx9::getInstance()->IsKeyLeftDownAndKeyRightUp())
	{
		m_Physic->setVelocityX(VELOCITY_Y_MOVE_TO_LEFT);
		m_DirectAttack = eDirectAttack::AD_TOP_LEFT;
	}
	if (HandleInputShooting())
	{
		Shoot();
		m_timeDelayRunAndShootRun = 0;
	}
	if (m_RamboSprite->IsCompleteAnimation())
	{
		m_timeDelayRunAndShootRun += CGameTimeDx9::getInstance()->getElapsedGameTime().getMilliseconds();
		if (m_timeDelayRunAndShootRun > 800)
		{
			m_ObjectState = eObjectState::STATE_RAMBO_SWIM;
			m_timeDelayRunAndShootRun = 0;
		}
		return 0;
	}
	return 0;
}

void Rambo::UpdateAnimation()
{
	m_RamboSprite->UpdateAnimation(m_ObjectState);
	if (isInvulnerable)
	{
		UpdateInvulnerableAnimation();
	}
}

int Rambo::UpdateInvulnerableAnimation()
{
	m_colorAlpha -= 2 * m_inverseColorAlpha;
	if (m_colorAlpha < 100 + 2 || m_colorAlpha > 255 - 2)
	{
		m_inverseColorAlpha *= -1;
		m_timeInvulnerable++;
	}
	if (m_colorAlpha < 100)
	{
		m_colorAlpha = 100;
	}
	if (m_colorAlpha > 255)
	{
		m_colorAlpha = 255;
	}
	if (m_timeInvulnerable > 6 && m_colorAlpha == 255)
	{
		m_timeInvulnerable = 0;
		isInvulnerable = false;
	}
	return 0;
}

int Rambo::RunScript(const char* scriptPath)
{
	fstream fScript(scriptPath, ios::in);
	string buffer;
	fScript >> buffer;
	if (buffer == "RUNTO")
	{
		int DestinationX;
		fScript >> DestinationX;
		m_Position.x = (float)(int)m_Position.x;
		if (DestinationX > m_Position.x)
		{
			CInputDx9::getInstance()->SetKeyDown(DIK_RIGHT);
		}
		if (DestinationX < m_Position.x)
		{
			CInputDx9::getInstance()->SetKeyDown(DIK_LEFT);
		}
	}
	fScript >> buffer;
	if (buffer == "JUMP")
	{
		int location = 0;
		fScript >> location;
		if (m_Position.x == location)
		{
			CInputDx9::getInstance()->SetKeyDown(DIK_RIGHT);
			CInputDx9::getInstance()->SetKeyDown(DIK_X);
		}
	}

	fScript.close();
	return 0;
}

void Rambo::SetFallFlag()
{
	switch (m_ObjectState)
	{
	case STATE_RAMBO_AIM_BOTTOM_RIGHT:
	case STATE_RAMBO_AIM_TOP_RIGHT:
	case STATE_RAMBO_AIM_UP:
	case STATE_RAMBO_IDLE:
	case STATE_RAMBO_LIE:
	case STATE_RAMBO_MOVE:
	case STATE_RAMBO_RUN:
	case STATE_RAMBO_SHOOT:
	case STATE_RAMBO_SHOOT_RUN:
	case STATE_RAMBO_FALL:
		isFall = true;
		break;
	case STATE_RAMBO_BEFORE_DEAD:
	case STATE_RAMBO_WATER_BOMB:
	case STATE_RAMBO_JUMP:
	case STATE_RAMBO_DIVE:
	case STATE_RAMBO_SWIM:
	case STATE_RAMBO_CLIMB:
	case STATE_RAMBO_SWIM_SHOOT:
	case STATE_RAMBO_SWIM_SHOOT_UP:
	case STATE_RAMBO_SWIM_SHOOT_TOP_RIGHT:
		isFall = false;
		break;
	default:
		break;
	}
}

void Rambo::SetVelocityXZero()
{
	switch (m_ObjectState)
	{
	case STATE_RAMBO_AIM_BOTTOM_RIGHT:
	case STATE_RAMBO_AIM_TOP_RIGHT:
	case STATE_RAMBO_JUMP:
	case STATE_RAMBO_MOVE:
	case STATE_RAMBO_RUN:
	case STATE_RAMBO_SHOOT:
	case STATE_RAMBO_SHOOT_RUN:
	case STATE_RAMBO_SWIM:
	case STATE_RAMBO_SWIM_SHOOT_TOP_RIGHT:
	case STATE_RAMBO_SWIM_SHOOT:

		return;
	case STATE_RAMBO_IDLE:
	case STATE_RAMBO_SWIM_SHOOT_UP:
	case STATE_RAMBO_FALL:
	case STATE_RAMBO_CLIMB:
	case STATE_RAMBO_DIVE:
	case STATE_RAMBO_WATER_BOMB:
	case STATE_RAMBO_AIM_UP:
	case STATE_RAMBO_LIE:
	case STATE_RAMBO_DEAD:
		m_Physic->setVelocityX(0.0f);
		return;
	default:
		break;
	}
}

void Rambo::SetVelocityYZero()
{
	switch (m_ObjectState)
	{
	case STATE_RAMBO_AIM_BOTTOM_RIGHT:
	case STATE_RAMBO_AIM_TOP_RIGHT:
	case STATE_RAMBO_AIM_UP:
	case STATE_RAMBO_IDLE:
	case STATE_RAMBO_JUMP:
	case STATE_RAMBO_LIE:
	case STATE_RAMBO_MOVE:
	case STATE_RAMBO_RUN:
	case STATE_RAMBO_SHOOT:
	case STATE_RAMBO_SHOOT_RUN:
	case STATE_RAMBO_FALL:
		return;
	case STATE_RAMBO_SWIM:
	case STATE_RAMBO_SWIM_SHOOT:
	case STATE_RAMBO_CLIMB:
	case STATE_RAMBO_DIVE:
	case STATE_RAMBO_WATER_BOMB:
	case STATE_RAMBO_SWIM_SHOOT_UP:
	case STATE_RAMBO_SWIM_SHOOT_TOP_RIGHT:
	case STATE_RAMBO_DEAD:
		m_Physic->setVelocityY(0.0f);
		return;
	default:
		break;
	}
}

void Rambo::setRectangleCheckingObjectBelow()
{
	m_RectangleCheckingObjectBelow.left = (long)(int)(m_Position.x - 5);
	m_RectangleCheckingObjectBelow.top = (long)(int)(m_Position.y - 17);
	
	m_RectangleCheckingObjectBelow.right = (long)(int)(m_Position.x + 5);
	m_RectangleCheckingObjectBelow.bottom = (long)(int)(0);
}

int Rambo::UpdateCollisionTileBase(IDDirection collideDirection, Object* checkingObject)
{
	if (collideDirection == IDDirection::DIR_TOP && checkingObject != m_ignoreCollisionObject)
	{
		isFall = false;

		if (m_ObjectState == STATE_RAMBO_JUMP || m_ObjectState == STATE_RAMBO_BEFORE_DEAD)
		{
			if (m_Physic->getVelocity().y < 0)
			{
				if (m_ObjectState == STATE_RAMBO_JUMP && m_maxPositionY - checkingObject->getPositionVec2().y > 37)
				{
					m_ObjectState = STATE_RAMBO_IDLE;
					this->m_Position.y += this->m_Collision->m_MoveY;
					m_Physic->setVelocityY(0.0f);
					m_maxPositionY = 0;
				}
				if (m_ObjectState == STATE_RAMBO_BEFORE_DEAD && m_maxPositionY - checkingObject->getPositionVec2().y > 60)
				{
					m_ObjectState = STATE_RAMBO_DEAD;
					this->m_Position.y += this->m_Collision->m_MoveY;
					m_Physic->setVelocityY(0.0f);
					m_maxPositionY = 0;
				}
				return 0;
			}
		}
		else
		{
			if (m_ObjectState == STATE_RAMBO_FALL)
			{
				m_ObjectState = STATE_RAMBO_IDLE;
				this->m_Position.y += this->m_Collision->m_MoveY;
				m_Physic->setVelocityY(0.0f);
				return 0;
			}
			else
			{
				if (m_ObjectState == STATE_RAMBO_LIE)
				{
					this->m_Position.y += this->m_Collision->m_MoveY;
					m_Physic->setVelocityY(0.0f);
					if (CInputDx9::getInstance()->IsKeyDown(DIK_X) && m_objectBelowPrevious.size() > 1)
					{
						m_ignoreCollisionObject = checkingObject;
						m_ObjectState = STATE_RAMBO_FALL;
					}
					return 0;
				}
				else
				{
					if (m_ObjectState == eObjectState::STATE_RAMBO_SWIM || m_ObjectState == eObjectState::STATE_RAMBO_SWIM_SHOOT ||
						m_ObjectState == eObjectState::STATE_RAMBO_SWIM_SHOOT_UP || m_ObjectState == eObjectState::STATE_RAMBO_SWIM_SHOOT_TOP_RIGHT)
					{
						if ((m_Position.x >= checkingObject->getBound().left && m_Physic->getVelocity().x > 0) ||
							(m_Position.x <= checkingObject->getBound().right && m_Physic->getVelocity().x < 0))
						{
							m_ObjectState = eObjectState::STATE_RAMBO_CLIMB;
							return 0;
						}
						return 0;
					}
					else
					{
						if (m_ObjectState == eObjectState::STATE_RAMBO_CLIMB)
						{
							m_Physic->setVelocityX(0.0f);
							return 0;
						}
						else
						{
							this->m_Position.y += this->m_Collision->m_MoveY;
							m_Physic->setVelocityY(0.0f);
							return 0;
						}
					}
				}
			}
		}


		return 0;
	}
	else if (collideDirection == IDDirection::DIR_BOTTOM)
	{

		if (m_ignoreCollisionObject == checkingObject)
		{
			m_ignoreCollisionObject = 0;
		}
		return 0;
	}

	else if (collideDirection == IDDirection::DIR_LEFT)
	{

		if (m_ObjectState == eObjectState::STATE_RAMBO_SWIM)
		{
			m_Position.x += 1;
		}
		return 0;
	}

	else if (collideDirection == IDDirection::DIR_RIGHT)
	{

		if (m_ObjectState == eObjectState::STATE_RAMBO_SWIM)
		{
			m_Position.x -= 1;
		}
		return 0;
	}
	return 1;
}

void Rambo::UpdateCollision(Object* checkingObject)
{
	if (m_ObjectState == eObjectState::STATE_RAMBO_BEFORE_DEAD)
	{
		CGlobal::LiveOfRambo = true;
	}
	else
	{
		CGlobal::LiveOfRambo = false;
	}

	if (checkingObject->getID() == eObjectID::FIRE_BRIDGE)
	{
		FireBridge* fireBridge = (FireBridge*)checkingObject;
		IDDirection collideDirection1 = this->m_Collision->CheckCollision(this, (Object*)fireBridge->m_fire_1);
		IDDirection collideDirection2 = this->m_Collision->CheckCollision(this, (Object*)fireBridge->m_fire_2);
		if (collideDirection1 != IDDirection::DIR_NONE || collideDirection2 != IDDirection::DIR_NONE)
		{
			m_ObjectState = eObjectState::STATE_RAMBO_BEFORE_DEAD;
			SoundManagerDx9::getInstance()->getSoundBuffer(eSoundID::rambo_dead_sfx)->Play();
		}
	}
	else if (checkingObject->getID() == eObjectID::BIG_CAPSULE_BOSS)
	{
		BigCapsuleBoss* bigCapsule = (BigCapsuleBoss*)checkingObject;
		if (bigCapsule->m_ListEnemy.empty() != true)
		{
			for (list<CapsuleBoss*>::iterator i = bigCapsule->m_ListEnemy.begin();
				i != bigCapsule->m_ListEnemy.end(); i++)
			{
				UpdateCollision((*i));
			}
		}
		if (bigCapsule->m_ListBullet.empty() != true)
		{
			for (list<BossBullet*>::iterator i = bigCapsule->m_ListBullet.begin();
				i != bigCapsule->m_ListBullet.end(); i++)
			{
				UpdateCollision((*i));
			}
		}
	}
	else
	{
		if (checkingObject->getObjectState() != eObjectState::STATE_BEFORE_DEATH && checkingObject->getObjectState() != eObjectState::STATE_DEATH)
		{
			D3DXVECTOR3 curPos;
			if (isInvulnerable)
			{
				curPos = this->m_Position;
			}
			IDDirection collideDirection = this->m_Collision->CheckCollision(this, checkingObject);
			if (isInvulnerable)
			{
				m_Position = curPos;
			}
			setRectangleCheckingObjectBelow();
			if (checkingObject->getTypeObject() == ETypeObject::VIRTUAL_OBJECT)
			{
				if (Intersect(checkingObject->getBound(), m_RectangleCheckingObjectBelow))
				{
					m_objectBelowCurrent.push_back(checkingObject);
				}
			}

			if (collideDirection != IDDirection::DIR_NONE)
			{
				switch (checkingObject->getTypeObject())
				{
				case ETypeObject::VIRTUAL_OBJECT:
				{
					switch (checkingObject->getID())
					{
					case eObjectID::TILE_BASE:
						//case eObjectID::MAGIC_ROCK:
#pragma region TILE_BASE
					{
						UpdateCollisionTileBase(collideDirection, checkingObject);
					}
						break;
#pragma endregion
					case eObjectID::VIRTUAL_OBJECT_WATER:
					{
						if (collideDirection == IDDirection::DIR_TOP)
						{
							if (m_ObjectState == eObjectState::STATE_RAMBO_DEAD || m_ObjectState == eObjectState::STATE_RAMBO_BEFORE_DEAD)
							{
								return;
							}
							if (m_ObjectState == eObjectState::STATE_RAMBO_WATER_BOMB)
							{
								this->m_Position.y += this->m_Collision->m_MoveY;
							}
							else
							{
								if (m_ObjectState == eObjectState::STATE_RAMBO_JUMP || m_ObjectState == eObjectState::STATE_RAMBO_FALL)
								{
									m_ObjectState = eObjectState::STATE_RAMBO_WATER_BOMB;
									this->m_Position.y += this->m_Collision->m_MoveY;
								}
								else
								{
									m_ObjectState = eObjectState::STATE_RAMBO_SWIM;
									this->m_Position.y += this->m_Collision->m_MoveY;
								}
							}
						}
						isFall = false;
					}
						break;
					default:
						break;
					}
				}
					break;
				default:
				{

					switch (checkingObject->getID())
					{
					case eObjectID::ROSHAN:
					{
						Roshan* roshan = (Roshan*)checkingObject;
						this->UpdateCollision(roshan->mRoshanHead);
						if (roshan->mLeftHand->mPunch)
						{
							this->UpdateCollision(roshan->mLeftHand->mPunch);
						}
						if (roshan->mRightHand->mPunch)
						{
							this->UpdateCollision(roshan->mRightHand->mPunch);
						}
						if (roshan->mRoshanHead->mListRoshanBullet.empty() != true)
						{
							for (list<BossBullet*>::iterator i = roshan->mRoshanHead->mListRoshanBullet.begin();
								i != roshan->mRoshanHead->mListRoshanBullet.end(); i++)
							{
								UpdateCollision((*i));
							}
						}
						if (roshan->mLeftHand->mListRoshanBullet.empty() != true)
						{
							for (list<BossBullet*>::iterator i = roshan->mLeftHand->mListRoshanBullet.begin();
								i != roshan->mLeftHand->mListRoshanBullet.end(); i++)
							{
								UpdateCollision((*i));
							}
						}
						if (roshan->mRightHand->mListRoshanBullet.empty() != true)
						{
							for (list<BossBullet*>::iterator i = roshan->mRightHand->mListRoshanBullet.begin();
								i != roshan->mRightHand->mListRoshanBullet.end(); i++)
							{
								UpdateCollision((*i));
							}
						}
					}
						break;

						//case eObjectID::BIG_CAPSULE_BOSS:
						//{
						//						   BigCapsuleBoss* bigCapsule = (BigCapsuleBoss*) checkingObject;
						//						   if (bigCapsule->m_ListEnemy.empty() != true)
						//						   {
						//							   for (list<CapsuleBoss*>::iterator i = bigCapsule->m_ListEnemy.begin();
						//								   i != bigCapsule->m_ListEnemy.end(); i++)
						//							   {
						//								   UpdateCollision((*i));
						//							   }
						//						   }
						//						   if (bigCapsule->m_ListBullet.empty() != true)
						//						   {
						//							   for (list<BossBullet*>::iterator i = bigCapsule->m_ListBullet.begin();
						//								   i != bigCapsule->m_ListBullet.end(); i++)
						//							   {
						//								   UpdateCollision((*i));
						//							   }
						//						   }
						//}
						// break;

					case eObjectID::ROSHAN_HEAD:
					case eObjectID::ROSHAN_PUNCH:
					case eObjectID::ENEMY_RUN:
					case eObjectID::GUN_ROTATING:
					case eObjectID::BIG_GUN_ROTATING:
					case eObjectID::ENEMY_RUN_SHOOTING:
						if (isInvulnerable || m_ObjectState == eObjectState::STATE_RAMBO_DIVE)
						{
							break;
						}
						if (checkingObject->getObjectState() != eObjectState::STATE_BEFORE_DEATH && checkingObject->getObjectState() != eObjectState::STATE_DEATH)
						{
							m_ObjectState = eObjectState::STATE_RAMBO_BEFORE_DEAD;
							SoundManagerDx9::getInstance()->getSoundBuffer(eSoundID::rambo_dead_sfx)->Play();
						}
						break;
#pragma region Update Collision with Bridge
					case eObjectID::BRIDGE:
						if (collideDirection == IDDirection::DIR_TOP)
						{
							isFall = false;
							if (this->m_ObjectState == eObjectState::STATE_RAMBO_RUN)
							{
								this->m_Position.y += this->m_Collision->m_MoveY;
								this->getPhysic()->setVelocityY(0.0f);
								return;
							}
							else if (this->m_ObjectState == eObjectState::STATE_RAMBO_JUMP)
							{
								this->m_ObjectState = eObjectState::STATE_RAMBO_IDLE;
								this->m_Position.y += this->m_Collision->m_MoveY;
								this->getPhysic()->setVelocityY(0.0f);

								return;
							}
							else if (m_ObjectState == eObjectState::STATE_RAMBO_FALL)
							{
								m_ObjectState = STATE_RAMBO_IDLE;
								this->m_Position.y += this->m_Collision->m_MoveY;
								m_Physic->setVelocityY(0.0f);
								return;
							}
							else
							{
								this->m_Position.y += this->m_Collision->m_MoveY;
								m_Physic->setVelocityY(0.0f);
								return;
							}
						}
						break;
#pragma endregion
					case eObjectID::MAGIC_ROCK:
						if (collideDirection == IDDirection::DIR_TOP && this->getPhysic()->getVelocity().y < 0)
						{
							isFall = false;
							if (this->m_ObjectState == eObjectState::STATE_RAMBO_RUN)
							{
								this->m_Position.y += this->m_Collision->m_MoveY;
								this->getPhysic()->setVelocityY(0.0f);
								prePosX = 0;
								finalPosX = 0;
								return;
							}
							else if (this->m_ObjectState == eObjectState::STATE_RAMBO_JUMP)
							{
								this->m_ObjectState = eObjectState::STATE_RAMBO_IDLE;
								this->m_Position.y += this->m_Collision->m_MoveY;
								this->getPhysic()->setVelocityY(0.0f);
								prePosX = 0;
								finalPosX = 0;
								return;
							}
							else if (m_ObjectState == eObjectState::STATE_RAMBO_FALL)
							{
								m_ObjectState = STATE_RAMBO_IDLE;
								this->m_Position.y += this->m_Collision->m_MoveY;
								m_Physic->setVelocityY(0.0f);
								prePosX = 0;
								finalPosX = 0;
								return;
							}
							else
							{
								finalPosX = checkingObject->getPositionVec2().x;
								if (prePosX == 0)
								{
									prePosX = finalPosX;
								}
								this->m_Position.y += this->m_Collision->m_MoveY;
								m_Physic->setVelocityY(0.0f);
								this->m_Position.x += finalPosX - prePosX;
								prePosX = checkingObject->getPositionVec2().x;
								return;
							}


						}
						break;
					case eObjectID::CAPSULE_BOSS:
					case eObjectID::BULLET_ENEMY:
					case eObjectID::FIRE:
						if (isInvulnerable || m_ObjectState == eObjectState::STATE_RAMBO_DIVE)
						{
							break;
						}
						if (this->m_ObjectState != eObjectState::STATE_RAMBO_DEAD && this->m_ObjectState != eObjectState::STATE_RAMBO_BEFORE_DEAD)
						{
							m_ObjectState = eObjectState::STATE_RAMBO_BEFORE_DEAD;
							SoundManagerDx9::getInstance()->getSoundBuffer(eSoundID::rambo_dead_sfx)->Play();
							checkingObject->setObjectState(eObjectState::STATE_DEATH);
						}
						break;
					case eObjectID::BULLET_BOSS1:
					case eObjectID::TANK:
					case eObjectID::BOOM:
					case eObjectID::STONE:
						if (isInvulnerable)
						{
							break;
						}
						if (this->m_ObjectState != eObjectState::STATE_RAMBO_DEAD && this->m_ObjectState != eObjectState::STATE_RAMBO_BEFORE_DEAD)
						{
							m_ObjectState = eObjectState::STATE_RAMBO_BEFORE_DEAD;
							SoundManagerDx9::getInstance()->getSoundBuffer(eSoundID::rambo_dead_sfx)->Play();
						}
						break;
					case eObjectID::FIRE_BRIDGE:


						break;
						/*case eObjectID::STONE:
							m_ObjectState = eObjectState::STATE_RAMBO_BEFORE_DEAD;
							SoundManagerDx9::getInstance()->getSoundBuffer(eSoundID::rambo_dead_sfx)->Play();
							break;*/
					default:
						break;
					}
					break;
				}
				}
			}
		}
	}
}

void Rambo::SetFlag()
{
	SetFallFlag();	
	SetVelocityXZero();
	SetVelocityYZero();
}

void Rambo::CleanIgnoreList()
{
	m_objectBelowCurrent.clear();
}

void Rambo::UpdatePreviousIgnoreList()
{
	m_objectBelowPrevious.clear();
	m_objectBelowPrevious = m_objectBelowCurrent;
}

void Rambo::UpdateMovement()
{
	if(Camera::getInstance()->getBound().left > this->m_Position.x - m_RamboSprite->GetFrameSize().x/2)
	{
		m_Position.x = Camera::getInstance()->getBound().left + 1 + m_RamboSprite->GetFrameSize().x/2;
		m_Physic->setVelocityX(0.0f);
	}

	if(Camera::getInstance()->getBound().right < this->m_Position.x + m_RamboSprite->GetFrameSize().x/2)
	{
		m_Position.x = Camera::getInstance()->getBound().right - 1 - m_RamboSprite->GetFrameSize().x/2;
		m_Physic->setVelocityX(0.0f);
	}

	this->m_Physic->UpdateMovement(&m_Position);

	if (m_ObjectState == STATE_RAMBO_JUMP || m_ObjectState == STATE_RAMBO_BEFORE_DEAD)
	{
		if (m_maxPositionY < m_Position.y)
		{
			m_maxPositionY = m_Position.y;
		}
	}
	CGlobal::Rambo_X = (int)(getPositionVec2().x);
	CGlobal::Rambo_Y = (int)(getPositionVec2().y);
}

void Rambo::Update()
{

}

void Rambo::Render(SPRITEHANDLE spriteHandle)
{
	int numRender = m_life;
	if (m_life > 5) {
		numRender = 5;
	}
	else {
		numRender = m_life;
	}
	for (int i = 1; i < numRender; i++)
	{
		SpriteManager::getInstance()->getSprite(eSpriteID::SPRITE_MEDAL)->RenderWithoutTransform(
			spriteHandle,
			D3DXVECTOR2((float)(32 * i), 408),
			ESpriteEffect::None,
			0.0f,
			1.0f,
			1.0f);
	}
	D3DXCOLOR color;
	if (isInvulnerable)
	{
		color = D3DCOLOR_ARGB(m_colorAlpha, 0xff, 0xff, 0xff);
	}
	else
	{
		color = D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff);
	}
	if (m_Direction == eDirection::RIGHT)
	{
		m_RamboSprite->Render(spriteHandle,
			getPositionVec2(),
			ESpriteEffect::None,
			0.0f,
			1.0f,
			1.0f,
			color);
		return;
	}
	if (m_Direction == eDirection::LEFT)
	{
		m_RamboSprite->Render(spriteHandle,
			getPositionVec2(),
			ESpriteEffect::Horizontally,
			0.0f,
			1.0f,
			1.0f,
			color);
		return;
	}
}

void Rambo::Release()
{

}

Rambo::~Rambo()
{

}