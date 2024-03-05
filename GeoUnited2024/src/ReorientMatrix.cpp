// ReorientMatrix.cpp

#include "stdafx.h"
#include "ReorientMatrix.h"
#include "CoordinateSystem.h"
#include "GaussJordanElimination.h"
#include "MoveMatrix.h"
#include "RotationMatrix.h"


ReorientMatrix::ReorientMatrix()
:TransformationMatrix(),base(),destiny()
{

}
ReorientMatrix::ReorientMatrix (const ReorientMatrix &toCopy)
:TransformationMatrix(toCopy),base(toCopy.base),destiny(toCopy.destiny)
{

}
ReorientMatrix::~ReorientMatrix()
{

}
ReorientMatrix::ReorientMatrix ( const CoordinateSystem &_base , const CoordinateSystem &_destiny )
: base( _base ) , destiny ( _destiny )
{
	//const VPoint &baseOrigin=base.GetOrigin();
	//VPoint baseEndx=baseOrigin+base.GetDirx();
	//VPoint baseEndy=baseOrigin+base.GetDiry();
	//VPoint baseEndz=baseOrigin+base.GetDirz();
	//
	//const VPoint &destinyOrigin=destiny.GetOrigin();
	//VPoint destinyEndx=destinyOrigin+destiny.GetDirx();
	//VPoint destinyEndy=destinyOrigin+destiny.GetDiry();
	//VPoint destinyEndz=destinyOrigin+destiny.GetDirz();	
	//
	//TransformationMatrix BASE;
	//BASE.SetCol(1,baseOrigin.x(),baseOrigin.y(),baseOrigin.z(),0.);
	//BASE.SetCol(2,baseEndx.x(),baseEndx.y(),baseEndx.z(),0.);
	//BASE.SetCol(3,baseEndy.x(),baseEndy.y(),baseEndy.z(),0.);
	//BASE.SetCol(4,0.,0.,0.,1.);

	//TransformationMatrix DESTINY;
	//DESTINY.SetCol(1,destinyOrigin.x(),destinyOrigin.y(),destinyOrigin.z(),0.);
	//DESTINY.SetCol(2,destinyEndx.x(),destinyEndx.y(),destinyEndx.z(),0.);
	//DESTINY.SetCol(3,destinyEndy.x(),destinyEndy.y(),destinyEndy.z(),0.);
	//DESTINY.SetCol(4,0.,0.,0.,1.);

	////T x BASE = DESTINY
	////T x BASE x BASEINVERSE = DESTINY x BASEINVERSE

	//TransformationMatrix BASEINVERSE = BASE.GetInverse();
	//TransformationMatrix result = DESTINY*BASEINVERSE;
	//
	//TransformationMatrix::operator =(result);

	TransformationMatrix M1,M2,M3;//by default all are Identity
	//first moving base to destiny origin
	MoveMatrix moveM(destiny.GetOrigin()-base.GetOrigin());
	M1=moveM;
	base.Transform(M1);
	//aligning z axis
	UnitVector aroundVec1;
	vreal theta1;
	GetAlignTwoDirectionsParam(base.GetDirz(),destiny.GetDirz(),aroundVec1,theta1);

	RotationMatrix rM2(destiny.GetOrigin(),aroundVec1,theta1);
	M2=rM2;
	base.Transform(M2);//z axis are aligned

	//aligning y axis
	UnitVector aroundVec2 = destiny.GetDirz();
	vreal theta2;
	bool ck = GetAlignTwoDirectionsAround(base.GetDirx(),destiny.GetDirx(),aroundVec2,theta2);
	if (ck == false)
		throw VException (( IDS_ReoConExcAroVec),1);
	RotationMatrix rM3(destiny.GetOrigin(),aroundVec2,theta2);	
	M3=rM3;
	base.Transform(M3);	

//#if _DEBUG
	if (base != destiny)
		LogFileWriter::WriteUnknownExceptionLog("ReorientMatrix::Constructor exception.","base!=destiny");
		//		throw VException (("ReorientMatrix exception"),1);
//#endif
	*this *= M1;
	*this *= M2;
	*this *= M3;
	//TransformationMatrix::operator= (M3 * M2 * M1);
}	

//#############################################################################

ReorientMatrix &ReorientMatrix::operator =(const ReorientMatrix &toCopy)
{
	TransformationMatrix::operator=(toCopy);
	base=toCopy.base;
	destiny=toCopy.destiny;
	return(*this);
}

void ReorientMatrix::Serialize(Bofstream &outfile)const
{
	TransformationMatrix::Serialize(outfile);
	base.Serialize(outfile);
	destiny.Serialize(outfile);
}

void ReorientMatrix::Serialize(Bifstream &infile)
{
	TransformationMatrix::Serialize(infile);
	base.Serialize(infile);
	destiny.Serialize(infile);
}
