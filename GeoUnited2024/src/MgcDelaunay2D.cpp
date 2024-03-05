// Magic Software, Inc.
// http://www.magic-software.com
// Copyright (c) 2000, 2001.  All Rights Reserved
//
// Source code from Magic Software is supplied under the terms of a license
// agreement and may not be copied or disclosed except in accordance with the
// terms of that agreement.  The various license agreements may be found at
// the Magic Software web site.  This file is subject to the license
//
// FREE SOURCE CODE
// http://www.magic-software.com/License/free.pdf

#include "stdafx.h"
#include "memory.h"
#include "stdlib.h"
#include "MgcDelaunay2D.h"
#include "VPoint2d.h"
//#include "MgcMath.h"
//#include "MgcRTLib.h"
//using namespace Mgc;


vreal Delaunay2D::ms_fEpsilon = 1.e-8;//0.00001f;
vreal Delaunay2D::ms_fRange = 10.0;
int Delaunay2D::ms_iTSize = 600;//75;

//#define FLT_MAX 1e6

inline vreal SymmetricRandom (vreal fSeed)
{
    if ( fSeed > 0.0 )
        srand((unsigned int)fSeed);

    return 2.0f*vreal(rand())/vreal(RAND_MAX) - 1.0f;
}

//----------------------------------------------------------------------------
Delaunay2D::Delaunay2D (int iVertexQuantity, VPoint2d* akVertex)
{
    //assert( iVertexQuantity >= 3 && akVertex );
    if ( iVertexQuantity <= 3 || akVertex == 0 )
		throw (VException(( IDS_DelCorExc),0));

	m_akTriangle = NULL;

    // for edge processing
    typedef int _Index[2];
    _Index* akIndex = NULL;
    int iE;

    m_bOwner = true;

    m_iVertexQuantity = iVertexQuantity;
    m_akVertex = akVertex;

    // Make a copy of the input vertices.  These will be modified.  The
    // extra three slots are required for temporary storage.
    VPoint2d* akPoint = new VPoint2d[m_iVertexQuantity+3];
    //memcpy(akPoint,akVertex,m_iVertexQuantity*sizeof(VPoint2d)*2);
	for ( int myi=0; myi<m_iVertexQuantity; myi++) 	
		akPoint[myi]=akVertex[myi];

    // compute the axis-aligned bounding rectangle of the vertices
    m_fXMin = akPoint[0].x();
    m_fXMax = m_fXMin;
    m_fYMin = akPoint[0].y();
    m_fYMax = m_fYMin;

    int i;
    for (i = 1; i < m_iVertexQuantity; i++)
    {
        vreal fValue = akPoint[i].x();
        if ( m_fXMax < fValue )
            m_fXMax = fValue;
        if ( m_fXMin > fValue )
            m_fXMin = fValue;

        fValue = akPoint[i].y();
        if ( m_fYMax < fValue )
            m_fYMax = fValue;
        if ( m_fYMin > fValue )
            m_fYMin = fValue;
    }

    m_fXRange = m_fXMax-m_fXMin;
    m_fYRange = m_fYMax-m_fYMin;

    // need to scale the data later to do a correct triangle count
    vreal fMaxRange = ( m_fXRange > m_fYRange ? m_fXRange : m_fYRange );
    vreal fMaxRangeSqr = fMaxRange*fMaxRange;

    // Tweak the points by very small random numbers to help avoid
    // cocircularities in the vertices.
    vreal fAmplitude = 0.5*ms_fEpsilon*fMaxRange;
    for (i = 0; i < m_iVertexQuantity; i++) 
    {
        akPoint[i].x() += fAmplitude*SymmetricRandom(0.0);
        akPoint[i].y() += fAmplitude*SymmetricRandom(0.0);
    }

    vreal aafWork[3][2] =
    {
        { 5.0*ms_fRange, -ms_fRange },
        { -ms_fRange, 5.0*ms_fRange },
        { -ms_fRange, -ms_fRange }
    };

    for (i = 0; i < 3; i++)
    {
        akPoint[m_iVertexQuantity+i].x() = m_fXMin+m_fXRange*aafWork[i][0];
        akPoint[m_iVertexQuantity+i].y() = m_fYMin+m_fYRange*aafWork[i][1];
    }

    int i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i11, aiII[3];
    vreal fTmp;

    int iTwoTSize = 2*ms_iTSize;
    int** aaiTmp = new int*[iTwoTSize+1];
    aaiTmp[0] = new int[2*(iTwoTSize+1)];
    for (i0 = 1; i0 < iTwoTSize+1; i0++)
        aaiTmp[i0] = aaiTmp[0] + 2*i0;
    i1 = 2*(m_iVertexQuantity + 2);

    int* aiID = new int[i1];
    for (i0 = 0; i0 < i1; i0++) 
        aiID[i0] = i0; 

    int** aaiA3S = new int*[i1];
    aaiA3S[0] = new int[3*i1];
    for (i0 = 1; i0 < i1; i0++)
        aaiA3S[i0] = aaiA3S[0] + 3*i0;
    aaiA3S[0][0] = m_iVertexQuantity;
    aaiA3S[0][1] = m_iVertexQuantity+1;
    aaiA3S[0][2] = m_iVertexQuantity+2;

    // circumscribed centers and radii
    vreal** aafCCR = new vreal*[i1];  int arrbound = i1;
    aafCCR[0] = new vreal[3*i1];
    for (i0 = 1; i0 < i1; i0++)
        aafCCR[i0] = aafCCR[0] + 3*i0;
    aafCCR[0][0] = 0.0;
    aafCCR[0][1] = 0.0;

    aafCCR[0][2] = FLT_MAX;

    int iTriangleQuantity = 1;  // number of triangles
    i4 = 1;

    // compute triangulation
    for (i0 = 0; i0 < m_iVertexQuantity; i0++)
    {  
        i1 = i7 = -1;
        i9 = 0;
        for (i11 = 0; i11 < iTriangleQuantity; i11++)
        {
            i1++;
            while ( aaiA3S[i1][0] < 0 ) 
                i1++;
            fTmp = aafCCR[i1][2];
            for (i2 = 0; i2 < 2; i2++)
            {  
                vreal fZ = akPoint[i0][i2] - aafCCR[i1][i2];
                fTmp -= fZ*fZ;
                if ( fTmp < 0.0 ) 
                    goto Corner3;
            }
            i9--;
            i4--;
            aiID[i4] = i1;
            for (i2 = 0; i2 < 3; i2++)
            {  
                aiII[0] = 0;
                if ( aiII[0] == i2 )
                    aiII[0]++;
                for (i3 = 1; i3 < 2; i3++)
                {  
                    aiII[i3] = aiII[i3-1] + 1;
                    if ( aiII[i3] == i2 )
                        aiII[i3]++;
                }
                if ( i7 > 1 )
                {  
                    i8 = i7;
                    for (i3 = 0; i3 <= i8; i3++)
                    {  
                        for (i5 = 0; i5 < 2; i5++) 
                            if ( aaiA3S[i1][aiII[i5]] != aaiTmp[i3][i5] ) 
                                goto Corner1;
                        for (i6 = 0; i6 < 2; i6++) 
                            aaiTmp[i3][i6] = aaiTmp[i8][i6];
                        i7--;
                        goto Corner2;
Corner1:;
                    }
                }
                if ( ++i7 > iTwoTSize )
                {
                    // Temporary storage exceeded.  Increase ms_iTSize and
                    // call the constructor again.
//                    assert( false );
                    throw (VException(( IDS_DelCorExc1),0));
					goto ExitDelaunay;
                }
                for (i3 = 0; i3 < 2; i3++) 
                    aaiTmp[i7][i3] = aaiA3S[i1][aiII[i3]];
Corner2:;
            }
            aaiA3S[i1][0] = -1;
Corner3:;
        }

        for (i1 = 0; i1 <= i7; i1++)
        {  
            for (i2 = 0; i2 < 2; i2++)
                for (aafWork[2][i2] = 0, i3 = 0; i3 < 2; i3++)
                {  
                    aafWork[i3][i2] = akPoint[aaiTmp[i1][i2]][i3] -
                        akPoint[i0][i3];
                    aafWork[2][i2] += 0.5f*aafWork[i3][i2]*(
                        akPoint[aaiTmp[i1][i2]][i3] + akPoint[i0][i3]);
                }

            fTmp = aafWork[0][0]*aafWork[1][1] - aafWork[1][0]*aafWork[0][1];
//            assert( fabsf(fTmp) > 0.0f );
            if ( fabs(fTmp)<0.0 )
				throw(VException(( IDS_DelCorExcBin),0));
			
			// Ata added this exception
			if ( aiID[i4] >= arrbound )
				throw ( VException ( ( IDS_DelCorExcArrBou) , 0));

			fTmp = 1.0/fTmp;
            aafCCR[aiID[i4]][0] = (aafWork[2][0]*aafWork[1][1] -
                aafWork[2][1]*aafWork[1][0])*fTmp;
            aafCCR[aiID[i4]][1] = (aafWork[0][0]*aafWork[2][1] -
                aafWork[0][1]*aafWork[2][0])*fTmp;

            for (aafCCR[aiID[i4]][2] = 0, i2 = 0; i2 < 2; i2++) 
            {  
                vreal fZ = akPoint[i0][i2] - aafCCR[aiID[i4]][i2];
                aafCCR[aiID[i4]][2] += fZ*fZ;
                aaiA3S[aiID[i4]][i2] = aaiTmp[i1][i2];
            }

            aaiA3S[aiID[i4]][2] = i0;
            i4++;
            i9++;
        }
        iTriangleQuantity += i9;
    }

    // count the number of triangles
    m_iTriangleQuantity = 0;
    i0 = -1;
    for (i11 = 0; i11 < iTriangleQuantity; i11++)
    {  
        i0++;
        while ( aaiA3S[i0][0] < 0 ) 
            i0++;
        if ( aaiA3S[i0][0] < m_iVertexQuantity )
        {  
            for (i1 = 0; i1 < 2; i1++)
            {
                for (i2 = 0; i2 < 2; i2++)
                {
                    aafWork[i2][i1] = akPoint[aaiA3S[i0][i1]][i2] -
                        akPoint[aaiA3S[i0][2]][i2];
                }
            }

            fTmp = aafWork[0][0]*aafWork[1][1] - aafWork[0][1]*aafWork[1][0];
            if ( fabs(fTmp) > ms_fEpsilon*fMaxRangeSqr )
                m_iTriangleQuantity++;
        }
    }

    // create the triangles
    m_akTriangle = new Triangle[m_iTriangleQuantity];
    m_iTriangleQuantity = 0;
    i0 = -1;
    for (i11 = 0; i11 < iTriangleQuantity; i11++)
    {  
        i0++;
        while ( aaiA3S[i0][0] < 0 ) 
            i0++;
        if ( aaiA3S[i0][0] < m_iVertexQuantity )
        {  
            for (i1 = 0; i1 < 2; i1++)
            {
                for (i2 = 0; i2 < 2; i2++)
                {
                    aafWork[i2][i1] = akPoint[aaiA3S[i0][i1]][i2] -
                        akPoint[aaiA3S[i0][2]][i2];
                }
            }

            fTmp = aafWork[0][0]*aafWork[1][1] - aafWork[0][1]*aafWork[1][0];
            if ( fabs(fTmp) > ms_fEpsilon*fMaxRangeSqr )
            {  
                int iDelta = (fTmp < 0.0 ? 1 : 0);
                Triangle& rkTri = m_akTriangle[m_iTriangleQuantity];
                rkTri.m_aiVertex[0] = aaiA3S[i0][0];
                rkTri.m_aiVertex[1] = aaiA3S[i0][1+iDelta];
                rkTri.m_aiVertex[2] = aaiA3S[i0][2-iDelta];
                rkTri.m_aiAdjacent[0] = -1;
                rkTri.m_aiAdjacent[1] = -1;
                rkTri.m_aiAdjacent[2] = -1;
                m_iTriangleQuantity++;
            }
        }
    }

    // build edge table
    m_iEdgeQuantity = 0;
    m_akEdge = new Edge[3*m_iTriangleQuantity];
    akIndex = new _Index[3*m_iTriangleQuantity];

    int j, j0, j1;
    for (i = 0; i < m_iTriangleQuantity; i++)
    {
        Triangle& rkTri = m_akTriangle[i];

        for (j0 = 0, j1 = 1; j0 < 3; j0++, j1 = (j1+1)%3)
        {
            for (j = 0; j < m_iEdgeQuantity; j++)
            {
                Edge& rkEdge = m_akEdge[j];
                if ( (rkTri.m_aiVertex[j0] == rkEdge.m_aiVertex[0] 
                   && rkTri.m_aiVertex[j1] == rkEdge.m_aiVertex[1])
                ||   (rkTri.m_aiVertex[j0] == rkEdge.m_aiVertex[1] 
                   && rkTri.m_aiVertex[j1] == rkEdge.m_aiVertex[0]) )
                {
                    break;
                }
            }
            if ( j == m_iEdgeQuantity )  // add edge to table
            {
                m_akEdge[j].m_aiVertex[0] = rkTri.m_aiVertex[j0];
                m_akEdge[j].m_aiVertex[1] = rkTri.m_aiVertex[j1];
                m_akEdge[j].m_aiTriangle[0] = i;
                akIndex[j][0] = j0;
                m_akEdge[j].m_aiTriangle[1] = -1;
                m_iEdgeQuantity++;
            }
            else  // edge already exists, add triangle to table
            {
                m_akEdge[j].m_aiTriangle[1] = i;
                akIndex[j][1] = j0;
            }
        }
    }

    // count boundary edges (the convex hull of the points)
    // and establish links between adjacent triangles
    m_iExtraTriangleQuantity = 0;
    for (i = 0; i < m_iEdgeQuantity; i++)
    {
        if ( m_akEdge[i].m_aiTriangle[1] != -1 )
        {
            j0 = m_akEdge[i].m_aiTriangle[0];
            j1 = m_akEdge[i].m_aiTriangle[1];
            m_akTriangle[j0].m_aiAdjacent[akIndex[i][0]] = j1;
            m_akTriangle[j1].m_aiAdjacent[akIndex[i][1]] = j0;
        }
        else
        {
            m_iExtraTriangleQuantity++;
        }
    }
    delete[] akIndex;

    // set up extra triangle data
    m_akExtraTriangle = new Triangle[m_iExtraTriangleQuantity];
    for (i = 0, iE = 0; i < m_iTriangleQuantity; i++)
    {
        Triangle& rkTri = m_akTriangle[i];
        for (int j = 0; j < 3; j++)
        {
            if ( rkTri.m_aiAdjacent[j] == -1 )
            {
                Triangle& rkETri = m_akExtraTriangle[iE];
                int j1 = (j+1) % 3, j2 = (j+2) % 3;
                int iS0 = rkTri.m_aiVertex[j];
                int iS1 = rkTri.m_aiVertex[j1];
                rkETri.m_aiVertex[j] = iS0;
                rkETri.m_aiVertex[j1] = iS1;
                rkETri.m_aiVertex[j2] = -1;
                rkTri.m_aiAdjacent[j] = iE + m_iTriangleQuantity;
                iE++;
            }
        }
    }

ExitDelaunay:;
    delete[] aaiTmp[0];
    delete[] aaiTmp;
    delete[] aiID;
    delete[] aaiA3S[0];
    delete[] aaiA3S;
    delete[] aafCCR[0];
    delete[] aafCCR;
    delete[] akPoint;
}
//----------------------------------------------------------------------------
Delaunay2D::Delaunay2D (Delaunay2D& rkNet)
{
    m_bOwner = false;

    m_iVertexQuantity = rkNet.m_iVertexQuantity;
    m_akVertex = rkNet.m_akVertex;
    m_fXMin = rkNet.m_fXMin;
    m_fXMax = rkNet.m_fXMax;
    m_fXRange = rkNet.m_fXRange;
    m_fYMin = rkNet.m_fYMin;
    m_fYMax = rkNet.m_fYMax;
    m_fYRange = rkNet.m_fYRange;
    m_iEdgeQuantity = rkNet.m_iEdgeQuantity;
    m_akEdge = rkNet.m_akEdge;
    m_iTriangleQuantity = rkNet.m_iTriangleQuantity;
    m_akTriangle = rkNet.m_akTriangle;
    m_iExtraTriangleQuantity = rkNet.m_iExtraTriangleQuantity;
    m_akExtraTriangle = rkNet.m_akExtraTriangle;
}
//----------------------------------------------------------------------------
Delaunay2D::~Delaunay2D ()
{
    if ( m_bOwner )
    {
        //delete[] m_akVertex;
        delete[] m_akEdge;
        delete[] m_akTriangle;
        delete[] m_akExtraTriangle;
    }
}
//----------------------------------------------------------------------------
void Delaunay2D::ComputeBarycenter (const VPoint2d& rkV0, const VPoint2d& rkV1,
    const VPoint2d& rkV2, const VPoint2d& rkP, vreal afBary[3])
{
    VPoint2d kV02 = rkV0 - rkV2;
    VPoint2d kV12 = rkV1 - rkV2;
    VPoint2d kPV2 = rkP - rkV2;

    vreal fM00 = kV02%(kV02);
    vreal fM01 = kV02%(kV12);
    vreal fM11 = kV12%(kV12);
    vreal fR0 = kV02%(kPV2);
    vreal fR1 = kV12%(kPV2);
    vreal fDet = fM00*fM11 - fM01*fM01;
    //assert( fabsf(fDet) > 0.0f );
    if ( fabs(fDet) < 0.0 )
		throw(VException(( IDS_DelCorExc2),0));

	vreal fInvDet = 1.0/fDet;

    afBary[0] = (fM11*fR0 - fM01*fR1)*fInvDet;
    afBary[1] = (fM00*fR1 - fM01*fR0)*fInvDet;
    afBary[2] = 1.0 - afBary[0] - afBary[1];
}
//----------------------------------------------------------------------------
bool Delaunay2D::InTriangle (const VPoint2d& rkV0, const VPoint2d& rkV1,
    const VPoint2d& rkV2, const VPoint2d& rkTest)
{
    const vreal fEpsilon = 1e-08;//6;

    // test against normal to first edge
    VPoint2d kDir = rkTest - rkV0;
    VPoint2d kNor(rkV0.y() - rkV1.y(), rkV1.x() - rkV0.x());
    if ( kDir%(kNor) < -fEpsilon )
        return false;

    // test against normal to second edge
    kDir = rkTest - rkV1;
    kNor = VPoint2d(rkV1.y() - rkV2.y(), rkV2.x() - rkV1.x());
    if ( kDir%(kNor) < -fEpsilon )
        return false;

    // test against normal to third edge
    kDir = rkTest - rkV2;
    kNor = VPoint2d(rkV2.y() - rkV0.y(), rkV0.x() - rkV2.x());
    if ( kDir%(kNor) < -fEpsilon )
        return false;

    return true;
}
//----------------------------------------------------------------------------
void Delaunay2D::ComputeInscribedCenter (const VPoint2d& rkV0,
    const VPoint2d& rkV1, const VPoint2d& rkV2, VPoint2d& rkCenter)
{
    VPoint2d kD10 = rkV1 - rkV0, kD20 = rkV2 - rkV0, kD21 = rkV2 - rkV1;
    vreal fL10 = kD10.GetLength(), fL20 = kD20.GetLength(), fL21 = kD21.GetLength();
    vreal fPerimeter = fL10 + fL20 + fL21;
    if ( fPerimeter > 0.0 )
    {
        vreal fInv = 1.0f/fPerimeter;
        fL10 *= fInv;
        fL20 *= fInv;
        fL21 *= fInv;
    }

    rkCenter = rkV0*fL21 + rkV1*fL20 + rkV2*fL10 ;
}
//----------------------------------------------------------------------------
