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
#include "bullet.h"

// Static Variables

// Static Function Prototypes

// Implementation

CBullet::CBullet()
: m_fVelocityX(0.0f)
, m_fVelocityY(0.0f)
{

}

CBullet::~CBullet()
{

}

bool
CBullet::Initialise(float _fPosX, float _fPosY, float _fVelocityX, float _fVelocityY)
{
    VALIDATE(CEntity::Initialise(IDB_BALLSPRITE, IDB_BALLMASK));
    
    m_fX = _fPosX;
    m_fY = _fPosY;

    m_fVelocityX = _fVelocityX;
    m_fVelocityY = _fVelocityY;

    return (true);
}

void
CBullet::Draw()
{
	if (m_bCanHit)
	{
		CEntity::Draw();
	}
}

void
CBullet::Process(float _fDeltaTick)
{
    // m_fX += m_fVelocityX * _fDeltaTick;
    m_fY -= 300 * _fDeltaTick;


	if (m_fY <= 0)
	{
		m_bCanHit = false;

	}
	if (m_bCanHit == false)
	{
		
	}

    CEntity::Process(_fDeltaTick);
}

float 
CBullet::GetVelocityX() const
{
    return (m_fVelocityX);
}

float 
CBullet::GetVelocityY() const
{
    return (m_fVelocityY);
}

void 
CBullet::SetVelocityX(float _fX)
{
    m_fVelocityX = _fX;
}

void 
CBullet::SetVelocityY(float _fY)
{
    m_fVelocityY = _fY;
}


bool
CBullet::GetCanHit() const
{
	return (m_bCanHit);
}

void 
CBullet::SetCanHit(bool _bCanHit)
{
	m_bCanHit = _bCanHit;
}

float
CBullet::GetRadius() const
{
    return (GetWidth() / 2.0f);
}



