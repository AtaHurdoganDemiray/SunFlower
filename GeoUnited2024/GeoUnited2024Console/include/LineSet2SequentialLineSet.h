// LineSet2SequentialLineSet.h

#ifndef __LineSet2SequentialLineSet_h__
#define __LineSet2SequentialLineSet_h__

class SequentialLineSet;
class LineSet;

class LineSet2SequentialLineSet
{
public:
	std::vector<SequentialLineSet *> Make (const LineSet &lineset);
};

#endif