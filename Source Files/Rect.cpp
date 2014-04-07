////////////////////////////////////////////////////////////////////////
// File: Rect.cpp			Class: Rect
// 
// Description:
// A simple representation of a rectangle.
//
////////////////////////////////////////////////////////////////////////

#include "Rect.h"

// -- Object Management --

Rect::Rect()
{
	vTopLeft = D3DXVECTOR2(0,0);
	vBottomRight = D3DXVECTOR2(0,0);
}

Rect::Rect(D3DXVECTOR2 _tl, D3DXVECTOR2 _br)
{
	vTopLeft = _tl;
	vBottomRight = _br;
}

Rect::~Rect()
{
}

// -- Get / Set Methods

D3DXVECTOR2 Rect::GetTopLeft()
{
	return vTopLeft;
}

void Rect::SetTopLeft(D3DXVECTOR2 topLeft)
{
	vTopLeft = topLeft;
}

D3DXVECTOR2 Rect::GetBottomRight()
{
	return vBottomRight;
}

void Rect::SetBottomRight(D3DXVECTOR2 bottomRight)
{
	vBottomRight = bottomRight;
}

// -- Methods --

bool Rect::Contains(D3DXVECTOR2 point)
{
	if ((point.x > vTopLeft.x && point.x < vBottomRight.x) &&
		(point.y > vTopLeft.y && point.y < vBottomRight.y))
	{
		return true;
	}
	return false;
}