// TransformationMatrixAnalyzer.h

#ifndef __TransformationMatrixAnalyzer_h__
#define __TransformationMatrixAnalyzer_h__

class TransformationMatrix;
struct VLine;

class TransformationMatrixAnalyzer
{
public:
	bool IsUniform (const TransformationMatrix &tm);
	void GetScaleFactors (const TransformationMatrix &tm , double &xscale , double &yscale , double &zscale);
	bool IsMirror (const TransformationMatrix &tm);

	void Transform3AxLines (VLine &lx , VLine &ly , VLine &lz , const TransformationMatrix &tm);
};	

#endif