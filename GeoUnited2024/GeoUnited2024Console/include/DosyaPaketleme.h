// DosyaPaketleme.h

#ifndef __DosyaPaketleme_h__
#define __DosyaPaketleme_h__

#include <string>
#include <fstream>
#include <iostream>
#include<vector>
#include<map>
#include<Windows.h>
#include<tchar.h>

class DosyaPaketleme
{
public:
	static void KlasoruPaketle (const VStr &klasorDir , const VStr &paketismi);
	static void PaketiKlasoreAc (const VStr &paketDir , const VStr &nereyeAcayim);//dikkat nereyeAcayim in sonunda // olmali!!!
	  
//private:	
	static void HeaderYaz (VofStream &f , const std::map<VStr,size_t> &dosyalar);
	static void HeaderOku (VIFStream &f , std::map<VStr,size_t> &dosyalar);
	static void Paketle (const std::map<VStr,size_t> &dosyalar , VofStream &f ,const VStr &nereyiPaketliyim);
	static void PaketiAc (VIFStream &f , const std::map<VStr,size_t> &dosyalar , const VStr &nereyeAcayim);
	static void PaketeEkle (const VStr &dosyaAd , size_t dosyaBoyu , VofStream &f ,const VStr &nereyiPaketliyim);
	static void PakettenCikar (VIFStream &f , const VStr &dosyaAd , const VStr &anaDizin , size_t dosyaBoyu);
	static void KlasordeGezVeDosyaIsimleriniDoldur (std::map<VStr , size_t> &dosyalar , const VStr &klasorKokDizini);
	static VStr NereyeAcilacaginiBelirleFonks(const VStr &paketDir );
	
	static size_t ALTPAKETBOYU;
	static char * m_arr;
};

#endif