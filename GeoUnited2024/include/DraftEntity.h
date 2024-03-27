// DraftEntity.h

#ifndef __DraftEntity_h__
#define __DraftEntity_h__
//#include "Model.h"
struct OglWindowData;
class LineVisualAtt;
class Mouse;
struct VColor;
class Model;
struct ModelID;
class Entity; 
class VMenu;

#include "NameList.h"

class DraftEntity //: public Model
{
public:
	DraftEntity (Model *model);
	//DraftEntity ( const DraftEntity & Foo );
	virtual void Draw() = 0;
	virtual void OnResizeWindow ( OglWindowData & mywindow );
	virtual void OnTreeViewClicked();
	virtual void OnTreeViewDoubleClicked();
	virtual void OnTreeViewRightClicked(const VStr &commandcap);	
	virtual LineVisualAtt & GetLineVisualAtt() = 0;
	virtual bool IsHit(Mouse &mouse)const;
	virtual bool IsHit (vreal &depthvalue)const;
	virtual const VColor & GetColor()const = 0;
	Model * GetDraftEntityModel ();
	void SetDraftEntityModel (Model *model);
	const VStr &GetModelType() const;
	bool GetDeleted()const;
	void SetDeleted(bool isdeleted);
	void SetFolderPath (const FolderPath &folderpath);
	const FolderPath & GetFolderPath ()const;
	bool IsVisible() const;
	void SetVisible ( bool vis );
	bool IsSelected() const;
	void SetSelected ( bool sel );
	const ModelID & GetModelID()const;
	ModelID & GetModelID();
	void SetPEntity (Entity *ent);
	Entity * GetPEntity ();
	const VStr GetModelTypeEng () const;
#ifndef V_NO_MFC
	VMenu * GetContextMenu ();
	void SetContextMenu (VMenu *contextmenu);
#endif
	VStr & GetName();

protected:
	Model *m_model;
};

#endif