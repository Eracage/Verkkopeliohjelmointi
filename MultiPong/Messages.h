#pragma once

#include "util.h"

template <class T>
class Message
{
public:
	virtual char* Create() = 0;
	virtual T Receive(char *) = 0;
};

class TimeMessage : Message<TimeMessage>
{
public:
	TimeMessage();
	TimeMessage(float Time);
	~TimeMessage();

	const uint32_t type = 0;
	float time;

	// Inherited via Message
	virtual char * Create() override;
	virtual TimeMessage Receive(char *) override;
};

