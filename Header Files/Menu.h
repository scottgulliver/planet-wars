////////////////////////////////////////////////////////////////////////
// File: Menu.h			Class: Menu
// 
// Description:
// A simple introduction screen which is first presented to the player.
//
////////////////////////////////////////////////////////////////////////

#ifndef HG_MENU
#define HG_MENU

#include "Planet.h"
#include "Quad.h"
#include "Engine.h"
#include <d3dx9.h>

class Menu
{
private:
	// -- Instance --
	static Menu*	pInstance;

	// -- Variables --
	Planet	m_planet;
	Quad	m_BG;
	Quad	m_titleBanner;
	Quad	m_insBanner;
	bool	bFirstTimeRound;

public:
	// -- Object Management --
	Menu	();
	~Menu	();
	static Menu* Instance();

	// -- Methods --
	VOID Init	();
	VOID Update	();
	VOID Render	();
};

#endif