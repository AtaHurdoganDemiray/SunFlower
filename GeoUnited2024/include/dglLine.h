// dglLine.h

#ifndef __dglLine_h__
#define __dglLine_h__

#include "VLine.h"
#include "LineVisualAtt.h"
class TransformationMatrix;

class dglLine
{
public:
	dglLine ();
	dglLine (const VLine &l);
	void Draw ()const;
	const VLine & GetLine ()const;
	VLine & GetLine();
	void SetLine (const VLine &l);
	bool GetShowStart()const;
	void SetShowStart(bool showStart);
	bool GetShowEnd ()const;
	void SetShowEnd (bool showEnd);
	const LineVisualAtt & GetVisAtt ()const;
	void SetVisAtt (const LineVisualAtt &visatt);
	void HideShow (bool isHide);
	void Transform (const TransformationMatrix &tm);
private:
	VLine m_l;
	bool m_showStart;
	bool m_showEnd;
	LineVisualAtt m_visatt;
};

#endif