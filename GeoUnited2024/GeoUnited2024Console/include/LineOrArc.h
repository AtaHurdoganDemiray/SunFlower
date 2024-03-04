// LineOrArc.h

#ifndef __LineOrArc_h__
#define __LineOrArc_h__

#include "Bofstream.h"
#include "Bifstream.h"
#include "VPlane.h"

struct VLine;
class VArc;
//class VCurve;
struct VPoint;
class OffsLineOrArc;
struct VLine2d;
class TransformationMatrix;
class SequentialLineSet;
struct CoordinateSystem;
class VariableOffsetManager;
class VInterval;
class VSignedInterval;
class LAIntersectionResult;

class LineOrArc
{
public:
	enum type : char{line,arc};
	LineOrArc ();
	LineOrArc (const VLine &l);
	LineOrArc (const VArc &l);
	LineOrArc (const LineOrArc &other);
	LineOrArc & operator = (const LineOrArc &other);
	~LineOrArc ();
	type GetType ()const;	
	const VLine * GetLine ()const;
	const VArc * GetArc ()const;
	void SetLine (const VLine &l);
	void SetArc (const VArc &a);
	void Reverse ();
	//VCurve * GetCurve();
	//const VCurve * GetCurve()const;
	//void SetCurve (VCurve *crv);
	
	// This function assumes lines and arcs are on a plane parallel to X-Y principal plane
	static bool IsPointInLoop (const std::vector<LineOrArc> &loop , const VPoint &p, vreal tol , bool& isPointOnLoop , bool choiceOnSuspicion);
	static int IsPointInLoop (const std::vector<LineOrArc> &loop , const VPoint &p,const VLine &fake , const VLine2d &fake2d , vreal tol);//0 on loop , 1 in loop , 2 out of loop
	static bool IsPointInLoop(const std::list<LineOrArc>& loop, const VPoint& p, const VLine& fake, const VLine2d& fake2d);
	static bool IsPointInLoop (const std::vector<LineOrArc *> &loop , const VPoint &p);
	static bool IsPointInLoop (const std::vector<LineOrArc *> &loop , const VPoint &p,const VLine &fake , const VLine2d &fake2d);
	static bool IsPointInLoop(const std::list<LineOrArc>& loop, const VPoint& p);

	static bool IsCCW (const std::vector<LineOrArc> &vec);
	static bool IsCCW (const std::vector<LineOrArc*> &vec);

	static void ReverseIf (bool makeccw , std::vector<LineOrArc> &loop , vreal tol);
	static void ReverseIf (bool makeccw , std::vector<LineOrArc*> &loop , vreal tol);

	static void Offset (const std::vector<LineOrArc> &vec , std::vector<OffsLineOrArc> &ofseted , double offsetval , double jointol);
	static void Offset (const std::vector<LineOrArc*> &vec , std::vector<OffsLineOrArc> &ofseted , double offsetval , double jointol);
	static void OffsetByExtendingElements (const std::vector<LineOrArc> &vec , std::vector<OffsLineOrArc> &ofseted , double offsetval , double jointol);
	static void OffsetByExtendingElements (const std::vector<LineOrArc*> &vec , std::vector<OffsLineOrArc> &ofseted , double offsetval , double jointol);
	static void OffsetByExtendingElements (const std::vector<LineOrArc> &vec , std::vector<OffsLineOrArc> &ofseted , std::vector<vreal> offsValVec , vreal jointol);

	static int GetNextValidElement ( const std::vector<OffsLineOrArc> &ofsetSeq , int curi ,bool isClosed);//returns -1 if no valid element after cur index

	static void NewOffset (const std::vector<LineOrArc> &vec , std::vector<LineOrArc> &ofseted , double offsetval , double joinTol , const std::vector<LineOrArc> &collSl,double tol,bool isSharpOffset,VariableOffsetManager *va=0);
	static void NewOffset (const std::vector<LineOrArc*> &vec , std::vector<LineOrArc*> &ofseted , double offsetval  , double joinTol , const std::vector<LineOrArc*> &collSl,double tol,bool isSharpOffset,VariableOffsetManager *va=0);
	static void NewOffset(const std::vector<LineOrArc*>& vec, std::vector<LineOrArc*>& ofseted, double offsetval, double joinTol, const std::vector<LineOrArc>& collSl, double tol, bool isSharpOffset, VariableOffsetManager* va = 0);
	static void NewSharpOffsetForConvexLoop (const std::vector<LineOrArc> &vec , std::vector<LineOrArc> &ofseted , std::vector<double> offsetvalVec , double joinTol , const std::vector<LineOrArc> &collSl,double tol);
	static void NoTrimOffset(const std::vector<LineOrArc>& vec, std::vector<LineOrArc>& ofseted, double offsetval, double joinTol, const std::vector<LineOrArc>& collSl, double tol, bool isSharpOffset);


	static void AddTrimmed (const LineOrArc &gr , std::vector<LineOrArc> &ofseted,const std::vector<LineOrArc> &collSl , double offsetRad , double tol , double joinTol , int parenti , int startNeighbori , int endNeighbori);
	static void AddTrimmed (const LineOrArc &gr , std::vector<LineOrArc*> &ofseted,const std::vector<LineOrArc*> &collSl , double offsetRad , double tol , double joinTol , int parenti , int startNeighbori , int endNeighbori);
	static void AddTrimmed(const LineOrArc& gr, std::vector<LineOrArc*>& ofseted, const std::vector<LineOrArc>& collSl, double offsetRad, double tol, double joinTol, int parenti, int startNeighbori, int endNeighbori);


	VPoint GetStartPoint ()const;
	VPoint GetEndPoint ()const;
	bool RoughOffsetInXY (double offsetval , OffsLineOrArc &og)const;
	void TrimBothIf (LineOrArc &g , double joinTol);
	void TrimOrExtendTo (LineOrArc &g , double joinTol);
	void GetIntersections (LineOrArc &g /*, double joinTol*/ , std::vector<VPoint> &wpthis , std::vector<VPoint> &wpg,vreal tol);
	static void GetMinMax(const std::list<LineOrArc>& vec, VPoint& minp, VPoint& maxp);
	static void GetMinMax (const std::vector<LineOrArc> &vec , VPoint &minp , VPoint &maxp);
	static void GetMinMax (const std::vector<LineOrArc *> &vec , VPoint &minp , VPoint &maxp);
	void SetZ (double newz);
	static void SetZ (std::vector<OffsLineOrArc> &vec,double newz);
	static void SetZ (std::vector<LineOrArc> &vec , double newz);
	static void SetZ(std::vector<LineOrArc*>& vec, double newz);
	double GetLength ()const;
	std::vector<LineOrArc> Split (double lenToSplit , double threshold)const;
	std::vector<LineOrArc> Split (const VPoint &p , double threshold)const;
	static void Reverse (std::vector<LineOrArc> &vec);
	static void Reverse (std::vector<LineOrArc *> &vec);

	static double GetLength (const std::vector<LineOrArc> &vec);
	double GetParLength (const double t)const;
	bool TrimLineTo (VLine &l)const;
	void Transform (const TransformationMatrix &T);
	bool IsDegenerate ()const;

	LAIntersectionResult GetIntersections (const LineOrArc &g, vreal tol)const;
	void GetNearestPoints(const LineOrArc &g, double distthreshold , double arctol
		,std::vector<VPoint> &pvec1 , std::vector<VPoint> &pvec2)const;
	SequentialLineSet GetSl (double tol)const;
	LineOrArc GetReversed ()const;
	static void FindMinPoint (const std::vector<LineOrArc> &loop , const VPoint &startpoint , size_t &mingeoi , VPoint &minp , double &mint);
	static bool FindIntPoint (const std::vector<LineOrArc> &loop , const VPoint &startpoint , double angle , size_t &mingeoi , VPoint &minp , double &mint);
	static void ReorderLoop (std::vector<LineOrArc> &loop , size_t mingeoi , const VPoint &p , double mint , vreal tol);
	static const double SplitThreshold;
	void Serialize (Bofstream &f)const;
	void Serialize (Bifstream &f);
	static void SerializeVec (const std::vector<LineOrArc> &vec , Bofstream &f);
	static void SerializeVec (std::vector<LineOrArc> &vec , Bifstream &f);

	static bool RepairGaps (const std::vector<LineOrArc> &original , std::vector<LineOrArc> &repaired , double tol);
	static bool RepairGapByExtendAndCut (LineOrArc &first , LineOrArc &second);
	static void RepairGapByChangingFirstsEnd (LineOrArc &first , LineOrArc &second);
	static void RepairGapByChangingSecondsStart (/*LineOrArc &first*/const VPoint &ep , LineOrArc &second);
	static double MaxDeviation (const LineOrArc &original , const LineOrArc &modified);
	VPoint GetStartTangent ()const;
	VPoint GetEndTangent ()const;
	// This function explodes a vector of line or arcs to connected sets called loops
	// A loop may be closed or open but lines and arcs in a loop are sorted such that they are sequentially connected
	// two entities are connected if previous end point and next start point are coincident within coinTol
	// function connects two entities if the distance between previous end and next start point is smaller than sew max dist
	static void LoopSort (std::vector<LineOrArc> &toSort 
						, const VPoint &startpoint 
						, std::vector<std::vector<LineOrArc>> &loopsVec
						, std::vector<bool> &isClosed
						, double coinTol
						, double sewMaxDist);
	static void LoopSortNoReverse
						(std::vector<LineOrArc> &toSort 
						, const VPoint &startpoint 
						, std::vector<std::vector<LineOrArc>> &loopsVec
						, std::vector<bool> &isClosed
						, double coinTol
						, double sewMaxDist);

	static double GetClosest (std::vector<LineOrArc> &toSort 
							, const VPoint &p 
							, std::vector<LineOrArc>::iterator &closeCrvItr 
							, bool &doReverse 
							, const double coinTol 
							, int &coinCount);
	static double GetClosestNoReverse (std::vector<LineOrArc> &toSort 
							  , const VPoint &p 
							  , std::vector<LineOrArc>::iterator &closeCrvItr
							  , const double coinTol
							  , int &coinCount);
	static VPoint MergeOpenContours(const VPoint &startPoint, std::vector<std::vector<LineOrArc>> &contourVec, std::vector<bool> &isClosedVec, double coinTol, double sewMaxDist);
	static bool Mergeif(std::vector<LineOrArc> &cv0, std::vector<LineOrArc> &cv1, double sewMaxDist);

	static VArc LimitArcOnXYwith1PointOnit (const VArc &tolimit , const VPoint &p , bool limitstart);
	static VArc LimitArcOnXYwith2PointsOnit (const VArc &tolimit,const VPoint &p0 , const VPoint &p1, bool limitstart);
	static void Transform (std::vector<LineOrArc> &vec , const TransformationMatrix &tm);
	static void TransformFromAbsToLocalCs (const std::vector<LineOrArc> &vecOnAbs , const CoordinateSystem &cs, std::vector<LineOrArc> &vecToCs);
	static void TransformBackFromLocalCsToAbs (const std::vector<LineOrArc> &vecOnLocalXY , const CoordinateSystem &cs , std::vector<LineOrArc> &vecToAbs);
	static void Transform(std::vector<LineOrArc*>& vec, const TransformationMatrix& tm);
	static void TransformFromAbsToLocalCs(const std::vector<LineOrArc*>& vecOnAbs, const CoordinateSystem& cs, std::vector<LineOrArc*>& vecToCs);
	static void TransformBackFromLocalCsToAbs(const std::vector<LineOrArc*>& vecOnLocalXY, const CoordinateSystem& cs, std::vector<LineOrArc*>& vecToAbs);
	void ReplaceMinMax(VPoint &mn , VPoint &mx , int arcDivideCount)const;
	static bool IsParallelToCsXY (const std::vector<LineOrArc> &loop , const CoordinateSystem &cs , double tol );
	bool IsParallelToCsXY (const CoordinateSystem &cs , double tol)const;
	void DigitiseByLengthStep (double lenStep , std::vector<VPoint> &digiVec)const;
	VPoint GetP (double par)const;// par is between 0 and 1 , 0 for start point 1 for end point
	double GetMinDist (const VPoint &p , VPoint &pointOnGeo)const;
	static void Get2dBox (std::vector<LineOrArc> &box2d , const VPoint &mn , const VPoint &mx , double z,double r,const VPoint &dirz);
	static void GetHeightLines (std::vector<LineOrArc> &heights , const VPoint &mn , const VPoint &mx , double r , const VPoint &dirz);
	void ReplaceMinMaxInCs (const CoordinateSystem &cs,VPoint &mn,VPoint &mx,int arcDivideCount)const;
	static bool IsSingleFullCircle (const std::vector<LineOrArc> &contour );
	static void Digitise (const std::vector<LineOrArc> &contour , double tol , SequentialLineSet &sl);
	bool Compare (const LineOrArc &g2 , double tol)const;
	bool CompareReversed (const LineOrArc &g2 , double tol)const;
	static int FindOffsetParentIf (const std::vector<LineOrArc> &originalSrcVec , const std::vector<LineOrArc> &collCkVec , int originalParent , double tol);
	static int FindOffsetParentIf (const std::vector<LineOrArc *> &originalSrcVec , const std::vector<LineOrArc *> &collCkVec , int originalParent , double tol);
	static int FindOffsetParentIf(const std::vector<LineOrArc*>& originalSrcVec, const std::vector<LineOrArc>& collCkVec, int originalParenti, double tol);
	static std::vector<LineOrArc> FromPointer(const std::vector<LineOrArc*>& src);
	static std::vector<LineOrArc*> ToPointer(const std::vector<LineOrArc>& src);
private:
	union loa
	{
		VLine *m_l;
		VArc *m_a;
	};
	void DeleteData();
	loa m_geo;
	type m_type;	
	//VCurve *m_crv;
public:
	static bool m_ckLineMinLength;
	static double m_lineMinLengthAllowed;
	static bool m_ckLineMaxLength;
	static double m_lineMaxLengthAllowed;

	static bool m_ckArcMinPerimeter;
	static double m_minArcPerimeterAllowed;
	static bool m_ckArcMaxPerimeter;
	static double m_maxArcPerimeterAllowed;
	static bool m_ckArcRadius;
	static double m_minArcRadius;
	static bool m_ckStartVectorRadiusConformance;
	static double m_startVectorRadiusConformanceTolerance;
	static bool m_ckIsAxisUnit;
	static double m_axisVectorUnitTolerance;
	static bool m_ckIsAxisPerpToStartVector;
	static double m_axisPerpToStartVectorTolerance;
	static bool m_ckSweepAngle;
	static double m_minSweepAngleTolerance;
public:
	bool IsCircle()const;
	static void DestroyVector (std::vector<LineOrArc *> &toDelete);
	static void AddToLineOrArcVec (const std::vector<VPoint> &pVec , std::vector<LineOrArc> &geoVec);
	static void AddToLineOrArcVec (const std::vector<VPoint> &pVec , std::vector<LineOrArc *> &geoVec);
	static void SerializeVecVec (const std::vector<std::vector<LineOrArc>> &vecvec , Bofstream &f);
	static void SerializeVecVec (std::vector<std::vector<LineOrArc>> &vecvec , Bifstream &f);
	static void SerializeVecVec (const std::vector<std::vector<double>> &vecvec , Bofstream &f);
	static void SerializeVecVec (std::vector<std::vector<double>> &vecvec , Bifstream &f);
	static void FillTapFrontVec(std::vector<LineOrArc> &frontVec , bool isstart , bool isccw , int tapiCCW , int tapiCW , const std::vector<LineOrArc> &geoVec);
	static void FillTapRemainingVec(std::vector<LineOrArc> &remainingVec , bool isstart , bool isccw , int tapiCCW , int tapiCW , const std::vector<LineOrArc> &geoVec);
	static int AddTapPoint(std::vector<LineOrArc> &vecGeo , double distFromBegining);
	static bool IsContourSingleFullCircle (const std::vector<LineOrArc> &contour);
	bool ExtendGeoByLengthAtStart (vreal extensionLen);
	bool ExtendGeoByLengthAtEnd (vreal extensionLen);
	static void AddExtensionToClosedLoopAtEnd (const std::vector<LineOrArc> &originalLoop,vreal endExtension,std::vector<LineOrArc> &loopToAdd);
	static void AddExtensionToClosedLoopAtBegining (const std::vector<LineOrArc> &originalLoop , vreal frontExtension , std::vector<LineOrArc> &loopToAdd);
	static bool IsContourCircle (const std::vector<LineOrArc> &loop , vreal tol , VArc &resultCircle);
	static void DigitiseByLengthStep (const std::vector<LineOrArc> &laVec, vreal step, SequentialLineSet &sl);
	void DigitiseByLengthStep (double lenStep , SequentialLineSet &sl)const;
	/*static void GetSubLineOrArcVec(const std::vector<LineOrArc> &laVec, const VPoint &p1, const VPoint &p2
		, std::vector<LineOrArc> &result);
	static void GetSubLineOrArcVec2(const std::vector<LineOrArc> &laVec, const VPoint &p1, const VPoint &p2
		, std::vector<LineOrArc> &result);*/
	VPoint GetTan(vreal par) const;

	bool GetParIf ( const VPoint &p , vreal &t , vreal tol )const;// returns false if point is not on unlimited geometry, it may return par bigger than 1 or smaller than 0 if point is on infinite geometry
	static void GetMinMaxInCs (const std::vector<LineOrArc> &geoVec , const CoordinateSystem &cs , VPoint &mn , VPoint &mx);
	VPoint GetMinInCs (const CoordinateSystem &cs)const;
	VPoint GetMaxInCs (const CoordinateSystem &cs)const;
	void GetMinMaxInCs (const CoordinateSystem &cs , VPoint &mn , VPoint &mx)const;
	LineOrArc GetSubSet (vreal t1 , vreal t2)const;// t1 and t2 are start and end parameters for the sub segment between 0 and 1
	static void FindNearestAngle (const std::vector<LineOrArc> &geoVec , vreal angle , size_t &mingeoi , VPoint &minp , vreal &mint,vreal tol);
	//void GroupByTangentAngleRange (std::list<std::pair<VInterval , std::list<VSignedInterval>>> &angleRangeGroupMap)const;
	//static void GroupByTangentAngleRange (const std::vector<LineOrArc> &contour , std::list<std::pair<VInterval , std::vector<LineOrArc>>> &angleRangeGroupMap);
	static void GroupByTangentAngle (const std::vector<LineOrArc> &contour 
		, std::list<std::pair<VInterval,std::list<LineOrArc>>> &angleRangeLineGeoGroups
		, std::list<std::pair<VInterval,std::list<LineOrArc>>> &angleRangeArcGeoGroups
		, bool letReversingGeometry);
	void GroupByTangentAngle (std::list<std::pair<VInterval,std::list<LineOrArc>>> &angleRangeLineGeoGroups
								, std::list<std::pair<VInterval,std::list<LineOrArc>>> &angleRangeArcGeoGroups
								, bool letReversingGeometry)const;
	VPlane::IntType GetIntPoints (const VPlane &pln , std::vector<VPoint> &intPoints , std::vector<VPoint> &intPointsFull)const;
	LineOrArc GetTrimmedAtStart (vreal distance)const; //  Trim the geo by distance at start
	LineOrArc GetTrimmedAtEnd (vreal distance)const;// Trim the geo by distance at end
	LineOrArc GetTrimmedAtStartAndEnd (vreal distance)const;

	static bool IsInside(const std::vector<LineOrArc>* bigLoop, const std::vector<LineOrArc>* smallLoop, vreal tol); //, bool fullCheck);
	static bool IsIntersecting(const std::vector<LineOrArc> *bigLoop, const std::vector<LineOrArc> *smallLoop, vreal tol);
	static void GroupForSolid(std::multimap<vreal, std::pair<std::vector<LineOrArc> *,bool>> &allLoops, vreal tol
		, std::list<std::pair<std::vector<LineOrArc>* , std::vector<std::vector<LineOrArc>*>>> &allGroups);
	static void ExplodeArcsToLines(std::vector<LineOrArc> &loop, vreal tol);
	static int GetParIf(const VPoint &p
		, const std::vector<LineOrArc> &contour
		, const std::vector<vreal> &lengthVec
		, vreal contourLen
		, vreal &t
		, vreal &parongeo
		, vreal tol);
	static void FillLengthVec(const std::vector<LineOrArc> &contour , std::vector<vreal> &lengthVec,vreal &contourLen);
	
	// This function needs 2 closed contours (first and second)
	// There is a boolean flag trueForInsideFalseForOutside
	// if this is true , the geometry of first contour which stays inside second contour will be added to geoVec
	// if this flag is false , the geometries of first contour that stays outside the second contour will be added to geoVec
	static void GetSubsetGeoVec(const std::vector<LineOrArc> &first
		, const std::vector<vreal> &firstLenVec
		, vreal firstContourLen
		, const std::vector<LineOrArc> &second
		, const std::vector<vreal> &secondLenVec
		, vreal secondContourLen
		, const std::vector<VPoint> & vPvec
		, std::vector<LineOrArc> &geoVec
		, bool trueForInsideFalseForOutside
		, vreal tol
		, std::vector<LineOrArc> &overlapsVec);

	static int GetP (const std::vector<LineOrArc> &contour
		, const std::vector<vreal> &lenVec
		, vreal contourLen
		, vreal t
		, VPoint &p
		, vreal & parongeo);
	// returns true if there is overlap
	static bool GetIntervalsInOtherContour(const std::vector<LineOrArc> &contour
		, const std::vector<vreal> &lenVec
		, vreal contourLen
		, const std::map<vreal, VPoint> &pointVec
		, std::vector<VInterval> &intervals
		, const std::vector<LineOrArc> & other
		, const std::vector<vreal> &otherLenVec
		, vreal otherContourLen
		, std::vector<VInterval> &overlapsFirst
		, std::vector<VInterval> &overlapsSecond
		, vreal tol);
	static void LineOrArc::AppendSubset
		(const std::vector<LineOrArc> &contour
		, const std::vector<vreal> &lenVec
		, vreal contourLen
		, vreal t1, vreal t2
		, std::vector<LineOrArc> &subGeoVec);
	static void Intersect(const std::vector<LineOrArc> &firstContour
		, const std::vector<vreal> &firstLenVec
		, vreal firstContourLen
		, const std::vector<LineOrArc> &secondContour
		, const std::vector<vreal> &secondLenVec
		, vreal secondContourLen
		, std::vector<std::vector<LineOrArc>> &intersection
		, std::vector<bool> &isClosedVec
		, vreal tol);

	// returns 0 if no intersection , returns 1 if regular intersection , returns 2 if second is inside first 
	// , returns 3 if first contour is inside second
	static int Subtract(const std::vector<LineOrArc> &firstContour
		, const std::vector<vreal> &firstLenVec
		, vreal firstContourLen
		, const std::vector<LineOrArc> &secondContour
		, const std::vector<vreal> &secondLenVec
		, vreal secondContourLen
		, std::vector<std::vector<LineOrArc>> &subtraction
		, std::vector<bool> &isClosedVec
		, vreal tol);
	
	// returns 0 if no intersection , returns 1 if regular intersection , returns 2 if second is inside first 
	// , returns 3 if first contour is inside second
	static int Unite(const std::vector<LineOrArc> &firstContour
		, const std::vector<vreal> &firstLenVec
		, vreal firstContourLen
		, const std::vector<LineOrArc> &secondContour
		, const std::vector<vreal> &secondLenVec
		, vreal secondContourLen
		, std::vector<std::vector<LineOrArc>> &uniteRes
		, std::vector<bool> &isClosedVec
		, vreal tol);

	static void GetIntersectionPoints(const std::vector<LineOrArc> &firstContour
		, const std::vector<LineOrArc> &secondContour
		, std::vector<VPoint> &intersectionPoints
		, std::vector<LineOrArc> &firstIntElements
		, std::vector<LineOrArc> &secondIntElements
		,vreal tol);
	static void MakeAllArcNormalsUZ(std::vector<LineOrArc> *loop);
	static std::vector<std::vector<LineOrArc> *> MakeStarForClosed(const std::vector<std::vector<LineOrArc>> &src
		,const std::vector<bool> &isClosed);

	void GiveElevationAtStart(vreal h , vreal tol , std::vector<LineOrArc> &rampVec , vreal bigAngle , vreal plungeH , vreal geoLen)const;
	void GiveElevationAtEnd (vreal h , vreal tol , std::vector<LineOrArc> &rampVec , vreal bigAngle , vreal plungeH , vreal geoLen)const;
	//vreal GetMinDistOnXY(LineOrArc &other)const;
	static bool Compare(const std::vector<LineOrArc> &vec1, const std::vector<LineOrArc> &vec2, vreal tol);
	static int MergeLineOrArcsIf(std::vector<LineOrArc> &connectedLoop,vreal tol);// returns the number of merged loops 
	static bool CanIMerge(std::vector<LineOrArc> &connectedLoop, int gi, int ginext,vreal tol);
	static LineOrArc MergeSingle(std::vector<LineOrArc> &connectedLoop, int gi, int ginext , vreal tol);
	static void MergeUpdate(std::vector<LineOrArc> &connectedLoop, int gi, int ginext, LineOrArc &geo);
	
	static void PhiXY(const std::vector<LineOrArc> & l1,const std::vector<LineOrArc> &l2,
		std::vector<std::pair<VPoint, VPoint>> & toolPartMinLeft,
		std::vector<std::pair<VPoint, VPoint>> & toolPartMinRigth,
		std::vector<std::pair<VPoint, VPoint>> & toolPartMaxLeft,
		std::vector<std::pair<VPoint, VPoint>> & toolPartMaxRigth);
	static void PhiXY(const LineOrArc & l1, const LineOrArc &l2 ,
		std::pair<VPoint, VPoint> & toolPartMinLeft,
		std::pair<VPoint, VPoint> & toolPartMinRigth,
		std::pair<VPoint, VPoint> & toolPartMaxLeft,
		std::pair<VPoint, VPoint> & toolPartMaxRigth);
	static void PhiXYLineLine(const LineOrArc & l1, const LineOrArc &l2, 
		std::pair<VPoint, VPoint> & toolPartMinLeft,
		std::pair<VPoint, VPoint> & toolPartMinRigth,
		std::pair<VPoint, VPoint> & toolPartMaxLeft,
		std::pair<VPoint, VPoint> & toolPartMaxRigth);
	static void PhiXYLineArc(const LineOrArc & l1, const LineOrArc &l2,
		std::pair<VPoint, VPoint> & toolPartMinLeft,
		std::pair<VPoint, VPoint> & toolPartMinRigth,
		std::pair<VPoint, VPoint> & toolPartMaxLeft,
		std::pair<VPoint, VPoint> & toolPartMaxRigth);
	static void PhiXYArcLine(const LineOrArc & l1, const LineOrArc &l2, 
		std::pair<VPoint, VPoint> & toolPartMinLeft,
		std::pair<VPoint, VPoint> & toolPartMinRigth,
		std::pair<VPoint, VPoint> & toolPartMaxLeft,
		std::pair<VPoint, VPoint> & toolPartMaxRigth);
	static void PhiXYArcArc(const LineOrArc & l1, const LineOrArc &l2, 
	    std::pair<VPoint, VPoint> & toolPartMinLeft,
	    std::pair<VPoint, VPoint> & toolPartMinRigth,
	    std::pair<VPoint, VPoint> & toolPartMaxLeft,
	    std::pair<VPoint, VPoint> & toolPartMaxRigth);
	// Why are we calculating 2 overlap geometries ?  They are same but their directions are conforming to first and second geometries
	static bool GetOverlapIf(const LineOrArc &first, const LineOrArc &second, std::vector<VInterval> &firstoverlap, std::vector<VInterval> &secondoverlap, vreal tol);
	static bool SubtractOverlaps(LineOrArc &geo, const std::vector<LineOrArc> &overlaps, std::vector<LineOrArc> &subractionResult);
	static bool SubtractOverlaps(std::vector<LineOrArc> &contour, const std::vector<LineOrArc> &overlaps);
	static void GetOverlapIntervalOnContour(const std::vector<LineOrArc> &contour, const std::vector<vreal> &contourLenVec, vreal contourLen, const LineOrArc &overlap, std::vector<VInterval> &viVec, vreal tol, bool trueForInsideFalseForOutside);
	bool static IsPointOnLoop(const VPoint& p, const std::vector<LineOrArc> &contour, std::vector<vreal> contourLenVec, vreal contourLen, vreal tol);
	static void DrawInTryCatch(const std::vector<LineOrArc> &vec);
	static bool IsSingleFullArc(const std::vector<LineOrArc> &connectedLoop , vreal tol , VArc &singleFullArc);
	static void SplitBy(const std::vector<LineOrArc> &contour , const VPlane &pln, std::vector<LineOrArc> &backVec, std::vector<LineOrArc> &frontVec , std::vector<VPoint> &intPoints,vreal tol);
	void SplitBy(const VPlane &pln, std::vector<LineOrArc> &backVec , std::vector<LineOrArc> &frontVec,std::vector<VPoint> &intPoints,vreal tol)const;
	static void AddAnglesForConvexHullCalculation(const std::vector<LineOrArc> &vec, const CoordinateSystem &cs, std::map<vreal, vreal> &allAngles , vreal tol);
	void AddAnglesForConvexHullCalculation(const CoordinateSystem &cs , std::map<vreal,vreal> &allAngles , vreal tol)const;
	static void UpdateMinMaxPointAlongCsX(const CoordinateSystem &cs, const std::vector<LineOrArc> &vec, VPoint &mn, VPoint &mx , vreal tol);
	void UpdateMinMaxPointAlongCsX(const CoordinateSystem &cs, VPoint &mn, VPoint &mx , vreal tol)const;
	static void GreedySortLevelOfContours(const VPoint &sortingStartPoint , std::list<std::vector<LineOrArc>> &oneLevel , bool makeCCW);
	static std::list<std::vector<LineOrArc>>::iterator GetNearestContour(const VPoint &sp , std::list<std::vector<LineOrArc>> &oneLevel);
	static bool IsClosed(const std::vector<LineOrArc> &loop , vreal tol);
	static bool IsClosed(const std::vector<LineOrArc *> &loop, vreal tol);
	//static void CheckConcaveCorners(bool isCCW, bool isToolRight, LineOrArc &prevGeo, LineOrArc &geo, LineOrArc &nextGeo, bool &isPrevConcave, bool &isNextConcave , vreal tol);
	static void CheckConcaveCorners(bool isToolRight, const LineOrArc &prevGeo, const LineOrArc &geo, const LineOrArc &nextGeo, bool &isPrevConcave, bool &isNextConcave, vreal tol);
	static bool IsCornerConcave(const LineOrArc& geo, const LineOrArc& nextGeo, bool isToolRight , vreal tol);
	static void GetPositiveAngleIntervalsOfArcToAbsXAxis(const VArc *a, std::vector<VInterval> &anglesInDegrees , vreal tol);
	static std::vector<LineOrArc> GetSubset(const LineOrArc &);
	static void WriteLoopToFileAsciiAsOneLine(const std::vector<LineOrArc> &loop , std::ofstream &f , char coordinateSeperator , int floatPrecision , vreal arcDigitiseTol);
	static void WriteSinglePoint(const VPoint &p, std::ofstream &f, char coordinateSeperator , int floatPrecision);
	static void WriteArcGeo(const LineOrArc &arcGeo, std::ofstream &f, char coordinateSeperator, int floatPrecision , vreal arcDigitiseTol);
	static std::vector<std::vector<LineOrArc>> BreakAtSharpCorners(const std::vector<LineOrArc> &toBreak , vreal sharpAngleThreshold);
	static bool IsOffsetSinglePoint(const std::vector<LineOrArc>& vec, vreal offsetval , vreal tol , VArc& tofill);
	static bool IsOffsetSinglePoint(const std::vector<LineOrArc*>& vec, vreal offsetval, vreal tol, VArc& tofill);
	static bool IsSingleArc(const std::vector<LineOrArc>& vec, vreal tol, VArc& singleArc , bool& isThereOverlap);
	static bool IsSingleArc(const std::vector<LineOrArc*>& vec, vreal tol, VArc& singleArc, bool& isThereOverlap);
	static void MakeAndAddToVec(const std::vector<LineOrArc>& srcVec, std::vector<LineOrArc*>& toAdd);
	static void DestroyVecVec(std::vector<std::vector<LineOrArc*>>& vec);
	static void DestroyVecVec(std::vector<std::vector<LineOrArc*>*>& vec);
	static std::vector<LineOrArc*>* CopyClone(const std::vector<LineOrArc*>* src);
	static void FillVectorByMaking(const std::vector<LineOrArc*>& src, std::vector<LineOrArc*>& toFill);
	LineOrArc GetOffseted(vreal offsVal , const VPoint& planeNormal)const;
};

typedef std::vector<LineOrArc>* LineOrArcVecPtr;
typedef const std::vector<LineOrArc>* ConstLineOrArcVecPtr;

#endif