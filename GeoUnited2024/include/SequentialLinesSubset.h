// SequentialLinesSubset.h

#ifndef __SequentialLinesSubset_h__
#define __SequentialLinesSubset_h__

class SequentialLineSet2d;

class SequentialLinesSubset
{
public:
	static void GetSubset (const SequentialLineSet2d &sl
						, const std::vector<vreal> &param
						, const vreal t0
						, const vreal t1
						, SequentialLineSet2d &sub);

};

#endif