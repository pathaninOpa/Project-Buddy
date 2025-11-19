#pragma once
#include "zegotypes.h"

class IZEGOStreamReader
{
public:
	IZEGOStreamReader(){};
	virtual ~IZEGOStreamReader() {};
	virtual boolean Seek(int64 iOffset, int where) = 0;
	virtual void    Close(void) = 0;
	virtual int64   GetSize(void) = 0;
	virtual int64   Read(uint8* pOut, int64 iOutMax) = 0;
    virtual IZEGOStreamReader* Clone(){return NULL;}
};
