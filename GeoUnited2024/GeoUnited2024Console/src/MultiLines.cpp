// MultiLines.cpp

#include "stdafx.h"
#include "MultiLines.h"

MultiLines::MultiLines (const std::vector<const Lines*> &linesVec)
:MultiGV<VLine,Lines> (linesVec)
{

}
MultiLines::MultiLines (int totalSize , const std::vector<const Lines*> &linesVec)
:MultiGV<VLine,Lines> (totalSize,linesVec)
{

}
MultiLines::MultiLines (const MultiLines &toCopy)
:MultiGV<VLine,Lines> (toCopy)
{

}

MultiLines & MultiLines::operator = (const MultiLines &toCopy)
{
	if (this != &toCopy)
	{	
		MultiGV <VLine,Lines>::operator = (toCopy);
	}
	return *this;
}

