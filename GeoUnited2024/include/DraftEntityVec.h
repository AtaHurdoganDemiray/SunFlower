// DraftEntityVec.h

#ifndef __DraftEntityVec_h__
#define __DraftEntityVec_h__

class DraftEntity;
struct ModelID;
class ViewPointItem;

class DraftEntityVec
{
public:
	typedef std::vector<DraftEntity *>::const_iterator pos;
	typedef std::vector<DraftEntity *>::const_iterator upos;

	virtual ~DraftEntityVec();
	int Size() const;
	DraftEntity * operator[] ( int index );
	unsigned int InsertItem ( DraftEntity * newitem );
	pos Begin()const;
	pos End()const;
	upos Begin();
	upos End();
	//DraftEntity * GetEntity (const ModelID &id);
	ViewPointItem * GetModel (const ModelID &id);
protected:
	std::vector <DraftEntity*> allitems;

};

#endif