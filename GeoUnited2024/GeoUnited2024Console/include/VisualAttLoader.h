// VisualAttLoader.h

#ifndef __VisualAttLoader_h__
#define __VisualAttLoader_h__

class PointVisualAtt;
class LineVisualAtt;
class FacetVisualAtt;
struct VColor;

class VisualAttLoader
{
public:
	VisualAttLoader ();
	void Load ( const PointVisualAtt & att , bool modelselected , const VColor & selcolor ) const;
	void Load ( const LineVisualAtt & att , bool modelselected , const VColor & selcolor ) const;
	void Load ( const FacetVisualAtt & att , bool modelselected , const VColor & selcolor ) const;
	void ResetPrev ();
protected:
	GLfloat no_mat [4];
	GLfloat mat_ambient [4];
	GLfloat mat_ambient_color [4];
	GLfloat mat_shininess[1];
	GLfloat mat_emission [4];
    GLfloat mat_specular[4];
	VUChr halftone[128];
	mutable const PointVisualAtt * m_prevLoadedPVA;
	mutable const LineVisualAtt *m_prevLoadedLVA;
	mutable const FacetVisualAtt *m_prevLoadedFVA;
	mutable bool m_prevSelected;
};

#endif