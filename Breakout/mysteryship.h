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

#pragma once

#if !defined(__MYSTERYSHIP_H__)
#define __MYSTERYSHIP_H__

// Library Includes

// Local Includes
#include "entity.h"

// Types

// Constants

// Prototypes

class CMysteryShip : public CEntity
{
	// Member Functions
public:
	CMysteryShip();
	virtual ~CMysteryShip();

	virtual bool Initialise();

	virtual void Draw();
	virtual void Process(float _fDeltaTick);

	void SetHit(bool _b);
	bool IsHit();

protected:

private:
	CMysteryShip(const CMysteryShip& _kr);
	CMysteryShip& operator= (const CMysteryShip& _kr);

	// Member Variables
public:

protected:
	bool m_bHit;

private:

};


#endif    // __MYSTERYSHIP_H__

