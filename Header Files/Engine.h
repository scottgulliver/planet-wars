////////////////////////////////////////////////////////////////////////
// File: Engine.h			Class: Engine
// 
// Description:
// Handling of the main DirectX rendering members, and provides methods
// for initiation and rendering to the screen.
//
////////////////////////////////////////////////////////////////////////

#ifndef HG_ENGINE
#define HG_ENGINE

#include <d3dx9.h>
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
#include "MainGame.h"
#include "LevelSelect.h"

class Engine
{
public:
	// -- Constants --
	static const int STATE_MAINMENU = 1;
	static const int STATE_LEVELSELECT = 2;
	static const int STATE_MAINGAME = 3;

private:
	// -- Instance --
	static Engine*		pInstance;

	// -- Variables --
	LPDIRECT3D9         lpD3D;			// Used to create the D3DDevice
	LPDIRECT3DDEVICE9   lpDevice;	// Our rendering device
	LPD3DXEFFECT		lpEffect;		// pointer to effect object
	Camera				m_Camera;
	Inputs				m_Inputs;
	int					m_state;
	float				fFOV;
    LPDIRECT3DTEXTURE9	lpAtmosTexture;

	
public:
	// -- Object Management --
	Engine					();
	~Engine					();
	static Engine* Instance	();

	// -- Get / Set Methods --
	LPDIRECT3DDEVICE9*	GetDevice	();
	void				SetDevice	(LPDIRECT3DDEVICE9 device);
	LPD3DXEFFECT		GetEffect	();
	void				SetEffect	(LPD3DXEFFECT effect);
	Inputs*				GetInput	();
	void				SetInput	(Inputs input);
	Camera				GetCamera	();
	void				SetCamera	(Camera camera);
	int					GetState	();
	void				SetState	(int state);

	// -- Methods --
	HRESULT InitD3D			(HWND hWnd);
	VOID	Cleanup			();
	VOID	SetupMatrices	(Model * model);
	VOID	SetupMatrices	(Quad * Quad);
	VOID	SetupMatrices	(Planet * planet);
	VOID	SetupMatrices	(D3DXVECTOR3* g_Position, D3DXVECTOR3* g_Rotation, D3DXVECTOR3* g_Scaling);
	VOID	Update			();
	VOID	Render			();
};

#endif