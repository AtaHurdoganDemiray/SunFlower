// VFileDirectoryFunctions.h

#ifndef __VFileDirectoryFunctions_h__
#define __VFileDirectoryFunctions_h__

class VFileDirectoryFunctions
{
public:
	static bool CollectAllFilesUnder(const VStr &path, std::list<VStr> &allFiles);
	static bool CollectAllFilesUnderNonRecursive(const VStr& path, std::list<VStr>& allFiles);
	static bool ImitateAllDirectories(const VStr &imitationBaseLikeViewpoint , const VStr &dirBaseLikeViewpoint , const VStr &imitationBasePath, const VStr &dirBasePath , const VStr &startPath);
	//static bool CollectFilesRecursive(const VStr &rootpath , std::list<WIN32_FIND_DATA> ffdparents , WIN32_FIND_DATA &ffd , std::list<VStr> &allFiles);
	//static VStr GetCompletePath(const VStr &rootpath , const std::list<WIN32_FIND_DATA> &ffdparents, const WIN32_FIND_DATA &ffd);
	static bool CopyFromOriginalToDir(const VStr &originalBaseLikeViewpoint , const VStr &dirBaseLikeViewpoint, const VStr &imitationBaseLikeViewpoint , const VStr &originalBasePath , const VStr &dirBasePath , const VStr &imitationBasePath);
	static bool CopyFromDirToOriginal(const VStr &originalBaseLikeViewpoint , const VStr &dirBaseLikeViewpoint, const VStr &imitationBaseLikeViewpoint , const VStr &originalBasePath , const VStr &dirBasePath , const VStr &imitationBasePath);
	static void DirToOriginal(const VStr &originalBaseLikeViewpoint , const VStr &dirBaseLikeViewpoint , const VStr &originalBasePath , const std::list<VStr> &dirs, std::list<VStr> &originals);
	static void OriginalToDir(const VStr &originalBaseLikeViewpoint , const VStr &dirBaseLikeViewpoint, const VStr &dirBasePath , const std::list<VStr> &originals, std::list<VStr> &dirs);
	static VStr DirToOriginal(const VStr &originalBaseLikeViewpoint , const VStr &dirBaseLikeViewpoint, const VStr &originalBasePath , const VStr &dirName);
	static VStr OriginalToDir(const VStr &originalBaseLikeViewpoint, const VStr &dirBaseLikeViewpoint , const VStr &dirBasePath , const VStr &originalName);
	static bool CopyFromDirToPack(const VStr &fullPackPath , /*const VStr &originalBaseLikeViewpoint,*/ const VStr &dirBaseLikeViewpoint, const VStr &imitationBaseLikeViewpoint/*, const VStr &originalBasePath*/, const VStr &dirBasePath, const VStr &imitationBasePath);
	static bool CollectAllFilesAndSizesUnder(const VStr &path, std::list<VStr> &allFiles, std::list<size_t>&allSizes);
	static bool OpenPackToDir(const VStr &fullPackPath , VStr &dirBaseLikeViewpoint , VStr &dirBasePath);
	static BOOL CreateDirectoryWithMissingPath(const VStr &dir);
};

#endif