// PosCubeVisualiser.h

#ifndef __PosCubeVisualiser_h__
#define __PosCubeVisualiser_h__

#include "PointSet.h"
#include "LineSet.h"
class PosCubes;

class PosCubesVisualiser
{
public:
	PosCubesVisualiser ( PosCubes &poscubes);
	LineSet m_cubeedges;
	PointSet m_cubecenters;
	PosCubes & m_poscubes;
};

#endif
