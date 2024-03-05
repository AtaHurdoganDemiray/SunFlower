// MultiLines.h

#ifndef __MultiLines_h__
#define __MultiLines_h__
#include "MultiGV.h"

class MultiLines : public MultiGV<VLine,Lines>
{
public:
	MultiLines (const std::vector <const Lines*> &linesVec);
	MultiLines (int totalSize , const std::vector <const Lines *> &linesVec);
	MultiLines (const MultiLines &toCopy);
	MultiLines & operator = (const MultiLines &toCopy);
};

#endif

