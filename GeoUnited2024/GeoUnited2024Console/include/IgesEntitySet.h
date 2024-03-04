// IgesEntitySet.h

#ifndef __IgesEntitySet_h__
#define __IgesEntitySet_h__

//#include <vector>
#include "IgesEntity.h"
class BSplineCurve;
class BSplineSurface;
class CompositeCurve;
class CurveOnParametricSurface;
class TrimmedSurface;
class BoundedSurface;
class TabulatedCylinder;
class SurfaceOfRevolution;
class TransformationMatrix;
class ColorDefinition;
struct LongVec;
class igesTransformationMatrix;
class IgesFileWrite;
class PlaneSurface;
class RightCircularCylindricalSurface;
class RightCircularConicalSurface;
class SphericalSurface;
class ToroidalSurface;
class GeneralNote;

class IgesEntitySet
{
public:
	IgesEntitySet();
	~IgesEntitySet();
	long Size() const;
	IgesEntity * operator[] ( long index ) const;
	void AddBSplineCurve ( long DEno , long PEno , DirectoryEntry & de);
	void AddBSplineSurface ( long DEno , long PEno , DirectoryEntry & de);
	void AddCompositeCurve ( long DEno , long PEno , DirectoryEntry & de);
	void AddCurveOnParametricSurface ( long DEno , long PEno , DirectoryEntry & de);
	void AddTrimmedSurface ( long DEno , long PEno , DirectoryEntry & de);
	void AddBoundary ( long DEno , long PEno , DirectoryEntry & de);
	void AddBoundedSurface  ( long DEno , long PEno , DirectoryEntry & de);	
	void AddLine ( long DEno , long PEno , DirectoryEntry & de);
	void AddArc ( long DEno , long PEno , DirectoryEntry & de);
	void AddPoint ( long DEno , long PEno , DirectoryEntry & de);
	void AddCopiousData ( long DEno , long PEno , DirectoryEntry & de);
	void AddConicArc ( long DEno , long PEno , DirectoryEntry & de);
	void AddParametricSpline ( long DEno , long PEno , DirectoryEntry & de);
	void AddParametricSplineSurface (long DEno , long PEno , DirectoryEntry & de);

	void AddPlaneEntity ( long DEno , long PEno , DirectoryEntry & de);
	void AddRuledSurface ( long DEno , long PEno , DirectoryEntry & de);
	void AddEntity (IgesEntity *newent , long DEno , long PEno , DirectoryEntry & de);

	IgesEntity * GetEntityPointer ( long DEno ) const;
	void DestroySet();
	void MakePartOfTrimmedSurface ( const IgesEntity * pigs );
	void TakeOutOfTrimmedSurface ( const IgesEntity * pigs );
	bool IsPartOfTrimmedSurface ( int index ) const;
	bool IsSupportedSurfaceType ( const IgesEntity * pigs ) const;
	bool IsSupportedCurveType ( const IgesEntity * pigs ) const;	
	static int GetIndex ( const IgesEntity * pigs );
	static int GetIndex (int deno);
	void AddTabulatedCylinder ( long DEno , long PEno , DirectoryEntry & de);
	void AddSurfaceOfRevolution ( long DEno , long PEno , DirectoryEntry & de);
	void AddTransformationMatrix ( long DEno , long PEno , DirectoryEntry & de);
	void AddOffsetSurface ( long DEno , long PEno , DirectoryEntry & de);
	void AddColorDefinition ( long DEno , long PEno , DirectoryEntry & de);
	void AddGeneralNote (long DEno,long PEno,DirectoryEntry &de);
	void AddVertexEntity (long DEno , long PEno , DirectoryEntry &de);
	void AddEdgeEntity (long DEno , long PEno , DirectoryEntry &de);
	void AddSubFigureDefinition (long DEno , long PEno , DirectoryEntry &de);
	void AddSingularSubFigureInstance (long DEno , long PEno , DirectoryEntry &de);
	void AddPlaneSurfaceEntity(long DEno, long PEno, DirectoryEntry &de);
	void AddRightCircularCylindricalSurfaceEntity(long DEno, long PEno, DirectoryEntry &de);
	void AddRightCircularConicalSurfaceEntity(long DEno, long PEno, DirectoryEntry &de);
	void AddSphericalSurfaceEntity(long DEno, long PEno, DirectoryEntry &de);
	void AddToroidalSurfaceEntity(long DEno, long PEno, DirectoryEntry &de);
	void AddLoopEntity(long DEno, long PEno, DirectoryEntry &de);
	void AddFaceEntity(long DEno, long PEno, DirectoryEntry &de);
	void AddDirectionEntity(long DEno, long PEno, DirectoryEntry &de);
	void AddNullElement (long DEno , long PEno , DirectoryEntry &de);
	void GetAllRelatedEntity (long parentEnt , LongVec &relEnt);
	int GetPointerOffset()const;
	//void SetPointerOffset(int poffs);
	int InsertEntity (IgesEntity *newent);
	int InsertTM (const TransformationMatrix &tm);
	void Transform (const TransformationMatrix &tm , int de);
	void MakeAllSubsetIndex0 ()const;
	int GetParameterLineNo()const;
	void SetParameterLineNo (int newno);
	void AddBack (IgesEntitySet &toAdd);
	int InsertColor (const ColorDefinition &toAdd);
	void SetDEParamLineBeginAndCount (IgesEntity &e);
	std::map<int,int> & GetCurveEdgeDePairsMap ();
	void OverrideBigZCoordinates (const std::vector<int> &ivec , double zLimit , double zWork);
private:
	std::vector <IgesEntity *> entityset;
	std::vector<bool> ispartoftrimmedsurface;
	void AddSet ( IgesEntity * newent );
	void MakePartOfTrimmedSurface ( int index );
	void TakeOutOfTrimmedSurface ( int index );
	void Transform (IgesEntity *ent , const TransformationMatrix &tm);
	int SearchTM (const igesTransformationMatrix &im);
	int SearchColor (const ColorDefinition &col);
	int m_parameterlineno;
	static VStr TMPFILEPATH;
	IgesFileWrite *m_tmpWrt;
	//int m_maxde;
	//int m_poffs;
	std::map<int,int> m_curveEdgeDePairsMap;
	GeneralNote * m_latestVTransformationGroupNote;
public:
	void SetLatestVTransformationGroupNote(GeneralNote *note);
	GeneralNote * GetLatestVTransformationGroupNote();
};

#endif // IgesEntitySet
