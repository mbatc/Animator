#include "CSystemMessagePipeline.h"
#include "Helper.h"

CSystemMessage::CSystemMessage(unsigned int cargo, unsigned int subMsg, CSystemMessage* previous, CSystemMessage* next)
	:
	m_cargo(cargo),
	m_MsgBefore(previous),
	m_MsgAfter(next),
	m_subMsg(subMsg)

{}

unsigned int CSystemMessage::GetCargo()
{
	return m_cargo;
}

unsigned int CSystemMessage::GetSubMessage()
{
	return m_subMsg;
}

CSystemMessage* CSystemMessage::GetPrevious()
{
	return m_MsgBefore;
}

CSystemMessage* CSystemMessage::GetNext()
{
	return m_MsgAfter;
}

void CSystemMessage::AddMessage(unsigned int msg, unsigned int subMsg)
{
	if(m_MsgAfter == nullptr)
		m_MsgAfter = new CSystemMessage(msg, subMsg,this,nullptr);
	else
		m_MsgAfter->AddMessage(msg, subMsg);
}

CSystemMessagePipeline::CSystemMessagePipeline()
	:
	m_firstMsg(nullptr)
{}

CSystemMessagePipeline::~CSystemMessagePipeline()
{}

void CSystemMessagePipeline::GetSystemMessage(unsigned int* msg, unsigned int* subMsg)
{
	if (m_firstMsg)
	{
		*msg = m_firstMsg->GetCargo();
		*subMsg = m_firstMsg->GetSubMessage();
	}
	else
	{
		*msg = 0;
		*subMsg = 0;
	}
}

bool CSystemMessagePipeline::AddMessage(unsigned int msg, unsigned int subMsg, bool notify)
{
	if(m_firstMsg)
		m_firstMsg->AddMessage(msg, subMsg);
	else
		m_firstMsg = new CSystemMessage(msg, subMsg, nullptr, nullptr);
	if(!m_firstMsg)
		return false;

	if(notify) cout << "CSystemMessagePipeline: message '" << msg << "' sub message '" << subMsg << "' added\n";

	return true;
}

bool CSystemMessagePipeline::RemoveMessage()
{
	CSystemMessage* temp = nullptr;
	if(m_firstMsg)
		temp = m_firstMsg->GetNext();

	MEMORY::SafeDelete(m_firstMsg);

	m_firstMsg = temp;

	return true;
}