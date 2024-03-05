// VectorSerializer.h

#ifndef __VectorSerializer_h__
#define __VectorSerializer_h__


#include "UntypedException.h"
#include "ExceptionLogger.h"
#include <stdexcept>

class VectorSerializer
{
public:
	static bool IsSizeValid (const int mysize);
	static void SerializeStringVec (Bifstream &f,std::vector<VStr> &vec);
	static void SerializeStringVec (Bofstream &f,const std::vector<VStr> &vec);
	static void SerializeStringVec (VofStream &f,const std::vector<VStr> &vec);
	static void SerializeStringVec (VIFStream &f,std::vector<VStr> &vec);
	static void SerializeVecStrVec (const std::vector<std::vector<VStr>> &vecvec , const std::vector<std::vector<std::map<VStr,VStr>>> &vecVecPropertiesMap, VofStream &f);
	static void SerializeVecStrVec (std::vector<std::vector<VStr>> &vecvec , std::vector<std::vector<std::map<VStr,VStr>>> &vecVecPropertiesMap,VIFStream &f);
	
	template <class T> static void SerializeBasicTypeVec(Bofstream &outfile,const std::vector<T> &basictypevec)
	{
		try
		{
			int mysize=(int)basictypevec.size();
			outfile.write((VChr*)&mysize,sizeof(int));
			for(int i=0;i<mysize;i++)
			{
				T currt=basictypevec[i];
				outfile.write((VChr*)&currt,sizeof(T));
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
		catch (...)
		{	
			throw VException ("SerializeBasicTypeVec<T> exception , unknown exception 1",1);
			//UntypedException uexc("Unkown exception in SerializeBasicTypeVec",999);
			//ExceptionLogger::log(uexc, __FILE__, __LINE__);
		}
	};

	template <class T> static void SerializeBasicTypeVec(Bifstream &infile,std::vector<T> &basictypevec)
	{
		try
		{
			int mysize;
			infile.read((VChr*)&mysize,sizeof(int));
			//if (IsSizeValid (mysize) == false)
			//	throw VException ("Vector serialize exception",1);
			for(int i=0;i<mysize;i++)
			{
				T currt;
				infile.read((VChr*)&currt,sizeof(T));
				basictypevec.push_back(currt);
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
		catch (...)
		{	
			throw VException ("SerializeBasicTypeVec<T> unknown exception 2!",1);
			//UntypedException uexc("Unkown exception in SerializeBasicTypeVec",999);
			//ExceptionLogger::log(uexc, __FILE__, __LINE__);
		}
	};

	template <class T> static void SerializePtrVec (Bofstream &outfile,const std::vector<T*> &ptrvec)
	{
		try
		{
			int myvecsize=(int)ptrvec.size();
			outfile.write((VChr*)&myvecsize,sizeof(int));
			for (int i=0;i<myvecsize;i++)
			{
				T *currp=ptrvec[i];
				size_t myptr=(size_t)(currp);
				outfile.write((VChr*)&myptr,sizeof(size_t));	
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
		catch (...)
		{	
			throw VException ("SerializePtrVec unknown exception 3",1);
			//UntypedException uexc("Unkown exception in SerializePtrVec",999);
			//ExceptionLogger::log(uexc, __FILE__, __LINE__);
		}
	}

	template <class T> static void SerializePtrVec(Bifstream &infile,std::vector<T*> &ptrvec)
	{
		try
		{
			int myvecsize;
			infile.read((VChr*)&myvecsize,sizeof(int));
			//if (IsSizeValid (myvecsize) == false)
			//	throw VException ("Vector serialize exception",1);
			for (int i=0;i<myvecsize;i++)
			{
				size_t myptr;
				infile.read((VChr*)&myptr,sizeof(size_t));	
				T *currp=(T*)(myptr);
				ptrvec.push_back(currp);
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
		catch (...)
		{	
			throw VException ("SerializePtrVec unknown exception 5",4);
			//UntypedException uexc("Unkown exception in SerializePtrVec",999);
			//ExceptionLogger::log(uexc, __FILE__, __LINE__);
		}
	};

	template <class T> static void SerializeTVec (Bofstream &outfile,const std::vector<T> &tvec)
	{
		try
		{
			int myvecsize=(int)tvec.size();
			outfile.write((VChr*)&myvecsize,sizeof(int));
			for (int i=0;i<myvecsize;i++)
			{
				const T & currt = tvec[i];
				currt.Serialize(outfile);
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
		catch (...)
		{
			throw VException ("SerializeTVec<T> unknown exception 6",1);
		}
	};
	template <class T> void static SerializeTVec (Bifstream &infile,std::vector<T> &tvec)
	{
		try
		{
			int myvecsize;
			infile.read((VChr*)&myvecsize,sizeof(int));
			//if (IsSizeValid (myvecsize) == false)
			//	throw VException ("Vector serialize exception",1);
			for (int i=0;i<myvecsize;i++)
			{
				T currt;
				currt.Serialize(infile);
				tvec.push_back(currt);
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
		catch (...)
		{	
			throw VException ("SerializeTVec<T> unknown exception 7",1);
			//UntypedException uexc("Unkown exception in SerializeTVec",999);
			//ExceptionLogger::log(uexc, __FILE__, __LINE__);
		}
	};
	template <class T> static void SerializeTVec001 (Bifstream &infile,std::vector<T> &tvec)
	{
		try
		{
			int initialcapacity;
			infile.read((VChr*)&initialcapacity,sizeof(int));
			int myvecsize;
			infile.read((VChr*)&myvecsize,sizeof(int));
			//if (IsSizeValid (myvecsize) == false)
			//	throw VException ("Vector serialize exception",1);
			int myveccapacity;
			infile.read((VChr*)&myveccapacity,sizeof(int));
			for (int i=0;i<myvecsize;i++)
			{
				T currt;
				currt.Serialize(infile);
				tvec.push_back(currt);
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
		catch (...)
		{	
			throw VException ("SerializeTVec001<T> unknown exception 8",1);
			//UntypedException uexc("Unkown exception in SerializeTVec001",999);
			//ExceptionLogger::log(uexc, __FILE__, __LINE__);
		}
	};
	template <class T> static void SerializeTEntityStarVec (Bofstream &f , const std::vector<const T*> &tvec)
	{
		try
		{
			int mysize = (int)tvec.size();
			//f.WriteInt (mysize);
			f.write((VChr*)&mysize,sizeof(int));
			for (int i=0;i<mysize;++i)
			{
				const T * currstar = tvec[i];
				currstar->StartSerialize(f);
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
		catch (...)
		{	
			throw VException ("SerializeTEntityStarVec unknown exception 9",1);
			//UntypedException uexc("Unkown exception in SerializeTEntityStarVec",999);
			//ExceptionLogger::log(uexc, __FILE__, __LINE__);
		}
	};
	template <class T> static void SerializeTEntityStarVec (Bifstream &f , std::vector <const T*> &tvec)
	{
		try
		{
			size_t mysize;
			//f.ReadInt (mysize);
			f.read((VChr *)&mysize , sizeof (int));
			//if (IsSizeValid (mysize) == false)
			//	throw VException ("Vector serialize exception",1);

			for (size_t i=0;i<mysize;++i)
			{
				T *newstar = new T;
				newstar->StartSerialize (f);
				tvec.push_back (newstar);
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
		catch (...)
		{	
			throw VException ("SerializeTEntityStarVec unknown exception 10",1);
			//UntypedException uexc("Unkown exception in SerializeTEntityStarVec",999);
			//ExceptionLogger::log(uexc, __FILE__, __LINE__);
		}
	};
	static void SerializeBoolVecVec (const std::vector<std::vector<bool>> &toWrite , Bofstream &f);
	static void SerializeBoolVecVec (std::vector<std::vector<bool>> &toRead , Bifstream &f);
};
#endif

