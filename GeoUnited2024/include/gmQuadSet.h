// gmQuadSet.h

#ifndef __gmQuadSet_h__
#define __gmQuadSet_h__

#include "DoubleVec.h"
#include "PointSet.h"
#include "PointSet2d.h"
#include "gmQuad2d.h"
#include "SurfPointsPack.h"

struct CornerRef
{
public:
	CornerRef ( const long &cor1 , const long &cor2 , const long &cor3 , const long &cor4)
	{	
		cor1ref = cor1;
		cor2ref = cor2;
		cor3ref = cor3;
		cor4ref = cor4;
	}

	~CornerRef(){};
	long cor1ref;
	long cor2ref;
	long cor3ref;
	long cor4ref;

};

class gmQuadSet
{
public:
	gmQuadSet( SurfPointsPack &surfpackref);
	gmQuadSet ( const gmQuadSet & Foo , bool isquadvalid[] );
	~gmQuadSet();
	void FillSet ();
	void FillSet ( const int &uno , const int &vno );
	long GetIndexForPointSet ( const int &uno , const int &vno , const int &u , const int &v );
	long Size() const;
	const CornerRef * operator [] ( long index ) const;
	void AddItem ( const long &cor1ref 
					, const long &cor2ref 
					, const long &cor3ref 
					, const long &cor4ref );
	VPoint2d GetMin2d () const;
	VPoint2d GetMax2d () const;
	VPoint GetMin () const;
	VPoint GetMax () const;
	gmQuad2d  GetQuad2d ( long quadno ) const;
	long GetPointSetSize() const
	{
		return surfpack.parampoints.Size();
	}
	const VPoint2d & GetParamPoint ( long pointindex ) const
	{
		VPoint2d *tmp = surfpack.parampoints[pointindex];
		return (*tmp);
	}	
	const VPoint & GetModelPoint ( long pointindex ) const
	{
		VPoint *tmp = surfpack.modelpoints[pointindex];
		return (*tmp);
	}
	const VPoint & GetNormal ( long pointindex ) const
	{
		VPoint * tmp = surfpack.normals[pointindex];
		return (*tmp);
	}
	SurfPointsPack &surfpack;
private:
	std::vector<CornerRef *> allcornerref;
	
};

struct EdgeInfo 
{
public:
	EdgeInfo (){};
	bool IsEdgeInfoRecorded ( long edgepoint2ref , DoubleVec &theinfo)
	{
		bool goon = true;
		bool succ = false;
		int i = 0;
		while ( goon )
		{	
			if ( ed[i].Size() != 0 )
			{
				if ( ed[i][0] == (vreal) edgepoint2ref )
				{
					for ( int j = 1; j < ed[i].Size() ; j++)
					{
						theinfo.Addvreal ( ed[i][j] );
					}
					goon = false;
					succ = true;
				}

			}
			i++;
			if ( i == 4 )
				goon = false;
		}
		return ( succ );
	}

	void RecordEdgeInfo ( long edgepoint2ref , DoubleVec &thevec)
	{
		// maximum 4 edges canbe inserted
		bool goon = true;
		int i = 0;
		while ( goon )
		{	
			if ( ed[i].Size() == 0 )
			{
				ed[i].Addvreal ( (vreal)edgepoint2ref );
				// add thevec to ed[i] 
				// for debug
				//int coinc = 0;
				for ( int j = 0; j < thevec.Size(); j++ )
				{
					//if ( fabs ( thevec[j] - 0.525125 ) < 0.005
					//	|| fabs ( thevec[j] - 0.83333 ) < 0.005 )
					//	coinc ++;

					//if ( coinc == 2 ) 
					//	int stp = 1;
				
					ed[i].Addvreal ( thevec[j] );
				}
		
				goon = false;
			}
			i++;
			if ( i==4 )
				goon = false;//Exception!
		}
	}
				
	~EdgeInfo (){};
	DoubleVec ed[4];
};

class QuadInfoSet
{
public:
	QuadInfoSet( const gmQuadSet &newbaseset )
	: m_baseset ( newbaseset )
	{
		long pointno = m_baseset.GetPointSetSize();
		// realise all edgeinfo and pointinfo s
		for ( long i = 0; i < pointno; i++ )
		{
			// edge info
			EdgeInfo * newval = new EdgeInfo;
			alledgeinfo.push_back ( newval );

			// point info
			allpointinfo.push_back ( false );
			allpointinfo.push_back ( false );
		}
	}
	
	~QuadInfoSet()
	{
		for ( unsigned long i = 0; i < alledgeinfo.size(); i++ )
			delete alledgeinfo[i];
	}

	bool IsEdgeInfoRecorded ( long edgepoint1ref , long edgepoint2ref , DoubleVec &theinfo)
	{
		long smallindex , bigindex;
		
		if ( edgepoint1ref < edgepoint2ref )
		{
			smallindex = edgepoint1ref;
			bigindex = edgepoint2ref;
		}
		else
		{
			smallindex = edgepoint2ref;
			bigindex = edgepoint1ref;
		}

		bool retval = alledgeinfo[smallindex]->IsEdgeInfoRecorded ( bigindex , theinfo );

		return retval;
	}

	void RecordEdgeInfo ( long edgepoint1ref , long edgepoint2ref , DoubleVec &thevec )
	{
		long smallindex , bigindex;
		
		if ( edgepoint1ref < edgepoint2ref )
		{
			smallindex = edgepoint1ref;
			bigindex = edgepoint2ref;
		}
		else
		{
			smallindex = edgepoint2ref;
			bigindex = edgepoint1ref;
		}

		alledgeinfo[smallindex]->RecordEdgeInfo ( bigindex , thevec );
	}

	bool IsPointInfoRecorded ( long pointref , bool &theinfo )
	{
		long indexs = pointref * 2;
		if ( allpointinfo[indexs] == true )
		{
			theinfo = allpointinfo[indexs+1];
			return true;
		}

		return false;
	}
	
	void RecordPointInfo ( long pointref , bool theinfo )
	{
		long indexs = pointref * 2;
		allpointinfo[indexs] = true;
		allpointinfo[indexs+1] = theinfo;
	}

private:
	const gmQuadSet &m_baseset;
	std::vector<EdgeInfo *> alledgeinfo;
	std::vector<bool> allpointinfo;	
};

#endif //__gmQuadSet_h__