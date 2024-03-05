// LoopLineInt.h

#ifndef __LoopLineInt_h__
#define __LoopLineInt_h__

#include "VPoint2d.h"
#include <map>
class SequentialLineSet2d;
class PosCubes2d;
struct VLine2d;
class SequentialLineSet;
struct VPoint;
struct CoordinateSystem;

class LoopLineInt
{
public:
	static 	bool IsPointInLoop ( const VPoint2d &p
								, const SequentialLineSet2d *sl 
								, PosCubes2d *cubes);
	static void LineLoopInt (const SequentialLineSet2d *loop 
							, PosCubes2d *cubes 
							, const VLine2d &l1 
							, std::vector<VPoint2d> &intpvec);

	struct LoopLineIntData //this struct is used for getting int p and loop int parameter 
	{
	public:
		VPoint2d m_p;//this is the intersection point itself
		vreal m_t;//this is intersection point's parameter on loop
		int m_loopno;
	};

	static void CalcParamOfLoopPoints (const SequentialLineSet2d *loop,std::vector<vreal> &paramVec);// Calculates the parameter value for each point of loop
	static void LineLoopInt (const SequentialLineSet2d *loop
							,const std::vector<vreal> &paramVec
							,int loopNo
							,PosCubes2d *cubes
							,const VLine2d &l1
							,std::vector<LoopLineIntData> &intpvec);
	static int GetPrevNodeParam (const std::vector <vreal> &paramVec , const vreal t);

	static bool IsPointInLoop ( const VPoint &p , const SequentialLineSet *sl,const VPoint &outp);
	static bool IsPointInLoop (const VPoint &p , const SequentialLineSet *sl , const VPoint &outp , const CoordinateSystem &planeCs);

	static void SingleLoopLineInt (const SequentialLineSet2d *loop , const VLine2d &l , std::map<int,VPoint2d> &res , bool keyIsU , double keyScaleF);
	static void AllLoopsLineInt (const SequentialLineSet2d *outerLoop , const std::vector<SequentialLineSet2d *> &innerLoopVec , const VLine2d &l , std::map<int,VPoint2d> &res , bool keyIsU , double keyScaleF); 

};

#endif