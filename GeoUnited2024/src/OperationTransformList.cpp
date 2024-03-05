// OperationTransformList.cpp

#include "stdafx.h"
#include "OperationTransformList.h"
#include "Model.h"
#include "ViewPointItemTypes.h"
#include "VTransformable.h"
#include "TransformationMatrix.h"
#include "ModelList.h"
#include "ViewPointItem.h"
#include "VVoxel.h"

OperationTransformList::OperationTransformList()
//:m_latestMatModRef(0)
:m_activeDynamicTransformation (-1)
{

}
OperationTransformList::~OperationTransformList()
{
	Itr i = m_allop.begin();
	Itr e = m_allop.end();
	for (i;i!=e;++i)
		delete (*i);
}

void OperationTransformList::SetModelList (ModelList *modellist)
{
	m_modellist = modellist;
}

void OperationTransformList::SetVoxel(VVoxel *voxel)
{
	m_voxel = voxel;
}
OperationTransform & OperationTransformList::MakeOperationTransform (const TransformationMatrix &tm , std::vector<VTransformable *> &hs , bool addToTreeView)
{
	CString temp;
	temp.Format ( IDS_D,(int)m_allop.size()+1);
	//(ViewPointItemTypes::m_transform);

	RemoveAnyModelTransformedDynamic (hs);
	VStr name("Tr");
	name += (LPCTSTR)temp;
	name += "(";
	name += tm.GetInformationText();
	name += ")";
	OperationTransform *tr = new OperationTransform 
							( tm
							, hs 
							, ViewPointItemTypes::m_transform 
							, name);
	m_allop.push_back (tr);
	NewTransform (tm,hs);
	this->m_modellist->AddModelIf(tr);
	m_voxel->UpdateTransform(tr);
	return *tr;
}

//OperationTransform * OperationTransformList::GetTransform (const ModelID &id)
//{
//	Itr i = m_allop.begin();
//	Itr e = m_allop.end();
//	for (i;i!=e;i++)
//	{
//		if((*i)->GetModelID() == id)
//			return *i;
//	}
//	return 0;
//}

//void OperationTransformList::UpdateMatModForOperation (const int otRef)
//{
//	OperationTransform *toAdd = m_allop.at(otRef);
//	if (toAdd->m_active == false)
//		return;
//	int mysize = (int)toAdd->m_selitems.size();
//	for (int i=0;i<mysize;++i)
//	{
//		VTransformable *mytransformable = toAdd->m_selitems.at(i);
//		TransformationMatrix latest;
//		if(mytransformable->GetMatModRef()!=-1)
//		{
//			Mat_Mod &mymatmod = *m_allmat.at(mytransformable->GetMatModRef());
//			Mat_Mod::RemoveItem (mymatmod.m_mvec , mytransformable);
//			latest = toAdd->m_data.m_tm * mymatmod.m_tm.m_tm;
//		}
//		else
//		{
//			// This is first transformation for this model
//			Mat_Mod::RemoveItem (m_untransformed , mytransformable);
//			latest = toAdd->m_data.m_tm;			
//		}
//
//		// Find existing MatMod for matrix or make new one
//		int ref = GetMatModRef (latest);
//		if (ref!=-1)
//		{
//			Mat_Mod &existingMat=*m_allmat.at(ref);
//			existingMat.m_mvec.push_back(mytransformable);
//			existingMat.AddOperationIf(otRef);
//			mytransformable->SetMatModRef (ref);
//		}
//		else
//		{
//			std::vector<VTransformable *> tmp;
//			tmp.push_back (mytransformable);
//			std::vector<int> opRefVec;
//			opRefVec.push_back (otRef);
//			int newref = MakeMatMod (latest,tmp,opRefVec);
//			//m_allmat[newref].AddOperationIf(otRef);
//			mytransformable->SetMatModRef (newref);
//		}
//
//		mytransformable->OnTransform (toAdd->m_data.m_tm);
//	}
//}

OperationTransform & OperationTransformList::GetLastOperation()
{
	return *m_allop.back();
}

void OperationTransformList::ReverseOperation (const int operationNo)
{
	OperationTransform *t=m_allop.at(operationNo);
	//  reverse operation
	t->m_data.isactive = !t->m_data.isactive;
	if (t->m_data.isactive == false)
	{
		TransformationMatrix invM = t->m_data.m_tm.GetInverse();
		NewTransform (invM , t->m_selitems);
	}
	else
	{
		NewTransform (t->m_data.m_tm , t->m_selitems);
	}
	// get all other operations that need update	
	//std::vector<int> opvec;
	//if (t->m_active == false)
	//	opvec.push_back (operationNo);
	//GetRelatedOperationTransform (*t,opvec);
	//// get related mat_mod
	//std::vector<int> matmodvec;
	//GetRelatedMatMod (opvec,matmodvec);
	//// undo all related mat_mod
	//int mysize=(int)matmodvec.size();
	//for (int i=0;i<mysize;++i)
	//{
	//	UndoMatMod (matmodvec[i]);
	//}
	//// remove all related mat_mod from m_allmat
	//std::vector<Mat_Mod *> newmatmodvec;
	//int mysize1 = (int)m_allmat.size();
	//for (int i=0;i<mysize1;++i)
	//{
	//	int mysize2 = (int)matmodvec.size();
	//	bool isexst=false;
	//	for(int j=0;j<mysize2;++j)
	//	{
	//		if (i==matmodvec[j])
	//		{
	//			isexst=true;
	//			break;
	//		}
	//	}
	//	if (isexst==false)
	//		newmatmodvec.push_back (m_allmat[i]);
	//}
	//m_allmat = newmatmodvec;

	////  redo all related operation transform 
	//int mysize3 = (int)opvec.size();
	//for (int j=0;j<mysize3;++j)
	//{
	//	UpdateMatModForOperation (opvec[j]);
	//}
}

//void OperationTransformList::UndoMatMod (const int matmodref)
//{
//	// calculate inverse transform matrix
//	Mat_Mod &mm = *m_allmat.at(matmodref);
//	TransformationMatrix &inv = mm.m_tm.m_tm.GetInverse();
//	// for all model do inverse transform
//	int mysize=(int)mm.m_mvec.size();
//	for (int i=0;i<mysize;++i)
//	{
//		//ModelID &id = mm.m_mvec.GetID(i);
//		//VTransformable *currt = m_modellist->GetModel(id)->GetTransformable();//GetVTransformable (id);
//		VTransformable *currt = mm.m_mvec.at(i);
//		// change mat_mod ref to 0
//		currt->SetMatModRef (-1);
//		// undo transform physically
//		currt->OnTransform (inv);
//		// add model to untransformed
//		m_untransformed.push_back (currt);
//	}
//
//}

int OperationTransformList::MakeMatMod(const std::vector<VTransformable*> &mvec)//(const TransformationMatrix &tm, const std::vector<VTransformable *> &mvec,const std::vector<int> &opRefVec)
{
	TransformData td;
	td.isactive=true;
	TransformationMatrix tm;
	td.m_tm=tm;
	OglMatrix tmp(tm);
	td.m_om=tmp;
	Mat_Mod *matmod = new Mat_Mod();
	matmod->m_tm=td;
	matmod->m_mvec = mvec;
	//matmod->m_opvec = opRefVec;
	m_allmat.push_back(matmod);
	return (int)m_allmat.size()-1;
}

//int OperationTransformList::GetMatModRef (const TransformationMatrix &tm)const 
//{
//	vreal mytol=1.e-7;
//	int mysize=(int)m_allmat.size();
//	for(int i=mysize-1;i>=0/*m_latestMatModRef*/;--i)
//	{
//		const Mat_Mod &matmod=*m_allmat[i];
//		if (matmod.m_isOpen == false)
//		{
//			if (tm.Compare (matmod.m_tm.m_tm,mytol))
//				return i;
//		}
//	}
//	return -1;
//}

//void OperationTransformList::GetRelatedOperationTransform(const OperationTransform &t, std::vector<int> &relV)
//{
//	std::vector<OperationTransform*>::iterator i=m_allop.begin();
//	std::vector<OperationTransform*>::iterator e=m_allop.end();
//	int counter =0;
//	for (i;i!=e;++i)
//	{
//		if ((*i)->m_active == false)
//		{
//			++counter;
//			continue;
//		}
//		else
//		{
//			const int mysize=(int)t.m_selitems.size();
//			for (int ii=0;ii<mysize;++ii)
//			{
//				//const ModelID &id = t.m_selitems.GetID(ii);
//				VTransformable *tr = t.m_selitems.at(ii);
//				if (OperationTransform::IsExist ((*i)->m_selitems,tr))
//				{
//					relV.push_back (counter);
//					break;
//				}
//			}
//			++counter;
//		}
//	}
//}

//void OperationTransformList::GetRelatedMatMod(const std::vector<int> &otRefVec, std::vector<int> &matmodRefVec)
//{
//	std::vector<Mat_Mod *>::iterator i=m_allmat.begin();
//	std::vector<Mat_Mod *>::iterator e=m_allmat.end();
//	int counter=0;
//	for(i;i!=e;++i)
//	{
//		int mysize1=(int) otRefVec.size();
//		bool done=false;
//		for(int j=0;j<mysize1;++j)
//		{
//			const int curri = otRefVec[j];
//			int mysize2 = (int)(*i)->m_opvec.size();
//			for (int w=0;w<mysize2;++w)
//			{
//				if (curri == (*i)->m_opvec[w])
//				{
//					matmodRefVec.push_back (counter);
//					done=true;
//					break;
//				}
//			}
//			if (done)
//				break;
//		} 
//		++counter;
//	}
//}

//void OperationTransformList::Mat_Mod::AddOperationIf(const int opRef)
//{
//	std::vector<int>::iterator i=m_opvec.begin();
//	std::vector<int>::iterator e=m_opvec.end();
//	for (i;i!=e;++i)
//	{
//		if (*i==opRef)
//		{
//			//ref already recorded
//			return;
//		}
//	}
//	//ref isnot recorded yet so add
//	m_opvec.push_back(opRef);
//}

void OperationTransformList::AddUntransformed(VTransformable *tr)
{
	m_untransformed.push_back(tr);
}


const std::vector<VTransformable *> & OperationTransformList::GetUntransformed() const
{
	return m_untransformed;
}

const std::vector<OperationTransformList::Mat_Mod *> & OperationTransformList::GetAllMat()const
{
	return m_allmat;
}

void OperationTransformList::ReverseOperation (ViewPointItem *clicked)
{
	OperationTransform *myopr = (OperationTransform *)(clicked);	
	int mysize = (int)m_allop.size();
	int ref = -1;
	for (int i=0;i<mysize;++i)
	{
		OperationTransform *currop = m_allop[i];
		if (currop == myopr)
		{
			ref = i;
			break;
		}
	}
	if (ref!=-1)
		ReverseOperation (ref);
}

//void OperationTransformList::SetTreeCtrl (CMyTreeCtrl *treecontrol)
//{
//	m_treecontrol = treecontrol;
//}

//OperationTransform & OperationTransformList::MakeDynamicTransform(const std::vector<VTransformable *> &hs, std::vector<int> &matmodvec)
//{
//	// Make an OperationTransform and add
//	TransformationMatrix tm;
//	OperationTransform * op = new OperationTransform (tm,hs,ViewPointItemTypes::m_transform,( IDS_Dt1));
//	m_allop.push_back (op);
//	// group the models according to their mat_mod
//	std::vector<std::vector<VTransformable *>> groupedVec;
//	GroupSameMatMod (hs,groupedVec);
//	// for each group
//	int mysize = (int)groupedVec.size();		
//	for (int i=0;i<mysize;++i)
//	{
//		std::vector<VTransformable *> &currhs = groupedVec[i];
//		//ModelID fid = currhs.GetID(0);
//		//Model *m=m_modellist->GetModel(fid);
//		//VTransformable *currtr = m->GetTransformable();
//		VTransformable *currtr = currhs.at(0);
//		//if (currtr)
//		//{
//			int matmodref = currtr->GetMatModRef();
//			if (matmodref == -1)
//			{
//				// this group is not transformed yet, make transform for them
//				std::vector<int>opRefVec;
//				opRefVec.push_back ((int)m_allop.size()-1);
//				matmodref=MakeMatMod (op->m_data.m_tm,currhs,opRefVec);
//				// set mat_mod ref of all model
//				UpdateMatModRefOfModel(currhs,matmodref);
//				//matmodvec.push_back ((int)m_allmat.size()-1);
//				Mat_Mod &newmm = *m_allmat.at(matmodref);
//				newmm.m_isOpen = true;
//			}
//			else
//			{
//				// Divide the group into 2 matmod
//				Mat_Mod &tomove=*m_allmat.at(matmodref);
//				DivideMatMod_ToMove_NotToMove (currhs,tomove);
//				// fill res with tomove
//				tomove.m_isOpen = true;
//			}
//			matmodvec.push_back(matmodref);
//			size_t mysize22 = currhs.size();
//			TransformData *mycurtrdata = &m_allmat.at(matmodref)->m_tm;
//			for (size_t i22 = 0; i22 < mysize22; ++i22)
//			{
//				VTransformable *mycurtr = currhs.at(i22);
//				mycurtr->GetTrModel()->GetSnapEnt().SetDynamicTransformData (mycurtrdata);
//			}
//
//		//}
//	}
//	//m_latestMatModRef=(int)m_allmat.size();	
//	return *op;
//}
int OperationTransformList::MakeDynamicTransform (const std::vector<VTransformable *> &hs)
{
	std::vector<VTransformable *> hs2 (hs);
	RemoveAnyModelTransformedDynamic (hs2);
	if (hs2.size())
	{
		if (m_activeDynamicTransformation != -1)
			SuspendDynamicTransformation(m_activeDynamicTransformation);
		m_activeDynamicTransformation = MakeMatMod (hs2);
		UpdateMatModRefOfModel (hs,m_activeDynamicTransformation);
	}
	else
	{
		throw VException ("OperationTransformList::MakeDynamicTransform exception , no entities left for transformation after removing dynamic transformations",1);
	}
	return m_activeDynamicTransformation;
}
void OperationTransformList::UpdateMatModRefOfModel(const std::vector<VTransformable *> &hs, const int matmodRef)
{
	int mysize=(int)hs.size();
	TransformData *trData = 0;
	if (matmodRef != -1)
	{
		trData = &m_allmat.at(matmodRef)->m_tm;
	}
	for (int w=0;w<mysize;++w)
	{
		//ModelID myid = hs.GetID(w);
		//Model *m=m_modellist->GetModel(myid);
		//VTransformable *mytr=m->GetTransformable();
		VTransformable *mytr = hs.at(w);	
		//m_untransformed.RemoveItem (myid);		
		if (mytr)
		{
			mytr->SetMatModRef (matmodRef);
			// remove the item from untransformed if
			if (matmodRef != -1)
			{
				Mat_Mod::RemoveItem (m_untransformed , mytr);
			}
			mytr->GetTrModel()->GetSnapEnt().SetDynamicTransformData (trData);// even if it is 0 , it should be re-set
		}
	}
}

//void OperationTransformList::GroupSameMatMod(const std::vector<VTransformable *> &hs
//											, std::vector<std::vector<VTransformable *>> &groupedVec)
//{
//	std::vector<int> refvec;
//	int mysize=(int)hs.size();
//	for (int i=0;i<mysize;++i)
//	{
//		//ModelID currid = hs.GetID(i);
//		//Model * currm=m_modellist->GetModel(currid);
//		//VTransformable *currt=currm->GetTransformable();
//		VTransformable *currt = hs.at(i);
//		if (currt)
//		{
//			bool added = false;
//			int mysize2=(int)refvec.size();
//			for (int j=0;j<mysize2;++j)
//			{
//				if (refvec[j]==currt->GetMatModRef())
//				{
//					groupedVec.at(j).push_back (currt);
//					added=true;
//				}
//			}
//			if (added==false)
//			{
//				std::vector<VTransformable *> newhs;
//				newhs.push_back (currt);
//				groupedVec.push_back(newhs);
//				refvec.push_back (currt->GetMatModRef());
//			}
//		}
//	}
//}

//int OperationTransformList::DivideMatMod_ToMove_NotToMove(std::vector<VTransformable *> &hs
//										 , Mat_Mod &tomove)
//{
//	std::vector<VTransformable *> tomovehs , nomovehs;
//	int mysize = (int)tomove.m_mvec.size();
//	for (int i=0;i<mysize;++i)
//	{
//		//ModelID currid = tomove.m_mvec.GetID (i);
//		VTransformable *curtr = tomove.m_mvec.at(i);
//		if (OperationTransform::IsExist (hs , curtr))
//		//if(hs.IsExist(currid))
//		{
//			//tomovehs.AddItem (currid);
//			tomovehs.push_back (curtr);
//		}
//		else
//		{
//			//nomovehs.AddItem (currid);
//			nomovehs.push_back (curtr);
//		}
//	}
//	tomove.m_mvec = tomovehs;
//	if (nomovehs.size())
//	{
//		// tomove is original mat mod  ,  add nomove to allmatmod
//		int nomovematmodref = MakeMatMod (tomove.m_tm.m_tm,nomovehs,tomove.m_opvec);
//		UpdateMatModRefOfModel (nomovehs,nomovematmodref);
//	}
//	// update tomove with new operation transform
//	int latestoperationtransformref = (int)m_allop.size()-1;
//	tomove.m_opvec.push_back(latestoperationtransformref);
//	return (int)nomovehs.size();
//}

//void OperationTransformList::FinishDynamicTransform(int matModNo)
//{
//	int mysize = (int)op.m_selitems.size();
//	// check if this dynamic transformation is already closed or not
//	if (mysize)
//	{
//		VTransformable *tr = op.m_selitems.front();
//		int curmatmodi = tr->GetMatModRef();
//		if (curmatmodi == -1)
//			LogFileWriter::WriteUnknownExceptionLog (( IDS_OpeFinUneSitMat));
//		else 
//		{
//			Mat_Mod *curmatmod = m_allmat.at(curmatmodi);
//			if (curmatmod->m_isOpen == false)
//			{
//				return; // this is already closed , so interrupt
//			}
//		}
//	}
//	for (int i=0;i<mysize;++i)
//	{
//		//ModelID currid = op.m_selitems.GetID(i);
//		//Model * m=m_modellist->GetModel(currid);
//		//VTransformable *tr = m->GetTransformable();
//		VTransformable *tr = op.m_selitems.at(i);
//		int curmatmodi = tr->GetMatModRef();
//		if (curmatmodi == -1)
//			LogFileWriter::WriteUnknownExceptionLog (( IDS_OpeFinUneSitMat));
//		else
//		{
//			Mat_Mod *curmatmod = m_allmat.at(curmatmodi);
//			curmatmod->m_isOpen = false;
//			tr->OnTransform (op.m_data.m_tm);
//			tr->GetTrModel()->GetSnapEnt().SetDynamicTransformData (0);
//		}
//	}
//}

OperationTransform * OperationTransformList::FinishDynamicTransform (int matModRef)
{
	Mat_Mod *curTr = m_allmat.at(matModRef);
	curTr->m_isOpen = false;
	NewTransform (curTr->m_tm.m_tm , curTr->m_mvec);
	//OperationTransform &added = MakeOperationTransform(curTr->m_tm.m_tm,curTr->m_mvec);
	int n = (int)curTr->m_mvec.size();
	for (int i=0;i<n;++i)
		this->AddUntransformed (curTr->m_mvec.at(i));
	UpdateMatModRefOfModel (curTr->m_mvec,-1);
	//curTr->m_mvec.clear();
	m_activeDynamicTransformation = -1;

	CString temp;
	temp.Format ( IDS_D,(int)m_allop.size());
	OperationTransform *tr = new OperationTransform 
		( curTr->m_tm.m_tm
		, curTr->m_mvec 
		, ViewPointItemTypes::m_transform 
		, ViewPointItemTypes::m_transform + (LPCTSTR)temp);
	this->m_modellist->AddModelIf (tr);
	m_allop.push_back (tr);
	m_voxel->UpdateTransform(tr);
	return tr;
}

std::vector<OperationTransformList::Mat_Mod *> & OperationTransformList::GetAllMat()
{
	return m_allmat;
}

//void OperationTransformList::IncrementDynamicTransformation (const std::vector<int> &matmodVec
//															, OperationTransform &op
//															, const TransformationMatrix &tm)
//{
//	int mysize = (int) matmodVec.size();
//	for (int i=0;i<mysize;++i)
//	{
//		OperationTransformList::Mat_Mod & currmm = *m_allmat.at(matmodVec[i]);
//		currmm.m_tm.m_tm *= tm;
//		OglMatrix myglm (currmm.m_tm.m_tm);
//		currmm.m_tm.m_om = myglm;
//	}
//	op.m_data.m_tm *= tm;
//	OglMatrix tmp (op.m_data.m_tm);
//	op.m_data.m_om = tmp;
//}

void OperationTransformList::IncrementDynamicTransformation (int matModi , const TransformationMatrix &tm)
{
	if (matModi < 0)
		return;

	Mat_Mod *curm = m_allmat.at(matModi);
	curm->m_tm.m_tm *= tm;
	OglMatrix myglm (curm->m_tm.m_tm);
	curm->m_tm.m_om = myglm;
}

//void OperationTransformList::ChangeDynamicTransformation(const std::vector<int> &matmodVec
//															, OperationTransform &op
//															, const TransformationMatrix &tm)
//{
//	int mysize = (int) matmodVec.size();
//	for (int i=0;i<mysize;++i)
//	{
//		OperationTransformList::Mat_Mod & currmm = *m_allmat.at(matmodVec[i]);
//		currmm.m_tm.m_tm = tm;
//		OglMatrix myglm (currmm.m_tm.m_tm);
//		currmm.m_tm.m_om = myglm;
//	}
//	op.m_data.m_tm = tm;
//	OglMatrix tmp (op.m_data.m_tm);
//	op.m_data.m_om = tmp;
//}
void OperationTransformList::ChangeDynamicTransformation (int matModi , const TransformationMatrix &tm)
{
	Mat_Mod *curm = m_allmat.at(matModi);
	curm->m_tm.m_tm = tm;
	OglMatrix myglm (curm->m_tm.m_tm);
	curm->m_tm.m_om = myglm;
}
//void OperationTransformList::UpdateAllHs ()
//{	
//	UpdateSingleHs(m_untransformed);
//	std::vector<Mat_Mod> &allmat = GetAllMat();
//	size_t mysize = allmat.size();
//	for (size_t i=0;i<mysize; ++i)
//	{
//		Mat_Mod &mm = allmat[i];
//		UpdateSingleHs(mm.m_mvec);
//	}
//}

//void OperationTransformList::UpdateSingleHs (std::vector<VTransformable *> &hs)
//{
//	int mysize = hs.Size();
//	for (int i=0;i<mysize;++i)
//	{
//		ModelID &id = hs.GetID(i);
//		ViewPointItem *vi = (ViewPointItem *)id.GetModelPtr();
//		id.SetBirthDate(vi->GetModelID().GetBirthDate());
//		id.SetBirthPlace(vi->GetModelID().GetBirthPlace());
//	}
//}

void OperationTransformList::Mat_Mod::RemoveItem(std::vector<VTransformable *> &vec , VTransformable *itemToRemove)
{
	std::vector<VTransformable *> newVec;
	bool isRemoved = false;
	size_t mysize = vec.size();
	for (size_t i=0;i<mysize;++i)
	{
		VTransformable *curitem = vec.at(i);
		if (curitem == itemToRemove)
			isRemoved = true;
		else
			newVec.push_back (curitem);
	}
	if (isRemoved)
		vec = newVec;
}

OperationTransformList::Mat_Mod::Mat_Mod()
: m_isOpen (true)
{

}

//void OperationTransformList::FinishAllRelatedDynamicTransformations(std::vector<VTransformable*> &toMove)
//{
//	size_t mysize = toMove.size();
//	for (size_t i=0;i<mysize;++i)
//	{
//		VTransformable *curtr = toMove[i];
//		int curmatmodref = curtr->GetMatModRef();
//		if (curmatmodref != -1)
//		{
//			Mat_Mod *curmatmod = m_allmat.at(curmatmodref);
//			if (curmatmod->m_isOpen)
//			{
//				if (curmatmod->m_opvec.size() == 0)
//					throw VException (( IDS_OpeFinExcDynMat),1);
//				int dynamicTransformRef = curmatmod->m_opvec.back();
//				OperationTransform *dynamicTransform = m_allop.at(dynamicTransformRef);
//				this->FinishDynamicTransform (*dynamicTransform);
//			}
//		}
//	}
//}

void OperationTransformList::CancelDynamicTransformation (int matmodi)
{
	Mat_Mod *curTr = m_allmat.at(matmodi);
	curTr->m_isOpen = false;
	//NewTransform (curTr->m_tm.m_tm , curTr->m_mvec);
	int n = (int)curTr->m_mvec.size();
	for (int i=0;i<n;++i)
	{
		this->AddUntransformed (curTr->m_mvec.at(i));
	}
	UpdateMatModRefOfModel (curTr->m_mvec,-1);
	//curTr->m_mvec.clear();
	if (matmodi == m_activeDynamicTransformation)
		m_activeDynamicTransformation = -1;	
}

void OperationTransformList::SuspendDynamicTransformation (int matmodi)
{
	Mat_Mod *curTr = m_allmat.at(matmodi);
	//curTr->m_isOpen = false;
	//NewTransform (curTr->m_tm.m_tm , curTr->m_mvec);
	//int n = (int)curTr->m_mvec.size();
	//for (int i=0;i<n;++i)
	//	this->AddUntransformed (curTr->m_mvec.at(i));
	//UpdateMatModRefOfModel (curTr->m_mvec,-1);
	//curTr->m_mvec.clear();
	if (matmodi == m_activeDynamicTransformation)
		m_activeDynamicTransformation = -1;	
}
void OperationTransformList::NewTransform (const TransformationMatrix &tm , std::vector<VTransformable *> &trVec)
{
	size_t mysize = trVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		VTransformable *curtr = trVec.at(i);
		curtr->OnTransform (tm);
	}
}

bool OperationTransformList::IsThereDynamicTransformation()const
{
	if (m_activeDynamicTransformation != -1)
		return true;
	size_t mysize = m_allmat.size();
	for (size_t i=0;i<mysize;++i)
	{
		OperationTransformList::Mat_Mod *curmm = m_allmat.at(i);
		if (curmm->m_isOpen)
			return true;
	}
	return false;
}

bool OperationTransformList::RemoveAnyModelTransformedDynamic (std::vector<VTransformable *> &trVec)const//returns true if any VTransformable is used any existing Mat_Mod
{
	size_t mysize = trVec.size();
	std::vector<VTransformable *> newVec;
	bool ck = false;
	for (size_t i=0;i<mysize;++i)
	{
		VTransformable *curtr = trVec.at(i);
		if (curtr->GetMatModRef() != -1)
			ck = true;
		else
			newVec.push_back (curtr);
	}
	trVec = newVec;
	return ck;
}

void OperationTransformList::HideShowOperationTransform(int matmodi, bool isVisible)
{
	Mat_Mod *curmatmod = m_allmat.at(matmodi);
	curmatmod->m_isOpen = isVisible;
}

void OperationTransformList::AddModelToDynamicTransformation(int matmodi, std::vector<VTransformable*>& newModel)
{
	Mat_Mod* curmatmod = m_allmat.at(matmodi);
	for (auto i = newModel.begin(); i != newModel.end(); ++i)
	{
		VTransformable* newTr = *i;
		curmatmod->m_mvec.push_back(newTr);
	}
	UpdateMatModRefOfModel(newModel, matmodi);
}

void OperationTransformList::AddModelToDynamicTransformationIfNotExisting(int matmodi, std::vector<VTransformable*>& newModel)
{
	Mat_Mod* curmatmod = m_allmat.at(matmodi);
	for (auto i = newModel.begin(); i != newModel.end(); ++i)
	{
		VTransformable* newTr = *i;
		if (std::find(curmatmod->m_mvec.begin(),curmatmod->m_mvec.end(),newTr) == curmatmod->m_mvec.end())
			curmatmod->m_mvec.push_back(newTr);
	}
}

const TransformationMatrix& OperationTransformList::GetTransformationOf(int matmodi)const
{
	Mat_Mod* curmatmod = m_allmat.at(matmodi);
	return curmatmod->m_tm.m_tm;
}
