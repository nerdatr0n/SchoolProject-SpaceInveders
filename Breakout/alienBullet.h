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

#if !defined(__ALIENBULLET_H__)
#define __ALIENBULLET_H__

// Library Includes

// Local Includes
#include "entity.h"

// Types

// Constants

// Prototypes
class CSprite;

class CAlienBullet : public CEntity
{
	// Member Functions
public:
	CAlienBullet();
	virtual ~CAlienBullet();

	virtual bool Initialise(float _fPosX, float _fPosY, float _fVelocityX, float _fVelocityY);

	virtual void Draw();
	virtual void Process(float _fDeltaTick);

	float GetVelocityX() const;
	float GetVelocityY() const;
	void SetVelocityX(float _fX);
	void SetVelocityY(float _fY);

	bool GetCanHit() const;
	void SetCanHit(bool _bCanHit);

	float GetRadius() const;

protected:

private:
	CAlienBullet(const CAlienBullet& _kr);
	CAlienBullet& operator= (const CAlienBullet& _kr);

	// Member Variables
public:

protected:
	float m_fVelocityX;
	float m_fVelocityY;
	bool m_bCanHit;

private:

};


#endif    // __ALIENBULLET_H__
