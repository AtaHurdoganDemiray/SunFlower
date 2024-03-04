// TransformationMatrixAnalyzer.cpp

#include "stdafx.h"
#include "TransformationMatrixAnalyzer.h"
#include "TransformationMatrix.h"
#include "VLine.h"

bool TransformationMatrixAnalyzer::IsUniform(const TransformationMatrix &tm)
{
	// Make 3 lines and transform all
	VLine lx (VPoint(0.),UX);
	VLine ly (VPoint(0.),UY);
	VLine lz (VPoint(0.),UZ);
	Transform3AxLines(lx,ly,lz,tm);
	if (fabs(lx.GetLength()-ly.GetLength())>1.e-10
		|| fabs (lx.GetLength()-lz.GetLength()) > 1.e-10)
		return false;
	return true;
}

void TransformationMatrixAnalyzer::GetScaleFactors(const TransformationMatrix &tm, double &xscale, double &yscale, double &zscale)
{
	VLine lx (VPoint(0.0),UX);
	VLine ly (VPoint(0.0),UY);
	VLine lz (VPoint(0.0),UZ);
	Transform3AxLines(lx,ly,lz,tm);

	xscale = lx.GetLength();
	yscale = ly.GetLength();
	zscale = lz.GetLength();
}

bool TransformationMatrixAnalyzer::IsMirror(const TransformationMatrix &tm)
{
	VLine lx (VPoint(0.0),UX);
	VLine ly (VPoint(0.0),UY);
	VLine lz (VPoint(0.0),UZ);
	Transform3AxLines(lx,ly,lz,tm);
	
	if (( (lx.GetV() ^ ly.GetV()) % lz.GetV()) > 0.0)
		return false;
	else
		return true;
}

void TransformationMatrixAnalyzer::Transform3AxLines(VLine &lx, VLine &ly, VLine &lz, const TransformationMatrix &tm)
{
	lx = tm.Transform(lx);
	ly = tm.Transform(ly);
	lz = tm.Transform(lz);
}
