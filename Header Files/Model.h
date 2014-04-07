////////////////////////////////////////////////////////////////////////
// File: Model.h			Class: Model
// 
// Description:
// Initiation, movement, and rendering of a model.
//
////////////////////////////////////////////////////////////////////////

#ifndef HG_MODEL
#define HG_MODEL

#include <d3dx9.h>
#include "Camera.h"

class Model
{
private:
	// -- Variables --
	LPD3DXMESH			lpMesh;
    DWORD				dNumMaterials;
    D3DMATERIAL9*		pMeshMaterials;
    LPDIRECT3DTEXTURE9*	lppMeshTextures;
	D3DXVECTOR3			vPosition;
	D3DXVECTOR3			vVelocity;
	D3DXVECTOR3			vRotation;
	D3DXVECTOR3			vScaling;
	float				fMass;
	float				fRadius;

public:
	// -- Object Management --
	Model();
	~Model();

	// -- Get / Set Methods --
	D3DXVECTOR3	GetPosition		();
	void		SetPosition		(D3DXVECTOR3 pos);
	D3DXVECTOR3	GetRotation		();
	void		SetRotation		(D3DXVECTOR3 rot);
	D3DXVECTOR3	GetScaling		();
	void		SetScaling		(D3DXVECTOR3 sca);
	D3DXVECTOR3	GetVelocity		();
	void		SetVelocity		(D3DXVECTOR3 vel);
	float		GetRadius		();
	void		SetRadius		(float radius);
	float		GetMass			();
	void		SetMass			(float mass);

	// -- Methpds --
	HRESULT		Initialise		(LPDIRECT3DDEVICE9 g_pd3dDevice, LPCTSTR pFilename);
	void		Render			(LPDIRECT3DDEVICE9 g_pd3dDevice);
	void		RenderWithEffect(LPDIRECT3DDEVICE9 g_pd3dDevice, LPD3DXEFFECT m_pEffect);
};


#endif // HG_MODEL