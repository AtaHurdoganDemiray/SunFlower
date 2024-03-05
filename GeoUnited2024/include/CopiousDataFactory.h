// CopiousDataFactory.h

#ifndef __CopiousDataFactory_h__
#define __CopiousDataFactory_h__

class SequentialLineSet;
class CopiousData;
class SequentialLineSet2d;

class CopiousDataFactory
{
public:
	CopiousData * Make (const SequentialLineSet &sl);
	CopiousData * Make (const SequentialLineSet2d &sl);
};

#endif