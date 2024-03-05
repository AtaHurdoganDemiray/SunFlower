// ViewVolume.h: interface for the ViewVolume class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ViewVolume_h__
#define __ViewVolume_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Bifstream;
class Bofstream;

class ViewVolume
{
public:
	ViewVolume ();
	ViewVolume (const vreal L
				,const int x
				,const int y);
	ViewVolume (const vreal near,
		const vreal far,
		const vreal bottom,
		const vreal top,
		const vreal left,
		const vreal right,
		const int x,
		const int y);
	~ViewVolume();

	friend const ViewVolume operator - (const ViewVolume &v1 , const ViewVolume &v2);
	friend const ViewVolume operator + (const ViewVolume &v1 , const ViewVolume &v2);
	friend const ViewVolume operator * (const ViewVolume &v , const double &f);
	friend const ViewVolume operator * (const double f , const ViewVolume &v);

	vreal m_left ;
	vreal m_right ;
	vreal m_top ; 
	vreal m_bottom ;
	vreal m_near ; 
	vreal m_far;

	int corner_x;
	int corner_y;

	void Serialize(Bifstream &ar);
	void Serialize (Bofstream &ar)const;
	void SetViewVolume();
	void SetLeft( vreal newleft );
	void SetRight( vreal newright );
	void SetTop( vreal newtop );
	void SetBottom( vreal newbottom );
	void SetFar ( vreal newfar );
	void SetNear( vreal newnear );
	vreal ConvertOneUnitOnWindowToVolume () const;
	void ScreenCoordToViewVolume ( vreal x , vreal y , vreal & vx , vreal & vy , double rotcenterx , double rotcentery )const;
	vreal GetAspectRatio() const;
	vreal GetDiaogonalRadius () const;
};

const ViewVolume operator - (const ViewVolume &v1 , const ViewVolume &v2);
const ViewVolume operator + (const ViewVolume &v1 , const ViewVolume &v2);
const ViewVolume operator * (const ViewVolume &v , const double &f);
const ViewVolume operator * (const double f , const ViewVolume &v);

#endif 

