// BspToConvexLoops.h

#ifndef __BspToConvexLoops_h__
#define __BspToConvexLoops_h__

#include "BspTree.h"
#include "LineSet2d.h"

struct VLine2dE;

class BspToConvexLoops
{
public:
	BspToConvexLoops (geo::BspTree *rbsp , std::list<std::vector<VLine2dE>> &convexLoops);
	void DivideTreePart (geo::BspTree *parent,
						std::list <geo::BspTree *> &purepositive,
						std::list<geo::BspTree *> &others,
						std::list<geo::BspTree *> &leafs);
	LineSet2d m_purepositive;
private:
	geo::BspTree *m_rbsp;
	std::list <std::vector<VLine2dE>> &m_convexLoops;
	void PosToLoopNegToList (geo::BspTree *r , std::vector<VLine2dE> &loop , std::list<geo::BspTree *> &startList);
	void LeafToLoop (geo::BspTree *leaf);
	void CollectLeafs (geo::BspTree *b,std::list<geo::BspTree *> &allLeaf);

};

#endif