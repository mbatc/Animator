#pragma once

class CD3DGraphics;
class CGraphicsComponent;

class CGraphicsInitObj
{
public:
	CGraphicsInitObj(CGraphicsComponent* gfxComp);
	~CGraphicsInitObj();

	CGraphicsInitObj* GetNext()		{return m_next;		}

	void Clear();
	void AddToBuffer(CGraphicsComponent* gfxComp);
	bool doFunction(CD3DGraphics& gfx);
private:
	CGraphicsComponent* m_pGfxComp;
	CGraphicsInitObj* m_next;
};

class CGraphicsInitBuffer
{
public:
	CGraphicsInitBuffer();
	~CGraphicsInitBuffer();

	bool MessageInQueue() { return m_isMessage; }

	void AddToBuffer(CGraphicsComponent* gfxComp);
	bool ProcessNext(CD3DGraphics& gfx);
	void Clear();

private:
	CGraphicsInitObj* m_first;
	bool m_isMessage;
};