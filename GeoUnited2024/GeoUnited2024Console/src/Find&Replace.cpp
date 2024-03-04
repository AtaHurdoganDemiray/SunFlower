#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <locale.h>
#include <clocale>
using namespace std;

int main(){
	int a;
	int b;
	int offset =0;
	std::ifstream dosyaoku;
	std::ofstream dosyayaz;
	dosyaoku.open("deneme.txt");
	dosyayaz.open("new.txt");
	string satir;
	string orginaltext;	
	std::string tosearch;

	if(dosyaoku.is_open())
	{
		while(getline(dosyaoku,satir))
		{
			orginaltext += satir ;
			orginaltext += "\n";
			b =orginaltext.length();
		
		}
		cout << orginaltext << endl;
	}
	
	cout << "Search= ";
	cin >> tosearch;
	a = orginaltext.find(tosearch,offset);
	
		if(a==-1)
		{
			cout<<"!!!Search Not Found..."<<endl;
			cout << "Search=";
			cin >> tosearch;
		}
		
		else{
			
	std::string toreplace ;
		cout << endl<< "Replace=";
		cin >> toreplace;
	std::string modifiedArray;
	


	
	while(a != -1)
	{
		for (int i=offset; i<a;++i)
		{
			modifiedArray += orginaltext[i];		
		}
		modifiedArray += toreplace;
		offset = a + tosearch.length();
		a = orginaltext.find(tosearch,offset);
	}
	for (int i=offset;i<orginaltext.length();++i)
	{
		modifiedArray += orginaltext[i];
	}
	cout << modifiedArray<<endl;
	dosyayaz<<modifiedArray<<endl;
	dosyayaz.close();
	dosyaoku.close();
			}

}
