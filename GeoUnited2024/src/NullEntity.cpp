// NullEntity.cpp

#include "stdafx.h"
#include "NullEntity.h"
#include "IgesEntityVisitor.h"

NullEntity::NullEntity()
{
	entitytype = 0;
}
void NullEntity::GetEntityIn( FileLines & mylines , int poffs)
{

}
void NullEntity::WriteEntityOut (std::vector <VStr> &strvec)const
{

}
void NullEntity::GetChildren (std::vector<int> &childVec)const
{
	return;
}
void NullEntity::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}
//bool NullEntity::IsPrimitiveEntity()const
//{
//	return true;
//}
//double NullEntity::GetSortNo ()const
//{
//	return 0.;
//}
IgesEntity * NullEntity::MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const
{
	return 0;
}
