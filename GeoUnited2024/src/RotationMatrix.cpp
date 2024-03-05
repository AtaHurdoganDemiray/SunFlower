// RotationMatrix.cpp

#include "stdafx.h"
#include "RotationMatrix.h"
#include "MoveMatrix.h"


RotationMatrix::RotationMatrix()
:m_rotationaxis(UX),m_rotationcenter(0,0,0)
{
	m_theta=0.0;
}
RotationMatrix::RotationMatrix (const VPoint &rotationcenter,const UnitVector &rotationaxis , const vreal &theta)
:m_rotationcenter(rotationcenter),m_rotationaxis(rotationaxis),m_theta(theta)
{
	vreal radianTheta=m_theta*DEGREE_TO_RAD;
	vreal ctheta=cos(radianTheta);
	vreal stheta=sin(radianTheta);
	
	TransformationMatrix RM;
	
	if(m_rotationaxis==UX)
	{
		RM.SetRow(1 , 1.0 , 0.0 , 0.0 , 0.0);	
		RM.SetRow(2 , 0.0 ,ctheta,-stheta,0.0);
		RM.SetRow(3 , 0.0 ,stheta,ctheta,0.0);
		RM.SetRow(4 , 0.0 , 0.0 ,0.0 , 1.0);
	}
	else if(m_rotationaxis==UY)
	{
		RM.SetRow(1 , ctheta , 0.0 , stheta , 0.0);	
		RM.SetRow(2 , 0.0 , 1.0 , 0.0 , 0.0);
		RM.SetRow(3 , -stheta , 0.0 , ctheta , 0.0);
		RM.SetRow(4 , 0.0 , 0.0 ,0.0 , 1.0);
	}
	else if(m_rotationaxis==UZ)
	{
		RM.SetRow(1 , ctheta , -stheta , 0.0 , 0.0);	
		RM.SetRow(2 , stheta , ctheta , 0.0 , 0.0);
		RM.SetRow(3 , 0.0 , 0.0 , 1.0 , 0.0);
		RM.SetRow(4 , 0.0 , 0.0 , 0.0 , 1.0);
	}
	else
	{
		vreal f1=stheta;
		vreal f2=1.0-ctheta;
		vreal ux=m_rotationaxis.x();
		vreal uy=m_rotationaxis.y();
		vreal uz=m_rotationaxis.z();
		TransformationMatrix I;
		TransformationMatrix S;
		S.SetRow(1 ,0.0,-uz,uy, 0.0);
		S.SetRow(2 ,uz,0.0,-ux, 0.0);
		S.SetRow(3 ,-uy,ux,0.0, 0.0);
		S.SetRow(4 ,0.0,0.0,0.0 , 1.0);

		TransformationMatrix S2=S*S;
		RM = I + f1*S + f2*S2;
		RM.SetElement(1.,4,4);
	}
	MoveMatrix M1(-m_rotationcenter);
	MoveMatrix M2(m_rotationcenter);
	TransformationMatrix TM=M2*RM*M1;
	TransformationMatrix::operator =(TM);
}
RotationMatrix::RotationMatrix(const RotationMatrix &toCopy)
:TransformationMatrix(toCopy)
{
	m_rotationcenter=toCopy.m_rotationcenter;
	m_rotationaxis=toCopy.m_rotationaxis;
	m_theta=toCopy.m_theta;
}
RotationMatrix & RotationMatrix::operator=(const RotationMatrix &toCopy)
{
	m_rotationcenter=toCopy.m_rotationcenter;
	m_rotationaxis=toCopy.m_rotationaxis;
	m_theta=toCopy.m_theta;
	return *this;
}
RotationMatrix::~RotationMatrix()
{

}
void RotationMatrix::Serialize (Bofstream &outfile)const
{
	TransformationMatrix::Serialize(outfile);
	m_rotationcenter.Serialize(outfile);
	m_rotationaxis.Serialize(outfile);
	outfile.write((VChr*)&m_theta,sizeof(vreal));
}
void RotationMatrix::Serialize (Bifstream &infile)
{
	TransformationMatrix::Serialize(infile);
	m_rotationcenter.Serialize(infile);
	m_rotationaxis.Serialize(infile);
	infile.read((VChr*)&m_theta,sizeof(vreal));
}
bool RotationMatrix::operator == (const RotationMatrix &toCompare)
{
	if(this==&toCompare)
		return true;
	if(m_theta!=toCompare.m_theta)
		return false;
	if(m_rotationaxis!=toCompare.m_rotationaxis)
		return false;
	if(TransformationMatrix::operator !=(toCompare))
		return false;

	return true;
}
bool RotationMatrix::operator != (const RotationMatrix &toCompare)
{
	return !(operator==(toCompare));
}
VStr RotationMatrix::GetInformationText()const
{
	VStr info;
	CString tmp1;
	tmp1.Format(" X%.4f Y%.4f Z%.4f ", m_rotationcenter.x(), m_rotationcenter.y(), m_rotationcenter.z());
	CString tmp2;
	tmp1.Format(" X%.4f Y%.4f Z%.4f ", m_rotationaxis.x(), m_rotationaxis.y(), m_rotationaxis.z());
	CString tmp3;
	tmp3.Format(" %.4f",m_theta);
	info += sozluk("Rotate");
	info += "(";
	info += sozluk("Center");
	info += (LPCTSTR)tmp1;
	info += sozluk("Axis");
	info += (LPCTSTR)tmp2;
	info += sozluk("Angle");
	info += (LPCSTR)tmp3;
	info += ")";
	return info;
}