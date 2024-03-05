// VolBoxModel.h

#ifndef __VolBoxModel_h__
#define __VolBoxModel_h__
#include "Model.h"

class VolBoxModel:public Model
{
public:
	VolBoxModel (const VStr &type,const VStr &name
				,const vreal &myx
				,const vreal &myy
				,const vreal &myz);
	VolBoxModel (const VolBoxModel &toCopy);
	VolBoxModel & operator = (const VolBoxModel &toCopy);
	virtual ~VolBoxModel();
	virtual void Serialize(Bofstream &outfile)const;
	virtual void Serialize(Bifstream &infile);
	vreal m_x,m_y,m_z;
};

#endif
