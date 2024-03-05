// DTableKeyAttributes.h

#ifndef __DTableKeyAttributes_h__
#define __DTableKeyAttributes_h__


class DTableKeyAttributes
{
public:
	DTableKeyAttributes ();
	//static VStr DataTypeInteger,DataTypeReal,DataTypeString,DataTypePointer;

	// returns KEY_NOT_FOUND_STR if not existing in map
	const VStr & GetName()const;
	void SetName (const VStr &name);
	VStr GetIsReadOnly ()const;
	VStr GetDefaultValue ()const;
	VStr GetDataType ()const;
	VStr GetShortDescription ()const;
	VStr GetInfoPicturePath()const;
	VStr GetSearchItemsVec()const; // VP_ROW_SEPARATOR ('|') seperated single string
	VStr GetIsAddEverySingleInstanceToSearch ()const;
	bool Read (const VStr &baseFolder);
	bool Write (const VStr &baseFolder)const;
private:
	VStr m_name;
	//VStr m_isReadOnly;
	//VStr m_defaultValue;
	//VStr m_dataType;
	//VStr m_shortDescription;
	//VStr m_infoPicturePath;
	//VStr m_commaSeparatedSearchItemsVec;
	//VStr m_isAddEverySingleInstanceToSearch;
	//VStr m_isVisible;
	//VStr m_isList;
	//VStr m_minValue;
	//VStr m_maxValue;
	std::map<VStr,VStr> m_attrNameValueMap;
public:
	VStr GetIsVisible ()const;
	VStr GetIsList()const;
	VStr GetMinValue()const;
	VStr GetMaxValue()const;
	std::map<VStr,VStr> & GetAttrNameValueMap();
	VStr GetValueFor (const VStr &key)const;
	void AddAttr (const VStr &attrName , const VStr &value);
	const static VStr KEY_NOT_FOUND_STR;
	static std::map<VStr,VStr> MergeAttributes (const std::vector<DTableKeyAttributes *> &attrVec);
private:
	VStr SearchValue (const VStr &key)const;
public:
	void AddSearchItemsIf ( std::vector<VStr> &searchItems );

};

#endif