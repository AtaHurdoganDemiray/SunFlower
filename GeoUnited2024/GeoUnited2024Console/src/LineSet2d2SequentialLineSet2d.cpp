// LineSet2d2SequentialLineSet2d.cpp

#include "stdafx.h"
#include "LineSet2d2SequentialLineSet2d.h"
#include "LineSet2d.h"
#include "SequentialLineSet2d.h"
#include "PosCubes2d.h"
#include "LineSetToSequentialLineSet.h"

std::vector<SequentialLineSet2d *> LineSet2d2SequentialLineSet2d::Make(const LineSet2d &lineset)
{
	LineSetToSequentialLineSet <VLine2d , LineSet2d , PosCubes2d , SequentialLineSet2d> make;
	return make.Make (lineset);
}
