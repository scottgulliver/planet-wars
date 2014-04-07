////////////////////////////////////////////////////////////////////////
// File: Quad.h			Class: Quad
// 
// Description:
// A 3D textured quad.
//
////////////////////////////////////////////////////////////////////////

#ifndef HG_QUAD
#define HG_QUAD

#include <Windows.h>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <strsafe.h>
#include<fstream>

class Quad
{
public:
	// -- Vertex Struct --
	struct CUSTOMVERTEX
	{
		float x,y,z;
		float fU, fV;

		CUSTOMVERTEX()
		{
		}

		CUSTOMVERTEX(float _x, float _y, float _z, float _fU, float _fV)
		{
			x = _x;
			y = _y;
			z = _z;
			fU = _fU;
			fV = _fV;
		}
	};


private:
	// -- Variables --
    LPDIRECT3DVERTEXBUFFER9 lpQuadVB; //vertex buffer
    LPDIRECT3DINDEXBUFFER9  lpQuadIB; //index buffer
	LPDIRECT3DTEXTURE9		lpTexture;
	D3DXVECTOR3				vPosition;
	D3DXVECTOR3				vRotation;
	D3DXVECTOR3				vScaling;

public:
	// -- Object Management --
    Quad();

	// -- Get / Set Methods --
	D3DXVECTOR3			GetPosition		();
	void				SetPosition		(D3DXVECTOR3 pos);
	D3DXVECTOR3			GetRotation		();
	void				SetRotation		(D3DXVECTOR3 rot);
	D3DXVECTOR3			GetScaling		();
	void				SetScaling		(D3DXVECTOR3 sca);
	LPDIRECT3DTEXTURE9*	GetTexture		();
	void				SetTexture		(LPDIRECT3DTEXTURE9 texture);

	// -- Methods --
    void    CreateQuad	(LPDIRECT3DDEVICE9 p_dx_Device, D3DXVECTOR3 TL, D3DXVECTOR3 TR, D3DXVECTOR3 BL, D3DXVECTOR3 BR); //load file and create buffers
	void    CreateQuad	(LPDIRECT3DDEVICE9 p_dx_Device, CUSTOMVERTEX TL, CUSTOMVERTEX TR, CUSTOMVERTEX BL, CUSTOMVERTEX BR); //load file and create buffers
    void    Render		(LPDIRECT3DDEVICE9 g_pd3dDevice, LPD3DXEFFECT m_pEffect); //draw the whole Quad
};

#endif