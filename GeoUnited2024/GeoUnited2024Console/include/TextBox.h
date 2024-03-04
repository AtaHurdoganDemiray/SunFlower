// TextBox.h

#ifndef __TextBox_h__
#define __TextBox_h__

struct OglWindowData;
class Display;

struct TextBox
{
public:
	TextBox ( const vreal &_xwidth , const vreal &_ywidth 
			, const CString &_mtext 
			, const std::vector<Display *> *asciiDisp);//, const GLfloat _mcolor[3] );
	
	vreal xwidth , ywidth;
	CString mtext;
	//GLfloat mcolor[3];
	const std::vector <Display *> *m_asciiDisp;
	vreal fonth, fontw;
};

struct TextBoxPosData
{
public:
	TextBoxPosData 
		( const GLfloat _base[2] , const GLfloat _linestart[2] , const GLfloat _lineend[2] ); 
	
	// right , bottom corner of box
	GLfloat base[2];
	GLfloat linestart[2];
	GLfloat lineend[2];
};

struct TextBoxCursor
{
public:
	TextBoxCursor ( const TextBox &txtbox , const TextBoxPosData &posdata 
					, const vreal &fontxwidth , const vreal &fontywidth );
	
	vreal operator ++();
	void NextLine();
	
	vreal mpos[2];
	const TextBox &mtxtbox;
	const TextBoxPosData &mposdata;
private:
	vreal mfontxwidth , mfontywidth;
	int lineno , linefontno;
};

class TextBoxDisplayer
{
public:
	TextBoxDisplayer (const OglWindowData &wnddata);
	TextBoxDisplayer & operator = (const TextBoxDisplayer &toCopy);
	void Display (const TextBox &txtbox , const TextBoxPosData &posdata)const;
	void DisplayOnlyText (const TextBox &txtbox , const TextBoxPosData &posdata)const;	
	const OglWindowData &mwnddata;
};

#endif