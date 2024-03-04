
#ifndef VTTextMap_h
#define VTTextMap_h

class VTTextMap
{
public:
	VTTextMap(const VStr &parentName);
	~VTTextMap(void);

	VStr Add(const VStr & srcVal);
//	VStr GetTrnText(const VStr & srcVal);
private:
//	 
//	std::map<VStr,VStr> m_data; 
	VStr m_parentName;  
//
//	VStr m_folderPath;
//	VStr m_selLangFilePath;
//	VStr m_srcFilePath; 
  
//	void CreatePaths(); 
//	VStr GetSelectedLanguange(const VStr& filePath); 
//	bool LoadTranslatedData();  
//
//public :
//	void WriteToFileAllItems(std::ofstream &ofs); 


};

#endif