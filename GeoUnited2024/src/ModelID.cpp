// ModelID.cpp

#include "stdafx.h"
#include "ModelID.h"

int ModelID::m_modelIdCounter = 1;

ModelID::ModelID()
{
	//m_birthdate = VTime::GetCurrentTime ();
	//m_birthdate = (LONGLONG)this;
	//m_model = 0;
	VTime curtime = VTime::GetCurrentTime();
	CString tempStr;
	//VPoint center = VPoint(0.0);
	//vreal rad = 0.0;
	tempStr.Format("%d%d%d%d%d%d_%d"
		, curtime.GetYear()
		, curtime.GetMonth()
		, curtime.GetDay()
		, curtime.GetHour()
		, curtime.GetMinute()
		, curtime.GetSecond()
		, m_modelIdCounter);
		//, (int)(center.x()*1000)
		//, (int)(center.y()*1000)
		//, (int)(center.z()*1000) );
	++m_modelIdCounter;
	m_id = (LPCTSTR)tempStr;
	m_runTimeSeperator = VGlobalValues::m_runTimeSeperator;
}

const bool ModelID::operator == (const ModelID &id)const 
{
	//if (m_birthdate != id.m_birthdate)
	//	return false;
	//else if (m_birthplace != id.m_birthplace)
	//	return false;
	//return true;
	return m_id == id.m_id && m_runTimeSeperator == id.m_runTimeSeperator;
}
const bool ModelID::operator != (const ModelID &id)const
{
	return ! this->operator == (id);
}
void ModelID::Serialize (Bofstream &outfile)const
{
	int currversion=3;
	outfile.WriteInt (currversion);
	//if(currversion==1)
	//{
	//	outfile.WriteUnsInt ((unsigned int)m_birthplace);
	//	outfile.WriteTime (m_birthdate);
	//	void * tmpp = 0;
	//	outfile.WriteVoidPtr (&tmpp);//(&m_model);
	//}
	//if (currversion==2)
	//{
	//	outfile.WriteSizeT (m_birthplace);
	//	outfile.WriteTime (m_birthdate);
	//	void *tmpp = 0;
	//	outfile.WriteVoidPtr (&tmpp);
	//}
	//outfile.WriteTime (m_birthdate);
	//outfile.write ((VChr *)&m_birthplace , sizeof (LONGLONG));
	outfile.WriteString (m_id);
}

void ModelID::Serialize (Bifstream &infile)
{
	int currversion;
	infile.ReadInt (currversion);
	//if(currversion==1)
	//{
	//	infile.ReadUnsInt ((unsigned int)m_birthplace);
	//	infile.ReadTime (m_birthdate);
	//	//infile.ReadVoidPtr (&m_model);
	//	void *tmpptr = 0;
	//	infile.ReadVoidPtr (&tmpptr);
	//}
	//else if (currversion == 2)
	//{
	//	infile.ReadSizeT (m_birthplace);
	//	infile.ReadTime (m_birthdate);
	//	//infile.ReadVoidPtr (&m_model);	
	//	void *tmpptr = 0;
	//	infile.ReadVoidPtr (&tmpptr);
	//}
	//if (currversion == 2)
	//{
	//	infile.ReadTime (m_birthdate);
	//	infile.read ((VChr *)&m_birthplace , sizeof (LONGLONG));
	//}
	if (currversion >= 3)
	{
		infile.ReadString (m_id);	
	}
	//else 
	//	throw VException (( IDS_ModSerExcOldVer),1);
	//AddCenterAndRadiusIf();
}

//void * ModelID::GetModelPtr()const
//{
//	return m_model;
//}
//
//void ModelID::SetModelPtr (void *model)
//{
//	m_model = model;
//}

//const VTime & ModelID::GetBirthDate() const
//{
//	return m_birthdate;
//}

//void ModelID::SetBirthDate(const ATL::VTime &bd)
//{
//	m_birthdate = bd;
//}

//LONGLONG ModelID::GetBirthPlace() const
//{
//	return m_birthplace;
//}

bool ModelID::operator <(const ModelID &second) const
{
	//if (m_birthplace < second.m_birthplace)
	//	return true;
	//else if (m_birthplace > second.m_birthplace)
	//	return false;
	//else
	//{
	//	if (m_birthdate  < second.m_birthdate)
	//		return true;
	//	else
	//		return false;
	//}
	return m_id < second.m_id;
}

bool ModelID::operator >(const ModelID &second) const
{
	return ! (this->operator < (second));
}

void ModelID::SetAsNull()
{
	m_id = ("0");
}

bool ModelID::IsNull ()const
{
	return m_id == ("0");
}

const VStr & ModelID::GetIdString()const
{
	return m_id;
}

int ModelID::GetCounterNumber()const
{
	//std::vector<VStr> tmpVec;
	//TypeConverters::SplitString(m_id, ';', tmpVec);
	//VStr oldid = tmpVec.at(0);
	VStr counterStr = m_id.substr (m_id.rfind (("_"))+1,m_id.size());
	VStrStream mystr;
	mystr << counterStr;
	int myresult;
	mystr >> myresult;
	return myresult;
}

void ModelID::SetIdString(const VStr &idStr)
{
	m_id = idStr;
	//AddCenterAndRadiusIf();
}

//void ModelID::ResetCenterAndRadius(const VPoint &cen, vreal rad)
//{
//	std::vector<VStr> tmpVec;
//	TypeConverters::SplitString(m_id, ';', tmpVec);
//	VStr oldid = tmpVec.at(0);
//	CString temp;
//	temp.Format(";%d;%d;%d;%d", (int)(cen.x()*1000), (int)(cen.y()*1000), (int)(cen.z()*1000));
//	m_id = oldid;
//	m_id += (LPCTSTR)(temp);
//}
//
//void ModelID::GetCenterAndRadius(VPoint &cen, vreal &rad)const
//{
//	std::vector<VStr> tmpVec;
//	TypeConverters::SplitString(m_id, ';', tmpVec);
//	vreal x = TypeConverters::ConvertStringTovreal(tmpVec.at(1));
//	x *= 0.001;
//	vreal y = TypeConverters::ConvertStringTovreal(tmpVec.at(2));
//	y *= 0.001;
//	vreal z = TypeConverters::ConvertStringTovreal(tmpVec.at(3));
//	z *= 0.001;
//	vreal r = TypeConverters::ConvertStringTovreal(tmpVec.at(4));
//	r *= 0.001;
//	cen = VPoint(x, y, z);
//	rad = r;
//}
//
//void ModelID::AddCenterAndRadiusIf()
//{
//	std::vector<VStr> tmpVec;
//	TypeConverters::SplitString(m_id, ';', tmpVec);
//	if (tmpVec.size() != 5)
//	{
//		VPoint cen(0.0);
//		vreal rad(0.0);
//		CString temp;
//		temp.Format(";%d;%d;%d;%d", (int)(cen.x() * 1000), (int)(cen.y() * 1000), (int)(cen.z() * 1000));
//		m_id += (LPCTSTR)(temp);
//	}
//}

unsigned int ModelID::GetRuntimeSeperator()const
{
	return m_runTimeSeperator;
}
