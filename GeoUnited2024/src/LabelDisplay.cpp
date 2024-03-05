// LabelDisplay.cpp

#include "stdafx.h"
#include "LabelDisplay.h"
#include "Model.h"
#include "OglWindowData.h"
#include "TextBox.h"
#include "ViewVolume.h"
#include "Mouse.h"
#include "ViewPointItemTypes.h"

vreal LABELTEXT_X = 120.;
vreal LABELTEXT_Y = 20.;

LabelDisplay::LabelDisplay (const std::vector<Display *> * asciiDisp , Model *model , vreal textX , vreal textY)
	:m_vv(0), DraftEntity(model), m_asciiDisp(asciiDisp)
{
	m_textX = textX;
	m_textY = textY;
	m_visatt.ChangeWireColor (VColor (0,1,0));
	m_xBase = 0.f;
	m_yBase = 0.f;
}

LabelDisplay::LabelDisplay (const std::vector<Display *> *asciiDisp 
							, Model *model 
							, const VPoint &p 
							, const VStr &txt
							, const VStr &name
							, vreal textX 
							, vreal textY)
							: DraftEntity (model)
							, m_vv(0),m_txt(txt)
							, m_asciiDisp(asciiDisp)
{
	modelpoint[0] = static_cast<GLfloat> (p.x());
	modelpoint[1] = static_cast<GLfloat> (p.y());
	modelpoint[2] = static_cast<GLfloat> (p.z());
	modelpoint[3] = 0.0f;

	m_textX = textX;
	m_textY = textY;

	// Initial positions
	//GLfloat labeltextxoffs = 130.0f;
	//GLfloat labeltextyoffs = m_vv->corner_y - 20.0f;
	base[0] = (float) (m_textX + 10.);//130.0f;//labeltextxoffs;
	//base[1] = labeltextyoffs;
	//labeltextxoffs += 120.0f;
	//labeltextyoffs -= 22.0f;
//	if (labeltextyoffs < 24.0f)
//	{
//		labeltextxoffs += 130.0f;
//		if (labeltextxoffs > m_vv->corner_x - 60.0f)
//		{
//			labeltextxoffs = 150.0f;
//		}
//		labeltextyoffs = m_vv->corner_y - 20.0f;
//	}
	m_visatt.ChangeWireColor (VColor (0,1,0));
	m_xBase = 0.f;
	m_yBase = 0.f;
}

//GLfloat LABELTEXTCOLOR[4] = {0.0f,0.0f,0.0f,0.0f};

void LabelDisplay::SetTextBoxXY (vreal textX , vreal textY)
{
	m_textX = textX;
	m_textY = textY;
	base[0] = (float) (m_textX + 10.);
	//base[1] = 
}
	
void LabelDisplay::SetModelPoint (const VPoint &p)
{
	modelpoint[0] = static_cast<GLfloat> (p.x());
	modelpoint[1] = static_cast<GLfloat> (p.y());
	modelpoint[2] = static_cast<GLfloat> (p.z());
	modelpoint[3] = 0.0f;
}

void LabelDisplay::Draw()
{
	if (m_model->IsVisible() == false || m_model->GetDeleted() == true || m_vv == 0)
		return;

	OglWindowData wd (m_vv->corner_x,m_vv->corner_y,m_vv->m_near,m_vv->m_far);
	GLfloat screenpoint[4];
	bool ck = wd.GetScreenCoordOfPoint (modelpoint,screenpoint);

	if (!ck)
	{
		for (int jj=0;jj<2;++jj)
		screenpoint[jj] = base[jj] + VGlobalValues::ONEMM_TO_VUNIT;
	}
		//base[0] = screenpoint[0]+10.0f;
		//base[1] = screenpoint[1]+15.0f;
		base[1] = GLfloat(m_vv->corner_y - m_textY);
		GLfloat newBase[2] = {base[0] + m_xBase , base[1] + m_yBase};
		TextBox txtbox (  m_textX
					    , m_textY
						, m_txt.data() 
						, m_asciiDisp);
						//,LABELTEXTCOLOR);
		TextBoxPosData boxpos (newBase,screenpoint,newBase);
		TextBoxDisplayer txtboxdisplayer (wd);
		txtboxdisplayer.Display (txtbox,boxpos);
	
}

void LabelDisplay::OnResizeWindow ( OglWindowData & mywindow )
{

}

bool LabelDisplay::IsHit(Mouse &mouse) const
{
	if (m_vv == 0)
		return false;
	vreal c1 = (vreal) base[0];
	vreal c2 = m_vv->corner_y - (vreal) base[1];
	if (fabs (mouse.GetLButtonDownP().x() - c1) < 5.0 && 
		fabs (mouse.GetLButtonDownP().y() - c2) < 5.0)
	{
		return true;
	}
	else
		return false;
}

void LabelDisplay::Run (Mouse &mouse)
{
	if (m_vv == 0)
		return;
	base[0] = (GLfloat)mouse.GetCurrentP().x();
	base[1] = (GLfloat) ( m_vv->corner_y - mouse.GetCurrentP().y());
}

LineVisualAtt & LabelDisplay::GetLineVisualAtt()
{
	return m_visatt;
}

const VColor & LabelDisplay::GetColor() const
{
	return m_visatt.GetWireColor();
}

void LabelDisplay::SetViewVolume (const ViewVolume *vv)
{
	m_vv = vv;
}

const VStr LabelDisplay::GetModelTypeEng ()const
{
	return ViewPointItemTypes::LabelTextEng();
}

void LabelDisplay::UpdateModelPoint(const VPoint &p)
{
	modelpoint[0] = static_cast<GLfloat> (p.x());
	modelpoint[1] = static_cast<GLfloat> (p.y());
	modelpoint[2] = static_cast<GLfloat> (p.z());
}
