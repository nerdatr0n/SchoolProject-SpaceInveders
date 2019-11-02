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
#include "Game.h"
#include "Player.h"
#include "Alien.h"
#include "bullet.h"
#include "utils.h"
#include "backbuffer.h"
#include "framecounter.h"
#include "background.h"

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

    m_pPlayer = new CPlayer();
    VALIDATE(m_pPlayer->Initialise());

    // Set the player's position to be centered on the x, 
    // and a little bit up from the bottom of the window.
    m_pPlayer->SetX(_iWidth / 2.0f);
    m_pPlayer->SetY(_iHeight - ( 1.5f * m_pPlayer->GetHeight()));

    const int kiNumAliens = 36;
    const int kiStartX = 20;
    const int kiGap = 5;

    int iCurrentX = kiStartX;
    int iCurrentY = kiStartX;

    for (int i = 0; i < kiNumAliens; ++i)
    {
        CAlien* pAlien = new CAlien();
        VALIDATE(pAlien->Initialise());

        pAlien->SetX(static_cast<float>(iCurrentX));
        pAlien->SetY(static_cast<float>(iCurrentY));

        iCurrentX += static_cast<int>(pAlien->GetWidth()) + kiGap;

        if (iCurrentX > _iWidth)
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

    DrawScore();
	DrawFPS();
}

void
CLevel::Process(float _fDeltaTick)
{
	m_pBackground->Process(_fDeltaTick);
	m_pBullet->Process(_fDeltaTick);
	m_pPlayer->Process(_fDeltaTick);

	ProcessFire(_fDeltaTick);
	//ProcessBulletWallCollision(_fDeltaTick);
	//ProcessPlayerWallCollison();
    //ProcessBulletPlayerCollision();
    
	if (m_pBullet->GetCanHit() == true)
	{
		ProcessBulletAlienCollision();
	}
	

    ProcessCheckForWin();
	//ProcessBulletBounds();

    for (unsigned int i = 0; i < m_vecAliens.size(); ++i)
    {
        m_vecAliens[i]->Process(_fDeltaTick);
    }
	
   
    
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
CLevel::ProcessBulletWallCollision(float _fDeltaTick)
{
    float fBulletX = m_pBullet->GetX();
    float fBulletY = m_pBullet->GetY();
    float fBulletW = m_pBullet->GetWidth();
    float fBulletH = m_pBullet->GetHeight();

    float fHalfBulletW = fBulletW / 2;
	float fHalfBulletH = fBulletH / 2;

    if (fBulletX <= fHalfBulletW) //represents the situation when the bullet has hit the left wall
    {
        m_pBullet->SetVelocityX(m_pBullet->GetVelocityX() * -1); //reverse the bullet's x velocity
    }
    else if (fBulletX >= m_iWidth - fHalfBulletW) //represents the situation when the bullet has hit the right wall
    {
        m_pBullet->SetVelocityX(m_pBullet->GetVelocityX() * -1); //reverse the bullet's x velocity direction
		//m_pBullet->SetX(m_pBullet->GetX() + m_pBullet->GetVelocityX() * _fDeltaTick);
	}

	if (fBulletY < fHalfBulletH) //represents the situation when the bullet has hit the top wall
    {
        m_pBullet->SetVelocityY(m_pBullet->GetVelocityY() * -1); //reverse the bullet's y velocity
    }

#ifdef CHEAT_BOUNCE_ON_BACK_WALL
	if (fBulletY  > m_iHeight - fHalfBulletH)  //represents the situation when the bullet has hit the bottom wall
    {
        m_pBullet->SetVelocityY(m_pBullet->GetVelocityY() * -1); //reverse the bullet's y velocity
    }
#endif //CHEAT_BOUNCE_ON_BACK_WALL
}




void
CLevel::ProcessBulletPlayerCollision()
{
    float fBulletR = m_pBullet->GetRadius();

    float fBulletX = m_pBullet->GetX();
    float fBulletY = m_pBullet->GetY(); 

    float fPlayerX = m_pPlayer->GetX();
    float fPlayerY = m_pPlayer->GetY();

    float fPlayerH = m_pPlayer->GetHeight();
    float fPlayerW = m_pPlayer->GetWidth();

    if ((fBulletX + fBulletR > fPlayerX - fPlayerW / 2) && //bullet.right > player.left
        (fBulletX - fBulletR < fPlayerX + fPlayerW / 2) && //bullet.left < player.right
        (fBulletY + fBulletR > fPlayerY - fPlayerH / 2) && //bullet.bottom > player.top
        (fBulletY - fBulletR < fPlayerY + fPlayerH / 2))  //bullet.top < player.bottom
    {
        m_pBullet->SetY((fPlayerY - fPlayerH / 2) - fBulletR);  //Set the bullet.bottom = player.top; to prevent the bullet from going through the player!
        m_pBullet->SetVelocityY(m_pBullet->GetVelocityY() * -1); //Reverse bullet's Y direction
    }
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

                SetAliensRemaining(GetAliensRemaining() - 1);
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
    m_strScore = "Aliens Remaining: ";

    m_strScore += ToString(GetAliensRemaining());
}


void 
CLevel::DrawFPS()
{
	HDC hdc = CGame::GetInstance().GetBackBuffer()->GetBFDC(); 

	m_fpsCounter->DrawFPSText(hdc, m_iWidth, m_iHeight);

}
