// AABB.h

#ifndef __AABB_h__
#define __AABB_h__
#include "Bifstream.h"
#include "Bofstream.h"
#include "sVector.h"

struct VPoint;
struct VPoint2d;
struct VLine;
struct VLine2d;
struct VTriangleE;
struct VTriangle2dE;
class TransformationMatrix;

template <int N> // N : dimension of the box eg:2 or 3d   
	struct AABB
	{
	public:
		friend void UpdateAABB (const VPoint &newp , AABB<3> &aabb);
		friend void UpdateAABB (const VPoint2d &newp , AABB<2> &aabb);
		friend void UpdateAABB (const VLine &newl , AABB<3> &aabb);
		friend void UpdateAABB (const VLine2d &newl , AABB<2> &aabb);
		friend void UpdateAABB (const VTriangleE &newt , AABB<3> &aabb);
		friend void UpdateAABB (const VTriangle2dE &newt , AABB<2> &aabb);
		
		AABB();
		AABB(const AABB<N> &toCopy);
		AABB & operator = (const AABB<N> &toCopy);
		bool operator == (const AABB<N> &toCompare)const;
		bool operator != (const AABB<N> &toCompare)const;		
		const sVector<double,N> & GetMin()const;
		const sVector<double,N> & GetMax()const;
		sVector<double,N> & GetMin();
		sVector<double,N> & GetMax();
		//void UpdateMinMax (const T &surroundedType);
		void Serialize (Bifstream &infile);
		void Serialize (Bofstream &outfile)const;
		//void Transform (const TransformationMatrix &tm);
		void Clear ();
		void ReplaceMin (const sVector<double,N> &p);
		void ReplaceMax (const sVector<double,N> &p);
	private:
		sVector<double,N> m_min;
		sVector<double,N> m_max;
    public:
		static bool CalculateNewAABBIntersectionXY (const  VPoint &oldMin , const VPoint &oldMax
			, const VPoint &newMin , const VPoint &newMax 
		, VPoint &newAABBIntersectionMin , VPoint &newAABBIntersectionMax);
	};

void UpdateAABB (const VPoint &newp , AABB<3> &aabb);
void UpdateAABB (const VPoint2d &newp , AABB<2> &aabb);
void UpdateAABB (const VLine &newl , AABB<3> &aabb);
void UpdateAABB (const VLine2d &newl , AABB<2> &aabb);
void UpdateAABB (const VTriangleE &newt , AABB<3> &aabb);
void UpdateAABB (const VTriangle2dE &newt , AABB<2> &aabb);

#include "AABB.inl"

#endif