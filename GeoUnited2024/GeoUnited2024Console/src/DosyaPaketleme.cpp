// DosyaPaketleme.cpp

#include "stdafx.h"
#include "DosyaPaketleme.h"
#include "PathStringFunctions.h"
#include "VFileDirectoryFunctions.h"

	size_t DosyaPaketleme::ALTPAKETBOYU = 1024;
	char * DosyaPaketleme::m_arr = new char[DosyaPaketleme::ALTPAKETBOYU];
	using namespace std;

	void DosyaPaketleme::KlasoruPaketle (const std::string &klasorDir,const VStr &paketismi)
	{
		try
		{
			//std::string paketismi (klasorDir);
			//paketismi += (".vpj");
			std::ofstream f (paketismi.c_str(),ios::binary);
			if (f.is_open() == false)
			{
				throw std::exception ( IDS_YazPakDosAci);
			}
			std::map<std::string , size_t> dosyalar;
			// Bu mapi klasör içindeki dosyalar adý ve boyutlarýný öðrenip eklemem lazým
			KlasordeGezVeDosyaIsimleriniDoldur (dosyalar,klasorDir); 
			HeaderYaz (f,dosyalar);
			Paketle (dosyalar , f , klasorDir);
			f.close();
		}
		catch (std::exception &e)
		{
			LogFileWriter::WriteUnknownExceptionLog (( IDS_DosKlaStdExc));
			LogFileWriter::WriteExceptionLog (e);
			throw VException (( IDS_DosKlaIsNotFin),1);
		}
	}
	void DosyaPaketleme::PaketiKlasoreAc (const std::string &paketDir , const VStr &nereyeAcayim)
	{
		//const std::string &nereyeAcayim = NereyeAcilacaginiBelirleFonks(paketDir); //dosylarýn acýlacagý klasor .vpj dosyasýyla ayný isimde ve ayný dizinde olmsý için...  
		CreateDirectory(nereyeAcayim.c_str(),NULL);  
		try
		{
			std::ifstream f (paketDir.c_str(),ios::binary);
			if (f.is_open() == false)
				throw std::exception ( IDS_AciPakDosIsOpeO);
			std::map<std::string,size_t> dosyalar ;
			
			const string paketDir_temp=paketDir;
			HeaderOku (f , dosyalar);
			
			PaketiAc (f , dosyalar,nereyeAcayim);
			f.close();
			
		}
		catch (std::exception &e)
		{
			//RemoveDirectory(nereyeAcayim.c_str());
			LogFileWriter::WriteUnknownExceptionLog (( IDS_DosPakStdExc));
			LogFileWriter::WriteExceptionLog (e);
			throw VException (( IDS_DosPakIsNotFin),1);
		}
	}
	void DosyaPaketleme::HeaderYaz (std::ofstream &f , const std::map<std::string,size_t> &dosyalar)
	{
		std::map<std::string,size_t>::const_iterator i = dosyalar.begin();
		std::map<std::string,size_t>::const_iterator e = dosyalar.end();
		f << dosyalar.size()<< IDS_N;
		for (i;i!=e;++i)
		{
			f << i->first<< IDS_N;
			f << i->second<< IDS_N;
		}
	
	}
	void DosyaPaketleme::HeaderOku (std::ifstream &f , std::map<std::string,size_t> &dosyalar)
	{
		std::string isim("");
		size_t boyut;
		size_t dosyaSayisi;
		char temp[1024];
		f.getline (temp,1024);
		std::stringstream mystr;
		mystr<<temp;
		mystr>>dosyaSayisi;
		for (size_t i=0;i<dosyaSayisi;++i)
		{
			std::stringstream mystr1;
			f.getline (temp,1024);
			std::string yeniIsim (temp);
			f.getline (temp,1024);
			mystr1 << temp;
			mystr1 >> boyut;
			/*if(VPJDoasyasimi(yeniIsim)==true)
			{
				HeaderOku(f,dosyalar);
			}*/
			dosyalar.insert (std::pair<std::string,size_t> (yeniIsim , boyut));

		}
	}
	void DosyaPaketleme::Paketle (const std::map<std::string,size_t> &dosyalar , std::ofstream &f , const std::string &nereyiPaketliyim)
	{
		std::map<std::string,size_t>::const_iterator i = dosyalar.begin();
		std::map<std::string,size_t>::const_iterator e = dosyalar.end();
		for (i;i!=e;++i)
		{
			PaketeEkle (i->first,i->second,f,nereyiPaketliyim);
		}
	}
	void DosyaPaketleme::PaketiAc (std::ifstream &f , const std::map<std::string,size_t> &dosyalar , const std::string &nereyeAcayim)
	{
		std::map<std::string,size_t>::const_iterator i = dosyalar.begin();
		std::map<std::string,size_t>::const_iterator e = dosyalar.end();
		for (i;i!=e;++i)
		{
			PakettenCikar (f , i->first , nereyeAcayim , i->second);
		}
	}
	void DosyaPaketleme::PaketeEkle (const std::string &dosyaAd , size_t dosyaBoyu , std::ofstream &f ,const std::string &nereyiPaketliyim)
	{
		std::string toplamYol="";
		if (nereyiPaketliyim.at(nereyiPaketliyim.size() - 1) != '\\')
			toplamYol += nereyiPaketliyim + "\\" + dosyaAd;
		else
			toplamYol = nereyiPaketliyim + dosyaAd;
		std::ifstream kucukDosya (toplamYol.c_str(),ios::binary);
		if (kucukDosya.is_open() == false)
			throw VException ("DosyaPaketleme::PaketeEkle exception , pack file is not open",1);
		size_t yazilanBoyut = 0;
		//char *arr = new char [ALTPAKETBOYU];
		while (dosyaBoyu > ALTPAKETBOYU + yazilanBoyut)
		{
			kucukDosya.read (m_arr,ALTPAKETBOYU);
			f.write (m_arr , ALTPAKETBOYU);
			yazilanBoyut += ALTPAKETBOYU;
		}
		kucukDosya.read (m_arr , dosyaBoyu - yazilanBoyut);
		f.write (m_arr , dosyaBoyu - yazilanBoyut);

		//delete [] arr;
		kucukDosya.close();
	}
	void DosyaPaketleme::PakettenCikar (std::ifstream &f , const std::string &dosyaAd , const std::string &anaDizin , size_t dosyaBoyu)
	{
	
		std::string toplamYol="";
		
		if (anaDizin.at(anaDizin.size() - 1) != '\\')
			toplamYol += (anaDizin + "\\");
		else
			toplamYol += anaDizin;
		//CreateDirectory(toplamYol.c_str(),0);
		VFileDirectoryFunctions::CreateDirectoryWithMissingPath(toplamYol.c_str());
		toplamYol += dosyaAd;
		std::ofstream kucukDosya ( toplamYol.c_str(), ios::binary|ios::app);
		size_t okunanBoyut = 0;
		//char *arr = new char [ALTPAKETBOYU];
		while (dosyaBoyu  > ALTPAKETBOYU + okunanBoyut)
		{
			f.read (m_arr , ALTPAKETBOYU);
			kucukDosya.write (m_arr , ALTPAKETBOYU);
			okunanBoyut += ALTPAKETBOYU;
		}
		f.read (m_arr , dosyaBoyu - okunanBoyut);
		kucukDosya.write (m_arr , dosyaBoyu - okunanBoyut);
		kucukDosya.close();
	}
	void DosyaPaketleme::KlasordeGezVeDosyaIsimleriniDoldur ( std::map<std::string , size_t> &Dosyalar , const std::string &kokDizin)
	{
		string ilkStr (kokDizin);
		if (ilkStr.at(ilkStr.size() - 1) != '\\')
			ilkStr += "\\*.*";
		else
			ilkStr += "*.*";

		WIN32_FIND_DATA dos_bil;
		HANDLE hDosya = FindFirstFile( ilkStr.c_str() , &dos_bil);
		if(hDosya == INVALID_HANDLE_VALUE)
		{
			//FindClose(hDosya);
			return;
		}
		do
		{
			if ( (dos_bil.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)		
			{
				// bu bir directory (folder) dizin  DOSYA DEGIL
				string tempDosya (dos_bil.cFileName);
				if (tempDosya != "." && tempDosya != "..")
				{
					//string yeniKokDizin (tempDosya);
					string yeniKokDizin(kokDizin);
					if (yeniKokDizin.at(yeniKokDizin.size() - 1) != '\\')
						yeniKokDizin +="\\";
					yeniKokDizin += dos_bil.cFileName;
					
					KlasordeGezVeDosyaIsimleriniDoldur (Dosyalar , yeniKokDizin);
				}	
			}
			else
			{
				// bu bir dosya
				string toplamyol (kokDizin);
				string myStr(dos_bil.cFileName);
				if (toplamyol.at(toplamyol.size() - 1) != '\\')
					toplamyol+= "\\";
				toplamyol +=dos_bil.cFileName;
			
				//string uzanti="";
				//for(size_t  i=toplamyol.length()-3;i<toplamyol.length();i++)
				//{
				//	uzanti+=toplamyol[i];
				//}
				//if (uzanti!=""|| uzanti!="." ||uzanti!="..")  

				{
					cout<<toplamyol<<endl;
					size_t myfilesize = dos_bil.nFileSizeHigh * MAXDWORD + dos_bil.nFileSizeLow;
					Dosyalar.insert(pair<string,size_t >(myStr, myfilesize));
				}	
			}
		}
		while (FindNextFile(hDosya, &dos_bil));
		FindClose(hDosya);
	}
	string DosyaPaketleme::NereyeAcilacaginiBelirleFonks(const std::string &paketDir)
	{
		string dosyaKlasoru="";
		int dizinuzunlugu=(int)paketDir.size()-4;
		
		for(int i=0;i<dizinuzunlugu;i++)
		{
			dosyaKlasoru+=paketDir[i];
		}
		return dosyaKlasoru;

	}

	
