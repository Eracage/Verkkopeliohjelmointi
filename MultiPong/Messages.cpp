#include "Messages.h"



TimeMessage::TimeMessage()
	: type(0)
{
}

TimeMessage::TimeMessage(float Time)
	: time(Time)
{
}


TimeMessage::~TimeMessage()
{
}

char * TimeMessage::Create()
{
	return nullptr;
}

TimeMessage TimeMessage::Receive(char *)
{
	return TimeMessage();
}
