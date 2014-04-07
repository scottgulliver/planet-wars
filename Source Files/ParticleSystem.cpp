////////////////////////////////////////////////////////////////////////
// File: ParticleSystem.cpp			Class: ParticleSystem
// 
// Description:
// A simple particle system, where particles shrink in size over time.
//
////////////////////////////////////////////////////////////////////////

#include "ParticleSystem.h"
using namespace std;

// -- Object Management --

ParticleSystem::ParticleSystem()
{
	lSpritelist = std::list<PARTICLE>();
	fMaxScale = 0.02f;
}

ParticleSystem::~ParticleSystem()
{
}

// -- Get / Set Methods --

LPDIRECT3DTEXTURE9* ParticleSystem::GetTexture()
{
	return &lpTexture;
}

void ParticleSystem::SetTexture(LPDIRECT3DTEXTURE9 texture)
{
	lpTexture = texture;
}

std::list<ParticleSystem::PARTICLE>* ParticleSystem::GetSpriteList()
{
	return &lSpritelist;
}

void ParticleSystem::SetSpriteList(std::list<PARTICLE> spriteList)
{
	lSpritelist = spriteList;
}

// -- Methods --

void ParticleSystem::Add(D3DXVECTOR3 position)
{
	PARTICLE particle = PARTICLE();
	particle.position = position;
	particle.life = 100.0f;
	lSpritelist.push_back(particle);
}

void ParticleSystem::Render(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	myownvertexformat * spritecoordsarray = new myownvertexformat[lSpritelist.size()];
	
	int index = 0;
	std::list<PARTICLE>::iterator j;
	for(j=lSpritelist.begin(); j != lSpritelist.end(); ++j)
	{
		PARTICLE* pos = &(*j);
		spritecoordsarray[index] = myownvertexformat(pos->position.x, pos->position.y, pos->position.z, (pos->life/100.0f) * fMaxScale, /*0xffffffff*/D3DCOLOR_ARGB(0,255,255,255));
		index++;
	}

	if (lSpritelist.size() > 0)
	{
		VOID* p_Vertices;
		LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
		g_pd3dDevice->CreateVertexBuffer(lSpritelist.size()*sizeof(myownvertexformat), 0, D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &pVB, NULL);
		pVB->Lock( 0, lSpritelist.size()*sizeof(myownvertexformat), (void**)&p_Vertices, D3DLOCK_DISCARD );
		memcpy(p_Vertices, spritecoordsarray, lSpritelist.size()*sizeof(myownvertexformat));
		pVB->Unlock();

		g_pd3dDevice->SetTexture(0, lpTexture);
		g_pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE);
		g_pd3dDevice->SetStreamSource(0,pVB,0,sizeof(myownvertexformat));
		g_pd3dDevice->DrawPrimitive(D3DPT_POINTLIST,0,lSpritelist.size());
	}
}

void ParticleSystem::Update()
{
	std::list<PARTICLE>::iterator j = lSpritelist.begin();
	while (j != lSpritelist.end())
	{
		PARTICLE* pos = &(*j);
		pos->life -= 2;

		if (pos->life < 20.0f)
		{
			lSpritelist.erase(j++);
		}
		else
		{
			++j;
		}
	}
}