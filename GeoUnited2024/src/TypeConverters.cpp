// TypeConverters.cpp

#include "stdafx.h"
#include "TypeConverters.h"
#include "TransformationMatrix.h"


VStr TypeConverters::exm1 = ( IDS_KeyNotFound);

VStr TypeConverters::ConvertvrealToString (const vreal &myscaler)
{
	VOStrStream mystream;
	mystream << std::setiosflags (std::ios_base::scientific);
	mystream << std::setprecision(VGlobalValues::DOUBLE_SIGNIFICANT_DIGITS)<< myscaler;
	VStr result = mystream.str();
	return result;
}
VStr TypeConverters::ConvertFloatToString (const float &myscaler)
{
	VOStrStream mystream;
	mystream << myscaler;
	VStr result = mystream.str();
	return result;
}
VStr TypeConverters::ConvertIntToString (const int &myinteger)
{
	VOStrStream mystream;
	mystream  /*std::setprecision(NUMBER_OF_DIGITS_AFTER_DECIMAL_POINT)*/ << myinteger;
	VStr result = mystream.str();
	return result;
}
vreal TypeConverters::ConvertStringTovreal (const VStr &mystr)
{
	VChr * errorAdress;
	vreal result = strtod(mystr.data() , & errorAdress);
	return result;
}
void TypeConverters::SplitString (const VStr &src, const VChr seperator, std::vector<VStr> &result)
{
	size_t mysize = src.length();
	VStr s;
	int quotCounter = 0;
	for (size_t i=0;i<mysize;++i)
	{
		VChr myc = src[i];
		if (myc == '\"')
			++quotCounter;
		if (myc == seperator && quotCounter % 2 == 0)
		{
			if (s.length())
			{
				result.push_back(s);
				s.clear();
			}
		}
		else
		{
			s+=myc;
		}
	}
	if (s.length())
		result.push_back(s);
}
void TypeConverters::MergeString (const std::vector<VStr> &src, const VChr seperator,  VStr &result)
{
	size_t mysize = src.size();
	for (size_t i=0;i<mysize;++i)
	{
		const VStr &curstr  = src[i];
		result += curstr;
		result += seperator; 
	}
}
bool TypeConverters::DetectPlaces(const VStr &str, int bi, int ei, std::map<char,std::vector<int>>&charPlaceIndexMap)
{
	try
	{
		for (int i=bi;i<=ei;++i)
		{
			char ci = str.at(i);
			std::map<char,std::vector<int>>::iterator fi = charPlaceIndexMap.find (ci);
			if (fi != charPlaceIndexMap.end())
			{
				fi->second.push_back (i);
			}
		}
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_TypDetVpExc));
		LogFileWriter::WriteExceptionLog (e);
		return false;
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_TypDetStdExc));
		LogFileWriter::WriteExceptionLog (e);
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_TypDetUnkExc));
		return false;
	}
}

VStr TypeConverters::GetSubSet (const VStr &str , int bi , int ei)
{
	try
	{
		VStr substr;
		for (int i=bi;i<=ei;++i)
		{
			substr += str.at(i);
		}
		return substr;
	}
	catch (VException &e)
	{
		VStr dummystr;
		LogFileWriter::WriteExceptionLog (e, IDS_TypGetVpE);
		return dummystr;
	}
	catch (std::exception &e)
	{
		VStr dummystr;
		LogFileWriter::WriteExceptionLog (e, IDS_TypGetStdE);
		return dummystr;
	}
	catch (...)
	{
		VStr dummystr;
		LogFileWriter::WriteUnknownExceptionLog ( IDS_UnkExc1, IDS_TypUnkE);
		return dummystr;
	}
}

//bool TypeConverters::RemoveCharsAtStartAndEnd (char cToRemove , VStr &str)
//{
//	std::map<char,std::vector<int>> charPlaceIndexMap;
//	charPlaceIndexMap.insert (std::pair<char,std::vector<int>> (cToRemove,std::vector<int>()));
//	TypeConverters::DetectPlaces (str,0,str.length()-1,charPlaceIndexMap);
//	std::vector<int> &v = charPlaceIndexMap.begin()->second;
//	if (v.size() < 2)
//	{
//		return false;
//	}
//	else
//	{
//		str = TypeConverters::GetSubSet (str,v.at(0),v.at(1));
//		return true;
//	}
//}

bool TypeConverters::RemoveCharsAtStartAndEnd ( char cToRemove , VStr &str)
{	
	if (str.length() == 0)
		return false;
	int bi = 0;
	while (str.at(bi) == cToRemove)
		++bi;
	int mysize = (int)(str.length())-1;
	int ei = mysize;
	while (str.at(ei) == cToRemove)
		ei--;
	VStr newStr;
	for (int i=bi;i<=ei;++i)
		newStr += str.at(i);
	str = newStr;
	return !(bi==0 && ei==mysize);
}

void TypeConverters::AddCharsAtStartAndEnd (char cToAdd , VStr &str)
{
	VStr snew;
	snew += cToAdd;
	snew += str;
	snew += cToAdd;
	str = snew;
}
bool TypeConverters::RemoveCharsAtStartAndEndForEveryStr (char cToRemove , std::vector<VStr> &sVec)
{
	size_t mysize = sVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		VStr &s = sVec.at(i);
		bool ck = RemoveCharsAtStartAndEnd (cToRemove , s);
		if (!ck)
		{
			LogFileWriter::WriteUnknownExceptionLog (( IDS_CharNotExisting),( IDS_TypRem));
			return false;
		}
	}
	return true;
}
void TypeConverters::AddCharsAtStartAndEndForEveryStr (char cToAdd , std::vector<VStr> &sVec)
{
	size_t mysize = sVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		VStr &s = sVec.at(i);
		AddCharsAtStartAndEnd (cToAdd,s);
	}
}

int TypeConverters::GetInt (const std::map<VStr,VStr> &argMap , const VStr &nameStr)
{
	itrs fi1 = argMap.find (nameStr);
	if (fi1 == argMap.end())
		throw VException (exm1+nameStr,1);
	VStrStream s1;
	s1 << fi1->second;
	int res;
	s1 >> res;
	return res;
}

vreal TypeConverters::GetScalar (const std::map<VStr,VStr> &argMap , const VStr &nameStr)
{
	itrs fi1 = argMap.find (nameStr);
	if (fi1 == argMap.end())
		throw VException (exm1+nameStr,1);
	VStrStream s1;
	s1 << fi1->second;
	vreal res;
	s1 >> res;
	return res;
}

VPoint TypeConverters::GetPoint (const std::map<VStr,VStr> &argMap , const VStr &nameStr)
{
	itrs fi1 = argMap.find (nameStr);
	if (fi1 == argMap.end())
		throw VException (exm1+nameStr,1);
	VStrStream s1;
	s1 << fi1->second;
	VStr temp (s1.str());
	std::vector<VStr> spVec;
	SplitString (temp,SEMICOLON_CHR,spVec);
	if (spVec.size() != 3)
		throw VException (( IDS_TypGetExcNumNot),1);
	VStrStream ps[3];
	VPoint p;
	for (int i=0;i<3;++i)
	{
		ps[i] << spVec.at(i);
		ps[i] >> p[i];
	}
	return p;
}
TransformationMatrix TypeConverters::GetMatrix(const std::map<VStr, VStr> &argMap, const VStr &nameStr)
{
	TransformationMatrix tm;
	itrs fi1 = argMap.find(nameStr);
	if (fi1 == argMap.end())
		throw VException(exm1 + nameStr, 1);
	std::vector<VStr> spVec;
	SplitString(fi1->second , SEMICOLON_CHR, spVec);
	if (spVec.size() != 16)
		throw VException("TransformationMatrix::TypeConverters", 1);
	for (int i = 0; i < 16; ++i)
	{
		vreal curVal = TypeConverters::ConvertStringTovreal(spVec.at(i));
		tm.GetMatrixArr()[i] = curVal;
	}
	return tm;
}
VLine TypeConverters::GetLine (const std::map<VStr,VStr> &argMap , const VStr &nameStr)
{
	VStr ss1 = nameStr + (( IDS_P));
	VStr ss2 = nameStr + (( IDS_P1));
	VPoint p1 = GetPoint (argMap , ss1);
	VPoint p2 = GetPoint (argMap , ss2);
	return VLine (p1,p2);
}

VArc TypeConverters::GetArc (const std::map<VStr,VStr> &argMap , const VStr &nameStr)
{
	VStr ss1 = nameStr + (( IDS_NormalVector));
	VStr ss2 = nameStr + (( IDS_Center));
	VStr ss3 = nameStr + (( IDS_StartPoint));
	VStr ss4 = nameStr + (( IDS_SweepAngle));
	VStr ss5 = nameStr + (( IDS_Radius));
	VPoint normalVector = GetPoint (argMap , ss1);
	VPoint center = GetPoint ( argMap , ss2);
	VPoint startPoint = GetPoint (argMap , ss3);
	vreal sweepAngle = GetScalar (argMap , ss4);
	vreal radius = GetScalar (argMap , ss5);
	return VArc (center , radius , normalVector , startPoint , sweepAngle);
}

void TypeConverters::StrVecToMap (const std::vector<VStr> &strVec , std::map<VStr,VStr> &keyValueMap)
{
	int mysize = (int)strVec.size();
	if (mysize % 2 != 0)
		throw VException (( IDS_TypStrExcVecSiz),1);
	for (int i=0;i<mysize;i=i+2)
	{
		keyValueMap.insert ( std::pair<VStr,VStr> (strVec.at(i) , strVec.at(i+1)));
	}
}

void TypeConverters::MapToStrVec (const std::map<VStr,VStr> &keyValueMap , std::vector<VStr> &strVec)
{
	std::map<VStr,VStr>::const_iterator i = keyValueMap.begin();
	std::map<VStr,VStr>::const_iterator e = keyValueMap.end();
	VStr res;
	for (i;i!=e;++i)
	{
		strVec.push_back (i->first);
		strVec.push_back (i->second);
	}
}

VStr TypeConverters::GetString (const std::map<VStr,VStr> &argMap , const VStr &keyName)
{
	std::map<VStr,VStr>::const_iterator fi = argMap.find (keyName);
	if (fi == argMap.end())
	{
		VStr msgStr (( IDS_TypGetValNotFou));
		msgStr += keyName;
		throw VException (msgStr,1);
	}
	return fi->second;
}

bool TypeConverters::DetectPlaces (const VStr &str , int bi , int ei , std::map<VStr,std::vector<int>> &strBeginIndexMap)
{
	try
	{
		int strsize = (int)str.size();
		if (bi < 0 || bi >= strsize)
			throw VException ("TypeConverters::DetectPlaces exception bi out of string size range!",1);
		if (ei < 0 || ei >= strsize)
			throw VException ("TypeConverters::DetectPlaces exception ei out of string size range!",1);
	
		VStr curTotal;
		curTotal.reserve (str.size());
		// for each string in map
		std::map<VStr,std::vector<int>>::iterator mi=strBeginIndexMap.begin();
		std::map<VStr,std::vector<int>>::iterator me = strBeginIndexMap.end();
		for (mi;mi!=me;++mi)
		{
			mi->second.clear();
			VStr curSearchString = mi->first;
			if (curSearchString.size() > str.size())
				continue;
			for (int i=bi;i<=ei;++i)
			{
				curTotal = "";
				int je = (i+(int)curSearchString.size())-1;
				if (je > ei)
					continue;
				// get sub string
				for (int j=i;j<=je;++j)
				{
					char curChr = str.at(j);
					curTotal += curChr;
				}
				if (curTotal == curSearchString)
				{
					mi->second.push_back (i);
				}
			}	 
		}
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog (e,"TypeConverters::DetectPlaces exception");
		return false;
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog (e,"TypeConverters::DetectPlaces exception");
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog ("Unknown exception","TypeConverters::DetectPlaces");
		return false;
	}

}

std::vector<vreal> TypeConverters::GetRealArray (const VStr &line , VChr sep)
{
	std::vector<vreal> res;
	std::vector<VStr> spVec;
	SplitString (line , sep , spVec);
	int mysize = (int)spVec.size();
	for (int i=0;i<mysize;++i)
	{
		const VStr &curStr = spVec[i];
		VStrStream str;
		str << curStr;
		vreal curVal;
		str >> curVal;
		res.push_back (curVal);
	}
	return res;
}

std::vector<int> TypeConverters::GetIntArray (const VStr &line , VChr sep)
{
	std::vector<int> res;
	std::vector<VStr> spVec;
	SplitString (line , sep , spVec);
	int mysize = (int)spVec.size();
	for (int i=0;i<mysize;++i)
	{
		const VStr &curStr = spVec[i];
		VStrStream str;
		str << curStr;
		int curVal;
		str >> curVal;
		res.push_back (curVal);
	}
	return res;
}
VStr TypeConverters::ConvertvrealToString2(const vreal &val)
{
	VOStrStream str;
	str << val;;
	VStr result = str.str();

	return result;
}
int TypeConverters::ConvertStringToInt(const VStr val)
{
	VStrStream s1;
	s1 << val;
	int res;
	s1 >> res;
	return res;
}
VPoint TypeConverters::GetPoint(const std::map<VStr, VStr> &argMap, const VStr &nameStr, const char &seperator)
{
	itrs fi1 = argMap.find(nameStr);
	if (fi1 == argMap.end())
		throw VException(exm1 + nameStr, 1);
	VStrStream s1;
	s1 << fi1->second;
	VStr temp(s1.str());
	std::vector<VStr> spVec;
	SplitString(temp, seperator, spVec);
	if (spVec.size() != 3)
		throw VException((IDS_TypGetExcNumNot), 1);
	VStrStream ps[3];
	VPoint p;
	for (int i = 0; i<3; ++i)
	{
		ps[i] << spVec.at(i);
		ps[i] >> p[i];
	}
	return p;
}
VStr TypeConverters::PointToString(const VPoint & p, const char &seperator)
{
	VStr x = ConvertvrealToString2(p.x());
	VStr y = ConvertvrealToString2(p.y());
	VStr z = ConvertvrealToString2(p.z());

	VStr strP = x + seperator + y + seperator + z;
	return strP;

}

VPoint TypeConverters::ConvertStringToPoint(VStr val, char seperator)
{
	VPoint rp;
	std::vector<VStr> splitvec;
	TypeConverters::SplitString(val, seperator, splitvec);
	if (splitvec.size() >= 3)
	{
		rp.x(TypeConverters::ConvertStringTovreal(splitvec.at(0)));
		rp.y(TypeConverters::ConvertStringTovreal(splitvec.at(1)));
		rp.z(TypeConverters::ConvertStringTovreal(splitvec.at(2)));
	}
	return rp;
}
