// ViewPointItem.h

#ifndef __ViewPointItem_h__
#define __ViewPointItem_h__
#include "Bifstream.h"
#include "Bofstream.h"
#include "NameList.h"
#include "ModelID.h"
class Entity; 
class VMenu;

class ViewPointItem
{
public:
	ViewPointItem ();
	virtual ~ViewPointItem(){};
	ViewPointItem (const VStr &type,const VStr &name,const FolderPath *folderpath = 0);
	friend class HandleSet;
	//friend class CommandToViewPoint;
protected:	
	ViewPointItem (const ViewPointItem &toCopy);
	ViewPointItem & operator = (const ViewPointItem &toCopy);
public:
	void SetName ( VStr newname );
	//VStr GetName();
	const VStr & GetName() const;
	//VStr GetModelType();
	const VStr &GetModelType() const;
	//void SetModelType( VStr newtype );	
	//void SetTreeNode(HTREEITEM item);
	//HTREEITEM GetTreeNode () const;	
	void OnClicked();
	//unsigned int GetTreeViewHandle ()const;
	bool GetDeleted()const;
	void SetDeleted(bool isdeleted);
	virtual void OnTreeViewClicked() = 0;
	virtual void OnTreeViewDoubleClicked() =0;
	virtual void OnTreeViewRightClicked(const VStr &commandcap) =0;
	void Serialize (Bifstream &infile);
	void Serialize (Bofstream &outfile)const;
	void SetFolderPath (const FolderPath &folderpath);
	const FolderPath & GetFolderPath ()const;
	bool IsVisible() const;
	void SetVisible ( bool vis );
	bool IsSelected() const;
	static int * m_selModelCount;
	void SetSelected ( bool sel );
	const ModelID & GetModelID()const;
	ModelID & GetModelID();
	void SetModelID (const ModelID &newid);
	void SetPEntity (Entity *ent);
	Entity * GetPEntity ();
	const VStr GetModelTypeEng () const;
	VMenu * GetContextMenu ();
	void SetContextMenu (VMenu *contextmenu);
	VStr & GetName();
protected:
	//HTREEITEM m_treenode;
	//ModelID m_id;
	VStr m_name;
	VStr m_modeltype;
	bool deleted;
	FolderPath m_folderpath;
private:
	bool selected;
protected:
	bool visible;
	Entity * m_pe;
	ModelID m_id;
	VMenu * p_contextmenu;
};

class VpItemNoGraph : public ViewPointItem
{
public:
	VpItemNoGraph ()
	:ViewPointItem ()
	{
		
	}
	VpItemNoGraph (const VStr &type,const VStr &name,const FolderPath *folderpath = 0)
	:ViewPointItem (type,name,0)
	{
	
	}
	virtual void OnTreeViewClicked()
	{
	
	}
	virtual void OnTreeViewDoubleClicked()
	{
	
	}
	virtual void OnTreeViewRightClicked(const VStr &commandcap)
	{
	
	}
	//virtual const VStr GetModelTypeEng () const
	//{
	//	return ("VpItemNoGraph");
	//}

};

const VStr TREEVIEWCONTEXTMENU_HIDE = ("hide");
const VStr TREEVIEWCONTEXTMENU_UNHIDE = ("unhide");

#endif
