/******************************************************************************
*
* Project:  TileChart
* Purpose:  TileChart plugin
* Author:   Bernd Cirotzki
*
***************************************************************************
*   Copyright (C) 2024 by Bernd Cirotzki                                  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
***************************************************************************
*
*/

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include <wx/glcanvas.h>
#include <wx/graphics.h>
#include <wx/progdlg.h>

#include "TileChartgui_impl.h"
#include "SatOverlayFactory.h"
#include <vector>


#ifdef __WXMSW__
#define snprintf _snprintf
#endif // __WXMSW__



class PlugIn_ViewPort;

SatOverlayFactory::SatOverlayFactory(Dlg *gui, TileChart *pPlugin)
{
    m_dlg = gui;
    p_Plugin = pPlugin;
    StartLat = 0;
    StopLat = 0;
    TempLon = 0;
    TempLat = 0;
}

SatOverlayFactory::~SatOverlayFactory()
{
}

bool SatOverlayFactory::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    if (StartLat == 0) return false; // Not in paint mode.
    m_pdc = NULL;  // inform lower layers that this is OpenGL render

    if (p_Plugin->ShowChartSelection)
        DrawAll(vp);
    DrawRealTile(vp);
	return true;
}

bool SatOverlayFactory::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    if (StartLat == 0) return false; // Not in paint mode.
#if wxUSE_GRAPHICS_CONTEXT
	wxMemoryDC *pmdc;
	pmdc = wxDynamicCast(&dc, wxMemoryDC);
	wxGraphicsContext *pgc = wxGraphicsContext::Create(*pmdc);
	m_gdc = pgc;
	m_pdc = &dc;
#else
	m_pdc = &dc;
#endif
    if(p_Plugin->ShowChartSelection)
        DrawAll(vp);
    DrawRealTile(vp);
	return true;
}

void SatOverlayFactory::DrawAll(PlugIn_ViewPort *BBox)
{
    wxPoint Start, Stop;

	GetCanvasPixLL(BBox, &Start, StartLat, StartLon);
    if (StopLat == 0)
    {
        if(TempLon == 0) 
            GetCanvasPixLL(BBox, &Stop, MouseLat, MouseLon);
        else
            GetCanvasPixLL(BBox, &Stop, TempLat, TempLon);
    }
    else
        GetCanvasPixLL(BBox, &Stop, StopLat, StopLon);
    wxColour myColour = wxColour(128, 0, 64, 40);
	if (m_pdc){
#if wxUSE_GRAPHICS_CONTEXT
		m_gdc->SetBrush(*wxTheBrushList->FindOrCreateBrush(myColour, wxSOLID));
		m_gdc->SetPen(m_gdc->CreatePen(*wxThePenList->FindOrCreatePen(wxColour(128, 0, 64, 80), 1, wxSOLID)));
		m_gdc->DrawRectangle(Start.x, Start.y, (Stop.x - Start.x), (Stop.y - Start.y));
        myColour = wxColour(128, 0, 64, 20);
        DrawMyLine(Start.x, Start.y, Stop.x, Start.y, myColour);
        DrawMyLine(Start.x, Start.y, Start.x, Stop.y, myColour);
        DrawMyLine(Start.x, Stop.y, Stop.x, Stop.y, myColour);
        DrawMyLine(Stop.x, Start.y, Stop.x, Stop.y, myColour);
#endif
	}

	if (!m_pdc){
		DrawGLBox(Start.x, Start.y, (Stop.x - Start.x), (Stop.y - Start.y), myColour);
        myColour = wxColour(128, 0, 64, 20);
        DrawMyLine(Start.x, Start.y, Stop.x, Start.y, myColour);
        DrawMyLine(Start.x, Start.y, Start.x, Stop.y, myColour);
        DrawMyLine(Start.x, Stop.y, Stop.x, Stop.y, myColour);
        DrawMyLine(Stop.x, Start.y, Stop.x, Stop.y, myColour);
	}
}

void SatOverlayFactory::DrawRealTile(PlugIn_ViewPort* BBox)
{
    if (TileStartLat == 0 || TileStopLat == 0)
        return;
    wxPoint Start, Stop;

    GetCanvasPixLL(BBox, &Start, TileStartLat, TileStartLon);    
    GetCanvasPixLL(BBox, &Stop, TileStopLat, TileStopLon);
    wxColour myColour = wxColour(77,136, 225, 30);
    if (m_pdc) {
#if wxUSE_GRAPHICS_CONTEXT
        m_gdc->SetBrush(*wxTheBrushList->FindOrCreateBrush(myColour, wxSOLID));
        m_gdc->SetPen(m_gdc->CreatePen(*wxThePenList->FindOrCreatePen(wxColour(77, 136, 225, 80), 1, wxSOLID)));
        m_gdc->DrawRectangle(Start.x, Start.y, (Stop.x - Start.x), (Stop.y - Start.y));
        myColour = wxColour(77, 136, 225, 20);
        DrawMyLine(Start.x, Start.y, Stop.x, Start.y, myColour);
        DrawMyLine(Start.x, Start.y, Start.x, Stop.y, myColour);
        DrawMyLine(Start.x, Stop.y, Stop.x, Stop.y, myColour);
        DrawMyLine(Stop.x, Start.y, Stop.x, Stop.y, myColour);
#endif
    }

    if (!m_pdc) {
        DrawGLBox(Start.x, Start.y, (Stop.x - Start.x), (Stop.y - Start.y), myColour);
        myColour = wxColour(77, 136, 225, 20);
        DrawMyLine(Start.x, Start.y, Stop.x, Start.y, myColour);
        DrawMyLine(Start.x, Start.y, Start.x, Stop.y, myColour);
        DrawMyLine(Start.x, Stop.y, Stop.x, Stop.y, myColour);
        DrawMyLine(Stop.x, Start.y, Stop.x, Stop.y, myColour);
    }
}

void SatOverlayFactory::DrawMyLine(int x, int y, int x1, int y1, wxColour c_blue)
{	
	if (m_pdc) {
        wxPen pen(c_blue, 2);
        wxBrush brush(c_blue);
		m_pdc->SetPen(pen);
		m_pdc->SetBrush(brush);	
		m_pdc->DrawLine(x, y, x1, y1);
	}
	else{
		DrawGLLine(x, y, x1, y1, 2, c_blue);
	}
}

void SatOverlayFactory::DrawGLLine(double x1, double y1, double x2, double y2, double width, wxColour myColour)
{
	{
		wxColour isoLineColor = myColour;
		glColor4ub(isoLineColor.Red(), isoLineColor.Green(), isoLineColor.Blue(),
			255/*isoLineColor.Alpha()*/);

		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LINE_BIT | GL_ENABLE_BIT |
			GL_POLYGON_BIT | GL_HINT_BIT); //Save state
		{

			//      Enable anti-aliased lines, at best quality
			glEnable(GL_LINE_SMOOTH);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			glLineWidth(width);

			glBegin(GL_LINES);
			glVertex2d(x1, y1);
			glVertex2d(x2, y2);
			glEnd();
		}

		glPopAttrib();
	}
}

void SatOverlayFactory::DrawGLBox(double x, double y, double w, double h, wxColour myColour)
{
	   glEnable(GL_BLEND);
	   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		wxColour isoLineColor = myColour;
		glColor4ub(isoLineColor.Red(), isoLineColor.Green(), isoLineColor.Blue(),
			isoLineColor.Alpha());

		/* draw bounding rectangle */
		glBegin(GL_QUADS);
		glVertex2i(x, y);
		glVertex2i(x + w, y);
		glVertex2i(x + w, y + h);
		glVertex2i(x, y + h);
		glEnd();	
}
