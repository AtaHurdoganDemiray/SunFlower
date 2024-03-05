// LineSet2d2SequentialLineSet2d.h

#ifndef __LineSet2d2SequentialLineSet2d_h__
#define __LineSet2d2SequentialLineSet2d_h__

class LineSet2d;
class SequentialLineSet2d;

class LineSet2d2SequentialLineSet2d
{
public:
	std::vector<SequentialLineSet2d *> Make (const LineSet2d &lineset);
};

#endif