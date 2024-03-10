/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  TileChart Plugin
 * Author:   Dipl. Ing. Bernd Cirotzki
 *
 ***************************************************************************
 *   Copyright (C) 2024 by Bernd Cirotzki                                  *
 *                                                   *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#ifndef _CALCULATORGUI_IMPL_H_
#define _CALCULATORGUI_IMPL_H_

#ifdef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "TileChartgui.h"
#include "TileChart_pi.h"


class TileChart;

class ParameterDialog : public m_Parameterdialog
{
public:
	ParameterDialog(TileChart* p, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style);
    void OnChooseSource(wxCommandEvent& event);
    void OnEnterText(wxCommandEvent& event);
    void OnShowChartselectionArea(wxCommandEvent& event);
    void OnShowRealTileChartArea(wxCommandEvent& event);
private:
	TileChart* ptoPlugin;
};
	
class Dlg : public m_dialog
{
public:
        Dlg( wxWindow* parent, long style = wxDEFAULT_DIALOG_STYLE, wxWindowID id = wxID_ANY, const wxString& title = _("TileChart"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ));			
		TileChart *plugin;

        void OnClickStartButton(wxCommandEvent& event);
        void OnChooseZoomLevel(wxCommandEvent& event);
        void OnChooseSource(wxCommandEvent& event);
        void OnClickGenerateChart(wxCommandEvent& event);
        void OnClickCancel(wxCommandEvent& event);
        void OnDeleteChart(wxCommandEvent& event);
        int GetZoomLevel();
        int  GetZoom(double myChartScale);
        void OnContextMenu(wxContextMenuEvent& evt);
        void OnContextMenuSelect(wxCommandEvent& evt);
private:
	    void OnClose( wxCloseEvent& event );        
};

class RenameDlg : public RenameDialog
{
public:
    RenameDlg(TileChart* p, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style);
private:
    TileChart* ptoPlugin;
};

#endif
