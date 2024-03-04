// IgesEntitySet.cpp

#include "stdafx.h"
#include "EntityPointer.h"
#include "BSplineCurve.h"
#include "BSplineSurface.h"
#include "CompositeCurve.h"
#include "CurveOnParametricSurface.h"
#include "TrimmedSurface.h"
#include "IgesEntitySet.h"
#include "Boundary.h"
#include "BoundedSurface.h"
#include "Line.h"
#include "Point.h"
#include "TabulatedCylinder.h"
#include "SurfaceOfRevolution.h"

#include "Arc.h"
#include "IGESTransformationMatrix.h"
#include "OffsetSurface.h"
#include "ColorDefinition.h"
#include "CopiousData.h"
#include "LongVec.h"
#include "ConicArc.h"
#include "ParametricSpline.h"
#include "PlaneEntity.h"
#include "RuledSurface.h"
#include "TransformationMatrix.h"
#include "GeneralNote.h"
#include "NullEntity.h"
#include "IgesFileWrite.h"
#include "ParametricSplineSurface.h"
#include "Vertex.h"
#include "Edge.h"
#include "SubFigureDefinition.h"
#include "SingularSubFigureInstance.h"
#include "PlaneSurface.h"
#include "RightCircularCylindricalSurface.h"
#include "RightCircularConicalSurface.h"
#include "SphericalSurface.h"
#include "ToroidalSurface.h"
#include "Loop.h"
#include "Face.h"
#include "DirectionEntity.h"

IgesEntitySet::IgesEntitySet()
//:m_maxde(-1)
{
	m_parameterlineno=1;
	m_tmpWrt = new IgesFileWrite;
	m_latestVTransformationGroupNote = NULL;
}

//#############################################################################

IgesEntitySet::~IgesEntitySet()
{
	DestroySet();
	delete m_tmpWrt;
}

//#############################################################################

long IgesEntitySet::Size() const
{
	return (long)entityset.size();
}

//#############################################################################

IgesEntity * IgesEntitySet::operator [] ( long index ) const
{
	if ( index < 0 || index >= Size() )
		return NULL; // exception.
	
	return entityset[index];

}

//#############################################################################

void IgesEntitySet::AddLine ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new Line ();
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	
	AddSet ( newent );

}

//#############################################################################

void IgesEntitySet::AddArc ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new igesArc ();
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	
	AddSet ( newent );

}

//#############################################################################

void IgesEntitySet::AddPoint ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new IgesPoint ();
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	
	AddSet ( newent );

}

//#############################################################################

void IgesEntitySet::AddBSplineCurve ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new BSplineCurve ();

	newent->entitytype = 126;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	
	AddSet ( newent );

}

//#############################################################################

void IgesEntitySet::AddBSplineSurface ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new BSplineSurface ();

	newent->entitytype = 128;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	
	AddSet ( newent );

}

//#############################################################################

void IgesEntitySet::AddTabulatedCylinder ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new TabulatedCylinder;
	
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	
	AddSet ( newent );

}

//#############################################################################

void IgesEntitySet::AddSurfaceOfRevolution ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new SurfaceOfRevolution;
	
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	
	AddSet ( newent );

}

//#############################################################################

void IgesEntitySet::AddCompositeCurve ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new CompositeCurve ();

	newent->entitytype = 102;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	
	AddSet ( newent );
	
}

//#############################################################################

void IgesEntitySet::AddCurveOnParametricSurface ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new CurveOnParametricSurface ();

	newent->entitytype = 142;
	newent->directorylineno = DEno;
	newent->DE = de;
	newent->DE.m_parameterBegin = PEno;

	AddSet ( newent );
	
}

//#############################################################################

void IgesEntitySet::AddTrimmedSurface ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new TrimmedSurface ();

	newent->entitytype = 144;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	
	AddSet ( newent );

}

//#############################################################################

void IgesEntitySet::AddBoundedSurface ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new BoundedSurface ();

	newent->entitytype = 143;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	
	AddSet ( newent );
	
}

//#############################################################################

void IgesEntitySet::AddBoundary ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new Boundary ();

	newent->entitytype = 141;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	
	AddSet ( newent );
	
}

//#############################################################################

void IgesEntitySet::AddCopiousData ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new CopiousData();

	newent->entitytype = 106;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	
	AddSet ( newent );
}
//#############################################################################

void IgesEntitySet::AddConicArc ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new ConicArc();
	newent->entitytype = 104;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet ( newent );
}
//#############################################################################
void IgesEntitySet::AddParametricSpline (long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new ParametricSpline();
	newent->entitytype = 112;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet ( newent );
}
//#############################################################################
void IgesEntitySet::AddParametricSplineSurface (long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new ParametricSplineSurface();
	newent->entitytype = 114;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet ( newent );
}
//#############################################################################
void IgesEntitySet::AddPlaneEntity ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new PlaneEntity();
	newent->entitytype = 108;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet ( newent );
}

//#############################################################################

void IgesEntitySet::AddVertexEntity (long DEno , long PEno , DirectoryEntry &de)
{
	IgesEntity * newent = new Vertex ();
	newent->entitytype = 502;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet(newent);
}

//#############################################################################

void IgesEntitySet::AddEdgeEntity (long DEno , long PEno , DirectoryEntry &de)
{
	IgesEntity * newent = new Edge ();
	newent->entitytype = 504;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet(newent);
}

//#############################################################################

void IgesEntitySet::AddSubFigureDefinition (long DEno , long PEno , DirectoryEntry &de)
{
	IgesEntity * newent = new SubFigureDefinition ();
	newent->entitytype = 308;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet(newent);
}

//#############################################################################

void IgesEntitySet::AddSingularSubFigureInstance (long DEno , long PEno , DirectoryEntry &de)
{
	IgesEntity * newent = new SingularSubFigureInstance ();
	newent->entitytype = 408;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet(newent);
}

//#############################################################################

void IgesEntitySet::AddNullElement (long DEno , long PEno , DirectoryEntry &de)
{
	IgesEntity *newent = new NullEntity();
	newent->entitytype = 0;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet ( newent );
}


void IgesEntitySet::DestroySet()
{
	for ( unsigned int i=0; i < entityset.size(); i++ )
		delete entityset[i];

	entityset.clear();
}

//#############################################################################

IgesEntity * IgesEntitySet::GetEntityPointer ( long DEno ) const
{
	size_t i = (size_t) ((DEno-1)/2);
	if (i < 0 || i >= entityset.size())
		throw VException (( IDS_IgeGetExcIndInv),0);
	return entityset[i];
	
	//IgesEntity * temp;
	//for ( unsigned int i=0; i < entityset.size(); i++ )
	//{
	//	temp = entityset[i];
	//	if ( temp->directorylineno == DEno )
	//		return temp;
	//}
	//
	//return 0;
}

//#############################################################################

bool IgesEntitySet::IsSupportedSurfaceType ( const IgesEntity * pigs ) const
{
	if ( pigs->entitytype == 128 )
		return true;
	
	else if ( pigs->entitytype == 122 )
		return true;

	else if ( pigs->entitytype == 120 )
		return true;	
	
	else if ( pigs->entitytype == 140 )
		return true;

	else if (pigs->entitytype == 108)
		return true;
	
	else if (pigs->entitytype == 118)
		return true;
	else if (pigs->entitytype == 114)
		return true;
	else if (pigs->entitytype == 190)// plane surface entity
		return true;
	else if (pigs->entitytype == 192)// right circular cylindrical surface entity
		return true;
	else if (pigs->entitytype == 194)// right circular conical surface entity
		return true;
	else if (pigs->entitytype == 196)// spherical surface entity
		return true;
	else if (pigs->entitytype == 198)// toroidal surface entity
		return true;
	return false;
}

//#############################################################################

bool IgesEntitySet::IsSupportedCurveType ( const IgesEntity * pigs ) const
{
	if ( pigs->entitytype == 110 )// Line
		return true;
	else if ( pigs->entitytype == 126 )// BSplineCurve
		return true;
	else if ( pigs->entitytype == 100 )// Arc
		return true;
	else if ( pigs->entitytype == 106)// CopiousData
		return true;
	else if ( pigs->entitytype == 104)// Conic Arc
		return true;
	else if ( pigs->entitytype == 112)// ParametricSpline
		return true;
	else
		return false;
}

//#############################################################################

void IgesEntitySet::AddSet ( IgesEntity * newent )
{
	entityset.push_back ( newent );
	ispartoftrimmedsurface.push_back ( false );
	SetParameterLineNo (GetParameterLineNo()+newent->DE.m_parameterLineCount);
	//if (newent->directorylineno > m_maxde)
	//	m_maxde = newent->directorylineno;
}

//#############################################################################

bool IgesEntitySet::IsPartOfTrimmedSurface ( int index ) const
{
	if ( index < 0 || index >= (int)ispartoftrimmedsurface.size() )
		throw ( VException ( ( IDS_IgeEntSetExc) , 0 ));

	return (ispartoftrimmedsurface[index]);
}

//#############################################################################

int IgesEntitySet::GetIndex ( const IgesEntity * pigs )
{
	
	return GetIndex(pigs->directorylineno);
}

int IgesEntitySet::GetIndex(int deno)
{
	return   (int) ( (deno - 1) * 0.5) ;
}
//#############################################################################

void IgesEntitySet::MakePartOfTrimmedSurface ( const IgesEntity * pigs )
{
	int index = GetIndex ( pigs );
	
	if ( index == -1 )
		return;//throw ( VException ( ("make part of trimmed surface exception") , 0 ));

	MakePartOfTrimmedSurface ( index );
}

//#############################################################################

void IgesEntitySet::TakeOutOfTrimmedSurface ( const IgesEntity * pigs )
{
	int index = GetIndex ( pigs );
	
	if ( index == -1 )
		return;//throw ( VException ( ("make part of trimmed surface exception") , 0 ));

	TakeOutOfTrimmedSurface ( index );
}

//#############################################################################

void IgesEntitySet::MakePartOfTrimmedSurface ( int index )
{
	ispartoftrimmedsurface.at ( index ) = true;	
}

//#############################################################################

void IgesEntitySet::TakeOutOfTrimmedSurface ( int index )
{
	ispartoftrimmedsurface.at ( index ) = false;
}

//#############################################################################

void IgesEntitySet::AddTransformationMatrix ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new igesTransformationMatrix();

	newent->entitytype = 124;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet ( newent );
	
}
//#############################################################################

void IgesEntitySet::AddOffsetSurface ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new OffsetSurface();

	newent->entitytype = 140;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet ( newent );	
}

//#############################################################################

void IgesEntitySet::AddRuledSurface ( long DEno , long PEno , DirectoryEntry & de)
{
	RuledSurface::Type mytype = RuledSurface::EQUAL_ARC_LENGTH;
	if (de.m_formNo == 1)
		mytype = RuledSurface::EQUAL_PAR_LENGTH;
	IgesEntity * newent = new RuledSurface(mytype);

	newent->entitytype = 118;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet ( newent );	
}

//#############################################################################

void IgesEntitySet::AddColorDefinition ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new ColorDefinition();

	newent->entitytype = 314;
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet ( newent );	
}

//#############################################################################
void IgesEntitySet::AddGeneralNote ( long DEno , long PEno , DirectoryEntry & de)
{
	IgesEntity * newent = new GeneralNote();
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet ( newent );	
}

//#############################################################################

void IgesEntitySet::AddEntity (IgesEntity *newent , long DEno , long PEno , DirectoryEntry & de)
{
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet (newent);
}

//#############################################################################

void IgesEntitySet::GetAllRelatedEntity (long parentEnt , LongVec &relEnt)
{
	std::vector<int> tmp;
	tmp.push_back (parentEnt);
	while (tmp.size())
	{
		int currde = tmp.back();
		if (currde)
		{
			relEnt.AddLong (currde);
			tmp.pop_back ();
			IgesEntity * current = GetEntityPointer (currde);
			if (current)
				current->GetChildren(tmp);
		}
		else
			tmp.pop_back();
	}
	relEnt.RemoveDuplicate();
}

int IgesEntitySet::GetPointerOffset ()const
{
	int mysize = Size();
	if (mysize == 0)
		return 0;
	IgesEntity *laste = entityset[mysize-1];
	return laste->directorylineno + 1;
	
	//
	//int mxval = -1;
	//for (int i = mysize-1; i>=0; i--)
	//{
	//	IgesEntity *curre = entityset[i];
	//	if (curre->directorylineno > mxval)
	//		mxval = curre->directorylineno;
	//}
	//return mxval;
	//return m_maxde;
	//return Size()*2;
}

//void IgesEntitySet::SetPointerOffset(int poffs)
//{
//	m_poffs = poffs;
//}

int IgesEntitySet::InsertEntity(IgesEntity *newent)
{
	//m_maxde += 2;
	int newdeno = Size()*2+1;
	newent->directorylineno = newdeno;//m_maxde;
	AddSet (newent);
	return newdeno;//m_maxde;
}

void IgesEntitySet::Transform(const TransformationMatrix &tm, int de)
{
	IgesEntity *toTransform = GetEntityPointer (de);
	if (toTransform)
	{
		if (IsSupportedCurveType(toTransform) || IsSupportedSurfaceType (toTransform) 
			|| toTransform->entitytype == 116
			|| toTransform->entitytype == 144
			|| toTransform->entitytype == 143
			|| toTransform->entitytype == 212)
		{
			Transform (toTransform , tm);
		}
	}
}

void IgesEntitySet::Transform (IgesEntity *ent , const TransformationMatrix &tm)
{
	if (ent->DE.pXform)
	{
		IgesEntity *mtr = GetEntityPointer (ent->DE.pXform);
		igesTransformationMatrix *currm = (igesTransformationMatrix *)(mtr);
		TransformationMatrix currtm (*currm);
		TransformationMatrix newtm = currtm * tm;
		ent->DE.pXform = InsertTM(newtm);
	}
	else
	{
		ent->DE.pXform = InsertTM (tm);
	}
}

int IgesEntitySet::InsertTM (const TransformationMatrix &tm)
{
	igesTransformationMatrix im = tm.GetIgesTransformation();
	int ck = SearchTM (im);
	if (ck!=0)
		return ck;
	else
	{
		igesTransformationMatrix *newIgesMtr = new igesTransformationMatrix;
		tm.SetIgesTransformation(*newIgesMtr);
		SetDEParamLineBeginAndCount (*newIgesMtr);
		int de = InsertEntity (newIgesMtr);
		return de;
	}
}

int IgesEntitySet::SearchTM (const igesTransformationMatrix &im)
{
	int mysize = Size();
	for (int i=0;i<mysize;i++)
	{
		IgesEntity *curre=operator[](i);
		if (curre->entitytype == 124)
		{
			//this is transformation matrix
			igesTransformationMatrix *currM = (igesTransformationMatrix *)curre;
			if (currM->IsEqualavent(im))
				return currM->directorylineno;
		}
	}
	return 0;
}

void IgesEntitySet::MakeAllSubsetIndex0 ()const
{
	int mysize = Size();
	for(int i=0;i<mysize;i++)
	{
		IgesEntity *curre = entityset.operator[](i);
		curre->m_subsetIndex = 0;
	}
}

int IgesEntitySet::GetParameterLineNo()const
{
	return m_parameterlineno;
}

void IgesEntitySet::SetParameterLineNo (int newno)
{
	m_parameterlineno = newno;
}

void IgesEntitySet::AddBack (IgesEntitySet &toAdd)
{
	toAdd.MakeAllSubsetIndex0();
	int mysize = toAdd.Size();
	for (int i=0;i<mysize;++i)
	{
		IgesEntity *curre = toAdd[i];
		curre->MakeCopyAndAddToSet (toAdd,*this);
	}
}

int IgesEntitySet::InsertColor (const ColorDefinition &toAdd)
{
	int ck = SearchColor (toAdd);
	if (ck)
		return -1*ck;
	else
	{
		ColorDefinition *newcol = new ColorDefinition (toAdd);
		return -1*InsertEntity (newcol);
	}
}

int IgesEntitySet::SearchColor(const ColorDefinition &col)
{
	int mysize = Size();
	for (int i=0;i<mysize;i++)
	{
		IgesEntity *curre=operator[](i);
		if (curre->entitytype == 314)
		{
			//this is color definition
			ColorDefinition *currC = (ColorDefinition *)curre;
			if (currC->IsEqualavent(col))
				return currC->directorylineno;
		}
	}
	return 0;	
}
VStr IgesEntitySet::TMPFILEPATH = ( IDS_VPTEMPIGES);
void IgesEntitySet::SetDEParamLineBeginAndCount(IgesEntity &e)
{
	e.DE.m_parameterBegin = GetParameterLineNo();
	VofStream tmpout(TMPFILEPATH.c_str());
	m_tmpWrt->WriteParameterData (e,tmpout);
	e.DE.m_parameterLineCount = m_tmpWrt->GetParameterLineNo()-1;
	m_tmpWrt->Clear();
	tmpout.close();

    DeleteFile (TMPFILEPATH.c_str());

   //       !!! c_str() LPCWSTR

}

std::map<int,int> & IgesEntitySet::GetCurveEdgeDePairsMap ()
{
	return m_curveEdgeDePairsMap;
}

void IgesEntitySet::OverrideBigZCoordinates (const std::vector<int> &ivec , double zLimit , double zWork)
{
	size_t mysize = ivec.size();
	for (size_t i=0;i<mysize;++i)
	{
		int curi = ivec.at(i);
		IgesEntity *cure = this->operator[](curi);
		cure->OverrideBigZCoordinates (zLimit , zWork);
	}
}

void IgesEntitySet::AddPlaneSurfaceEntity(long DEno, long PEno, DirectoryEntry &de)
{
	IgesEntity * newent = new PlaneSurface();
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet(newent);
}
void IgesEntitySet::AddRightCircularCylindricalSurfaceEntity(long DEno, long PEno, DirectoryEntry &de)
{
	IgesEntity * newent = new RightCircularCylindricalSurface();
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet(newent);
}
void IgesEntitySet::AddRightCircularConicalSurfaceEntity(long DEno, long PEno, DirectoryEntry &de)
{
	IgesEntity * newent = new RightCircularConicalSurface();
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet(newent);
}
void IgesEntitySet::AddSphericalSurfaceEntity(long DEno, long PEno, DirectoryEntry &de)
{
	IgesEntity * newent = new SphericalSurface();
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet(newent);
}
void IgesEntitySet::AddToroidalSurfaceEntity(long DEno, long PEno, DirectoryEntry &de)
{
	IgesEntity * newent = new ToroidalSurface();
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet(newent);
}
void IgesEntitySet::AddLoopEntity(long DENo, long PEno, DirectoryEntry &de)
{
	IgesEntity * newent = new Loop();
	newent->directorylineno = DENo;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet(newent);
}
void IgesEntitySet::AddFaceEntity(long DENo, long PEno, DirectoryEntry &de)
{
	IgesEntity * newent = new Face();
	newent->directorylineno = DENo;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet(newent);
}
void IgesEntitySet::AddDirectionEntity(long DEno, long PEno, DirectoryEntry &de)
{
	IgesEntity * newent = new DirectionEntity();
	newent->directorylineno = DEno;
	newent->DE.m_parameterBegin = PEno;
	newent->DE = de;
	AddSet(newent);
}

void IgesEntitySet::SetLatestVTransformationGroupNote(GeneralNote *note)
{
	m_latestVTransformationGroupNote = note;
}

GeneralNote * IgesEntitySet::GetLatestVTransformationGroupNote()
{
	return m_latestVTransformationGroupNote;
}