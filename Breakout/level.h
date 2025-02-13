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

#if !defined(__LEVEL_H__)
#define __LEVEL_H__

// Library Includes
#include <vector>
#include <string>

// Local Includes

// Types

// Constants

// Prototypes
class CBullet;
class CPlayer;
class CAlien;
class CFPSCounter;
class CBackGround;
class CAlienBullet;
class CMysteryShip;

class CLevel
{
    // Member Functions
public:
    CLevel();
    virtual ~CLevel();

    virtual bool Initialise(int _iWidth, int _iHeight);

    virtual void Draw();
    virtual void Process(float _fDeltaTick);

    CPlayer* GetPlayer() const;

    int GetAliensRemaining() const;

protected:
    void ProcessBulletPlayerCollision();
    void ProcessBulletAlienCollision();
	void ProcessMoveAliens(float _fDeltaTick);
	void ProcessAlienFire(float _fDeltaTick);
	void ProcessBulletMysteryShipCollision();


	void ProcessFire(float);

    void ProcessCheckForWin();
	void ProcessCheckForLose();

    void ProcessBulletBounds();

    void UpdateScoreText();
    void DrawScore();
	void DrawFPS();
	void DrawHealth();

    void SetAliensRemaining(int _i);

private:
    CLevel(const CLevel& _kr);
    CLevel& operator= (const CLevel& _kr);

    // Member Variables
public:

protected:
	CBackGround* m_pBackground;
    CBullet* m_pBullet;
	CAlienBullet* m_pAlienBullet;
    CPlayer* m_pPlayer;
	CMysteryShip* m_pMysteryShip;
	
    std::vector<CAlien*> m_vecAliens;
	CFPSCounter* m_fpsCounter;

    int m_iWidth;
    int m_iHeight;

	int m_iLives;
	int m_iScore;
	


	std::string m_strHealth;

    int m_iAliensRemaining;
	 
    std::string m_strScore;

private:

};

#endif    // __LEVEL_H__
