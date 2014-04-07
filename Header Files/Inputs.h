////////////////////////////////////////////////////////////////////////
// File: Inputs.h			Class: Inputs
// 
// Description:
// This holds the  current states of the inputs from the keyboard.
//
////////////////////////////////////////////////////////////////////////

#ifndef HG_INPUTS
#define HG_INPUTS

#include <d3dx9.h>

class Inputs
{
public:
	// -- Variables --
	bool	leftArrowDown;
	bool	rightArrowDown;
	bool	upArrowDown;
	bool	downArrowDown;
	bool	returnDown;
	bool	escapeDown;
	
	bool	aKeyDown;
	bool	bKeyDown;
	bool	cKeyDown;
	bool	dKeyDown;
	bool	eKeyDown;
	bool	fKeyDown;
	bool	gKeyDown;
	bool	hKeyDown;
	bool	iKeyDown;
	bool	jKeyDown;
	bool	kKeyDown;
	bool	lKeyDown;
	bool	mKeyDown;
	bool	nKeyDown;
	bool	oKeyDown;
	bool	pKeyDown;
	bool	qKeyDown;
	bool	rKeyDown;
	bool	sKeyDown;
	bool	tKeyDown;
	bool	uKeyDown;
	bool	vKeyDown;
	bool	wKeyDown;
	bool	xKeyDown;
	bool	yKeyDown;
	bool	zKeyDown;

	bool	n1KeyDown;

public:
	// -- Methods --
	bool	HandleInput	(UINT msg, WPARAM wParam);
};

#endif