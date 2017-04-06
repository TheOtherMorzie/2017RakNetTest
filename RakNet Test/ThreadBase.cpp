#include "ThreadBase.h"



ThreadBase::ThreadBase()
{

}


ThreadBase::~ThreadBase()
{
	m_isActive = false;
}

bool ThreadBase::isActive()
{
	return m_isActive;
}

void ThreadBase::setActive(bool state)
{
	m_isActive = state;
}
