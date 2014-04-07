////////////////////////////////////////////////////////////////////////
// File: ParticleSystem.h			Class: ParticleSystem
// 
// Description:
// A simple particle system, where particles shrink in size over time.
//
////////////////////////////////////////////////////////////////////////

#ifndef HG_PARTICLE
#define HG_PARTICLE

#include <d3dx9.h>
#include <list>

class ParticleSystem
{
public:
	// -- Vertex Struct --
	struct myownvertexformat
	{
		float	xval;
		float	yval;
		float	zval;
		float	PointSize;
		int		color;

		myownvertexformat()
		{
		}

		myownvertexformat(float fxval, float fyval, float fzval, float psize, int icolor)
		{
			xval = fxval;
			yval = fyval;
			zval = fzval;
			PointSize = psize;
			color = icolor;
		}
	};

	// -- Particles Struct --
	struct PARTICLE
	{
		D3DXVECTOR3 position;
		float		life;
	};

private:
	// -- Variables --
	std::list<PARTICLE> lSpritelist;
	LPDIRECT3DTEXTURE9	lpTexture;
	float				fMaxScale;

public:
	// -- Object Management --
	ParticleSystem	();
	~ParticleSystem	();

	// -- Get / Set Methods --
	LPDIRECT3DTEXTURE9*	GetTexture		();
	void				SetTexture		(LPDIRECT3DTEXTURE9 texture);
	std::list<PARTICLE>* GetSpriteList	();
	void				 SetSpriteList	(std::list<PARTICLE> spriteList);

	// -- Methods --
	void Add	(D3DXVECTOR3 position);
	void Render	(LPDIRECT3DDEVICE9 g_pd3dDevice);
	void Update	();
};

#endif