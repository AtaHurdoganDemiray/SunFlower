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

#ifndef MGCDELAUNAY2D_H
#define MGCDELAUNAY2D_H

// The Delaunay triangulation method is a modification of code written by 
// Dave Watson.  It uses an algorithm described in
//
//     Watson, D.F., 1981, Computing the n-dimensional Delaunay 
//     tessellation with application to Voronoi polytopes: 
//     The Computer J., 24(2), p. 167-172.

//#include "MagicFMLibType.h"
//#include "MgcRTLib.h"
//#include "MgcVector2.h"

//namespace Mgc {

#include "VPoint2d.h"

class /*MAGICFM*/ Delaunay2D
{
public:
    // Construction and destruction.  In the first constructor,
    // MgcDelaunay2D accepts ownership of the input array and will delete
    // it during destruction.  The second constructor is designed to allow
    // sharing of the network (note that the reference argument is not passed
    // as 'const').  Any other network that shares this object will not delete
    // the data in this object.
    Delaunay2D (int iVertexQuantity, VPoint2d* akVertex);
    Delaunay2D (Delaunay2D& rkNet);
    virtual ~Delaunay2D ();
    bool IsOwner () const;

    // vertices
    int GetVertexQuantity () const;
    const VPoint2d& GetVertex (int i) const;
    const VPoint2d* GetVertices () const;
    vreal GetXMin () const;
    vreal GetXMax () const;
    vreal GetXRange () const;
    vreal GetYMin () const;
    vreal GetYMax () const;
    vreal GetYRange () const;


    // edges
    class /*MAGICFM*/ Edge
    {
    public:
        // vertices forming edge
        int m_aiVertex[2];

        // triangles sharing edge
        int m_aiTriangle[2];
    };

    int GetEdgeQuantity () const;
    const Edge& GetEdge (int i) const;
    const Edge* GetEdges () const;


    // triangles
    class /*MAGICFM*/ Triangle
    {
    public:
        // vertices, listed in counterclockwise order
        int m_aiVertex[3];

        // adjacent triangles,
        //   adj[0] points to triangle sharing edge (ver[0],ver[1])
        //   adj[1] points to triangle sharing edge (ver[1],ver[2])
        //   adj[2] points to triangle sharing edge (ver[2],ver[0])
        int m_aiAdjacent[3];
    };

    int GetTriangleQuantity () const;
    Triangle& GetTriangle (int i);
    const Triangle& GetTriangle (int i) const;
    Triangle* GetTriangles ();
    const Triangle* GetTriangles () const;

    // extra triangles (added to boundary)
    int GetExtraTriangleQuantity () const;
    Triangle& GetExtraTriangle (int i);
    const Triangle& GetExtraTriangle (int i) const;
    Triangle* GetExtraTriangles ();
    const Triangle* GetExtraTriangles () const;

    // helper functions
    static void ComputeBarycenter (const VPoint2d& rkV0, const VPoint2d& rkV1,
        const VPoint2d& rkV2, const VPoint2d& rkP, vreal afBary[3]);

    static bool InTriangle (const VPoint2d& rkV0, const VPoint2d& rkV1,
        const VPoint2d& rkV2, const VPoint2d& rkTest);

    static void ComputeInscribedCenter (const VPoint2d& rkV0,
        const VPoint2d& rkV1, const VPoint2d& rkV2, VPoint2d& rkCenter);

    // tweaking parameters
    static vreal& Epsilon ();  // default = 0.00001
    static vreal& Range ();    // default = 10.0
    static int& TSize ();        // default = 75

protected:
    // for sharing
    bool m_bOwner;

    // vertices
    int m_iVertexQuantity;
    VPoint2d* m_akVertex;
    vreal m_fXMin, m_fXMax, m_fXRange;
    vreal m_fYMin, m_fYMax, m_fYRange;

    // edges
    int m_iEdgeQuantity;
    Edge* m_akEdge;

    // triangles
    int m_iTriangleQuantity;
    Triangle* m_akTriangle;

    // extra triangles to support interpolation on convex hull of vertices
    int m_iExtraTriangleQuantity;
    Triangle* m_akExtraTriangle;

    // tweaking parameters
    static vreal ms_fEpsilon;
    static vreal ms_fRange;
    static int ms_iTSize;
};

#include "MgcDelaunay2D.inl"

//} // namespace Mgc

#endif
