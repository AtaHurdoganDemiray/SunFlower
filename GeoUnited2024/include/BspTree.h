// BspTree.h

#ifndef __BspTree_h__
#define __BspTree_h__

#include "VLine2dE.h"
class LineSet2d;

namespace geo
{
	class BspTree
	{
	public:
		friend class BspTreeBooleanOp;
		friend class BspTreeConstructor;
		BspTree();
		//BspTree (const BspTree &toCopy);
		enum type {CROSSES,POSITIVE,NEGATIVE,COINCIDENT};
		virtual ~BspTree();
		//BspTree * ConstructTree(LineSet2d &L);
		//BspTree * ConstructTree(std::list<VLine2d *> &L);
		static type Classify(const VLine2dE &L , const VPoint2d &p);
		static type Classify (const VLine2d &L,const VLine2dE &E,VLine2d &subPos,VLine2d &subNeg);
		VLine2dE & GetLine();
		const VLine2dE &GetLine()const;
		BspTree * GetPosChild();
		const BspTree * GetPosChild()const;
		BspTree * GetNegChild();
		const BspTree * GetNegChild()const;
		std::vector<VLine2dE> & GetCoincident();
		const std::vector<VLine2dE> & GetCoincident()const;
		int PointLocation (const VPoint2d &p)const;
		bool IsEmptySet()const;
		BspTree *parent;
	private:
		BspTree *posChild;
		BspTree *negChild;
		std::vector <VLine2dE> m_coincident;
		
	};
};
#endif
