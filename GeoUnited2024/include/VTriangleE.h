/******************************************************************************
*               File: VTriangleE.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  01.07.2003 16:11:30 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#ifndef __VTriangleE_h__
#define __VTriangleE_h__

#include "VPoint.h"
#include "VTriangle.h"
class Bifstream;
class Bofstream;
struct VPlane;
struct CoordinateSystem;

struct VTriangleE
{
public:
	VTriangleE ();
	VTriangleE 
		( const VPoint & cor1 , const VPoint & cor2 , const VPoint & cor3 );
	VTriangleE ( vreal cor1[3] , vreal cor2[3] , vreal cor3[3] );
	VTriangleE ( const VTriangleE &Foo );
	VTriangleE ( VIFStream & infile );
	~VTriangleE();
	
	const VPoint & GetCorner1() const;
	const VPoint & GetCorner2() const;
	const VPoint & GetCorner3() const;
	VPoint & GetCorner1();
	VPoint & GetCorner2();
	VPoint & GetCorner3();

	const VPoint & GetNormal() const;
	const VPoint & GetEdge1() const;
	const VPoint & GetEdge2() const;
	const VPoint & GetEdge3() const;
	VPoint & GetNormal();
	VPoint & GetEdge1();
	VPoint & GetEdge2();
	VPoint & GetEdge3();

	const VPoint & operator[] ( int index ) const;
	//VPoint GetMin() const;
	//VPoint GetMax() const;
	void SetEdges ();
	void SetNormal ();
	void SetCorner1 ( const vreal newcorner[] );
	void SetCorner2 ( const vreal newcorner[] );
	void SetCorner3 ( const vreal newcorner[] );
	bool IsInTriangle ( const VPoint &apoint ) const;
	bool IsInTriangle ( const VPoint & apoint , const vreal & offsettol ) const;

	VPoint GetCenterOfMass() const;
	void operator = ( const VTriangleE &Foo );
	bool IsCorrupted () const;	
	vreal GetPerimeter() const;
	vreal GetArea() const;
	void operator << ( VIFStream & infile );
	void operator >> ( VofStream & outfile ) const;
	void Serialize (Bofstream &binfile)const;
	void Serialize (Bifstream &binfile);
	VTriangleE (Bifstream &binfile);
	bool operator == (const VTriangleE &toCompare)const;
	bool operator != (const VTriangleE &toCompare)const;
	void UpdateMinMax(VPoint &minp,VPoint &maxp)const;
	bool GetUV (const VPoint &p , vreal &u , vreal &v)const;
	VPoint GetP (const vreal &u , const vreal &v)const;
	VTriangle & GetTri ();
	const VTriangle & GetTri()const;
	bool IsNeighbor (const VTriangleE &t , VPoint &common1 , VPoint &common2 , vreal tol = 1.e-8)const;
	sVector<double,3> GetMin()const;
	sVector<double,3> GetMax()const;
	//vreal CalculateProjVolumeToPlane (const VPoint &planepoint , const VPoint &planenormal)const;
	vreal CalculateProjVolumeToPlane (const VPlane &plane)const;
	void Digitise (const vreal &step 
					, std::vector<VPoint> &ps
					, std::vector<VPoint> &ns)const;
	void Digitise (const vreal step
					, std::vector<VPoint> &pvec)const;
	void DigitiseConstU (const vreal u,const vreal dv,std::vector<VPoint> &pvec)const;

	VPlane GetPlane() const;
	CoordinateSystem GetCs1()const;
	CoordinateSystem GetCs2()const;
	CoordinateSystem GetCs3()const;

private:
	VTriangle m_tri;
	VPoint edge1;
	VPoint edge2;
	VPoint edge3;

	VPoint normal;
	void GetAc (const VPoint &p1,const VPoint &p2,const VPoint &p3,const VPoint &p4,vreal &c , vreal &A)const;
public:
	static VPoint SpaceConversionFromTriangleToTriangle (const VPoint &p , const VTriangleE &tri1 , const VTriangleE &tri2);
	VPoint GetMinInCs(const CoordinateSystem &cs)const;
	VPoint GetMaxInCs(const CoordinateSystem &cs)const;
	bool IsNeighbor(const VTriangleE &t, VPoint &common1, VPoint &common2, VPoint &myother, VPoint &tother , vreal tol)const;

};

#endif //__VTriangleE_h__
