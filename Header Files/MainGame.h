////////////////////////////////////////////////////////////////////////
// File: MainGame.h			Class: MainGame
// 
// Description:
// The core logic of the game - this class contains the main logic for a
// level.
//
////////////////////////////////////////////////////////////////////////

#ifndef HG_MAINGAME
#define HG_MAINGAME

#include <Windows.h>
#include <mmsystem.h>
#include <strsafe.h>
#include <math.h>
#include "Menu.h"
#include "Quad.h"
#include "Inputs.h"
#include "Model.h"
#include "Helper.h"
#include "Level.h"
#include "Menu.h"
#include "ParticleSystem.h"
#include "Rect.h"
#include "Engine.h"

class MainGame
{
private:
	// -- Instance --
	static MainGame* pInstance;

	// -- Variables --
	Model				m_ship;
	Model				m_enemy;
	Model				m_rocket;
	Quad				m_background;
	bool				bHasFired;
	float				fShotPower;
	Level				m_curLevel;
	Quad				m_powerBack;
	Quad				m_attemptBack;
	Quad				m_powerBar;
	Quad				m_powerBarFULL;
	float				fPowerBarMaxWidth;
	ParticleSystem		m_particles;
	int					iCurLevelID;
	Rect				m_screenBounds;
	Quad				m_offScreen;
	bool				bLevelPassed[20];
	int					iLevelAttempts[20];
	int					iAttempts;
	Quad				m_attQ1;
	Quad				m_attQ2;
	Quad				m_attQ3;

	bool				bShowInstructions;
	Quad				m_instructions;
	bool				bEnterDown;
	bool				bEscapeDown;

	LPDIRECT3DTEXTURE9	lpRenderTexture;
	LPDIRECT3DSURFACE9	lpRenderSurface;
	LPDIRECT3DSURFACE9	lpBackBuffer;
	D3DXMATRIX			mMatProjection;
	D3DXMATRIX			mMatOldProjection;

public:
	// -- Object Management --
	MainGame	();
	~MainGame	();
	static MainGame* Instance		();

	// -- Get / Set Methods --
	bool	GetLevelPassed(int level);
	void	SetLevelPassed(int level, bool passed);
	LPDIRECT3DTEXTURE9* GetRenderTexture	();
	void				SetRenderTexture	(LPDIRECT3DTEXTURE9 renderTexture);

	// -- Methods --
	VOID Init						();
	VOID Update						();
	VOID Render						(bool drawHUD);
	VOID GoToLevel					(int levelID);
	VOID GetLevelTexture			();
	VOID RecalculateAttemptImages	();
};

#endif