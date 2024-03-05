
#ifndef VStringTableTranslater_h
#define VStringTableTranslater_h

class VTranslatedData;

class VStringTableTranslater
{
public:
	VStringTableTranslater(void);
	~VStringTableTranslater(void);

	VStr Add(int id ,const VStr & srcVal);
  //  VStr GetTrnText(const VStr & srcText); 
private : 

	//void Translate(VTranslatedData *dt); 

	//std::vector<VTranslatedData*> m_trnData;
	//std::vector<VStr> m_srcData;
	//VStr m_folderPath;
	//VStr m_selLangFilePath;
	//VStr m_srcFilePath; 
 
//
//	void CreatePaths(); 
//	VStr GetSelectedLanguange(const VStr& filePath); 
//	bool LoadTranslatedData();  
//
//public :
//	void WriteToFileAllItems(std::ofstream &ofs); 

};


#endif