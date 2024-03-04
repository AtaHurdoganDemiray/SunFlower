// ListSerializer.h

#ifndef __ListSerializer_h__
#define __ListSerializer_h__

template <class T> void SerializeBasicTypeList(Bofstream &outfile,const std::list<T> &basictypelist)
{
	unsigned int mysize=(unsigned int)(basictypelist.size());
	outfile.write((VChr*)&mysize,sizeof(unsigned int));
    std::list<T>::const_iterator j = basictypelist.begin();
	for(unsigned int i=0;i<mysize;i++)
	{
        T currt = *j;
		outfile.write((VChr*)&currt,sizeof(T));
		j++;
	}
};

template <class T> void SerializeBasicTypeList(Bifstream &infile,std::list<T> &basictypelist)
{
	unsigned int mysize;
	infile.read((VChr*)&mysize,sizeof(unsigned int));
	for(unsigned int i=0;i<mysize;i++)
	{
		T currt;
		infile.read((VChr*)&currt,sizeof(T));
		basictypelist.push_back(currt);
	}
};

template <class T> void SerializePtrList (Bofstream &outfile,const std::list<T*> &ptrlist)
{
	unsigned int myvecsize=ptrlist.size();
	outfile.write((VChr*)&myvecsize,sizeof(unsigned int));
	std::list<T*>::const_iterator j=ptrlist.begin();

	for (int i=0;i<myvecsize;i++)
	{
		T *currp=*j;
		unsigned int myptr=(unsigned int)(currp);
		outfile.write((VChr*)&myptr,sizeof(unsigned int));	
		j++;
	}
}

template <class T> void SerializePtrList(Bifstream &infile,std::list<T*> &ptrlist)
{
	unsigned int myvecsize;
	infile.read((VChr*)&myvecsize,sizeof(unsigned int));
	for (int i=0;i<myvecsize;i++)
	{
		unsigned int myptr;
		infile.read((VChr*)&myptr,sizeof(unsigned int));	
		T *currp=(T*)(myptr);
		ptrlist.push_back(currp);
	}
};

template <class T> void SerializeTList (Bofstream &outfile,const std::list<T> &tlist)
{
	unsigned int myvecsize=(unsigned int) (tlist.size());
	outfile.write((VChr*)&myvecsize,sizeof(unsigned int));
	std::list<T>::const_iterator j=tlist.begin();	
	for (int i=0;i<myvecsize;i++)
	{
		T currt = *j;
		currt.Serialize(outfile);
		j++;
	}
};
template <class T> void SerializeTList (Bifstream &infile,std::list<T> &tlist)
{
	unsigned int myvecsize;
	infile.read((VChr*)&myvecsize,sizeof(unsigned int));
	for (int i=0;i<myvecsize;i++)
	{
		T currt;
		currt.Serialize(infile);
		tlist.push_back(currt);
	}
};

#endif
