// MedianFilter.h

#ifndef __MedianFilter_h__
#define __MedianFilter_h__

class MedianFilter
{
public:
	static void Filter (const std::map<vreal,vreal> &original , std::map<vreal,vreal> &result);
};
#endif