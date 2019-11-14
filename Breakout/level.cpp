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
#include <time.h>

// Local Includes
#include "Game.h"
#include "Player.h"
#include "Alien.h"
#include "bullet.h"
#include "utils.h"
#include "backbuffer.h"
#include "framecounter.h"
#include "background.h"
#include "alienBullet.h"
#include "mysteryship.h"

// This Include
#include "Level.h"

// Static Variables

// Static Function Prototypes

// Implementation

#define CHEAT_BOUNCE_ON_BACK_WALL

CLevel::CLevel()
: m_iAliensRemaining(0)
, m_pPlayer(0)
, m_pBullet(0)
, m_iWidth(0)
, m_iHeight(0)
, m_fpsCounter(0)
{
//bullet
}

CLevel::~CLevel()
{
    while (m_vecAliens.size() > 0)
    {
        CAlien* pAlien = m_vecAliens[m_vecAliens.size() - 1];

        m_vecAliens.pop_back();

        delete pAlien;
    }

    delete m_pPlayer;
    m_pPlayer = 0;

	delete m_pMysteryShip;
	m_pMysteryShip = 0;

	delete m_pAlienBullet;
	m_pAlienBullet = 0;

    delete m_pBullet;
	m_pBullet = 0;

	delete m_fpsCounter;
	m_fpsCounter = 0;

	delete m_pBackground;
	m_pBackground = 0;

}

bool
CLevel::Initialise(int _iWidth, int _iHeight)
{
    m_iWidth = _iWidth;
    m_iHeight = _iHeight;

    const float fBulletVelX = 200.0f;
    const float fBulletVelY = 75.0f;

	m_pBackground = new CBackGround();
	VALIDATE(m_pBackground->Initialise());
	//Set the background position to start from {0,0}
	m_pBackground->SetX((float)m_iWidth / 2);
	m_pBackground->SetY((float)m_iHeight / 2);

	m_pBullet = new CBullet();
    VALIDATE(m_pBullet->Initialise(m_iWidth / 2.0f, -10, fBulletVelX, fBulletVelY));



	m_pAlienBullet = new CAlienBullet();
	VALIDATE(m_pAlienBullet->Initialise(m_iWidth / 2.0f, -10, fBulletVelX, fBulletVelY));

	m_pMysteryShip = new CMysteryShip();
	VALIDATE(m_pMysteryShip->Initialise());


    m_pPlayer = new CPlayer();
    VALIDATE(m_pPlayer->Initialise());

    // Set the player's position to be centered on the x, 
    // and a little bit up from the bottom of the window.
    m_pPlayer->SetX(_iWidth / 2.0f);
    m_pPlayer->SetY(_iHeight - ( 1.5f * m_pPlayer->GetHeight()));

    const int kiNumAliens = 30;
    const int kiStartX = 20;
    const int kiGap = 5;

	m_iLives = 3;
	m_iScore = 0;

    int iCurrentX = kiStartX;
    int iCurrentY = 40;

    for (int i = 0; i < kiNumAliens; ++i)
    {
        CAlien* pAlien = new CAlien();
        VALIDATE(pAlien->Initialise());

        pAlien->SetX(static_cast<float>(iCurrentX));
        pAlien->SetY(static_cast<float>(iCurrentY));

        iCurrentX += static_cast<int>(pAlien->GetWidth()) + kiGap;

        if (iCurrentX > _iWidth - 100)
        {
            iCurrentX = kiStartX;
            iCurrentY += 20;
        }

        m_vecAliens.push_back(pAlien);
    }

    SetAliensRemaining(kiNumAliens);
	m_fpsCounter = new CFPSCounter();
	VALIDATE(m_fpsCounter->Initialise());

    return (true);
}

void
CLevel::Draw()
{
	m_pBackground->Draw();

	for (unsigned int i = 0; i < m_vecAliens.size(); ++i)
    {
        m_vecAliens[i]->Draw();
    }

    m_pPlayer->Draw();
    m_pBullet->Draw();
	m_pAlienBullet->Draw();
	m_pMysteryShip->Draw();

    DrawScore();
	DrawFPS();
	DrawHealth();
}

void
CLevel::Process(float _fDeltaTick)
{
	m_pBackground->Process(_fDeltaTick);
	m_pBullet->Process(_fDeltaTick);
	m_pAlienBullet->Process(_fDeltaTick);
	m_pPlayer->Process(_fDeltaTick);
	m_pMysteryShip->Process(_fDeltaTick);

	ProcessAlienFire(_fDeltaTick);
	ProcessFire(_fDeltaTick);
    ProcessBulletPlayerCollision();
	ProcessBulletMysteryShipCollision();


	if (m_pBullet->GetCanHit() == true)
	{
		ProcessBulletAlienCollision();
	}
	
	


    ProcessCheckForWin();
	ProcessCheckForLose();
	//ProcessBulletBounds();

    for (unsigned int i = 0; i < m_vecAliens.size(); ++i)
    {
        m_vecAliens[i]->Process(_fDeltaTick);
    }
	
	ProcessMoveAliens(_fDeltaTick);
    
	m_fpsCounter->CountFramesPerSecond(_fDeltaTick);
}

CPlayer* 
CLevel::GetPlayer() const
{
    return (m_pPlayer);
}

void
CLevel::ProcessFire(float _fDeltaTick)
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000 and !m_pBullet->GetCanHit())
	{
		m_pBullet->SetX(m_pPlayer->GetX());
		m_pBullet->SetY(m_pPlayer->GetY());
		m_pBullet->SetCanHit(true);
	}
}



void
CLevel::ProcessAlienFire(float _fDeltaTick)
{
	
	if (!m_pAlienBullet->GetCanHit())
	{
		//Brick Movement Code
		bool bMoveDown = true;
		static float fMoveXVel = 0.1f;
		//static float fAlienHeight = 20;
		// srand(time(NULL));

		int iAlienNumber = rand() % m_vecAliens.size();

		while (m_vecAliens[iAlienNumber]->IsHit())
		{
			iAlienNumber++;
			if (iAlienNumber >= m_vecAliens.size())
			{
				iAlienNumber = 0;
			}
		}
		CAlien* pAlien = m_vecAliens[iAlienNumber];

		float fAlienHighestY = pAlien->GetY();
		float fAlienX = pAlien->GetX();

		for (unsigned int i = 0; i < m_vecAliens.size(); ++i)
		{
			CAlien* pTempAlien = m_vecAliens[i];

			if (pTempAlien->GetX() == fAlienX && pTempAlien->GetY() > fAlienHighestY && !pTempAlien->IsHit())
			{
				if (pTempAlien->GetY() > fAlienHighestY)
				{
					pAlien = m_vecAliens[i];
					fAlienHighestY = pAlien->GetY();
				}

			}
		}


		m_pAlienBullet->SetX(pAlien->GetX() + 10);
		m_pAlienBullet->SetY(pAlien->GetY());
		m_pAlienBullet->SetCanHit(true);
	}

	
	
}


void
CLevel::ProcessBulletPlayerCollision()
{
    float fBulletR = m_pAlienBullet->GetRadius();

    float fBulletX = m_pAlienBullet->GetX();
    float fBulletY = m_pAlienBullet->GetY();

    float fPlayerX = m_pPlayer->GetX();
    float fPlayerY = m_pPlayer->GetY();

    float fPlayerH = m_pPlayer->GetHeight();
    float fPlayerW = m_pPlayer->GetWidth();

    if ((fBulletX + fBulletR > fPlayerX - fPlayerW / 2) && //bullet.right > player.left
        (fBulletX - fBulletR < fPlayerX + fPlayerW / 2) && //bullet.left < player.right
        (fBulletY + fBulletR > fPlayerY - fPlayerH / 2) && //bullet.bottom > player.top
        (fBulletY - fBulletR < fPlayerY + fPlayerH / 2))  //bullet.top < player.bottom
    {
		m_iLives -= 1;
		m_iScore -= 15;
		m_pAlienBullet->SetY(m_iHeight + 10);  
    }
}

void
CLevel::ProcessBulletMysteryShipCollision()
{
	if (!m_pBullet->GetCanHit() or !m_pMysteryShip->IsHit())
	{
		return;
	}
	float fBulletR = m_pBullet->GetRadius();

	float fBulletX = m_pBullet->GetX();
	float fBulletY = m_pBullet->GetY();

	float fMysteryShipX = m_pMysteryShip->GetX();
	float fMysteryShipY = m_pMysteryShip->GetY();

	float fMysteryShipH = m_pMysteryShip->GetHeight();
	float fMysteryShipW = m_pMysteryShip->GetWidth();

	if ((fBulletX + fBulletR > fMysteryShipX - fMysteryShipW / 2) && 
		(fBulletX - fBulletR < fMysteryShipX + fMysteryShipW / 2) && 
		(fBulletY + fBulletR > fMysteryShipY - fMysteryShipH / 2) && 
		(fBulletY - fBulletR < fMysteryShipY + fMysteryShipH / 2))
	{
		m_iScore += 50;
		m_pMysteryShip->SetHit(true);
	}

	CLevel::UpdateScoreText();
}

void
CLevel::ProcessBulletAlienCollision()
{
    for (unsigned int i = 0; i < m_vecAliens.size(); ++i)
    {
        if (!m_vecAliens[i]->IsHit())
        {
            float fBulletR = m_pBullet->GetRadius();

            float fBulletX = m_pBullet->GetX();
            float fBulletY = m_pBullet->GetY(); 

            float fAlienX = m_vecAliens[i]->GetX();
            float fAlienY = m_vecAliens[i]->GetY();

            float fAlienH = m_vecAliens[i]->GetHeight();
            float fAlienW = m_vecAliens[i]->GetWidth();

            if ((fBulletX + fBulletR > fAlienX - fAlienW / 2) &&
                (fBulletX - fBulletR < fAlienX + fAlienW / 2) &&
                (fBulletY + fBulletR > fAlienY - fAlienH / 2) &&
                (fBulletY - fBulletR < fAlienY + fAlienH / 2))
            {
                // Hit the front side of the alien...
                m_pBullet->SetY((fAlienY + fAlienH / 2.0f) + fBulletR);
                m_pBullet->SetVelocityY(m_pBullet->GetVelocityY() * -1);
                m_vecAliens[i]->SetHit(true);

				m_pBullet->SetCanHit(false);

				m_iScore += 10;

                SetAliensRemaining(GetAliensRemaining() - 1);
            }
        }
    }
}

void 
CLevel::ProcessMoveAliens(float _fDeltaTick)
{
	//Brick Movement Code
	bool bMoveDown = true;
	static float fMoveXVel = 0.1f;
	//static float fAlienHeight = 20;

	for (unsigned int i = 0; i < m_vecAliens.size(); ++i)
	{
		m_vecAliens[i]->SetX((m_vecAliens[i]->GetX() + fMoveXVel));
		//Move right
		if ((m_vecAliens[i]->GetX() + m_vecAliens[i]->GetWidth() <= 60) && !(m_vecAliens[i]->IsHit()))
		{
			fMoveXVel *= -1.0f;
			//Move down if at end of width of screen
			for (int j = 0; j < m_vecAliens.size(); j++)
			{
				m_vecAliens[j]->SetY(m_vecAliens[j]->GetY() + 20);
			}
		}
		//Move left
		if ((m_vecAliens[i]->GetX() + m_vecAliens[i]->GetWidth() >= m_iWidth) && !(m_vecAliens[i]->IsHit()))
		{
			fMoveXVel *= -1.0f;
			//Move down if at end of width of screen
			for (int j = 0; j < m_vecAliens.size(); j++)
			{
				m_vecAliens[j]->SetY(m_vecAliens[j]->GetY() + 20);
			}
		}
	}
}

void
CLevel::ProcessCheckForWin()
{
    for (unsigned int i = 0; i < m_vecAliens.size(); ++i)
    {
        if (!m_vecAliens[i]->IsHit())
        {
            return;
        }
    }

    CGame::GetInstance().GameOverWon();
}

void
CLevel::ProcessCheckForLose()
{
	if (m_iLives > 0)
	{
		return;
	}



	CGame::GetInstance().GameOverLost();
}



void
CLevel::ProcessBulletBounds()
{
	if (m_pBullet->GetX() < 0)
    {
        m_pBullet->SetX(0);
    }
	else if (m_pBullet->GetX() > m_iWidth)
    {
        m_pBullet->SetX(static_cast<float>(m_iWidth));
    }

    if (m_pBullet->GetY() < 0)
    {
        m_pBullet->SetY(0.0f);
    }
    else if (m_pBullet->GetY() > m_iHeight)
    {
        CGame::GetInstance().GameOverLost();
        //m_pBullet->SetY(static_cast<float>(m_iHeight));
    }
}

int 
CLevel::GetAliensRemaining() const
{
    return (m_iAliensRemaining);
}

void 
CLevel::SetAliensRemaining(int _i)
{
    m_iAliensRemaining = _i;
    UpdateScoreText();
}

void
CLevel::DrawScore()
{
    HDC hdc = CGame::GetInstance().GetBackBuffer()->GetBFDC();

    const int kiX = 0;
    const int kiY = m_iHeight - 14;
	SetBkMode(hdc, TRANSPARENT);
    
    TextOutA(hdc, kiX, kiY, m_strScore.c_str(), static_cast<int>(m_strScore.size()));
}



void 
CLevel::UpdateScoreText()
{
    m_strScore = "Score: ";

    m_strScore += ToString(m_iScore);
}


void 
CLevel::DrawFPS()
{
	HDC hdc = CGame::GetInstance().GetBackBuffer()->GetBFDC(); 

	m_fpsCounter->DrawFPSText(hdc, m_iWidth, m_iHeight);

}


void
CLevel::DrawHealth()
{
	HDC hdc = CGame::GetInstance().GetBackBuffer()->GetBFDC();


	m_strHealth = "Health: ";
	m_strHealth += ToString(m_iLives);



	TextOutA(hdc, 0, 0, m_strHealth.c_str(), static_cast<int>(m_strHealth.size()));

}

