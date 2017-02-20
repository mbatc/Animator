#include "CGraphicsInitBuffer.h"
#include "CD3DGraphics.h"
#include "Helper.h"

CGraphicsInitBuffer::CGraphicsInitBuffer()
	:
	m_isMessage(false),
	m_first(NULL)
{}

CGraphicsInitBuffer::~CGraphicsInitBuffer()
{}

void CGraphicsInitBuffer::AddToBuffer(CGraphicsComponent* gfxComp)
{
	if (m_first)
		m_first->AddToBuffer(gfxComp);
	else
		m_first = new CGraphicsInitObj(gfxComp);

	m_isMessage = true;
}

void CGraphicsInitBuffer::Clear()
{
	if(m_first)
		m_first->Clear();

	MEMORY::SafeDelete(m_first);
}

bool CGraphicsInitBuffer::ProcessNext(CD3DGraphics& gfx)
{
	if (!m_first)
		return true;
	CGraphicsInitObj* next = m_first->GetNext();
	bool result = m_first->doFunction(gfx);
	MEMORY::SafeDelete(m_first);
	m_first = next;

	if (!m_first)
		m_isMessage = false;

	return result;
}

CGraphicsInitObj::CGraphicsInitObj(CGraphicsComponent* gfxComp)
	:
	m_pGfxComp(gfxComp),
	m_next(NULL)
{}

CGraphicsInitObj::~CGraphicsInitObj()
{}

void CGraphicsInitObj::Clear()
{
	if (m_next)
		m_next->Clear();

	MEMORY::SafeDelete(m_next);
}

void CGraphicsInitObj::AddToBuffer(CGraphicsComponent* gfxComp)
{
	if (m_next)
		m_next->AddToBuffer(gfxComp);
	else
		m_next = new CGraphicsInitObj(gfxComp);
}

bool CGraphicsInitObj::doFunction(CD3DGraphics & gfx)
{
	return m_pGfxComp->InitGFX(gfx);
}
