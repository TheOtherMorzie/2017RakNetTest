#include "ThreadBase.h"



ThreadBase::ThreadBase()
{
	m_isActive = true;
}


ThreadBase::~ThreadBase()
{
	m_isActive = false;
}

bool ThreadBase::isActive()
{
	return m_isActive;
}
