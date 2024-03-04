// SequentialLineSet.h

#ifndef __SequentialLineSet_h__
#define __SequentialLineSet_h__

#include "PointSet.h"

struct VLine;
class TransformationMatrix;
class SequentialLineSet2d;
struct VPlane;
class LineOrArc;
class VInterval;
class LTable;

class SequentialLineSet
{
public:
	SequentialLineSet ( int initialcapacity );
	SequentialLineSet ( const SequentialLineSet & Foo );
	SequentialLineSet ( const SequentialLineSet2d &sl , const vreal z);
	SequentialLineSet (const PointSet &ps);
	VLine operator[] ( int index ) const;
	int Size() const;
	int GetPointSize()const;
	bool InsertPoint ( const VPoint & p );
	VPoint* GetPoint(int i);	
	const VPoint * GetPoint(int i)const;
	VPoint GetMin () const;
	VPoint GetMax () const;
	void Serialize (Bofstream &outfile)const;
	void Serialize (Bifstream &infile);
	void Transform (const TransformationMatrix &tm);
	PointSet & GetPointSet();
	const PointSet & GetPointSet()const;
	bool operator == (const SequentialLineSet &toCompare)const;
	bool operator != (const SequentialLineSet &toCompare)const;
	vreal GetLength()const;
	VPoint GetP (const vreal &t)const;
	VPoint GetP (const vreal &t , const vreal &setLength , int &sli)const;
	static VPoint GetP (const std::vector<VPoint> &seq , double &p0 , double &startL , int &starti , const vreal &t , const vreal &setLengthInv , double* mypar1 = 0, VPoint* tn = 0);
	class SLParametricInfo
	{
	public:
		// divides overall distance of the line sequence to arraySegmentCount and for i x 
		class SLParametricInfo(const std::vector<VPoint>& seq , vreal seqLength , int arraySegmentCount = 30);
		~SLParametricInfo();
	private:
		const std::vector<VPoint>& m_seq;
		int m_arraySegmentCount;
		vreal m_segmentDistanceStep; // segmentDistanceStep = m_seq.Length / arraySegmentCount
		vreal m_seqLength;
		vreal GetSegmentStartDistanceToSeqZero(int segmenti)const; // segmentStartDistanceToSequenceZero = i * segmentDistanceStep
		vreal GetSegmentEndDistanceToSeqZero(int segmenti)const; // segmentEndDistanceToSequenceZero = (i + 1) * segmentDistanceStep
		// For each line if a point on line distance to sequence zero is between segment start and end distances to sequence zero THEN line index is put inside  i  th index vector 
		std::list<std::pair<int,vreal>> * m_distanceFrom0LineIndexArray;// pair holds line index and line start distance from seq begin
	public:
		vreal GetSeqLength()const;
		int GetSegmentiForDistanceToSeqZero(vreal distanceFromSeqBegin)const;// i = (int) (distanceFromSeqBegin / segmentDistanceStep) , i must be in interval [0 , arraySegmentCount - 1] 
		std::pair<vreal, VLine> GetLineForDistance(vreal distanceFromSeqBegin,int &curli)const;
	};
	static VPoint GetP(const SequentialLineSet& sl, const vreal slLength , const vreal t , int &linei , vreal &lenUpToLineStart);

	const VPoint * Start()const;
	const VPoint * End()const;
	VPoint * Start();
	VPoint * End();
	void Reverse();
	void AddBack (const SequentialLineSet &toAdd);
	void Reserve (int newcapacity);
	bool GetPlaneIfPlanar (VPlane &p,double tol,bool &isclosed , bool &isccw)const;
	bool IsPointsClose (const VPlane &p , double tol)const;
	double GetT (const VPoint &pointOnSl , int ni , double slLength)const; // returns length par[0,1] of a point on sl :0 if at the start 1 if at the very end
	int GetNi (const VPoint &pointOnSl , double compTol )const;// Returns the vector index of the point if belongs the sequential points , returns -1 otherwise
	double GetT (const VPoint &pointOnSl , double slLength) const;
protected:
	PointSet sequentialpoints;
public:
	VPoint GetMaxPointInCs (const CoordinateSystem &cs)const;
	VPoint GetMinPointInCs (const CoordinateSystem &cs)const;
	void RemoveCoincidentWithinTol (double tol);
	VLine EndLine()const;
	bool GetSubSl(vreal t0, vreal t1, SequentialLineSet &sl) const;
	void GetNearestPoint(const VPoint &p1, VPoint &pointOnSet, int *ni = NULL) const;
	void DigitiseByLengthStep (SequentialLineSet &sl, vreal step, vreal t0, vreal t1) const;
	void DigitiseByLengthStepReverse (SequentialLineSet &sl, vreal step, vreal t1, vreal t0) const;
	bool GetSubSlRich(vreal t0, vreal t1, SequentialLineSet &sl, vreal tol) const;
	bool GetLineOrArcVec(vreal t0, vreal t1, std::vector<LineOrArc> &result, vreal stepTol, vreal mytol) const;
	void PrepareForArcFit (std::vector<SequentialLineSet> &slVec , vreal sharpAngleThreshold , vreal minDistanceStep)const;
	void PrepareForArcFitNoSharpCornerCheck (std::vector<SequentialLineSet> &slVec , vreal minDistanceStep)const;
	std::map<vreal,vreal> Intersection (const VLine &l)const;// return value is the parameters on l
	bool FitLineOrArcs (vreal sharpAngleThresholdInDegrees , vreal minDistanceStep,std::vector<LineOrArc> &res , const CoordinateSystem &ws , vreal tol , int useMinDistanceStepFlag);// useMinDistanceFlag = 1 means use , otherwise  only points of sl will be used
	static void ProjectPointsToPlane (const SequentialLineSet &sl , const VPoint &pnormal , const VPoint &pp , std::vector<VPoint> &res , vreal tol , bool refinepoints = true);
	static void RefinePoints (const std::vector<VPoint> &src , std::vector<VPoint> &res , double trsh);
	bool IsClosed(vreal tol)const;
	void GetMinMaxAlongCsX (const CoordinateSystem &cs , std::map<vreal,int> &mni , std::map<vreal,int> &mxi,vreal coinTol)const;
	void GetConvexHull (vreal angleStep , std::map<vreal,int> &angleIndexMap ,vreal coinTol)const;
	void GetPointsBetween (int i0,int i1,SequentialLineSet &subSl,vreal closedTol)const;
	void GetPointsBetween2 (int i0 , int i1 , SequentialLineSet &subSl , vreal closedTol)const;
	bool IsCCWOnXY ()const;
	// start to search int from starti if finds intersection and adds to ipVec
	// there may be more than one intersection , return value is the nearest point index to plane point
	// if there is no intersection return value is -1
	int GetInt (const VPlane &pln , std::map<int,vreal> &indexParMap , int starti)const; 
	void ReorderIfClosed (const VPoint &newStartp , int pi , vreal tol);
	bool IsSharpCorner (int pi , vreal cosSharpAngleThreshold)const;
	void PlanarPairWalkIfClosed ( SequentialLineSet *sl 
								, vreal maxLenChangeRatio 
								, vreal cosSharpAngleThreshold 
								, std::list<std::pair<VPoint2d , VLine>> &myParxSlParyLineMap
								, vreal tol);

	void CleanZOssilationOnXZ (SequentialLineSet &sl , const CoordinateSystem &cs , vreal slopeThreshold);
	void CleanZOssilationOnYZ (SequentialLineSet &sl , const CoordinateSystem &cs , vreal slopeThreshold);
	void BreakBySlopeChangeOnXZ (const SequentialLineSet &sl , const CoordinateSystem &cs , vreal slopeChangeThreshold , vreal sharpCornerThreshold , std::vector<SequentialLineSet *> &slVec);// slopeChange = fabs(dx0/dz0-dxcur/dzcur)  
	void BreakBySlopeChangeOnYZ (const SequentialLineSet &sl , const CoordinateSystem &cs , vreal slopeChangeThreshold , vreal sharpCornerThreshold , std::vector<SequentialLineSet *> &slVec);// slopeChange = fabs(dx0/dz0-dxcur/dzcur)  
	void GroupBySlopeIntervalsOnCsXY (const CoordinateSystem &cs , std::list<std::pair<VInterval,std::vector<int>>> &slopeIntervals)const;
	void IndexVecToParIntervals (const std::vector<int> &ivec , std::vector<VInterval> &parIntVec)const;

	SequentialLineSet * MakeSlAndModifyZ(const LTable &lt, const CoordinateSystem &cs)const;
	void MakeSubSl(const std::map<int, int> &points, std::list<SequentialLineSet *> &subSlList)const;
	void XFillBoundary(const CoordinateSystem &cs, std::map<vreal,std::map<vreal,vreal>> &boundary , vreal tol)const;
	bool IsTurningLine(int index);
	void YFillBoundary(const CoordinateSystem &cs, std::map<vreal, std::multimap<vreal, vreal>> &boundary, vreal tol)const;
	void YFillBoundary(const CoordinateSystem &cs, std::map<vreal, std::map<vreal, vreal>> &boundary, vreal tol)const;
	void XFillBoundary3D(const CoordinateSystem &cs, std::map<vreal, std::multimap<vreal, VPoint>> &xCoordYCoordPointMap, vreal tol)const;
	void YFillBoundary3D(const CoordinateSystem &cs, std::map<vreal, std::multimap<vreal, VPoint>> &yCoordXCoordPointMap, vreal tol)const;
	vreal GetGreenAreaOnXY()const;
	void BreakAtSharpCorners(vreal sharpAngleThresholdInDegrees , std::vector<SequentialLineSet *> &splitVec , vreal coincidentTol)const;
	void CollectSharpCorners(vreal sharpAngleThreshold, std::vector<VPoint>& sharpCorners, vreal coincidentTol)const;
	void SetZ(vreal z);
	void DigitiseByLengthStep(vreal thisSlLength, vreal step, vreal t0, vreal t1, std::vector<std::pair<vreal, VPoint>>& orderedLenPointPairs) const;
protected:
	//// vector hold overall distances from start of sequence to end of each line and index of lines 
	//mutable std::vector<std::pair<double, int>>* m_orderedLineEndLengthIndexPairs;
	//mutable int m_sizeWhenGetPInfoMade;
	//mutable VPoint m_minWhenGetPInfoMade;
	//mutable VPoint m_maxWhenGetPInfoMade;
	//void RemakeAndFillGetPInfoIf()const;
	static std::map<const SequentialLineSet*, const SequentialLineSet::SLParametricInfo*> *m_parametricInfoMap;
	static std::map<const vpVec*, const SequentialLineSet::SLParametricInfo*>* m_parametricInfoMap2;
public:	
	void DigitiseByLengthStepNew(SequentialLineSet& tofill , vreal step);
};

#endif