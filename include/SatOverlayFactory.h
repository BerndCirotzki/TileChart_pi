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
*/

#ifndef _CALCULATOR_SAT_H_
#define _CALCULATOR_SAT_H_

#include <map>
#include <wx/string.h>
#include "ocpn_plugin.h"

class Dlg;
class PlugIn_ViewPort;
class TileChart;

class SatOverlayFactory 
{
public:
	SatOverlayFactory(Dlg *gui, TileChart *pPlugin);
	~SatOverlayFactory();
	
	bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);
	bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
    double MouseLat;
    double MouseLon;
    double StartLat, StartLon, StopLat, StopLon;
    double TileStartLat, TileStartLon, TileStopLat, TileStopLon;
    double TempLon, TempLat;
private:
	wxDC *m_pdc;
	wxGraphicsContext *m_gdc;
	Dlg *m_dlg;
    TileChart *p_Plugin;
	void DrawAll(PlugIn_ViewPort *BBox);
    void DrawRealTile(PlugIn_ViewPort *BBox);
	void DrawMyLine(int x, int y, int x1, int y1, wxColour c_blue);
	void DrawGLLine(double x1, double y1, double x2, double y2, double width, wxColour myColour);
	void DrawGLBox(double x1, double y1, double width, double height, wxColour myColour);	
};

#endif
