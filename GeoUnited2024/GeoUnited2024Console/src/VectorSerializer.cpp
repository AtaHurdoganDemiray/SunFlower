// VectorSerializer.cpp

#include "stdafx.h"
#include "VectorSerializer.h"
#include "TypeConverters.h"
#include "UntypedException.h"
#include "ExceptionLogger.h"
#include <stdexcept>

const int MAX_SER_VEC_SIZE = 1000000;
bool VectorSerializer::IsSizeValid (const int mysize)
{
	if (mysize < 0 || mysize > MAX_SER_VEC_SIZE)
		return false;
	return true;
}

void VectorSerializer::SerializeStringVec (Bifstream &f,std::vector<VStr> &vec)
{
	try
	{
		int mysize;
		//f.ReadInt (mysize);
		f.read ((VChr *)&mysize , sizeof (int));
		//if (IsSizeValid (mysize) == false)
		//	throw VException (("Vector serialize exception"),1);
		for (int i=0;i<mysize;++i)
		{
			VStr tmp;
			f.ReadString (tmp);
			vec.push_back (tmp);
		}
	}
	catch (VException &e)
	{
		throw e;
	}
	catch (std::exception &e)
	{
		throw e;
	}
	catch(...)
	{
		throw VException (( IDS_SerUnkExc),1);
		//UntypedException uexc(("Unknown xception in SerializeStringVec"),999);
		//ExceptionLogger::log(uexc, __FILE__, __LINE__);
	}
}
void VectorSerializer::SerializeStringVec (Bofstream &f,const std::vector<VStr> &vec)
{
	try
	{
		int mysize = (int)vec.size();
		//f.WriteInt (mysize);
		f.write((VChr *)&mysize , sizeof (int));

		for (int i=0;i<mysize;++i)
			f.WriteString (vec[i]);
	}
	catch (VException &e)
	{
		throw e;
	}
	catch (std::exception &e)
	{
		throw e;
	}
	catch(...)
	{
		throw VException (( IDS_SerUnkExc1),1);
		//UntypedException uexc(("Unknown xception in SerializeStringVec"),999);
		//ExceptionLogger::log(uexc, __FILE__, __LINE__);
	}
}
void VectorSerializer::SerializeStringVec (VofStream &f,const std::vector<VStr> &vec)
{
	try
	{
		size_t mysize = vec.size();
		f << mysize << ( IDS_N);
		for (size_t i=0;i<mysize;++i)
		{
			f << vec[i].c_str() << ( IDS_N);
		}
	}
	catch (VException &e)
	{
		throw e;
	}
	catch (std::exception &e)
	{
		throw e;
	}
	catch(...)
	{
		throw VException (( IDS_SerUnkExc2),1);
		//UntypedException uexc(("Unknown xception in SerializeStringVec"),999);
		//ExceptionLogger::log(uexc, __FILE__, __LINE__);
	}
}
void VectorSerializer::SerializeStringVec (VIFStream &f,std::vector<VStr> &vec)
{
	try
	{
		size_t mysize;
		f >> mysize;
		VChr tmp[2048];
		for (size_t i=0;i<mysize;++i)
		{
			f >> tmp;
			vec.push_back (tmp);
		}
	}
	catch (VException &e)
	{
		throw e;
	}
	catch (std::exception &e)
	{
		throw e;
	}
	catch(...)
	{
		throw VException (( IDS_SerUnkExc3),1);
		//UntypedException uexc(("Unknown xception in SerializeStringVec"),999);
		//ExceptionLogger::log(uexc, __FILE__, __LINE__);
	}
}

void VectorSerializer::SerializeVecStrVec (const std::vector<std::vector<VStr>> &vecvec ,const std::vector<std::vector<std::map<VStr,VStr>>> &vecVecPropertiesMap , VofStream &f)
{
	int rowCount = (int)vecvec.size();
	f <<  IDS_RowCount <<  IDS_N;
	f << rowCount;
	for (int i=0;i<rowCount;++i)
	{
		const std::vector<VStr> &curRow = vecvec.at(i);
		VStr oneRow;
		TypeConverters::MergeString (curRow , '|' , oneRow);
		f << oneRow.c_str() <<  IDS_N;
	}
}

void VectorSerializer::SerializeVecStrVec (std::vector<std::vector<VStr>> &vecvec , std::vector<std::vector<std::map<VStr,VStr>>> &vecVecPropertiesMap , VIFStream &f)
{
	char temp[1000];
	int rowCount = 0;
	f.getline(temp,1000,'\n'); // "Row count:" << "\n";
	f.getline (temp,1000,'\n');
	VStr tempstr (temp);
	double temp2 = TypeConverters::ConvertStringTovreal(tempstr);
	rowCount = (int)temp2;
	for (int i=0;i<rowCount;++i)
	{
		f.getline (temp,1000,'\n');
		VStr oneRow (temp);
		std::vector<VStr> oneRowVec;
		TypeConverters::SplitString (oneRow,'|',oneRowVec);
		std::vector<VStr> processedRowVec;
		size_t mysize = oneRowVec.size();
		vecVecPropertiesMap.push_back (std::vector<std::map<VStr,VStr>>());

		for (size_t j=0;j<mysize;++j)
		{
			VStr curCell = oneRowVec.at(j);
			std::map<VStr,VStr> curPropertiesMap;
			std::map<char,std::vector<int>> myCharPlaces;
			myCharPlaces.insert(std::pair<char,std::vector<int>>('<',std::vector<int>()));
			myCharPlaces.insert(std::pair<char,std::vector<int>>('>',std::vector<int>()));
			TypeConverters::DetectPlaces (curCell , 0 , curCell.length()-1 , myCharPlaces);
			std::map<char,std::vector<int>>::iterator fi = myCharPlaces.find ('<');
			std::map<char,std::vector<int>>::iterator fii = myCharPlaces.find ('>');
			if (fi != myCharPlaces.end() && fii != myCharPlaces.end())
			{
				if (fi->second.size() && fii->second.size())
				{
					int propBegini = fi->second.at(0);
					int propEndi = fii->second.at(0);
					if (propEndi != (curCell.size()-1))
						throw VException ( IDS_VecSerExcProSho,1);
					VStr myKeyStr = TypeConverters::GetSubSet (curCell , 0 , propBegini);
					if (myKeyStr.size() == 0)
						throw VException ( IDS_VecSerExcKeyStr,1);
				
					VStr myPropertiesStr = TypeConverters::GetSubSet (curCell , propBegini , propEndi);
					processedRowVec.push_back (myKeyStr);
					std::vector<VStr> propertiesForOneCell;
					TypeConverters::SplitString (myPropertiesStr , ';' , propertiesForOneCell);
					size_t mysize5 = propertiesForOneCell.size();
					for (size_t jjj = 0; jjj<mysize5;++jjj)
					{
						VStr curProp = propertiesForOneCell.at(jjj);
						std::vector<VStr> curKeyVal;
						TypeConverters::SplitString (curProp , ':' , curKeyVal);
						if (curKeyVal.size() != 2)
							throw VException ( IDS_VecSerExcProKey,1);
						curPropertiesMap.insert (std::pair<VStr,VStr> (curKeyVal.at(0),curKeyVal.at(1)));
					}
				}
				else
				{
					processedRowVec.push_back (curCell);
				}
				vecVecPropertiesMap.back().push_back (curPropertiesMap);
			}
		}
		vecvec.push_back (processedRowVec);
	}
}

void VectorSerializer::SerializeBoolVecVec (const std::vector<std::vector<bool>> &toWrite , Bofstream &f)
{
	int mysizey = (int)toWrite.size();
	f.WriteInt (mysizey);
	for (int y=0;y<mysizey;++y)
	{
		const std::vector<bool> &curVec = toWrite.at(y);
		SerializeBasicTypeVec <bool> (f,curVec);
	}
}

void VectorSerializer::SerializeBoolVecVec (std::vector<std::vector<bool>> &toRead , Bifstream &f)
{
	int myysize;
	f.ReadInt (myysize);
	for (int y=0;y<myysize;++y)
	{
		std::vector<bool> curVec;
		SerializeBasicTypeVec <bool> (f , curVec);
		toRead.push_back (curVec);
	}
}