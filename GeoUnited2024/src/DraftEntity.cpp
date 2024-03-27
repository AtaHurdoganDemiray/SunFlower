// DraftEntity.cpp

#include "stdafx.h"
#include "DraftEntity.h"
#include "Model.h" 

#ifndef V_NO_MFC
#include "VMenu.h"
#endif

DraftEntity::DraftEntity (Model *model)
//:ViewPointItem (type,name)
:m_model (model)
{

}

//DraftEntity::DraftEntity ( const DraftEntity & Foo )
//:ViewPointItem (Foo)
//{

//}

void DraftEntity::OnResizeWindow(OglWindowData &mywindow)
{

}

void DraftEntity::OnTreeViewClicked()
{
	m_model->SetVisible (!m_model->IsVisible());
}
	
void DraftEntity::OnTreeViewDoubleClicked()
{

}
	
void DraftEntity::OnTreeViewRightClicked(const VStr &commandcap)
{
	if (commandcap == TREEVIEWCONTEXTMENU_HIDE)
		m_model->SetVisible(false);
	else if (commandcap == TREEVIEWCONTEXTMENU_UNHIDE)
		m_model->SetVisible(true);
}

bool DraftEntity::IsHit(Mouse &mouse) const
{
	return false;
}

bool DraftEntity::IsHit (vreal &depthvalue)const
{
	return false;
}

void DraftEntity::SetDraftEntityModel (Model *model)
{
	m_model = model;
}

const VStr & DraftEntity::GetModelType() const
{
	return m_model->GetModelType();
}
	
bool DraftEntity::GetDeleted()const
{
	return m_model->GetDeleted();
}
	
void DraftEntity::SetDeleted(bool isdeleted)
{
	m_model->SetDeleted (isdeleted);
}

void DraftEntity::SetFolderPath (const FolderPath &folderpath)
{
	m_model->SetFolderPath (folderpath);
}
	
const FolderPath & DraftEntity::GetFolderPath ()const
{
	return m_model->GetFolderPath();
}
	
bool DraftEntity::IsVisible() const
{
	return m_model->IsVisible();
}
	
void DraftEntity::SetVisible ( bool vis )
{
	m_model->SetVisible (vis);
}
	
bool DraftEntity::IsSelected() const
{
	return m_model->IsSelected();
}
	
void DraftEntity::SetSelected ( bool sel )
{
	m_model->SetSelected (sel);
}
	
const ModelID & DraftEntity::GetModelID()const
{
	return m_model->GetModelID();
}
	
ModelID & DraftEntity::GetModelID()
{
	return m_model->GetModelID();
}
	
void DraftEntity::SetPEntity (Entity *ent)
{
	m_model->SetPEntity (ent);
}
	
Entity * DraftEntity::GetPEntity ()
{
	return m_model->GetPEntity();
}
	
const VStr DraftEntity::GetModelTypeEng () const
{
	return m_model->GetModelTypeEng();
}
#ifndef V_NO_MFC
VMenu * DraftEntity::GetContextMenu ()
{
	return m_model->GetContextMenu();
}
	
void DraftEntity::SetContextMenu (VMenu *contextmenu)
{
	m_model->SetContextMenu (contextmenu);
}
#endif	
VStr & DraftEntity::GetName()
{
	return m_model->GetName();
}

Model * DraftEntity::GetDraftEntityModel()
{
	return m_model;
}
