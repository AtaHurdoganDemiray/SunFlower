// VRefTriangleMesh.h

#ifndef __VRefTriangleMesh_h__
#define __VRefTriangleMesh_h__

#include "SurfPointsPack.h"
#include "RefTriangleMesh.h"
#include "Triangles.h"

struct VPoint;
struct VPoint2d;
struct texture;
struct DoubleList;
//struct VLine;
class LineSet;
class PointSet;
class VTexture;
struct CoordinateSystem;
class SequentialLineSet;

class VRefTriangleMesh : public Triangles
{
public:	
	struct RefQuad
	{
	public:
		RefQuad (const int i1,const int i2)
		{
			t[0]=i1;
			t[1]=i2;
		}
		int t[2];
	};
	friend class Display;
	friend class OglRefMesh;
	VRefTriangleMesh ( const int &initialsize );
	VRefTriangleMesh ( const int initialPointSize,const int initialTriSize);
	VRefTriangleMesh ( const VRefTriangleMesh & Foo );
	//long Size() const;
	int AddTriangle ( const RefTriangle &newtri );
	int AddQuad (const RefQuad &q);
	void SetRefTri (const long &i , const RefTriangle &newtri);
	const RefTriangle & GetRefTri (const long &i )const;
	const VPoint& GetCor1Normal( const long &i ) const;
	const VPoint& GetCor2Normal( const long &i ) const;
	const VPoint& GetCor3Normal( const long &i ) const;
	const VPoint& GetModelTriCor1 ( const long &i )const;
	const VPoint& GetModelTriCor2 ( const long &i )const;
	const VPoint& GetModelTriCor3 ( const long &i )const;
	const VPoint2d& GetParamTriCor1 ( const long &i )const;
	const VPoint2d& GetParamTriCor2 ( const long &i )const;
	const VPoint2d& GetParamTriCor3 ( const long &i )const;
	//VPoint GetMin() const;
	//VPoint GetMax() const;
	//void UpdateMinMax();
	void UpdateMinMaxForTri ( const RefTriangle &newtri );
	vreal GetArea () const;
	vreal GetAreaProjXY ()const;
	vreal GetAreaProjXZ ()const;
	vreal GetAreaProjYZ ()const;
	SurfPointsPack surfpack;
	std::vector<RefQuad> m_quad;
	bool operator == (const VRefTriangleMesh &toCompare)const;
	bool operator != (const VRefTriangleMesh &toCompare)const;	
	void Serialize (Bofstream &outfile)const;
	void Serialize (Bifstream &infile);
	void CleanTrimmedTriangles();
	RefTriangleMesh & GetRefTriangleMesh ();
	const RefTriangleMesh & GetRefTriangleMesh ()const
	{
		return rdata;
	};
	VTriangle2dE GetParTri(int index)const;
	void Transform (const TransformationMatrix &tm);
	void CalculateNormals ();
	void Clear ();
private:
	RefTriangleMesh rdata;	
	//VPoint m_min,m_max;
public:
	virtual const VTriangleE operator[] (int i)const;
	virtual VTriangleE operator[] (int i);	
	virtual const VTriangle GetTri (int i)const;
	virtual int Size()const;
	virtual const VPoint GetMin()const;
	virtual const VPoint GetMax()const;
	//void DrawWithTexture ( const texture *mytexture ) const;
	void DrawWithTexture ( const VTexture *mytexture ) const;
	void PeriodicModifyTextureUV (VPoint2d &uv1 , VPoint2d &uv2 , VPoint2d &uv3)const;
	VPoint GetMinPointInCs (const CoordinateSystem &cs) const;
	VPoint GetMaxPointInCs (const CoordinateSystem &cs) const;
	void AddTriangleToSurfPackAndRefTri (const VTriangleE &tri);
	void FindEdges (LineSet &ls);
private:
	struct TriangleEdge
	{
	public:
		int m_trino;
		int m_vertex1i;
		int m_vertex2i;
		bool m_hasNeighbor;
		TriangleEdge()
		{
			m_hasNeighbor = false;
		};
	};
public:
	void MakeYSections(const std::map<vreal, vreal> coordMap, const CoordinateSystem &cs
		, std::multimap<vreal, SequentialLineSet *> &sections, const std::map<vreal, std::pair<vreal, vreal>> &boundary, vreal tol)const;
	static bool XTrimLineIf(VLine &l, const std::pair<vreal, vreal> &boundary);
	static bool YTrimLineIf(VLine &l, const std::pair<vreal, vreal> &boundary);
	static SequentialLineSet * OneLoopX(std::multimap<vreal, VLine> &lmap, const std::multimap<vreal, vreal> &rMap, const std::map<vreal, std::pair<vreal, vreal>> &boundary, vreal tol);
	static SequentialLineSet * OneLoopY(std::multimap<vreal, VLine> &lMap, const std::multimap<vreal, vreal> &rMap, const std::map<vreal, std::pair<vreal, vreal>> &boundary, vreal tol);
	static std::multimap<vreal, VLine>::iterator SearchConnected(std::multimap<vreal, VLine> &lMap, vreal key, const VPoint &keyPoint
		, vreal searchGap, vreal tol, VLine &connectedL
		, const std::multimap<vreal, vreal> &rMap);
	static int VTRI_MAX_SL_COUNT;
	void ReverseNormals();
};

#endif // __VRefTriangleMesh_h__