// RefTriangleMesh.cpp
#include "stdafx.h"
#include "RefTriangleMesh.h"



RefTriangleMesh::RefTriangleMesh ( const int &initialsize )
{
	mdata.reserve( initialsize );		
}

//#############################################################################

RefTriangleMesh::~RefTriangleMesh()
{
}

//#############################################################################

long RefTriangleMesh::Size() const
{
	//return mdata.Size();
	return (long)(mdata.size());
}
	
//#############################################################################

const RefTriangle * RefTriangleMesh::operator [] ( const long &index ) const
{
	if ( index < 0 || index >= Size() )
		throw ( VException ( ( IDS_RefInvInd) , 0 ) );

	//return (mdata[index]);
	return &(mdata[index]);
}

//#############################################################################

RefTriangle * RefTriangleMesh::operator [] ( const long &index ) 
{
	if ( index < 0 || index >= Size() )
		throw ( VException ( ( IDS_RefInvInd) , 0 ) );

	//return (mdata[index]);
	return &(mdata[index]);
}

//#############################################################################

void RefTriangleMesh::AddTriangle ( const RefTriangle &source )
{
	//mdata.AddItem ( mypt );
	mdata.push_back(source);
}

//#############################################################################

RefTriangleMesh::RefTriangleMesh ( const RefTriangleMesh & Foo )
:mdata(Foo.mdata)
{

}
//#############################################################################

bool RefTriangleMesh::operator == (const RefTriangleMesh &toCompare)const
{
	if(this==&toCompare)
		return true;
	
	if(mdata!=toCompare.mdata)
		return false;

	return true;
}

bool RefTriangleMesh::operator != (const RefTriangleMesh &toCompare)const
{
	return !(this->operator ==(toCompare));
}

void RefTriangleMesh::Serialize (Bofstream &outfile)const
{
	int version = 1;
	outfile.write ((VChr*)&version,sizeof(int));
	if (version == 1)
	{
		int mysize = Size();
		outfile.WriteInt (mysize);
		for (int i=0;i<mysize;i++)
		{
			const RefTriangle * currtri = operator [] (i);
			currtri->Serialize (outfile);
		}
	}
	else
	{
		throw VException (( IDS_RefSerExc),1);
	}
}

void RefTriangleMesh::Serialize (Bifstream &infile)
{
	int version;
	infile.read ((VChr*)&version , sizeof(int));
	if (version ==1)
	{
		int mysize;
		infile.ReadInt (mysize);
		for (int i=0;i<mysize;i++)
		{
			RefTriangle currtri;
			currtri.Serialize (infile);
			this->AddTriangle (currtri);
		}
	}
	else
	{
		throw VException (( IDS_RefSerExc),2);
	}
}

void RefTriangleMesh::Clear()
{
	mdata.clear();
}
