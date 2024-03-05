#include "stdafx.h"
#include "SerializeVector.h"
#include "sVector.h"
#include <vector>

using namespace std;

SerializeVector::SerializeVector(void)
{
}

SerializeVector::~SerializeVector(void)
{
}

void SerializeVector::write (const std::vector<sVector<double,3> > &points,
                                    const std::vector<sVector<double,3> > &normals,
                                    const std::vector<sVector<int,3> > &indices,
									VofStream &mystream)

{

    std::vector<sVector<double,3> >::const_iterator b;
    std::vector<sVector<double,3> >::const_iterator e;

	b = points.begin();
	e = points.end();	
	mystream << points.size() << std::endl;
	for ( b; b != e; b++)
	{
		mystream << b->operator [](0)
				 << (",")
				 << b->operator [](1)
				 << (",")
				 << b->operator [](2)
				 << std::endl;
	}
	
	b = normals.begin();
	e = normals.end();
	mystream << normals.size() << std::endl;
	for ( b; b != e; b++)
	{
		mystream << b->operator [](0)
				 << (",")
				 << b->operator [](1)
				 << (",")
				 << b->operator [](2)
				 << std::endl;
	}
	
    std::vector<sVector<int,3> >::const_iterator i;
    std::vector<sVector<int,3> >::const_iterator w;

	i = indices.begin();
	w = indices.end();
	mystream << indices.size() << std::endl;
	for ( i; i != w; i++)
	{
		mystream << i->operator [](0)
				 << (",")
				 << i->operator [](1)
				 << (",")
				 << i->operator [](2)
				 << std::endl;
	}
	
	mystream.close();
	
}
void SerializeVector::read (std::vector<sVector<double,3> > &points,
                              std::vector<sVector<double,3> > &normals,
                              std::vector<sVector<int,3> > &indices
							  , const VStr &filename)
{
	int size;
	VChr tmp;
	VIFStream mystream(filename.data());
	sVector<double,3> myTemp;

	mystream >> size;
	for ( int i = 0; i < size; i++)
	{
		mystream >> myTemp[0] 
				 >> tmp
				 >> myTemp[1]
				 >> tmp
				 >> myTemp[2];
		
		points.push_back(myTemp);
	}

	mystream >> size;
	for ( int i = 0; i < size; i++)
	{
		mystream >> myTemp[0] 
				 >> tmp
				 >> myTemp[1]
				 >> tmp
				 >> myTemp[2];
		
		normals.push_back(myTemp);
	}

	sVector<int,3> myTemp2;
	mystream >> size;
	for ( int i = 0; i < size; i++)
	{
		mystream >> myTemp2[0] 
				 >> tmp
				 >> myTemp2[1]
				 >> tmp
				 >> myTemp2[2];
		
		indices.push_back(myTemp2);
	}
	
}
