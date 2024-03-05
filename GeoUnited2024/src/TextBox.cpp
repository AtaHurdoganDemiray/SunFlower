// TextBox.cpp

#include "stdafx.h"
#include "TextBox.h"
#include "gl/gl.h"
//#include "gl/glut.h"
#include "OglWindowData.h"
//#include "NumberFonts.h"
#include "Display.h"
#include "ViewPointDefaults.h"

#define X 0
#define Y 1

TextBox::TextBox ( const vreal &_xwidth , const vreal &_ywidth 
				  , const CString &_mtext
				  , const std::vector<Display *> *asciiDisp)//, const GLfloat _mcolor[3] )
{
	xwidth = _xwidth;
	ywidth = _ywidth;
	mtext = _mtext;
	//mcolor[0] = _mcolor[0];
	//mcolor[1] = _mcolor[1];
	//mcolor[2] = _mcolor[2];
	fonth = VGlobalValues::viewpointdefaults.m_defaultFontHeight;//7.5;
	fontw = VGlobalValues::viewpointdefaults.m_defaultFontWidth;// 7.5;
	m_asciiDisp = asciiDisp;
}

TextBoxPosData::TextBoxPosData ( const GLfloat _base[] , const GLfloat _linestart[] , const GLfloat _lineend[] )
{
	base[0] = _base[0];
	base[1] = _base[1];
	//base[2] = _base[2];

	linestart[0] = _linestart[0];
	linestart[1] = _linestart[1];
	//linestart[2] = _linestart[2];

	lineend[0] = _lineend[0];
	lineend[1] = _lineend[1];
	//lineend[2] = _lineend[2];
}

TextBoxCursor::TextBoxCursor ( const TextBox &txtbox , const TextBoxPosData &posdata 
							  , const vreal & fontxwidth , const vreal & fontywidth )
:mtxtbox ( txtbox ) , mposdata ( posdata )
{
	mfontxwidth = fontxwidth;
	mfontywidth = fontywidth;
	lineno = static_cast <int> ( txtbox.ywidth / mfontywidth );
	linefontno = static_cast <int> ( txtbox.xwidth / mfontxwidth );
	
	mpos[X] = mposdata.base[X] - mtxtbox.xwidth;
	mpos[Y] = mposdata.base[Y] + mtxtbox.ywidth - mfontywidth*2;
}

vreal TextBoxCursor::operator ++ ()
{
	mpos[X] += mfontxwidth*1.6;
	return mpos[X];
}

void TextBoxCursor::NextLine ()
{
	mpos[X] = mposdata.base[X] - mtxtbox.xwidth;
	mpos[X] += mfontxwidth;
	mpos[Y] -= mfontywidth * 4;
}

TextBoxDisplayer::TextBoxDisplayer ( const OglWindowData &wnddata  )
: mwnddata ( wnddata )
{

}
GLubyte rasters[13] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
void TextBoxDisplayer::Display ( const TextBox &txtbox , const TextBoxPosData &posdata )const
{
	//glDisable ( GL_LIGHTING );
	//glColor3d ( txtbox.mcolor[0] , txtbox.mcolor[1] , txtbox.mcolor[2] );
	//glLineWidth ( 1.f );
	glMatrixMode ( GL_PROJECTION );
	glPushMatrix ();
	glLoadIdentity ();

	glOrtho ( 0.0 , (GLfloat) mwnddata.mcornerx , 0.0 , (GLfloat) mwnddata.mcornery 
				, mwnddata.mnear , mwnddata.mfar );
	VColor fc = VGlobalValues::viewpointdefaults.m_defaultFontColor;
	glColor3d(fc.R(), fc.G(), fc.B());
	glMatrixMode ( GL_MODELVIEW );
	glPushMatrix ();
	glLoadIdentity ();
	
	// Drawing the line.
	glBegin ( GL_LINES );
	
	// arrow
	glVertex3d ( posdata.lineend[X] , posdata.lineend[Y] , mwnddata.mfar*0.98 );
	glVertex3d ( posdata.linestart[X] , posdata.linestart[Y] , mwnddata.mfar*0.98 );
	
	// box of text
	glVertex3d ( posdata.base[X] , posdata.base[Y] , mwnddata.mfar*0.98 );
	glVertex3d ( posdata.base[X]-txtbox.xwidth , posdata.base[Y] , mwnddata.mfar*0.98 );	
	
	glVertex3d ( posdata.base[X] , posdata.base[Y] , mwnddata.mfar*0.98 );
	glVertex3d ( posdata.base[X] , posdata.base[Y]+txtbox.ywidth , mwnddata.mfar*0.98 );
	
	glVertex3d ( posdata.base[X]-txtbox.xwidth , posdata.base[Y] , mwnddata.mfar*0.98 );
	glVertex3d ( posdata.base[X]-txtbox.xwidth , posdata.base[Y]+txtbox.ywidth , mwnddata.mfar*0.98 );
	
	glVertex3d ( posdata.base[X]-txtbox.xwidth , posdata.base[Y]+txtbox.ywidth , mwnddata.mfar*0.98 );
	glVertex3d ( posdata.base[X], posdata.base[Y]+txtbox.ywidth , mwnddata.mfar*0.98 );

	glEnd ();

	unsigned char myVChr;
	vreal fontw = txtbox.fontw , fonth = txtbox.fonth;
	vreal scalef = 2.5 * txtbox.fonth / 7.5;
	TextBoxCursor crsr ( txtbox , posdata , fontw , fonth );
	++crsr;
	++crsr;
	int lineno = 0;
	for ( int i = 0; i<txtbox.mtext.GetLength(); i++ )
	{	
		myVChr = txtbox.mtext[i];
		if (myVChr == '\n')
		{
			crsr.NextLine();
			continue;
		}
		else if (myVChr == ' ')// || myVChr == ':')
		{
			++crsr;
			continue;
		}
		try
		{
			if (txtbox.m_asciiDisp->at((int)myVChr) != 0)
			{
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glLoadIdentity();
				glTranslated(crsr.mpos[X], crsr.mpos[Y], mwnddata.mfar*0.98);
				glScaled(scalef, scalef, scalef);
				txtbox.m_asciiDisp->at((int)myVChr)->DrawLines(false);
				glPopMatrix();
			}
			else
				continue;
		}
		catch (...)
		{
			continue;
		}

		//glRasterPos3f	( (GLfloat)((posdata.base[X] - 0.8*txtbox.xwidth)+(i*7)) 
		//				, (GLfloat)((posdata.base[Y] + 0.2*txtbox.ywidth)) 
		//				, (GLfloat)  mfar );

		//glRasterPos3f	( (GLfloat)( crsr.mpos[X] ) 
						//, (GLfloat)( crsr.mpos[Y] ) 
						//, (GLfloat)  mwnddata.mfar*0.98 );
	//	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, myVChr );
		//glBitmap(8, 12 , 0.0, 0.0, 0.0, 0.0, rasters);
		++crsr;
	}

	glMatrixMode ( GL_PROJECTION );
	glPopMatrix ();
	glMatrixMode ( GL_MODELVIEW );
	glPopMatrix ();
	
}

//void TextBoxDisplayer::DisplayOnlyText ( const TextBox &txtbox , const TextBoxPosData &posdata )const
//{
//	//glDisable ( GL_LIGHTING );
//	//glColor3d ( txtbox.mcolor[0] , txtbox.mcolor[1] , txtbox.mcolor[2] );
//	//glLineWidth ( 1.f );
//	glMatrixMode ( GL_PROJECTION );
//	glPushMatrix ();
//	glLoadIdentity ();
//
//	glOrtho ( 0.0 , (GLfloat) mwnddata.mcornerx , 0.0 , (GLfloat) mwnddata.mcornery 
//				, mwnddata.mnear , mwnddata.mfar );
//	
//	glMatrixMode ( GL_MODELVIEW );
//	glPushMatrix ();
//	glLoadIdentity ();
//	
//	// Drawing the line.
//	//glBegin ( GL_LINES );
//	
//	// arrow
//	//glVertex3d ( posdata.lineend[X] , posdata.lineend[Y] , mwnddata.mfar*0.98 );
//	//glVertex3d ( posdata.linestart[X] , posdata.linestart[Y] , mwnddata.mfar*0.98 );
//	
//	// box of text
//	//glVertex3d ( posdata.base[X] , posdata.base[Y] , mwnddata.mfar*0.98 );
//	//glVertex3d ( posdata.base[X]-txtbox.xwidth , posdata.base[Y] , mwnddata.mfar*0.98 );	
//	
//	//glVertex3d ( posdata.base[X] , posdata.base[Y] , mwnddata.mfar*0.98 );
//	//glVertex3d ( posdata.base[X] , posdata.base[Y]+txtbox.ywidth , mwnddata.mfar*0.98 );
//	
//	//glVertex3d ( posdata.base[X]-txtbox.xwidth , posdata.base[Y] , mwnddata.mfar*0.98 );
//	//glVertex3d ( posdata.base[X]-txtbox.xwidth , posdata.base[Y]+txtbox.ywidth , mwnddata.mfar*0.98 );
//	
//	//glVertex3d ( posdata.base[X]-txtbox.xwidth , posdata.base[Y]+txtbox.ywidth , mwnddata.mfar*0.98 );
//	//glVertex3d ( posdata.base[X], posdata.base[Y]+txtbox.ywidth , mwnddata.mfar*0.98 );
//
//	//glEnd ();
//
//	VChr myVChr;
//	vreal fontw = 7.0 , fonth = 7.0;
//	TextBoxCursor crsr ( txtbox , posdata , fontw , fonth );
//
//	++crsr;
//	++crsr;
//	for ( int i = 0; i<txtbox.mtext.GetLength(); i++ )
//	{	
//		myVChr = txtbox.mtext[i];
//
//		//glRasterPos3f	( (GLfloat)((posdata.base[X] - 0.8*txtbox.xwidth)+(i*7)) 
//		//				, (GLfloat)((posdata.base[Y] + 0.2*txtbox.ywidth)) 
//		//				, (GLfloat)  mfar );
//
//		//glRasterPos3f	( (GLfloat)( crsr.mpos[X] ) 
//		//				, (GLfloat)( crsr.mpos[Y] ) 
//		//				, (GLfloat)  mwnddata.mfar*0.98 );
////		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, myVChr );
//		//glBitmap(10, 12, 0.0, 0.0, 0.0, 0.0, rasters);
//		++crsr;
//	}
//
//	glMatrixMode ( GL_PROJECTION );
//	glPopMatrix ();
//	glMatrixMode ( GL_MODELVIEW );
//	glPopMatrix ();
//	
//}

void TextBoxDisplayer::DisplayOnlyText(const TextBox &txtbox, const TextBoxPosData &posdata)const
{	
	vreal fr = VGlobalValues::viewpointdefaults.m_defaultFontColor.R();
	vreal fg = VGlobalValues::viewpointdefaults.m_defaultFontColor.G();
	vreal fb = VGlobalValues::viewpointdefaults.m_defaultFontColor.B();

	glColor3d(fr, fg, fb);
	unsigned char myVChr;
	vreal fontw = txtbox.fontw, fonth = txtbox.fonth;
	vreal scalef = 2.5 * txtbox.fonth /7.5;
	TextBoxCursor crsr(txtbox, posdata, fontw, fonth);
	++crsr;
	++crsr;
	int lineno = 0;
	for (int i = 0; i<txtbox.mtext.GetLength(); i++)
	{
		myVChr = txtbox.mtext[i];
		if (myVChr == '\n')
		{
			crsr.NextLine();
			continue;
		}
		else if (myVChr == ' ' )//|| myVChr == ':')
		{
			++crsr;
			continue;
		}
		try
		{
			if (txtbox.m_asciiDisp->at((unsigned int)myVChr) != 0)
			{
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glLoadIdentity();
				glTranslated(crsr.mpos[X], crsr.mpos[Y], mwnddata.mfar*0.98);
				glScaled(scalef, scalef, scalef);
				txtbox.m_asciiDisp->at((unsigned int)myVChr)->DrawLines(false);
				glPopMatrix();
			}
			else
				continue;
		}
		catch (...)
		{
			continue;
		}
		++crsr;
	}
}
TextBoxDisplayer & TextBoxDisplayer::operator = (const TextBoxDisplayer &toCopy)
{
	return *this;
}

/*
ASCII printable characters
Dec	Hex	Binary	Char -
acter
Description
32	20	00100000	Space	space
33	21	00100001	!exclamation mark
34	22	00100010	"	double quote
35	23	00100011	#	number
36	24	00100100	$	dollar
37	25	00100101 % percent
38	26	00100110 & ampersand
39	27	00100111	'	single quote
40	28	00101000	(left parenthesis
41	29	00101001)	right parenthesis
42	2A	00101010 * asterisk
43	2B	00101011 + plus
44	2C	00101100, comma
45	2D	00101101 - minus
46	2E	00101110.period
47	2F	00101111 / slash
48	30	00110000	0	zero
49	31	00110001	1	one
50	32	00110010	2	two
51	33	00110011	3	three
52	34	00110100	4	four
53	35	00110101	5	five
54	36	00110110	6	six
55	37	00110111	7	seven
56	38	00111000	8	eight
57	39	00111001	9	nine
58	3A	00111010	:	colon
59	3B	00111011;	semicolon
60	3C	00111100	<	less than
61	3D	00111101 = equality sign
62	3E	00111110	>	greater than
63	3F	00111111 ? question mark
64	40	01000000	@	at sign
65	41	01000001	A
66	42	01000010	B
67	43	01000011	C
68	44	01000100	D
69	45	01000101	E
70	46	01000110	F
71	47	01000111	G
72	48	01001000	H
73	49	01001001	I
74	4A	01001010	J
75	4B	01001011	K
76	4C	01001100	L
77	4D	01001101	M
78	4E	01001110	N
79	4F	01001111	O
80	50	01010000	P
81	51	01010001	Q
82	52	01010010	R
83	53	01010011	S
84	54	01010100	T
85	55	01010101	U
86	56	01010110	V
87	57	01010111	W
88	58	01011000	X
89	59	01011001	Y
90	5A	01011010	Z
91	5B	01011011[left square bracket
92	5C	01011100	\	backslash
93	5D	01011101]	right square bracket
94	5E	01011110 ^ caret / circumflex
95	5F	01011111	_	underscore
96	60	01100000	`	grave / accent
97	61	01100001	a
98	62	01100010	b
99	63	01100011	c
100	64	01100100	d
101	65	01100101	e
102	66	01100110	f
103	67	01100111	g
104	68	01101000	h
105	69	01101001	i
106	6A	01101010	j
107	6B	01101011	k
108	6C	01101100	l
109	6D	01101101	m
110	6E	01101110	n
111	6F	01101111	o
112	70	01110000	p
113	71	01110001	q
114	72	01110010	r
115	73	01110011	s
116	74	01110100	t
117	75	01110101	u
118	76	01110110	v
119	77	01110111	w
120	78	01111000	x
121	79	01111001	y
122	7A	01111010	z
123	7B	01111011	{	left curly bracket
124	7C	01111100 | vertical bar
125	7D	01111101	}	right curly bracket
126	7E	01111110	~tilde
127	7F	01111111	DEL	delete
*/
/* 
Extended char
161 Ý
176   degree
199 Ç 
231 ç
214 Ö
246 ö
220 Ü
252 ü
*/

