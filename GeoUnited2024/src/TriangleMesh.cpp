/******************************************************************************
*               File: TriangleMesh.cpp										  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:23:08 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                     *
******************************************************************************/

#include "stdafx.h"
#include "BasicDefinitions.h"
#include "TriangleMesh.h"
#include "DoubleList.h"
#include "VLine.h"
#include "TransformationMatrix.h"
#include "VectorSerializer.h"
#include "VPlane.h"

#include "UntypedException.h"
#include "ExceptionLogger.h"
#include <stdexcept>
TriangleMesh::TriangleMesh ()
:PrimitiveGeoVec<VTriangleE,3> ()
{
	//tri.reserve (DEFAULT_INITIAL_SIZE);
	//m_min=GETMAXPOINT();
	//m_max=GETMINPOINT();
}

//#############################################################################

TriangleMesh::TriangleMesh ( const long &initialcapacity )
:PrimitiveGeoVec<VTriangleE,3> (initialcapacity)
{
	//tri.reserve (initialcapacity);
	//m_min=GETMAXPOINT();
	//m_max=GETMINPOINT();
}

//#############################################################################

bool TriangleMesh::GetPointAtPos (vreal mypoint[] , std::list<float>::iterator &mypos , std::list<float> &mylist)
{
	if (mypos == mylist.end())
		return false;
	mypoint[0] = *mypos;
	++mypos;
	if (mypos == mylist.end())
		return false;
	mypoint[1] = *mypos;
	++mypos;
	if (mypos == mylist.end())
		return false;
	mypoint[2] = *mypos;
	++mypos;
	return true;
}


TriangleMesh:: TriangleMesh 
( const long &initialcapacity , std::list<float> &mylist) //DoubleList & mylist )
:PrimitiveGeoVec<VTriangleE,3> (initialcapacity)
{
	//tri.reserve(initialcapacity);
	//m_min=GETMAXPOINT();
	//m_max=GETMINPOINT();
	//position pos = mylist.GetHeadPosition();
	try
	{
		std::list<float>::iterator pos = mylist.begin();
		VPoint cor1 , cor2 , cor3;
		vreal mypoint[3];

 		while ( pos != mylist.end())//mylist.End() )
		{
			//bool chk = mylist.GetPointAtPos ( mypoint , pos );
			bool chk = GetPointAtPos(mypoint,pos,mylist);
			if (chk == false)
				throw VException (( IDS_TriConExcLisIsN),1);
			
			//chk = mylist.GetPointAtPos ( mypoint , pos );
			chk = GetPointAtPos(mypoint , pos , mylist);
			if ( !chk )
				throw VException (( IDS_TriConExcLisIsN),1);
			
			cor1.x( mypoint[0] );
			cor1.y( mypoint[1] );
			cor1.z( mypoint[2] );
			
			//chk = mylist.GetPointAtPos ( mypoint , pos );
			chk = GetPointAtPos(mypoint , pos , mylist);
			if ( !chk )
				throw VException (( IDS_TriConExcLisIsN),1);

			cor2.x( mypoint[0] );
			cor2.y( mypoint[1] );
			cor2.z( mypoint[2] );

			//chk = mylist.GetPointAtPos ( mypoint , pos );
			chk = GetPointAtPos(mypoint , pos , mylist);
			if ( !chk )
				throw VException (( IDS_TriConExcLisIsN),1);
			
			cor3.x( mypoint[0] );
			cor3.y( mypoint[1] );
			cor3.z( mypoint[2] );

			AddTriangle ( cor1 , cor2 , cor3 );	
		}
	}
	catch (std::exception &exc)
	{	
		ExceptionLogger::log(exc, __FILE__, __LINE__);
	}
	catch (...)
	{
		UntypedException uexc(( IDS_UnkExcTriTri),999);
		ExceptionLogger::log(uexc, __FILE__, __LINE__);
	}
}

//#############################################################################

TriangleMesh:: TriangleMesh 
( long initialcapacity , std::list<float> &mylist , const VPlane &clipPlane) 
:PrimitiveGeoVec<VTriangleE,3> (initialcapacity)
{
	//tri.reserve(initialcapacity);
	//m_min=GETMAXPOINT();
	//m_max=GETMINPOINT();
	int counter = 0;
	//position pos = mylist.GetHeadPosition();
	try
	{
		std::list<float>::iterator pos = mylist.begin();
		VPoint cor1 , cor2 , cor3;
		vreal mypoint[3];

		while ( pos != mylist.end())//mylist.End() )
		{
			//bool chk = mylist.GetPointAtPos ( mypoint , pos );
			bool chk = GetPointAtPos(mypoint,pos,mylist);
			if (chk == false)
				throw VException (( IDS_TriConExcLisIsN),1);

			//chk = mylist.GetPointAtPos ( mypoint , pos );
			chk = GetPointAtPos(mypoint , pos , mylist);
			if ( !chk )
				throw VException (( IDS_TriConExcLisIsN),1);

			cor1.x( mypoint[0] );
			cor1.y( mypoint[1] );
			cor1.z( mypoint[2] );

			//chk = mylist.GetPointAtPos ( mypoint , pos );
			chk = GetPointAtPos(mypoint , pos , mylist);
			if ( !chk )
				throw VException (( IDS_TriConExcLisIsN),1);

			cor2.x( mypoint[0] );
			cor2.y( mypoint[1] );
			cor2.z( mypoint[2] );

			//chk = mylist.GetPointAtPos ( mypoint , pos );
			chk = GetPointAtPos(mypoint , pos , mylist);
			if ( !chk )
				throw VException (( IDS_TriConExcLisIsN),1);

			cor3.x( mypoint[0] );
			cor3.y( mypoint[1] );
			cor3.z( mypoint[2] );

			//AddTriangle ( cor1 , cor2 , cor3 );	
			VTriangleE mytri (cor1 , cor2 , cor3);
			std::vector<VTriangleE> remaining;
			clipPlane.ClipTriangle(mytri , remaining);
			size_t mysize2 = remaining.size();
			for (size_t w = 0;w<mysize2;++w)
			{
				AddTriangle(remaining.at(w));
			}
			//AddTriangle(mytri);
			++counter;
			if (counter == 10000)//m_vec.capacity())
				break;
		}
	}
	catch (std::exception &exc)
	{	
		ExceptionLogger::log(exc, __FILE__, __LINE__);	
	}
	catch (...)
	{
		UntypedException uexc(( IDS_UnkExcTriTri),999);
		ExceptionLogger::log(uexc, __FILE__, __LINE__);
	}
}

//#############################################################################

TriangleMesh::TriangleMesh ( const TriangleMesh & Foo )
:PrimitiveGeoVec<VTriangleE,3>(Foo)
//:tri ( Foo.tri )
{
	//m_min = Foo.m_min;
	//m_max = Foo.m_max;
}

//#############################################################################

void TriangleMesh::Serialize (Bifstream &infile)
{
	int currversion;
	infile.read((VChr*)&currversion,sizeof(int));
	
	if (currversion==2)
	{
		VPoint mymin (GetMin());
		VPoint mymax (GetMax());
		mymin.Serialize(infile);
		mymax.Serialize(infile);
		VectorSerializer::SerializeTVec(infile,m_vec);
		UpdateMinMax();
	}
	else if (currversion==1)
	{
		VPoint mymin (GetMin());
		VPoint mymax (GetMax());
		mymin.Serialize(infile);
		mymax.Serialize(infile);
		//tri.Serialize(infile);
		VectorSerializer::SerializeTVec001(infile,m_vec);
		UpdateMinMax();
	}
}

//#############################################################################

TriangleMesh::~TriangleMesh()
{
}

//#############################################################################

bool TriangleMesh::AddTriangle ( const VPoint &corner1 , const VPoint &corner2 , const VPoint &corner3 )
{
	// Check if this is a repeated triangle
	if (Size())
	{
		VTriangleE *lasttri = this->operator [] (this->Size() - 1);
		if (lasttri->GetCorner1() == corner1 && lasttri->GetCorner2() == corner2 && lasttri->GetCorner3() == corner3)
			return false;
	}

	VTriangleE newtri ( corner1 , corner2 , corner3 );
	return AddTriangle (newtri);
}

//#############################################################################

bool TriangleMesh::AddTriangle ( const VTriangleE & newtri ) 
{
	if ( newtri.IsCorrupted() == true )
	{
		#if _DEBUG	
			return false;
			//throw VException ( ("corrupted triangle given to mesh") , 1 );
		#else
			return false;
		#endif		
	}

	//tri.push_back(newtri);
	//UpdateMinMaxForSingleTri(newtri);
	
	PrimitiveGeoVec<VTriangleE,3>::PushBack(newtri);
	return true;
}

//#############################################################################

//long TriangleMesh::Size() const
//{
//	//return tri.Size();
//	return (long)tri.size();
//}

//#############################################################################

void TriangleMesh::UpdateMinMax ()
{
	m_aabb.Clear();
	for ( long i = 0; i < Size(); i++ )
	{
		VTriangleE * currtri = (*this)[i];
		UpdateAABB (*currtri , m_aabb);
	}
}

//#############################################################################
//void TriangleMesh::UpdateMinMaxForSingleTri ( const VTriangleE &newtri )
//{
//	m_min.ReplaceMin (newtri.GetCorner1());
//	m_min.ReplaceMin (newtri.GetCorner2());
//	m_min.ReplaceMin (newtri.GetCorner3());
//
//	m_max.ReplaceMax (newtri.GetCorner1());
//	m_max.ReplaceMax (newtri.GetCorner2());
//	m_max.ReplaceMax (newtri.GetCorner3());
//}

//#############################################################################

//const VPoint & TriangleMesh::GetMin() const
//{
//	return m_min;
//}

//#############################################################################

//const VPoint & TriangleMesh::GetMax() const
//{
//	return m_max;
//}

//void TriangleMesh::Clear()
//{
//	//tri.Clear();
//	tri.clear();
//}

VTriangleE * TriangleMesh::operator [] ( const long &index )
{
	//return (VTriangleE*)(&tri[index]);
	return &(PrimitiveGeoVec<VTriangleE,3>::At(index));
}

const VTriangleE * TriangleMesh::operator [] ( const long &index ) const
{
	//return (VTriangleE*)(&tri[index]);
	return &(PrimitiveGeoVec<VTriangleE,3>::At(index));
}

void TriangleMesh::Transform ( const TransformationMatrix & tm )
{
	m_aabb.Clear();
	for ( int i=0; i<Size(); i++ )
	{
		VTriangleE * currtri = (*this)[i];
		VTriangleE transformed = tm.Transform ( *currtri );
		*currtri = transformed;
		UpdateAABB (*currtri , m_aabb);
	}
}

vreal TriangleMesh::GetArea () const
{
	vreal totalA = 0;
	for ( int i = 0; i<Size(); i++ )
	{
		const VTriangleE * currtri = (*this)[i];
		vreal currA = currtri->GetArea();
		totalA += currA;
	}
	return totalA;
}

void TriangleMesh::Serialize(Bofstream &outfile)const
{
	int currversion=2;
	outfile.write((VChr*)&currversion,sizeof(int));
	if (currversion==2)
	{
		VPoint mymin (GetMin());
		VPoint mymax (GetMax());
		mymin.Serialize(outfile);
		mymax.Serialize(outfile);
		//tri.Serialize(outfile);
		VectorSerializer::SerializeTVec(outfile,m_vec);
	}
}

TriangleMesh & TriangleMesh::operator = (const TriangleMesh &toCopy)
{
	//m_min = toCopy.m_min;
	//m_max = toCopy.m_max;
	//tri = toCopy.tri;
	PrimitiveGeoVec<VTriangleE,3>::operator = (toCopy);
	return *this;
}

//bool TriangleMesh::operator == (const TriangleMesh &toCompare)const
//{
//	if(this==&toCompare)
//		return true;
//	if(tri.size()!=toCompare.tri.size())
//		return false;
//	
//	if(m_min!=toCompare.m_min)
//		return false;
//	if(m_max!=toCompare.m_max)
//		return false;
//	if(tri!=toCompare.tri)
//		return false;
//	
//	return true;
//}

//bool TriangleMesh::operator != (const TriangleMesh &toCompare)const
//{
//	return !(this->operator ==(toCompare));
//}
