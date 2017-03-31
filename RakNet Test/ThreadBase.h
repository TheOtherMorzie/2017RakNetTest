#pragma once
class ThreadBase
{
public:
	ThreadBase();
	virtual ~ThreadBase();

	virtual bool isActive();

protected:

	bool m_isActive;

};

