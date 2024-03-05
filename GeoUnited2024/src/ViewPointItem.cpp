// ViewPointItem.cpp

#include "stdafx.h"
#include "ViewPointItem.h"
#include "ModelID.h"
#include "Entity.h"
#include "VMenu.h"

int * ViewPointItem::m_selModelCount = 0;

ViewPointItem::ViewPointItem ()
:m_modeltype(( IDS_Undefined)),m_name(( IDS_Undefined)),deleted(false),p_contextmenu(0)
{
	m_folderpath.InsertName (( IDS_Undefined));
	visible = true;
	selected = false;
	m_pe = 0;
	//m_id.SetModelPtr (this);
}

//#############################################################################

ViewPointItem::ViewPointItem (const ViewPointItem &toCopy)
//:m_treenode(toCopy.m_treenode),
:m_modeltype(toCopy.m_modeltype),m_name(toCopy.m_name)
,deleted(toCopy.deleted)
,m_folderpath (toCopy.m_folderpath)
,visible (toCopy.visible)
,selected (toCopy.selected)
,m_pe (toCopy.m_pe)
//,m_id (toCopy.m_id)
,p_contextmenu(toCopy.p_contextmenu)
{
	// ???
	//m_id.SetModelPtr (this);
}

//#############################################################################

ViewPointItem::ViewPointItem (const VStr &type , const VStr &aname,const FolderPath * folderpath)
:m_modeltype(type),m_name(aname),deleted(false),p_contextmenu(0)
{
	if (folderpath !=0)
		m_folderpath = *folderpath;
	visible = true;
	selected = false;
	m_pe = 0;
	//m_id.SetModelPtr (this);
}

//#############################################################################

ViewPointItem & ViewPointItem::operator = (const ViewPointItem &toCopy)
{
	m_modeltype = toCopy.m_modeltype;
	m_name = toCopy.m_name;
	deleted=toCopy.deleted;
	visible = toCopy.visible;
	selected = toCopy.selected;
	m_folderpath = toCopy.m_folderpath;
	//m_pe = toCopy.m_pe;
//m_id = toCopy.m_id;
//m_id.SetModelPtr (this); ???
	p_contextmenu = toCopy.p_contextmenu;
	return *this;
}

//#############################################################################

void ViewPointItem::OnClicked()
{
	OnTreeViewClicked();
}

//#############################################################################
/*
void ViewPointItem::SetTreeNode(HTREEITEM item)
{
	m_treenode = item;
}
*/
//#############################################################################
/*
HTREEITEM ViewPointItem::GetTreeNode () const
{
	return m_treenode;
}
*/
//#############################################################################
	
const VStr & ViewPointItem::GetName () const
{
	return m_name;
}

//#############################################################################

const VStr & ViewPointItem::GetModelType () const
{
	return m_modeltype;
	//if (m_pe == 0)
	//{
	//	//throw VException (("ViewPointItem::GetModelType exception , entity pointer null!"),1);
	//	return ("NonEntityModel");
	//}
	//return m_pe->GetType();
}

const VStr ViewPointItem::GetModelTypeEng() const
{
	//return m_modeltype;
	//if (m_pe == 0)
	//{
	//	//	throw VException (("ViewPointItem::GetModelTypeEng exception , entity pointer null!"),1);
	//	return ("NonEntityModel");
	//}
	return m_pe->GetTypeEng();
}

//#############################################################################

//VStr ViewPointItem::GetName ()
//{
//	return m_name;
//}

//#############################################################################

//VStr ViewPointItem::GetModelType ()
//{
//	return m_modeltype;
//}

//#############################################################################

void ViewPointItem::SetName ( VStr newname )
{
	m_name = newname;
}

//#############################################################################

//void ViewPointItem::SetModelType ( VStr newtype )
//{
//	m_modeltype = newtype;
//}

//#############################################################################

bool ViewPointItem::GetDeleted()const
{
	return deleted;
}

//#############################################################################

void ViewPointItem::SetDeleted(bool isdeleted)
{
	deleted=isdeleted;
}

//#############################################################################

void ViewPointItem::Serialize (Bifstream &infile)
{
	unsigned int version;
	infile.read((VChr*)&version,sizeof(int));
	if(version==1)
	{
		infile.ReadString(m_name);
		//infile.ReadString(m_modeltype);
		//VStr m_modeltype;
		infile.ReadBool (deleted);
		m_folderpath.Serialize (infile);
		//infile.ReadBool (selected);
		infile.ReadBool (visible);
		//Entity * m_pe;
		m_id.Serialize (infile);
		//VMenu * p_contextmenu;
	}
}

//#############################################################################

void ViewPointItem::Serialize (Bofstream &outfile)const
{
	unsigned int version=1;
	outfile.write((VChr*)&version,sizeof(int));
	if (version==1)
	{
		outfile.WriteString(m_name);
		outfile.WriteBool (deleted);
		m_folderpath.Serialize (outfile);
		//infile.ReadBool (selected);
		outfile.WriteBool (visible);
		//Entity * m_pe;
		m_id.Serialize (outfile);
		//VMenu * p_contextmenu;
	}
}

void ViewPointItem::SetFolderPath(const FolderPath &folderpath)
{
	m_folderpath = folderpath;
}

const FolderPath & ViewPointItem::GetFolderPath() const
{
	return m_folderpath;
}

bool ViewPointItem::IsVisible() const
{
	return visible;
}	

void ViewPointItem::SetVisible ( bool vis )
{
	visible = vis;
}	

bool ViewPointItem::IsSelected() const
{
	return selected;
}	

void ViewPointItem::SetSelected ( bool sel )
{
	if (sel != selected)
	{
		selected = sel;
		if (selected)
			(*m_selModelCount) ++;
		else
			(*m_selModelCount) --;
	}
}
	
void ViewPointItem::SetPEntity(Entity *ent)
{
	m_pe = ent;
}

Entity * ViewPointItem::GetPEntity ()
{
	return m_pe;
}

const ModelID & ViewPointItem::GetModelID()const
{
	return m_id;
}
ModelID & ViewPointItem::GetModelID()
{
	return m_id;
}
void ViewPointItem::SetContextMenu(VMenu *contextmenu)
{
	p_contextmenu = contextmenu;
}

VMenu * ViewPointItem::GetContextMenu()
{
	return p_contextmenu;
}

VStr & ViewPointItem::GetName()
{
	return m_name;
}

void ViewPointItem::SetModelID (const ModelID &newid)
{
	m_id = newid;
}
