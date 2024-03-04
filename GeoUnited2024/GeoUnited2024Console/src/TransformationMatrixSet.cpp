// TransformationMatrixSet.cpp

#include "stdafx.h"
#include "TransformationMatrixSet.h"
#include "TransformationMatrix.h"


TransformationMatrixSet::TransformationMatrixSet ()
{
	mtrx.reserve(4);
}

TransformationMatrixSet::TransformationMatrixSet ( int initialcapacity )
{
	mtrx.reserve(initialcapacity);
}

TransformationMatrixSet::~TransformationMatrixSet ()
{
}

long TransformationMatrixSet::Size () const
{
	return (long)(mtrx.size());
}

TransformationMatrix * TransformationMatrixSet::operator [] ( int index )
{
	return &(mtrx.at(index));
}

const TransformationMatrix * TransformationMatrixSet::operator [] ( int index ) const
{
	return &(mtrx.at(index));
}

void TransformationMatrixSet::AddMatrix ( const TransformationMatrix & mt , unsigned int newhandle , bool isactive )
{
	mtrx.push_back (mt);
	ismatrixActive.push_back(isactive);
	allhandles.push_back ( newhandle );
}

void TransformationMatrixSet::ActivateMatrix (int matrixno)
{
	ismatrixActive.at(matrixno) = true;
}

void TransformationMatrixSet::DeactivateMatrix (int matrixno)
{
	ismatrixActive.at(matrixno) = false;
}

TransformationMatrix TransformationMatrixSet::GetOverallMatrix () const 
{
	TransformationMatrix result;
	
	for (int i=0; i < Size(); i++)
	{	
		const TransformationMatrix * currm = (*this)[i];
		
		if ( ismatrixActive[i] )
			result = (*currm) * result;
	}
	
	return result;
}

bool TransformationMatrixSet::IsActive ( int matrixno )
{
	return ismatrixActive.at(matrixno);
}

unsigned int TransformationMatrixSet::GetHandle ( int matrixno )
{
	return allhandles.at(matrixno);
}

int TransformationMatrixSet::GetTransformNoByHandle ( int handle )
{
	for ( int i=0; i < Size(); i++ )
	{
		unsigned int currhandle = allhandles[i];

		if ( currhandle == handle )
			return i;
	}
	
	return -1;
}
void TransformationMatrixSet::Serialize (Bofstream &outfile)const
{
	int version=1;
	WriteVersion (outfile,version);
	if(version==1)
	{
		VectorSerializer::SerializeTVec(outfile,mtrx);
		VectorSerializer::SerializeBasicTypeVec(outfile,ismatrixActive);
		VectorSerializer::SerializeBasicTypeVec(outfile,allhandles);
	}
}
void TransformationMatrixSet::Serialize ( Bifstream &infile )
{
	int version;
	ReadVersion(infile,version);
	if(version==1)
	{
		VectorSerializer::SerializeTVec(infile,mtrx);
		VectorSerializer::SerializeBasicTypeVec(infile,ismatrixActive);
		VectorSerializer::SerializeBasicTypeVec(infile,allhandles);
	}
}
void TransformationMatrixSet::SerializeOldVersion(Bofstream &outfile )const
{
	int mycount = (int)(mtrx.size());
	outfile.write ((VChr*)(&mycount) , sizeof(int) );
	
	for ( int i=0; i<Size(); i++ )
	{
		const TransformationMatrix *currMat=(*this)[i];
		currMat->Serialize(outfile);
		bool currisactive = ismatrixActive[i];
		outfile.write((VChr*)&currisactive,sizeof(bool));
	}
}
void TransformationMatrixSet::SerializeOldVersion(Bifstream &infile)
{
	int mycount;
	infile.read ( (VChr*)(&mycount) , sizeof (int) );
	
	for ( int i=0;i<mycount; i++ )
	{
		TransformationMatrix tmp;
		tmp.Serialize(infile);
		mtrx.push_back (tmp);
		bool currisactive;
		infile.read((VChr*)&currisactive,sizeof(bool));
		ismatrixActive.push_back(currisactive);
		allhandles.push_back(0);
	}
}
