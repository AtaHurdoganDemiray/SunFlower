// OGLSelection.h

#ifndef __OGLSelection_h__
#define __OGLSelection_h__

#include "ViewVolume.h"

class Mouse;
class ModelList;
class Quat;
struct VPoint;
class PointSet;
struct DoubleVec;
struct LongVec;
class HandleSet;
class SelectionFlags;
class SelectableTypes;
class SnapEntities;
class SelectionFlags;
class DraftEntityVec;
class LineSet;
class Model;
class VTransformable;

class OGLSelection
{
public:
	OGLSelection ( const ViewVolume & _viewvolume 
				, Quat & _quat 
				, const VPoint & _rotcenter );
	void SetSelectionVolume ( Mouse & mouse , bool selectwithbox , bool overWriteHitRadius);
	void MakeSelectionVolumeHalf ();
	void DoPointSelection ( Mouse & mouse 
						 , bool selectwithbox 
						 , const ModelList &modellist 
						 , const std::vector<VTransformable *> &hs 
						 , PointSet &selpoints
						 , DoubleVec &depthvalues 
						 , const VPoint &curserp
						 , const VPoint &lookdir);
	void DoVectorSelection ( Mouse & mouse 
						  , bool selectwithbox 
						  , const ModelList &modellist 
						  , const std::vector<VTransformable *> &hs 
						  , PointSet &selpoints 
						  , DoubleVec &depthvalues );
	void DoEdgeSelection ( Mouse & mouse 
						, const ModelList &modellist 
						, const std::vector<VTransformable *> &hs 
						, LineSet &sellines 
						, DoubleVec & depthvalues );
	void DoModelSelection ( Mouse & mouse 
							, bool deselect 
							, SelectionFlags & selflags 
							, const SelectableTypes & filter 
							, const ModelList &modellist 
							, const std::vector<VTransformable *> &hs
							, std::map<Model * , double> &selModelMap
							, bool curveChainingActive);
	void DoModelSelection2(Mouse &mouse
		, bool deselect
		, SelectionFlags &selflags
		, const SelectableTypes &filter
		//, const std::vector<VTransformable *> &hs
		, std::multimap<vreal, Model *> &depthModelMap
		, bool curveChainingActive);
	void DoDraftEntitySelection 
	( Mouse & mouse , bool deselect , SelectionFlags & selflags , const SelectableTypes & filter , const DraftEntityVec &modellist , const HandleSet &hs);
	void EndModelSelection ( ModelList & modellist , HandleSet * handles = 0 );
	vreal GetHitRadiusOnWindow () const;
	vreal GetHitRadiusOnVolume () const;
	bool GetTopPoint ( const PointSet & selpoints , const DoubleVec & depthvalues , VPoint & toppoint ) const;
	bool GetTopVector 
	( const PointSet & selpoints , const DoubleVec & depthvalues , VPoint & basepoint , VPoint & topvector ) const;	
	void SetSelectionFlags (SelectionFlags *selflags);
	bool GetTopLine (const LineSet &sellines , const DoubleVec &depthvalues , VPoint &basepoint , VPoint &topvector);
	const ViewVolume & viewvolume;
	Quat & quat;
	const VPoint & rotcenter;
	ViewVolume selectionvolume;
	SelectionFlags *m_selflags;
	void ResetSelectionTime ();
	int GetSelectionTime ()const;
	void IncrementSelectionTime();
private:
	int m_selectionTime;
public:
	//ModelList m_tmpList;
	std::vector<VTransformable *> m_potHits;
	std::vector<VTransformable *> m_boxPotHits;
};

#endif
