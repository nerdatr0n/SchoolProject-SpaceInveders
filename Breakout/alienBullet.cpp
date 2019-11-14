//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2019 Media Design School.
//
// File Name	: 
// Description	: 
// Author		: David Haverland
// Mail			: david.hav8466@mediadesign.school.nz
//

// Library Includes

// Local Includes
#include "resource.h"
#include "utils.h"

// This Includes
#include "alienBullet.h"

// Static Variables

// Static Function Prototypes

// Implementation

CAlienBullet::CAlienBullet()
	: m_fVelocityX(0.0f)
	, m_fVelocityY(0.0f)
{

}

CAlienBullet::~CAlienBullet()
{

}

bool
CAlienBullet::Initialise(float _fPosX, float _fPosY, float _fVelocityX, float _fVelocityY)
{
	VALIDATE(CEntity::Initialise(IDB_ALIENBULLETSPRITE, IDB_ALIENBULLETMASK));

	m_fX = _fPosX;
	m_fY = _fPosY;

	m_fVelocityX = _fVelocityX;
	m_fVelocityY = _fVelocityY;

	return (true);
} 

void
CAlienBullet::Draw()
{
	if (m_bCanHit)
	{
		CEntity::Draw();
	}
}

void
CAlienBullet::Process(float _fDeltaTick)
{
	// m_fX += m_fVelocityX * _fDeltaTick;
	m_fY += 200 * _fDeltaTick;


	if (m_fY >= 500)
	{
		m_bCanHit = false;

	}
	if (m_bCanHit == false)
	{

	}

	CEntity::Process(_fDeltaTick);
}

float
CAlienBullet::GetVelocityX() const
{
	return (m_fVelocityX);
}

float
CAlienBullet::GetVelocityY() const
{
	return (m_fVelocityY);
}

void
CAlienBullet::SetVelocityX(float _fX)
{
	m_fVelocityX = _fX;
}

void
CAlienBullet::SetVelocityY(float _fY)
{
	m_fVelocityY = _fY;
}


bool
CAlienBullet::GetCanHit() const
{
	return (m_bCanHit);
}

void
CAlienBullet::SetCanHit(bool _bCanHit)
{
	m_bCanHit = _bCanHit;
}

float
CAlienBullet::GetRadius() const
{
	return (GetWidth() / 2.0f);
}



