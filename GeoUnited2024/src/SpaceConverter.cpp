// SpaceConverter.cpp

#include "stdafx.h"
#include "SpaceConverter.h"
#include "VPoint.h"


void SpaceConverter::ConvertLoop(const std::vector<VPoint> &src, std::vector<VPoint> &dest)
{
	size_t mysize = src.size();
	size_t cksize = dest.size();
	if (mysize != cksize)
		throw VException (( IDS_SpaConExcSouAnd),1);

	for (size_t i=0;i<mysize;++i)
	{
		dest[i] = Convert(src[i]);
	}
}

bool SpaceConverter::ConvertLoopIfInside(const std::vector<VPoint> &src, std::vector<VPoint> &dest, double tol)
{
	try
	{
		size_t mysize = src.size();
		size_t cksize = dest.size();
		if (mysize != cksize)
			throw VException (( IDS_SpaConExcSouAnd),1);

		for (size_t i=0;i<mysize;++i)
		{
			dest[i] = ConvertThrowIfOut(src[i],tol);
		}

		return true;
	}
	catch (VException  &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_SpaConExc));
		LogFileWriter::WriteExceptionLog (e);
		return false;
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_SpaConExc));		
		LogFileWriter::WriteExceptionLog (e);
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_SpaConUnkExc));
		return false;
	}
}
