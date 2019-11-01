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
// Author		: Your Name
// Mail			: your.name@mediadesign.school.nz
//

// Library Includes

// Local Includes
#include "resource.h"
#include "utils.h"

// This Include
#include "Paddle.h"

// Static Variables

// Static Function Prototypes

// Implementation

CPaddle::CPaddle()
{

}

CPaddle::~CPaddle()
{

}

bool
CPaddle::Initialise()
{
    VALIDATE(CEntity::Initialise(IDB_PADDLESPRITE, IDB_PADDLEMASK));

    return (true);
}

void
CPaddle::Draw()
{
    CEntity::Draw();
}

void
CPaddle::Process(float _fDeltaTick)
{
    
	float fHalfPaddleW = static_cast<float>(m_pSprite->GetWidth() / 2.0);

	static float s_fShootCooldown = 0;


	if (s_fShootCooldown > 0) 
	{
		s_fShootCooldown -= _fDeltaTick;
	}
	else if (s_fShootCooldown != 0)
	{
		s_fShootCooldown = 0;
	}



	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_fX += 400.0f * _fDeltaTick;
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{ 
		m_fX -= 400.0f * _fDeltaTick;
	}
	if (m_fX - fHalfPaddleW <= 0)
	{
		m_fX = fHalfPaddleW;
	}
	else if (m_fX + fHalfPaddleW >= 385)
	{
		m_fX = 385-fHalfPaddleW;
	}
	

	// Does the shoot action
	if ((GetAsyncKeyState(VK_SPACE) & 0x2001) and s_fShootCooldown == 0)
	{
		s_fShootCooldown = 100;
	}

	//if 

	CEntity::Process(_fDeltaTick);
}
