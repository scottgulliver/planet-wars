////////////////////////////////////////////////////////////////////////
// File: Planet.h			Class: Planet
// 
// Description:
// Initiation, movement, and rendering of a planet. A sphere is used for
// the model.
//
////////////////////////////////////////////////////////////////////////

#ifndef HG_PLANET
#define HG_PLANET

#include <d3dx9.h>

class Planet
{
public:
	// -- Vertex Struct --
	struct CUSTOMVERTEX
	{
		float		x,y,z;
		D3DXVECTOR3 norm;
		float		fU, fV;
	};

private:
	// -- Variables --
	LPD3DXMESH			lpMesh;
    LPDIRECT3DTEXTURE9	lpTexture;
    LPDIRECT3DTEXTURE9	lpBumpMap;
	D3DXVECTOR3			vPosition;
	D3DXVECTOR3			vVelocity;
	D3DXVECTOR3			vRotation;
	D3DXVECTOR3			vScaling;
	D3DXVECTOR3			vRotVel;
	float				fMass;
	float				fRadius;

public:
	// -- Object Management --
	Planet();
	~Planet();

	// -- Get / Set Methods --
	D3DXVECTOR3			GetPosition		();
	void				SetPosition		(D3DXVECTOR3 pos);
	D3DXVECTOR3			GetRotation		();
	void				SetRotation		(D3DXVECTOR3 rot);
	D3DXVECTOR3			GetScaling		();
	void				SetScaling		(D3DXVECTOR3 sca);
	LPDIRECT3DTEXTURE9*	GetTexture		();
	void				SetTexture		(LPDIRECT3DTEXTURE9 texture);
	LPDIRECT3DTEXTURE9*	GetBumpMap		();
	void				SetBumpMap		(LPDIRECT3DTEXTURE9 bumpMap);
	float				GetRadius		();
	void				SetRadius		(float radius);
	float				GetMass			();
	void				SetMass			(float mass);
	D3DXVECTOR3			GetRotVel		();
	void				SetRotVel		(D3DXVECTOR3 rotVel);

	// -- Methods --
	HRESULT Initialise		(LPDIRECT3DDEVICE9 g_pd3dDevice, float radius);
	void	Render			(LPDIRECT3DDEVICE9 g_pd3dDevice);
	void	RenderWithEffect(LPDIRECT3DDEVICE9 g_pd3dDevice, LPD3DXEFFECT m_pEffect);
};

#endif