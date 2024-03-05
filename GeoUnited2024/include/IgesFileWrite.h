// IgesFileWrite.h

#ifndef __IgesFileWrite_h__
#define __IgesFileWrite_h__

class IgesEntitySet;
class IgesEntity;
struct LongVec;

class IgesFileWrite
{
public:
	friend class GlobalSection;
	friend class IgesEntitySet;
	IgesFileWrite ();
	void WriteOut (const IgesEntitySet &entset , VofStream &outfile);
	//void WriteOut (const IgesEntitySet &entset , VofStream &outfile , const LongVec &subset);
	int GetParameterLineNo()const;
	void Clear();
private:
	void WriteStartSection (VofStream &outfile);
	void WriteGlobalSection (VofStream &outfile);
	void WriteDirectoryEntrySection (const IgesEntitySet &entset , VofStream &outfile);
	void WriteDirectoryEntrySection (const IgesEntitySet &entset , VofStream &outfile , const LongVec &subset);
	void WriteParameterDataSection (const IgesEntitySet &entset , VofStream &outfile);
	void WriteParameterDataSection (const IgesEntitySet &entset , VofStream &outfile , const LongVec &subset);
	void WriteTerminateSection (VofStream &outfile);
	void WriteDirectoryEntry (const IgesEntity &ent , VofStream &outfile);
	void WriteParameterData (const IgesEntity &ent , VofStream &outfile);
	//returns true when line ends
	static bool WriteStringOut (const VStr &toWrite , VofStream &outfile , int &linespent,const int lineend=IGES_LINE_END,const VStr delimiter=",");
	//returns true when line ends
	static bool WriteIntegerOut (const int toWrite , VofStream &outfile,int &linespent,const int lineend=IGES_LINE_END,const VStr delimiter=",");
	//returns true when line ends
	static bool WritevrealOut (const vreal &toWrite , VofStream &outfile , int &linespent,const int lineend=IGES_LINE_END,const VStr delimiter=",");
	//returns true when line ends
	static bool WriteFloatOut (const float &toWrite , VofStream &outfile , int &linespent,const int lineend=IGES_LINE_END,const VStr delimiter=",");
	static void WriteLineEnd (VStr sectionHeader , const int lineNumber ,VofStream &outfile);
	int m_directorylineno;
	int m_parameterlineno;
	int m_globallineno;
	static int IGES_LINE_END;
	static int PARAMETER_LINE_END;
	//std::vector<int> m_paramLinesLength;
	//VofStream tempParam;
	void AddParamData (VofStream &outfile);
};

#endif