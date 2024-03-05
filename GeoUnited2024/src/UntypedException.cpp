// UntypedException.cpp
#include "stdafx.h"
#include "UntypedException.h"


UntypedException::UntypedException(const VStr &message, int messageNumber) : VException(message.data(), messageNumber) 
{
}

UntypedException::~UntypedException(void)
{
}
