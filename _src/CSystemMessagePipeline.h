#pragma once

class CSystem;

class CSystemMessage
{
public:
	CSystemMessage(unsigned int cargo, unsigned int subMsg, CSystemMessage* previous = nullptr, CSystemMessage* next = nullptr);
	~CSystemMessage(){}

	unsigned int GetCargo();
	unsigned int GetSubMessage();
	CSystemMessage* GetPrevious();
	CSystemMessage* GetNext();

	void AddMessage(unsigned int msg, unsigned int subMsg);

private:
	CSystemMessage* m_MsgBefore;
	CSystemMessage* m_MsgAfter;

	unsigned int m_cargo; 
	unsigned int m_subMsg;
};

class CSystemMessagePipeline
{
	friend CSystem;
public:
	CSystemMessagePipeline();
	~CSystemMessagePipeline();

	bool AddMessage(unsigned int msg, unsigned int subMsg, bool notify= false);
	bool RemoveMessage();
	void GetSystemMessage(unsigned int* msg, unsigned int* subMsg);

private:
	unsigned int m_nListLen;

	CSystemMessage* m_firstMsg;
};