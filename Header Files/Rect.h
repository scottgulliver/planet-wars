////////////////////////////////////////////////////////////////////////
// File: Rect.h			Class: Rect
// 
// Description:
// A simple representation of a rectangle.
//
////////////////////////////////////////////////////////////////////////

#ifndef HG_RECTANGLE
#define HG_RECTANGLE

#include <d3dx9.h>

class Rect
{
private:
	// -- Variables --
	D3DXVECTOR2 vTopLeft;
	D3DXVECTOR2 vBottomRight;

public:
	// -- Object Management --
	Rect	();
	Rect	(D3DXVECTOR2 _tl, D3DXVECTOR2 _br);
	~Rect	();

	// -- Get / Set Methods
	D3DXVECTOR2 GetTopLeft		();
	void		SetTopLeft		(D3DXVECTOR2 topLeft);
	D3DXVECTOR2 GetBottomRight	();
	void		SetBottomRight	(D3DXVECTOR2 bottomRight);

	// -- Methods --
	bool Contains	(D3DXVECTOR2 point);
};

#endif