// ZMap.h

#ifndef __CollCheckZMap_h__
#define __CollCheckZMap_h__

#include "VPoint.h"
class Triangles;
class PointSet;
struct VTriangleE;
class VSurface;
class VTriangles;
#include "ToolPrim.h"
struct LongVec;

namespace collcheck
{
	class ZMap
	{
	public:
		ZMap (const VPoint &mn , const VPoint &mx , double precision);
		ZMap (const VPoint &mn , const VPoint &mx , vreal defZ , vreal precision);
		~ZMap();
		const VPoint & GetMin()const;
		const VPoint & GetMax()const;
		double GetPrecision ()const;
		int GetXSize()const;
		int GetYSize()const;
		// returns false if the x , y is out of min-max of zmap
		bool GetZ (double x , double y , float &z)const;	
		void UpdateZ (const VPoint &p,int trii);
		static int QueryMemory (const VPoint &mn , const VPoint &mx , double precision);
		static double GetPrecisionForMemory (const VPoint &mn , const VPoint &mx , int memoryLimitInBytes);
		static double GetMaxDeltaForPrecision (double precision , int memoryLimitInBytes);
		static int m_memoryLimitInBytes;
		int GetXi (double xc)const;
		int GetYi (double yc)const;
		void FillZMap (const Triangles *trimesh,double sourceTesselationTol);
		void FillZMap (const Triangles *trimesh , const VPoint &mn , const VPoint &mx);
		PointSet * MakePointSet ()const;
		void UpdateWithSingleTri (const VTriangleE &tri , int trii);
		static collcheck::ZMap *MakeZMapAndFill (double tolerance , const std::vector<VSurface *> &surfVec,const std::vector<VTriangles *> &triVec , int *feedbackTotalTriangles , int *feedbackCalculatedTriangles);
		float GetLiftFor (const std::vector<int> &toolxi 
						, const std::vector<int> &toolyi 
						, const std::vector<float> &toolz
						, int &maxi
						, float &maxToolZ 
						, float &maxModelZ 
						, bool &isNearToZMapVerticalEdge);
		float GetZ(int xi,int yi)const;
		bool SetZ (int xi,int yi,float zc,int trii);
		VStr GetInfo ()const;
		double GetSourceTesselationTol ()const;
		void SetSourceTesselationTol (double tol);
		void IncrementSourceTriCount (int toAdd);
		int GetSourceTriCount();
		const std::vector<const Triangles *> & GetTriangles()const;
		int m_sourceTriCount;	// Calculated triangles.
		void SetZMapFeedbackParameters (int *totalTriangles , int *calculatedTriangles);
		ZMap * GetZMapInside (const VPoint &mn , const VPoint &mx , double newPrec)const;
	private:
		void MakeZMap(float defZ);
		void DestroyZMap();
		VPoint m_min;
		VPoint m_max;
		double m_minx , m_miny;
		double m_precision;
		double m_invPrecision;
		float ** m_zmap;
		int **m_imap;	// for everypoint source triangle index is hold. It will be used for refining later.
		int m_xsize , m_ysize;
		VPoint m_delta;
		double m_sourceTesselationTol;
		std::vector<const Triangles*> m_triangles;
		int *m_feedbackTotalTriangles;
		int *m_feedbackCalculatedTriangles;
	public:
		VPoint GetPoint (int xi , int yi)const;
		void GetAllPointsForXi (int xi,std::vector<VPoint> &ypVec)const;
		void GetAllPointsForYi (int yi,std::vector<VPoint> &xpVec)const;
	};
};
#endif