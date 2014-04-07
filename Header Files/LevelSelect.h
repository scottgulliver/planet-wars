////////////////////////////////////////////////////////////////////////
// File: LevelSelect.h			Class: LevelSelect
// 
// Description:
// Provides an interface for the player to be able to select a level to
// play.
//
////////////////////////////////////////////////////////////////////////

#ifndef HG_LEVELSELECT
#define HG_LEVELSELECT

#include "Engine.h"
#include <list>

class LevelSelect
{
private:
	// -- Instance --
	static LevelSelect* pInstance;

	// -- Variables --
	Quad			m_BG;
	Quad			m_titleBanner;
	Quad			m_insBanner;
	std::list<Quad>	lPanels;
	std::list<Quad>	lPanelPics;
	int				iSelection;
	bool			bLeftDown;
	bool			bRightDown;
	bool			bEnterDown;
	float			fTransition;
	bool			bMovingLeft;
	bool			bMovingRight;
	bool			bLastMovedLeft;

public:
	// -- Object Management --
	LevelSelect	();
	~LevelSelect();
	static LevelSelect* Instance();

	// -- Methods --
	VOID Init		();
	VOID Update		();
	VOID Render		();
	VOID MovePanels	();
};

#endif