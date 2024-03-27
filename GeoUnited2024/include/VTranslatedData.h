#ifndef VTranslatedData_h
#define VTranslatedData_h

class VTranslatedData
{
public: 
	VTranslatedData(const VStr & srcText ,const VStr &trnText ,const VStr & parentName /*,int id*/);
	~VTranslatedData(void);

	VStr m_srcText;
	VStr m_trnText;
	VStr m_parentName;
	//int m_id;
	static char VSEPERATOR ;

	static bool GetTranslatedData(std::ifstream &ifs,std::vector<VTranslatedData*> &data);
	static bool GetTranslatedData(const VStr &parentName ,std::ifstream &ifs ,std::map<VStr,VStr> &data);
	static VStr GetLine(VTranslatedData  *dt);
};

#endif