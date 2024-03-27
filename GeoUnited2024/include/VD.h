#ifndef VD_h
#define VD_h

//class GUICustomize;

class VD
{
private:

	class VTRN
	{  
	   
	public:

		VTRN();
		~VTRN();
	private:
		std::map<VStr, VStr> m_dic;
		std::map<VStr, std::vector<VStr>> m_category;
		//----lower for compare
		std::map<VStr, VStr> m_loweDic;
		std::map<VStr, std::vector<VStr>> m_loweCategory;
		//----
	public :
		VStr m_selLangFilePath;
		VStr m_visibleFilePath;
	private:
		void ReadDic(); 
		
	public:
		void WriteSRC(const VStr &rcFilePath);
		VStr Add(const VStr &key, const VStr &category);  
	private:
		VStr GetSelectedLanguange(const VStr& filePath); 
		void CreatePaths();
		void ToLower(VStr &str);
	public :
		void SetSlectedLanguange(const VStr & langName);
	};

public:
	VD();
	~VD();
 
	//static GUICustomize * m_guiCustomize; 
private:
	static VTRN *m_trn;
public:
	static char VD_SEPERATOR; 
	static VStr Add(const VStr &key, VStr category = "Text");
	static void WriteSRC(const VStr &srcFilePath); 
	static VStr GetSelTranslateFilePath();
	static VStr GetSelVisibleFilePath();
	static void SetSelectedLanguange(const VStr &langName);
	
};
#endif