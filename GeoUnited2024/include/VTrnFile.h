#ifndef VTrnFile_h
#define VTrnFile_h

class VTrnFile
{
public:
	VTrnFile(void);
	~VTrnFile(void);
	static void GetAllLines(std::ifstream &ifs ,std::vector<std::string> &lines);
	static void WriteAllLines(std::ofstream &ofs,std::vector<std::string> lines);
	static std::vector<std::string> Split(const std::string &sourceStr , char seperator );
};

#endif