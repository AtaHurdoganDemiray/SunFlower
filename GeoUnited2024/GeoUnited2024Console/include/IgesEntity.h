// IgesEntity.h

#ifndef __IgesEntity_h__
#define __IgesEntity_h__


#include "DirectoryEntry.h"
#include "ProcessLine.h"
//#include <vector>
using namespace std;

class IgesEntityVisitor;
class IgesEntitySet;

class IgesEntity
{
public:
	IgesEntity();
	virtual ~IgesEntity();
	//IgesEntity & operator = (const IgesEntity &);
	virtual void GetEntityIn( FileLines & mylines , int poffs) = 0;
	virtual void WriteEntityOut (std::vector <VStr> &strvec)const =0;
	virtual void GetChildren (std::vector<int> &childVec)const = 0;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const = 0;
	//virtual bool IsPrimitiveEntity()const =0;
	//virtual double GetSortNo ()const = 0;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const = 0;
	void GetAllParam (DoubleVector &myvect , FileLines &mylines);
	void GetAllParam ( std::vector<VStr> &myvect , FileLines &mylines);
	void GetDirectoryChild (std::vector<int> &childVec)const;
	void WriteParam (const vreal &param , std::vector <VStr> &strvec)const;
	void WriteParam (const int param , std::vector <VStr> &strvec)const;
	void WriteParam (const VStr &param , std::vector<VStr> &strvec)const;
    //long parameterlineno;
	long directorylineno;
	int entitytype;
	bool CopyDE (IgesEntity &newe , const IgesEntitySet &mainSet,IgesEntitySet &newSet)const;
	DirectoryEntry DE;
	mutable int m_subsetIndex;
	//bool m_isRead;
	virtual void OverrideBigZCoordinates (double zLimit , double zWork);
	const VStr & GetModelIdStr()const;
	void SetModelIdStr(const VStr &id);
protected:
	VStr m_modelid;
	void WriteModelId(std::vector<VStr> &strvec)const;
	void ReadModelId(int starti, const DoubleVector &vec);
private:
	
};

#endif // IgesEntity
