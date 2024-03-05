// DTableRow.h

#ifndef __DTableRow_h__
#define __DTableRow_h__

class DTableRow
{
public:
	DTableRow();
	DTableRow (const std::vector<VStr> &rowStr);
	const std::vector<VStr> & GetData ()const;
	std::vector<VStr> & GetData();
	bool Read (std::map<VStr,VStr> &keyValueMap , int rowNo);
	void Write (VofStream &f,int rowNo)const;
private:
	std::vector<VStr> m_data;
public:
	bool SearchForKey (const VStr &key , VStr &val,const std::vector<VStr> &keyVec)const;
};

#endif
