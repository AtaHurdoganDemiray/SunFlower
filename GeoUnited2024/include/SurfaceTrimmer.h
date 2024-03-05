// SurfaceTrimmer.h

#ifndef __SurfaceTrimmer_h__
#define __SurfaceTrimmer_h__

class UntrimmedSurface;
class SequentialLineSet2d;
class VRefTriangleMesh;
struct VTriangle2d;
struct VLine2d;

class SurfaceTrimmer
{
public:
	SurfaceTrimmer (const UntrimmedSurface &surf 
					, const SequentialLineSet2d *outerLoop
					, const std::vector<SequentialLineSet2d *> &innerLoopVec);
	void DoTrimInParSpace ( int uStepNo 
							, int vStepNo 
							, int bigvStepNo
							, const double &ubegin
							, const double &uend
							, const double &vbegin 
							, const double &vend
							, VRefTriangleMesh *trimmedTri);
private:
	const UntrimmedSurface &m_surf;
	const SequentialLineSet2d *p_outerLoop;
	const std::vector<SequentialLineSet2d *> &p_innerLoopVec;
	VRefTriangleMesh *m_trimmedTri;

	int m_uStepNo;
	int m_vStepNo;
	int m_bigvStepNo;
	double m_ubegin;
	double m_uend;
	double m_vbegin;
	double m_vend;
	double m_smallvstep;
	double m_vstep;
	double m_ustep;

	void AddT (const VTriangle2d &t);
	void TrimInParamSpace (std::vector <std::vector<VLine2d>> &leftPoints);
	void MarkTrimLoop (const SequentialLineSet2d *currLoop
						, std::vector <std::vector<double>> &uparsListVec );
	void AddTriOfUnitLine (const VLine2d &ul);
	void AddEndTriangle1 (std::vector<VLine2d>::iterator &a 
							, std::vector<VLine2d>::const_iterator &b
							, std::vector<VTriangle2d> &triVec);
	void AddEndTriangle2 (std::vector<VLine2d>::iterator &a 
								, std::vector<VLine2d>::const_iterator &b
								, std::vector<VTriangle2d> &triVec);
	void AddEndTriangles (std::vector<std::vector<VLine2d>> &leftPoints 
								, std::vector<VTriangle2d> &triVec);
	void MergeTrimLines (std::vector<std::vector <VLine2d>> &tV
							   , const double &maxUstep 
							   , const double &maxVStep);
	void DigitiseLineVec (std::vector<std::vector<VLine2d>> &leftPoints
						, std::vector<bool> &digitisedQuad
						, std::vector<VTriangle2d> &triVec);
	void DigitiseLine (std::vector <VLine2d>::iterator &a
						, std::vector<bool> &digitisedQuad
						, std::vector<VTriangle2d> &triVec);
	void AddQuad (const double &u1,const double &v1
					,const double &u2,const double &v2
					,std::vector<int> &indiceVec);
	void ExceptionalEndTriangles (std::vector<VLine2d> &v1,std::vector<VTriangle2d> &triVec);
	void MakeTrianglesFromQuads (std::vector<bool> &digitisedQuads);
};

#endif