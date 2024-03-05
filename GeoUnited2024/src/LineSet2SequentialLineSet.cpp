// LineSet2SequentialLineSet.cpp

#include "stdafx.h"
#include "LineSet2SequentialLineSet.h"
#include "LineSet.h"
#include "SequentialLineSet.h"
#include "PosCubes.h"
#include "LineSetToSequentialLineSet.h"

std::vector<SequentialLineSet *> LineSet2SequentialLineSet::Make(const LineSet &lineset)
{
	LineSetToSequentialLineSet <VLine , LineSet , PosCubes , SequentialLineSet>make; 
	
	return make.Make (lineset);
}
