#pragma once
class ThreadBase
{
public:
	ThreadBase();
	virtual ~ThreadBase();

	bool isActive();

protected:

	void setActive(bool state);

	bool m_isActive = false;

};

