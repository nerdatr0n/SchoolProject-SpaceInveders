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

// This Include
#include "mysteryship.h"

// Static Variables

// Static Function Prototypes

// Implementation

CMysteryShip::CMysteryShip()
	: m_bHit(false)
{

}

CMysteryShip::~CMysteryShip()
{

}

bool
CMysteryShip::Initialise()
{
	VALIDATE(CEntity::Initialise(IDB_BRICKSPRITE, IDB_BRICKMASK));

	m_fX = 0;
	m_fY = 20;

	return (true);
}

void
CMysteryShip::Draw()
{
	if (!m_bHit)
	{
		CEntity::Draw();
	}
}

void
CMysteryShip::Process(float _fDeltaTick)
{
	m_fX += 50 * _fDeltaTick;

	if (m_fX >= 500)
	{
		m_bHit = false;

		m_fX = -50;
	}

	if (!m_bHit)
	{
		CEntity::Process(_fDeltaTick);
	}
}

void
CMysteryShip::SetHit(bool _b)
{
	m_bHit = _b;
}

bool
CMysteryShip::IsHit()
{
	return (m_bHit);
}

