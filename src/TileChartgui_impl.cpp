/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  autopilot Plugin
 * Author:   Dipl.Ing. Bernd Cirotzki
 *
 ***************************************************************************
 *   Copyright (C) 2017 by Bernd Cirotzki                                  *
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
 ***************************************************************************
 */

#include "TileChartgui_impl.h"
#include <wx/progdlg.h>
#include <wx/wx.h>
#include "wx/dir.h"
#include <list>
#include <cmath>
#include "TileChart_pi.h"


ParameterDialog::ParameterDialog(TileChart* p, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : m_Parameterdialog(parent, id, title, pos, size, style)
{
	ptoPlugin = p;
}

void ParameterDialog::OnChooseSource(wxCommandEvent& event)
{
    if (m_DownloadURL->GetSelection() > 2) // Not Google
        m_Google_V->Enable(false);
    else
        m_Google_V->Enable(true);
    if (m_DownloadURL->GetSelection() == 4)
        m_UserURL->Enable(true);
    else
        m_UserURL->Enable(false);
}

void ParameterDialog::OnEnterText(wxCommandEvent& event)
{
    if(atoi(m_MaxTile->GetValue()) > 2000)
        m_MaxTile->SetForegroundColour(wxColour(255, 0, 0)); // Red
    else
        m_MaxTile->SetForegroundColour(wxColour(0,0,0));
    m_MaxTile->Refresh();
}

void ParameterDialog::OnShowChartselectionArea(wxCommandEvent& event)
{
    if (!m_ShowChartSelection->IsChecked())
        m_DrawRealTile->SetValue(true);
}

void ParameterDialog::OnShowRealTileChartArea(wxCommandEvent& event)
{
    if (!m_DrawRealTile->IsChecked())
        m_ShowChartSelection->SetValue(true);
}

RenameDlg::RenameDlg(TileChart* p, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : RenameDialog(parent, id, title, pos, size, style)
{
    ptoPlugin = p;
}

Dlg::Dlg( wxWindow* parent, long style, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size) : m_dialog( parent, id, title, pos, size, style )
{	
    this->Fit();
    m_parent = parent;
    Connect(wxEVT_CONTEXT_MENU,
        wxContextMenuEventHandler(Dlg::OnContextMenu), NULL,
        this);
    Connect(wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(Dlg::OnContextMenuSelect), NULL,
        this);
}

void Dlg::OnContextMenu(wxContextMenuEvent& evt)
{
    wxMenu* contextMenu = new wxMenu();    
    contextMenu->Append(0, _("Close Window"));
    contextMenu->AppendSeparator();
    contextMenu->Append(1, _("Preferences..."));
    PopupMenu(contextMenu);
    delete contextMenu;
}

void Dlg::OnContextMenuSelect(wxCommandEvent& evt)
{
    switch (evt.GetId()) {
    case 0: {
        Hide();
        return;
    }
    case 1: {
        plugin->ShowPreferencesDialog(m_parent);
        return;  // Does it's own save.
    }
    }
}

void Dlg::OnClickStartButton(wxCommandEvent& event)
{
    // Draw now the area
    m_StatusText->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
    m_StatusText->SetLabel(_("define chart startpoint"));
    m_TileValue->SetLabel("0");
    plugin->GetArea = true;
    plugin->MustSaveArea = true;
    if (plugin->m_pOverlayFactory)
    {
        plugin->m_pOverlayFactory->StartLat = 0;
        plugin->m_pOverlayFactory->StopLat = 0;
        plugin->m_pOverlayFactory->TempLon = 0;
        plugin->m_pOverlayFactory->TempLat = 0;
    }
    m_cancel->Enable(true);
    m_deletegenerated->Enable(false);
    m_generateChart->Enable(false);
    plugin->CurrentCursor = plugin->MyCursorCross;
    SetCursor_PlugIn(plugin->CurrentCursor);
}

void Dlg::OnChooseZoomLevel(wxCommandEvent& event)
{
    plugin->MyZoomLevel = GetZoomLevel();
    plugin->SetTileCount();
    plugin->reduceTile();
}

void Dlg::OnClickGenerateChart(wxCommandEvent& event)
{
    plugin->GenerateCharts();
}

void Dlg::OnDeleteChart(wxCommandEvent& event)
{
    plugin->DeleteChart();
}


void Dlg::OnClickCancel(wxCommandEvent& event)
{
    if (plugin->DownloadCancel())
        return;
    plugin->GetArea = false;
    plugin->MustSaveArea = false;
    m_StatusText->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
    m_StatusText->SetLabel("---");
    m_TileValue->SetLabel("---");
    plugin->RefreshOpenCPNCanvas();
    m_generateChart->Enable(false);
    m_deletegenerated->Enable(false);
    m_cancel->Enable(false);
}

int Dlg::GetZoomLevel()
{
    int re = m_choiceZoom->GetSelection();
    if (re == 0 && plugin->m_vp)
    {
        m_ErrorText->SetForegroundColour(wxColour("wxBLACK"));
        m_ErrorText->SetLabel(wxString::Format(wxT(" Zoom: %i"), GetZoom(plugin->m_vp->chart_scale)));
        return(GetZoom(plugin->m_vp->chart_scale));
    }
    m_ErrorText->SetLabel("");
    if (!plugin->m_vp)
        return 0;
    return (re + 8);
}


int Dlg::GetZoom(double myChartScale)
{
    // If myChartScale is not exactly as shown in OpenCPN get the best scale to use.

    if (myChartScale < 1200)
    {
        myChartScale = 800;
    }

    else if (myChartScale >= 1200 && myChartScale < 2400)
    {
        myChartScale = 1600;
    }

    else if (myChartScale >= 2400 && myChartScale < 4800)
    {
        myChartScale = 3200;
    }

    else if (myChartScale >= 4800 && myChartScale < 9600)
    {
        myChartScale = 6400;
    }

    else if (myChartScale >= 9600 && myChartScale < 19200)
    {
        myChartScale = 12800;
    }

    else if (myChartScale >= 19200 && myChartScale < 38400)
    {
        myChartScale = 25600;
    }

    else if (myChartScale >= 38400 && myChartScale < 76800)
    {
        myChartScale = 51200;
    }

    else if (myChartScale >= 76800 && myChartScale < 153550)
    {
        myChartScale = 102400;
    }

    else if (myChartScale >= 153550 && myChartScale < 306850)
    {
        myChartScale = 204700;
    }

    else if (myChartScale >= 306850 && myChartScale < 613700)
    {
        myChartScale = 409000;
    }

    else if (myChartScale >= 613700 && myChartScale < 1227400)
    {
        myChartScale = 818000;
    }

    else if (myChartScale >= 1227400 && myChartScale < 2454800)
    {
        myChartScale = 1636000;
    }

    else if (myChartScale >= 2454800 && myChartScale < 4909600)
    {
        myChartScale = 3272000;
    }

    else myChartScale = 9999000;


    // Use the modified scale to find the correct zoom level
    // Would switch case be quicker???

    if (myChartScale == 800)
    {
        return 19;
    }

    else if (myChartScale == 1600)
    {
        return 19;
    }

    else if (myChartScale == 3200)
    {
        return 19;
    }

    else if (myChartScale == 6400)
    {
        return 18;
    }

    else if (myChartScale == 12800)
    {
        return 17;
    }

    else if (myChartScale == 25600)
    {
        return 16;
    }

    else if (myChartScale == 51200)
    {
        return 15;
    }

    else if (myChartScale == 102400)
    {
        return 14;
    }

    else if (myChartScale == 204700)
    {
        return 13;
    }
    else if (myChartScale == 409000)
    {
        return 12;
    }

    else if (myChartScale == 818000)
    {
        return 11;
    }

    else if (myChartScale == 1636000)
    {
        return 10;
    }

    else if (myChartScale == 3272000)
    {
        return 9;
    }
    else return 8;
}
