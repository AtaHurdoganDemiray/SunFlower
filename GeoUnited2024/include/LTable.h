// LTable.h

#ifndef __LTable_h__
#define __LTable_h__

class LTable
{
public:
	LTable();
	void Construct(const std::map<vreal, vreal> &table);

	bool LookUp(vreal x, vreal &y)const;
private:
	std::map<vreal, vreal> m_table;

};

#endif