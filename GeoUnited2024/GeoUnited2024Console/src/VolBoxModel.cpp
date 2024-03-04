// VolBoxModel.cpp

#include "stdafx.h"
#include "VolBoxModel.h"
#include "ViewPointItemTypes.h"

VolBoxModel::VolBoxModel (const VStr &type,const VStr &name
			,const vreal &myx
			,const vreal &myy
			,const vreal &myz)
			:Model(type,name)
{
	m_x=myx;
	m_y=myy;
	m_z=myz;
}

VolBoxModel::VolBoxModel (const VolBoxModel &toCopy)
:Model(toCopy)
{
	m_x=toCopy.m_x;
	m_y=toCopy.m_y;
	m_z=toCopy.m_z;
}

VolBoxModel & VolBoxModel::operator = (const VolBoxModel &toCopy)
{
	Model::operator =(toCopy);
	m_x=toCopy.m_x;
	m_y=toCopy.m_y;
	m_z=toCopy.m_z;
	return (*this);
}

VolBoxModel::~VolBoxModel()
{

}

void VolBoxModel::Serialize(Bofstream &outfile)const
{
	unsigned int version=1;
	outfile.write((VChr*)&version,sizeof(unsigned int));
	if(version==1)
	{
		outfile.write((VChr*)&m_x,sizeof(vreal));
		outfile.write((VChr*)&m_y,sizeof(vreal));
		outfile.write((VChr*)&m_z,sizeof(vreal));
	}
	else
	{
		throw VException(( IDS_VolSerExc),1);
	}
}

void VolBoxModel::Serialize(Bifstream &infile)
{
	unsigned int version;
	infile.read((VChr*)&version,sizeof(unsigned int));
	if(version==1)
	{
		infile.read((VChr*)&m_x,sizeof(vreal));
		infile.read((VChr*)&m_y,sizeof(vreal));
		infile.read((VChr*)&m_z,sizeof(vreal));
	}
	else
	{
		throw VException(( IDS_VolSerExc),2);
	}
}
