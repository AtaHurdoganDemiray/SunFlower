// PointGrid.h

#ifndef __PointGrid_h__
#define __PointGrid_h__

struct VPoint;

class PointGrid
{
public:
	PointGrid (int nPointsU , int nPointsV , std::vector<vreal> &pointCoord);
	int GetNPointsU()const;
	int GetNPointsV()const;
	const std::vector<vreal> &GetPointCoord()const;
	std::vector<vreal> & GetPointCoord();
	VPoint GetPoint (int upointi , int vpointi)const;
	VPoint SetPoint (int upointi , int vpointi , const VPoint &p);// returns the changed point
private:
	int m_nPointsU;
	int m_nPointsV;
	std::vector<vreal> &m_pointCoord;
public:
	void GetLinkedPoints (int upointi , int vpointi , std::list<std::pair<int,int>> &uvpairs)const;
	void GetUSequence (int vpointi , std::vector<VPoint> &uSeq)const;
	void GetVSequence (int upointi , std::vector<VPoint> &vSeq)const;
	vreal GetNearestPoint (const VLine &l , int &upointi , int &vpointi , VPoint &p)const;// returns min dist
};

#endif