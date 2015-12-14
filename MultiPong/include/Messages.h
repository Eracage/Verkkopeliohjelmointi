#pragma once
#ifndef MESSAGES_H
#define MESSAGES_H

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
	TimeMessage(int Time);
	~TimeMessage();

	const uint32_t type = 0;
	int time;

	// Inherited via Message
	virtual char * Create() override;
	virtual TimeMessage Receive(char *) override;
};

#endif
