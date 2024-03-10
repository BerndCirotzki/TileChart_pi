/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  TileChart Plugin
 * Author:   Dipl.Ing. Bernd Cirotzki
 *
 ***************************************************************************
 *   Copyright (C) 2024 by Bernd Cirotzki                                  *
 *   eMail : Bernd.Cirotzki@t-online.de                                    *
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
 ***************************************************************************
 */
// See build wxWidgets https://docs.wxwidgets.org/latest/plat_msw_install.html

#define MAKING_PLUGIN

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <wx/event.h>
#include <wx/url.h>
#include "TileChart_pi.h"
#include "TileChartgui_impl.h"
#include "TileChartgui.h"

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new TileChart(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}


long long long2tilex(double lon, long z)
{
    return (long long)(floor((lon + 180.0) / 360.0 * ((long)1 << z)));
}

long long lat2tiley(double lat, long z)
{
    double latrad = lat * M_PI / 180.0;
    return (long long)(floor((1.0 - asinh(tan(latrad)) / M_PI) / 2.0 * ((long)1 << z)));
}

double tilex2long(long long x, long z)
{
    return x / (double)((long)1 << z) * 360.0 - 180;
}

double tiley2lat(long long y, long z)
{
    double n = M_PI - 2.0 * M_PI * y / (double)((long)1 << z);
    return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

//---------------------------------------------------------------------------------------------------------
//
//    TileChart PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

#include "icons.h"

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

TileChart::TileChart(void *ppimgr) :opencpn_plugin_116(ppimgr)
{
      // Create the PlugIn icons
      m_bShowTileChart = false;
      initialize_images();
      m_pOverlayFactory = NULL;
      m_pDialog = NULL;
      m_download_handle = 0;
      m_download_activ = false;
      ServerName = _T("---");
	  wxLogMessage("    Creating TileChart Plugin");
}

TileChart::~TileChart(void)
{
     delete _img_TileChart;
     _img_TileChart = NULL;
     delete _img_TileChart;
     _img_TileChart = NULL;
	 wxLogMessage("    Deleting TileChart Plugin");
}

int TileChart::Init(void)
{
      AddLocaleCatalog( _T("opencpn-TileChart_pi") );

      // Set some default private member parameters
      m_route_dialog_x = 0;
      m_route_dialog_y = 0;
      GetArea = false;
      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn display canvas, to use as a parent for the POI Manager dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();
      MyInstallPath = GetPluginDataDir("TileChart_pi");
	  //    And load the configuration items
      LoadConfig();	  
	  //    This PlugIn needs a toolbar icon, so request its insertion
	  if(m_bTileCharthowIcon)
		m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_TileChart, _img_TileChart, wxITEM_CHECK,
            _("TileChart"), _T(""), NULL,
             CALCULATOR_TOOL_POSITION, 0, this);

      m_pDialog = NULL;
	  m_pDialog = new Dlg(m_parent_window, wxDEFAULT_DIALOG_STYLE);
	  m_pDialog->plugin = this;
	  m_pDialog->Move(wxPoint(m_route_dialog_x, m_route_dialog_y));
      if (!m_pOverlayFactory)
          m_pOverlayFactory = new SatOverlayFactory(m_pDialog,this);
	  if (m_bShowTileChart) {
		  m_pDialog->Show();		  	  
	  }
	  else
	  {
		  m_pDialog->Hide();		  
	  }
      m_pDialog->m_ServerName->SetLabel(ServerName);
      m_vp = NULL;
      MyZoomLevel = m_pDialog->GetZoomLevel();
      GetArea = false;
      MustSaveArea = false;
      DownloadRunning = false;
      // Create the context menu item
      //contextMenu = new wxMenuItem(NULL, wxID_ANY, wxString(_("Select Tile-Chartarea")),
      //    wxString(_("Select the Chartarea for tile download"
      //        "cursor location")),
      //    wxITEM_NORMAL, NULL);
      // contextMenuId = AddCanvasContextMenuItem(contextMenu, this);

      return (WANTS_OVERLAY_CALLBACK | WANTS_OPENGL_OVERLAY_CALLBACK |
              WANTS_ONPAINT_VIEWPORT | WANTS_CURSOR_LATLON |
              WANTS_TOOLBAR_CALLBACK | INSTALLS_TOOLBAR_TOOL | INSTALLS_CONTEXTMENU_ITEMS |
              INSTALLS_PLUGIN_CHART | INSTALLS_PLUGIN_CHART_GL | WANTS_CONFIG |
              WANTS_PREFERENCES | WANTS_MOUSE_EVENTS );


    /*  return (WANTS_PREFERENCES |
		      WANTS_TOOLBAR_CALLBACK |
              WANTS_MOUSE_EVENTS |
              INSTALLS_CONTEXTMENU_ITEMS |
              WANTS_OPENGL_OVERLAY_CALLBACK |
              );  */
}

bool TileChart::DeInit(void)
{
      //    Record the dialog position
      wxLogMessage("    DeInit TileChart Plugin");
      //if (m_pOverlayFactory)
      //    delete m_pOverlayFactory;
      if (NULL != m_pDialog)
      {
            //Capture dialog position
            wxPoint p = m_pDialog->GetPosition();
            SetCalculatorDialogX(p.x);
            SetCalculatorDialogY(p.y);
            m_pDialog->Close();
            delete m_pDialog;
            m_pDialog = NULL;
			SetToolbarItemState( m_leftclick_tool_id, m_bShowTileChart );
      }      
	  
    SaveConfig();
    RequestRefresh(m_parent_window); // refresh mainn window 
    return true;
}

int TileChart::GetAPIVersionMajor()
{
      return OCPN_API_VERSION_MAJOR;
}

int TileChart::GetAPIVersionMinor()
{
      return OCPN_API_VERSION_MINOR;
}

int TileChart::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int TileChart::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

int TileChart::GetPlugInVersionPatch()
{
      return PLUGIN_VERSION_PATCH;
}

int TileChart::GetPlugInVersionPost()
{
      return PLUGIN_VERSION_TWEAK;
}

wxBitmap *TileChart::GetPlugInBitmap()
{
      return _img_TileChart_pi;
}

wxString TileChart::GetCommonName()
{
      return PLUGIN_COMMON_NAME;
}

wxString TileChart::GetShortDescription()
{
      // return _(PLUGIN_SHORT_DESCRIPTION); // Does not make a translation ?!?
    return _("Make kap charts from tile (Google Sat, ...)");
}

wxString TileChart::GetLongDescription()
{
      // return _(PLUGIN_LONG_DESCRIPTION); // Does not make a translation ?!?
      return _("Make kap charts from tile png by selecting the area on the OpenCPN canvas. Use Google-Satallite, OpenStreetview ... or self defined http(s) link. (Internet connection is required)");
}

int TileChart::GetToolbarToolCount(void)
{
      return 1;
}

void TileChart::OnToolbarToolCallback(int id)
{
      if(NULL == m_pDialog)
      {
            m_pDialog = new Dlg(m_parent_window, wxDEFAULT_DIALOG_STYLE);
            m_pDialog->plugin = this;
			m_pDialog->Move(wxPoint(m_route_dialog_x, m_route_dialog_y));
            if (!m_pOverlayFactory)
                m_pOverlayFactory = new SatOverlayFactory(m_pDialog,this);            
      }
      m_pDialog->m_ServerName->SetLabel(ServerName);
	  m_pDialog->Fit();
	  //Toggle 
	  m_bShowTileChart = !m_pDialog->IsShown();
      //    Toggle dialog? 
      if(m_bShowTileChart) {
          m_pDialog->Show();		  
	  }
	  else
	  {
		  m_pDialog->Hide();		  
	  }
      // Toggle is handled by the toolbar but we must keep plugin manager b_toggle updated
      // to actual status to ensure correct status upon toolbar rebuild
      SetToolbarItemState( m_leftclick_tool_id, m_bShowTileChart );

      RequestRefresh(m_parent_window); // refresh main window
}

bool TileChart::LoadConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/Settings/TileChart" ) );
			pConf->Read(_T("ShowTileChart"), &m_bShowTileChart, 1 );
            m_route_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
            m_route_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 20L );            
            pConf->Read(_T("TileDirectory"), &TileDirectory,_T("c:\\tmp"));
            pConf->Read(_T("KapDirectory"), &KapDirectory, _T("c:\\tmp"));
            pConf->Read(_T("UserURL"), &UserURL, _T("https://khms3.google.com/kh/v=969?x=[x]&y=[y]&z=[z]"));
            pConf->Read(_T("DownloadURL"), &DownloadURL, 0);
            LogTile = (bool)pConf->Read(_T("LogTilemerger"), LogTile);
            DeleteTile = (bool)pConf->Read(_T("DeleteTile"), DeleteTile);
            pConf->Read(_T("MaxTile"), &MaxTile, 2000);
            pConf->Read(_T("RenmameChartFile"), &RenmameChartFile, 0);
            pConf->Read(_T("DrawRealTile"), &DrawRealTile, 1);
            pConf->Read(_T("ShowChartSelection"), &ShowChartSelection, 1);
            ParameterDialog* dialog = new ParameterDialog(this, m_parent_window, wxID_ANY, _("TileChart"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
            ServerName = dialog->m_DownloadURL->GetString(DownloadURL);
            delete dialog;
            return true;
      }
      else
            return false;
}

bool TileChart::SaveConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T ( "/Settings/TileChart" ) );
			pConf->Write(_T("ShowTileChart"), m_bShowTileChart);
            pConf->Write(_T("DialogPosX"), m_route_dialog_x );
            pConf->Write(_T("DialogPosY"), m_route_dialog_y);
            pConf->Write(_T("TileDirectory"), TileDirectory);
            pConf->Write(_T("KapDirectory"), KapDirectory);
            pConf->Write(_T("UserURL"), UserURL);
            pConf->Write(_T("DownloadURL"), DownloadURL);
            pConf->Write(_T("LogTilemerger"), LogTile);
            pConf->Write(_T("DeleteTile"), DeleteTile);
            pConf->Write(_T("MaxTile"), MaxTile);
            pConf->Write(_T("RenmameChartFile"), RenmameChartFile);
            pConf->Write(_T("DrawRealTile"), DrawRealTile);
            pConf->Write(_T("ShowChartSelection"), ShowChartSelection);
            return true;
      }
      else
            return false;
}

void TileChart::ShowPreferencesDialog(wxWindow* parent)
{
    int x, y;
	ParameterDialog *dialog = new ParameterDialog(this, parent, wxID_ANY, _("TileChart"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);

	dialog->Fit();
    dialog->GetPosition(&x, &y);
    if (y >= 200)
        dialog->Move(wxPoint(x, y-200));
	DimeWindow(dialog);

    // Set the Values
    wxFileName Name;
    Name.SetPath(TileDirectory);
    dialog->m_tiledirectory->SetDirName(Name);
    Name.SetPath(KapDirectory);
    dialog->m_chartdirectory->SetDirName(Name);
    dialog->m_LogTile->SetValue(LogTile);
    dialog->m_DownloadURL->SetSelection(DownloadURL);
    dialog->m_UserURL->SetValue(UserURL);
    if (DownloadURL == 4) // Userurl is selected
        dialog->m_UserURL->Enable(true);
    else
        dialog->m_UserURL->Enable(false);
    dialog->m_deleteTile->SetValue(DeleteTile);
    dialog->m_RenameChartfile->SetValue(RenmameChartFile);
    dialog->m_DrawRealTile->SetValue(DrawRealTile);
    dialog->m_ShowChartSelection->SetValue(ShowChartSelection);
    dialog->m_MaxTile->SetValue(wxString::Format(wxT("%lu"), MaxTile));
	if (dialog->ShowModal() == wxID_OK)
	{        
		if (NULL != m_pDialog)
		{
			wxPoint p = m_pDialog->GetPosition();
			SetCalculatorDialogX(p.x);
			SetCalculatorDialogY(p.y);
			m_pDialog->Close();
            delete m_pDialog;
			m_pDialog = new Dlg(m_parent_window, wxDEFAULT_DIALOG_STYLE);
			m_pDialog->plugin = this;
			m_pDialog->Move(wxPoint(m_route_dialog_x, m_route_dialog_y));
            if (!m_pOverlayFactory)
                m_pOverlayFactory = new SatOverlayFactory(m_pDialog,this);
			if (m_bShowTileChart)
				m_pDialog->Show();
            TileDirectory = dialog->m_tiledirectory->GetDirName().GetFullPath();
            KapDirectory = dialog->m_chartdirectory->GetDirName().GetFullPath();
            DownloadURL = dialog->m_DownloadURL->GetSelection();
                UserURL = dialog->m_UserURL->GetValue();
            LogTile = dialog->m_LogTile->IsChecked();
            ServerName = dialog->m_DownloadURL->GetString(DownloadURL);
            m_pDialog->m_ServerName->SetLabel(ServerName);
            DeleteTile = dialog->m_deleteTile->GetValue();
            MaxTile = atol(dialog->m_MaxTile->GetValue());
            RenmameChartFile = dialog->m_RenameChartfile->GetValue();
            DrawRealTile = dialog->m_DrawRealTile->GetValue();
            ShowChartSelection = dialog->m_ShowChartSelection->GetValue();
		}
		SaveConfig();
	}
	delete dialog;
}

void TileChart::OnTileChartDialogClose()
{
    SetToolbarItemState( m_leftclick_tool_id, m_bShowTileChart );
    m_pDialog->Hide();
	SaveConfig();
    RequestRefresh(m_parent_window); // refresh main window
}

/*
void TileChart::OnContextMenuItemCallback(int id)
{
    wxString text;
    
    if (id == contextMenuId && GetArea == false)
    {
        // Draw now the area
        m_pDialog->m_StatusText->SetLabel(_("Tilechart Start"));
        GetArea = true;
        MustSaveArea = true;
        m_pOverlayFactory->StartLat = 0;
        m_pOverlayFactory->StopLat = 0;
        m_pOverlayFactory->TempLon = 0;
        m_pOverlayFactory->TempLat = 0;
        m_pDialog->m_TileValue->SetLabel("0");
        RequestRefresh(m_parent_window);
    }
}
*/

void TileChart::SetCursorLatLon(double lat, double lon)
{
    // Called each time mouse position changes. We store the latest value
    if (!m_pOverlayFactory) return;
    m_pOverlayFactory->MouseLat = (float)lat;
    m_pOverlayFactory->MouseLon = (float)lon;
    if (MustSaveArea == true)
    {
        SetTileCount();
        RequestRefresh(m_parent_window);
    }
}

void TileChart::SetTileCount()
{
    if (!m_pOverlayFactory) return;
    if (m_pOverlayFactory->StartLat == 0) return;
    if (m_pOverlayFactory->StopLat != 0)
    {
        TileCount = (long)GetTileCount(m_pOverlayFactory->StartLat, m_pOverlayFactory->StartLon, m_pOverlayFactory->StopLat, m_pOverlayFactory->StopLon, MyZoomLevel);
        m_pDialog->m_TileValue->SetLabel(wxString::Format(wxT("%lu"), TileCount));
    }
    else
    {
        TileCount = (long)GetTileCount(m_pOverlayFactory->StartLat, m_pOverlayFactory->StartLon, m_pOverlayFactory->MouseLat, m_pOverlayFactory->MouseLon, MyZoomLevel);
        if (m_pOverlayFactory->TempLon == 0)  // Not Maxtile
        {
            if (TileCount >= MaxTile)
            {
                m_pOverlayFactory->TempLon = m_pOverlayFactory->MouseLon;
                m_pOverlayFactory->TempLat = m_pOverlayFactory->MouseLat;
                bool Lat = false;
                // Mouse is too fast !!
                while (MaxTile < (long)GetTileCount(m_pOverlayFactory->StartLat, m_pOverlayFactory->StartLon, m_pOverlayFactory->TempLat, m_pOverlayFactory->TempLon, MyZoomLevel))
                {
                    Lat = !Lat;
                    if (Lat)
                    {
                        if (m_pOverlayFactory->TempLat > m_pOverlayFactory->StartLat)
                            m_pOverlayFactory->TempLat -= 0.0001;
                        else
                            m_pOverlayFactory->TempLat += 0.0001;
                    }
                    else
                    {
                        if (m_pOverlayFactory->TempLon > m_pOverlayFactory->StartLon)
                            m_pOverlayFactory->TempLon -= 0.0002;
                        else
                            m_pOverlayFactory->TempLon += 0.0002;
                    }
                }
            }
        }
        else
        {            
            if (TileCount < MaxTile)
            {
                m_pOverlayFactory->TempLon = 0;
                m_pOverlayFactory->TempLat = 0;
            }
            else
                TileCount = (long)GetTileCount(m_pOverlayFactory->StartLat, m_pOverlayFactory->StartLon, m_pOverlayFactory->TempLat, m_pOverlayFactory->TempLon, MyZoomLevel);
        }
        m_pDialog->m_TileValue->SetLabel(wxString::Format(wxT("%lu"), TileCount));
    }
}

void TileChart::reduceTile()
{
    if (m_pOverlayFactory->StopLat == 0) return;
    if (TileCount <= MaxTile)
    {
        if (m_pOverlayFactory->TempLat != 0)
        {
            // Set to old Stoppoint
            m_pOverlayFactory->StopLat = m_pOverlayFactory->TempLat;
            m_pOverlayFactory->StopLon = m_pOverlayFactory->TempLon;
            m_pOverlayFactory->TempLat = 0;
            m_pOverlayFactory->TempLon = 0;
        }        
        if (MaxTile >= (long)GetTileCount(m_pOverlayFactory->StartLat, m_pOverlayFactory->StartLon, m_pOverlayFactory->StopLat, m_pOverlayFactory->StopLon, MyZoomLevel))
        {
            RequestRefresh(m_parent_window);
            return;
        }
    }
    if (m_pOverlayFactory->TempLat == 0)
    {
        m_pOverlayFactory->TempLat = m_pOverlayFactory->StopLat;
        m_pOverlayFactory->TempLon = m_pOverlayFactory->StopLon;
    }
    else
    {
        // Always new calculate
        m_pOverlayFactory->StopLat = m_pOverlayFactory->TempLat;
        m_pOverlayFactory->StopLon = m_pOverlayFactory->TempLon;
    }

    bool Lat = false;
    while (MaxTile < (long)GetTileCount(m_pOverlayFactory->StartLat, m_pOverlayFactory->StartLon, m_pOverlayFactory->StopLat, m_pOverlayFactory->StopLon, MyZoomLevel))
    {        
        Lat = !Lat;
        if (Lat)
        {
            if (m_pOverlayFactory->StopLat > m_pOverlayFactory->StartLat)
                m_pOverlayFactory->StopLat -= 0.0001;
            else
                m_pOverlayFactory->StopLat += 0.0001;
        }
        else
        {
            if (m_pOverlayFactory->StopLon > m_pOverlayFactory->StartLon)
                m_pOverlayFactory->StopLon -= 0.0002;
            else
                m_pOverlayFactory->StopLon += 0.0002;
        }
    }
    m_pDialog->m_TileValue->SetLabel(wxString::Format(wxT("%lu"), (long)GetTileCount(m_pOverlayFactory->StartLat, m_pOverlayFactory->StartLon, m_pOverlayFactory->StopLat, m_pOverlayFactory->StopLon, MyZoomLevel)));
    RequestRefresh(m_parent_window);
}

bool TileChart::MouseEventHook(wxMouseEvent& event)
{
    if (GetArea == false && MustSaveArea == false) return false;
    if (event.GetEventType() == wxEVT_LEFT_DOWN) 
    {
        if (m_pOverlayFactory->StartLat == 0)
        {            
            m_pOverlayFactory->StartLat = m_pOverlayFactory->MouseLat;
            m_pOverlayFactory->StartLon = m_pOverlayFactory->MouseLon;
            m_pDialog->m_StatusText->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
            m_pDialog->m_StatusText->SetLabel(_("define chartendpoint"));
        }
        else
        {
            if (m_pOverlayFactory->StopLat != 0) // Dont show reckangel, wenn klick somwhereelse
            {
                // MustSaveArea = false;
                // m_pDialog->m_StatusText->SetLabel(_("---"));
                return false;
            }
            if (m_pOverlayFactory->TempLon == 0)
            {
                m_pOverlayFactory->StopLat = m_pOverlayFactory->MouseLat;
                m_pOverlayFactory->StopLon = m_pOverlayFactory->MouseLon;
            }
            else
            {
                m_pOverlayFactory->StopLat = m_pOverlayFactory->TempLat;
                m_pOverlayFactory->StopLon = m_pOverlayFactory->TempLon;
            }
            m_pDialog->m_StatusText->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
            m_pDialog->m_StatusText->SetLabel(_("chart area is defined"));
            m_pDialog->m_generateChart->Enable(true);
            RequestRefresh(m_parent_window);
            GetArea = false;
        }
        return true;
    }
    return false;
}

void TileChart::SetViewPort(PlugIn_ViewPort* vp)
{
    if (m_vp == vp)  return;
    m_vp = new PlugIn_ViewPort(*vp);
    if (m_pDialog) MyZoomLevel = m_pDialog->GetZoomLevel();    
}

bool TileChart::RenderOverlay(wxDC& dc, PlugIn_ViewPort* vp)
{
    SetViewPort(vp);
    if (!m_pDialog || !m_pDialog->IsShown() || !m_pOverlayFactory || !MustSaveArea) return false;
    MakeTilePosition();
    m_pOverlayFactory->RenderOverlay(dc, vp);

    return true;
}

bool TileChart::RenderGLOverlay(wxGLContext* pcontext, PlugIn_ViewPort* vp)
{
    SetViewPort(vp);
    if (!m_pDialog || !m_pDialog->IsShown() || !m_pOverlayFactory || !MustSaveArea) return false;
    MakeTilePosition();
    m_pOverlayFactory->RenderGLOverlay(pcontext, vp);

    return true;
}

Tile TileChart::GetTilefromPosition(double Lat, double Lon, long ZoomLevel)
{
    static Tile Re;
    Re.xtile = long2tilex(Lon, ZoomLevel);
    Re.ytile = lat2tiley(Lat, ZoomLevel);
    return Re;
}

long TileChart::GetTileCount(double Lat1, double Lon1, double Lat2, double Lon2, long ZoomLevel)
{
    double xcount, ycount;
    Tile corner1 = GetTilefromPosition(Lat1, Lon1, ZoomLevel);
    Tile corner2 = GetTilefromPosition(Lat2, Lon2, ZoomLevel);
    if (corner1.xtile >= corner2.xtile)
        xcount = corner1.xtile - corner2.xtile;
    else
        xcount = corner2.xtile - corner1.xtile;
    if (corner1.ytile >= corner2.ytile)
        ycount = corner1.ytile - corner2.ytile;
    else
        ycount = corner2.ytile - corner1.ytile;
    return (long)((xcount+1) * (ycount+1));
}

void TileChart::MakeTilePosition()
{
    if (m_pOverlayFactory->StartLat == 0 || !DrawRealTile)
    {
        m_pOverlayFactory->TileStartLat = 0;
        m_pOverlayFactory->TileStopLat = 0;
        m_pOverlayFactory->TileStartLon = 0;
        m_pOverlayFactory->TileStopLon = 0;
        return;
    }
    Tile corner1 = GetTilefromPosition(m_pOverlayFactory->StartLat, m_pOverlayFactory->StartLon, MyZoomLevel);
    Tile corner2;
    if (m_pOverlayFactory->StopLat == 0)
    {
        if (m_pOverlayFactory->TempLon != 0)
            corner2 = GetTilefromPosition(m_pOverlayFactory->TempLat, m_pOverlayFactory->TempLon, MyZoomLevel);
        else
            corner2 = GetTilefromPosition(m_pOverlayFactory->MouseLat, m_pOverlayFactory->MouseLon, MyZoomLevel);
    }
    else
        corner2 = GetTilefromPosition(m_pOverlayFactory->StopLat, m_pOverlayFactory->StopLon, MyZoomLevel);

    long TXbegin = corner1.xtile;
    long TYbegin = corner1.ytile;
    long TXEnd = corner2.xtile;
    long TYEnd = corner2.ytile;
    if (corner2.xtile < TXbegin)
    {
        TXbegin = corner2.xtile;
        TXEnd = corner1.xtile;
    }
    if (corner2.ytile < TYbegin)
    {
        TYbegin = corner2.ytile;
        TYEnd = corner1.ytile;
    }
    m_pOverlayFactory->TileStartLon = tilex2long(TXbegin, MyZoomLevel);
    m_pOverlayFactory->TileStartLat = tiley2lat(TYbegin, MyZoomLevel);
    m_pOverlayFactory->TileStopLon = tilex2long((TXEnd + 1), MyZoomLevel);  // Unteres Ende der Ecke !
    m_pOverlayFactory->TileStopLat = tiley2lat((TYEnd + 1), MyZoomLevel);
}

void TileChart::GenerateCharts()
{
    m_pDialog->m_deletegenerated->Enable(false);
    if (!wxDirExists(TileDirectory))
    {
        m_pDialog->m_StatusText->SetForegroundColour(wxColour(255, 0, 0)); // Red
        m_pDialog->m_StatusText->SetLabel(_("Tile directory not exist"));
        return;
    }
    if (!wxDirExists(KapDirectory))
    {
        m_pDialog->m_StatusText->SetForegroundColour(wxColour(255, 0, 0)); // Red
        m_pDialog->m_StatusText->SetLabel(_("Chart directory not exist"));
        return;
    }
    if (DownloadTile())
    {
        if (MergeTile())
        {
            if (DeleteTileFiles())
            {
                GenerateKapFile();
            }
        }
    }
}

wxString TileChart::OnPrepare(long v, long x, long y, long z)
{
    wxString GoogleServer;
    switch (DownloadURL)
    {
    case 0: GoogleServer = _T("https://khms1.");
    case 1: GoogleServer = _T("https://khms2.");
    case 2: GoogleServer = _T("https://khms3.");
            return (OnPrepareGoogle(v, x, y, z, GoogleServer));            
    case 3: return (OnPrepareOpenStreetmap(x, y, z));
    case 4: return(OnPrepareUserdef(v, x, y, z));
    }
    return(OnPrepareUserdef(v, x, y, z));
}

wxString TileChart::OnPrepareUserdef(long v, long x, long y, long z)
{
    int ix = UserURL.Find(_T("[x]"));
    int iy = UserURL.Find(_T("[y]"));
    int iz = UserURL.Find(_T("[z]"));
    if (ix == wxNOT_FOUND || iy == wxNOT_FOUND || iz == wxNOT_FOUND)
    {
        m_pDialog->m_StatusText->SetForegroundColour(wxColour(255, 0, 0)); // Red
        m_pDialog->m_StatusText->SetLabel(_("Error in User URL"));
        return (wxEmptyString);
    }
    wxString TmpSt = UserURL.AfterFirst('[');
    char Zeichen = TmpSt[0];
    wxString ReturnURL = UserURL.BeforeFirst('[');
    switch (Zeichen) {
    case 'v': ReturnURL = ReturnURL + wxString::Format(_T("%i"), v); break;
    case 'x': ReturnURL = ReturnURL + wxString::Format(_T("%i"), x); break;
    case 'y': ReturnURL = ReturnURL + wxString::Format(_T("%i"), y); break;
    case 'z': ReturnURL = ReturnURL + wxString::Format(_T("%i"), z); break;
    default : return (wxEmptyString);
    }
    TmpSt = TmpSt.AfterFirst(']');
    ReturnURL = ReturnURL + TmpSt.BeforeFirst('[');
    TmpSt = TmpSt.AfterFirst('[');
    Zeichen = TmpSt[0];
    switch (Zeichen) {
    case 'v': ReturnURL = ReturnURL + wxString::Format(_T("%i"), v); break;
    case 'x': ReturnURL = ReturnURL + wxString::Format(_T("%i"), x); break;
    case 'y': ReturnURL = ReturnURL + wxString::Format(_T("%i"), y); break;
    case 'z': ReturnURL = ReturnURL + wxString::Format(_T("%i"), z); break;
    default: return (wxEmptyString);
    }
    TmpSt = TmpSt.AfterFirst(']');
    ReturnURL = ReturnURL + TmpSt.BeforeFirst('[');
    TmpSt = TmpSt.AfterFirst('[');
    Zeichen = TmpSt[0];
    switch (Zeichen) {
    case 'v': ReturnURL = ReturnURL + wxString::Format(_T("%i"), v); break;
    case 'x': ReturnURL = ReturnURL + wxString::Format(_T("%i"), x); break;
    case 'y': ReturnURL = ReturnURL + wxString::Format(_T("%i"), y); break;
    case 'z': ReturnURL = ReturnURL + wxString::Format(_T("%i"), z); break;
    default: return (wxEmptyString);
    }
    TmpSt = TmpSt.AfterFirst(']');
    ReturnURL = ReturnURL + TmpSt.BeforeFirst('[');
    TmpSt = TmpSt.AfterFirst('[');
    if (TmpSt == wxEmptyString) // Url is ready there is no v Value ... only by google.
    {
        return ReturnURL;
    }
    Zeichen = TmpSt[0];
    switch (Zeichen) {
    case 'v': ReturnURL = ReturnURL + wxString::Format(_T("%i"), v); break;
    case 'x': ReturnURL = ReturnURL + wxString::Format(_T("%i"), x); break;
    case 'y': ReturnURL = ReturnURL + wxString::Format(_T("%i"), y); break;
    case 'z': ReturnURL = ReturnURL + wxString::Format(_T("%i"), z); break;
    default: return (wxEmptyString);
    }
    ReturnURL = ReturnURL + TmpSt.AfterFirst(']');
    return ReturnURL;
}

wxString TileChart::OnPrepareOpenStreetmap(long x, long y, long z)
{
    wxString VF = _T("https://tile.openstreetmap.de/");
    wxString m_z = wxString::Format(_T("%i/"), (int)z);
    wxString m_x = wxString::Format(_T("%lu/"), x);
    wxString m_y = wxString::Format(_T("%lu"), y);
    wxString ret = VF + m_z + m_x + m_y + _T(".png");
    return ret;
}

wxString TileChart::OnPrepareGoogle(long v, long x, long y, long z, wxString Server)
{
    // Form Google Server :  https://khms3.google.com/kh/v=969?x=2162&y=1321&z=12

    wxString VF = Server + _T("google.com/kh/");
    wxString m_v = wxString::Format(_T("v=%i?"), (int)v);
    wxString m_x = wxString::Format(_T("x=%lu&"), x);
    wxString m_y = wxString::Format(_T("y=%lu&"), y);
    wxString m_z = wxString::Format(_T("z=%i"), (int)z);
    wxString ret = VF + m_v + m_x + m_y + m_z;
    return ret;
}

bool TileChart::DownloadTile()
{
    m_pDialog->m_StatusText->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
    m_pDialog->m_StatusText->SetLabel(_("Downloading tiles..."));
    if (m_download_activ)
    {
        OCPN_cancelDownloadFileBackground(m_download_handle);
        m_download_handle = 0;
        m_download_activ = false;
        Disconnect(wxEVT_DOWNLOAD_EVENT,
            (wxObjectEventFunction)(wxEventFunction)&TileChart::OnDlEvent);
    }
    DownloadCanceled = false;
    Tile corner1 = GetTilefromPosition(m_pOverlayFactory->StartLat, m_pOverlayFactory->StartLon, MyZoomLevel);
    Tile corner2 = GetTilefromPosition(m_pOverlayFactory->StopLat, m_pOverlayFactory->StopLon, MyZoomLevel);

    Xbegin = corner1.xtile;
    Ybegin = corner1.ytile;
    XEnd = corner2.xtile;
    YEnd = corner2.ytile;
    if (corner2.xtile < Xbegin)
    {
        Xbegin = corner2.xtile;
        XEnd = corner1.xtile;
    }
    if (corner2.ytile < Ybegin)
    {
        Ybegin = corner2.ytile;
        YEnd = corner1.ytile;
    }
    DownloadError = false;
    DownloadRunning = true;
    TileFileString = _T("");
    long Downloaded = 0;
    long ToDownload = GetTileCount(m_pOverlayFactory->StartLat, m_pOverlayFactory->StartLon, m_pOverlayFactory->StopLat, m_pOverlayFactory->StopLon, MyZoomLevel);
    XTiles = XEnd - Xbegin + 1;
    for (int YR = Ybegin; YR <= YEnd; YR++)
    {
        for (int XR = Xbegin; XR <= XEnd; XR++)
        {
            wxTheApp->ProcessPendingEvents();
            while (!StartDownLoadFile(XR, YR))  // Wait if free asyncron slot
            {
                wxTheApp->ProcessPendingEvents();
                wxMilliSleep(10);
            }
            Downloaded++;
            m_pDialog->m_TileValue->SetLabel(wxString::Format(wxT("%lu/%lu"),Downloaded,ToDownload));
            RequestRefresh(m_parent_window);
            if (DownloadError  || DownloadCanceled) break;
        }
        if (DownloadError || DownloadCanceled) break;
    }
    while (m_download_activ)
    {
        // Wait for the last download to be complete
        wxTheApp->ProcessPendingEvents();
        wxMilliSleep(10);
    }
    if (DownloadCanceled)
    {
        DeleteTileFiles();
        m_pDialog->m_StatusText->SetForegroundColour(wxColour(255, 0, 0)); // Red
        m_pDialog->m_StatusText->SetLabel(_("Download Canceled"));
        DownloadRunning = false;
        return false;
    }
    if (DownloadError)
    {
        DeleteTileFiles();
        m_pDialog->m_StatusText->SetForegroundColour(wxColour(255, 0, 0)); // Red
        m_pDialog->m_StatusText->SetLabel(_("Download Error"));
        DownloadRunning = false;
        return false;
    }
    else
    {
        m_pDialog->m_StatusText->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));        
        m_pDialog->m_StatusText->SetLabel(_("Download ready"));
        DownloadRunning = false;
        return true;
    }
}

void TileChart::OnDlEvent(OCPN_downloadEvent& ev)
{
    _OCPN_DLStatus ret = ev.getDLEventStatus();
    switch (ev.getDLEventCondition()) {
    case OCPN_DL_EVENT_TYPE_END:
        if (ret == OCPN_DL_ABORTED) {
            m_pDialog->m_ErrorText->SetForegroundColour(wxColour(255, 0, 0)); // Red
            m_pDialog->m_ErrorText->SetLabel(_(wxString::Format(_("Abort"))));
            m_download_handle = 0;
            m_download_activ = false;
            DownloadError = true;
        }
        if (ret == OCPN_DL_FAILED) {
            m_pDialog->m_ErrorText->SetForegroundColour(wxColour(255, 0, 0)); // Red
            m_pDialog->m_ErrorText->SetLabel(_(wxString::Format(_("Failed"))));
            m_download_handle = 0;
            m_download_activ = false;
            DownloadError = true;
        }            
        Disconnect(wxEVT_DOWNLOAD_EVENT,
            (wxObjectEventFunction)(wxEventFunction)&TileChart::OnDlEvent);
        m_download_handle = 0;
        m_download_activ = false;
        wxYieldIfNeeded();
        break;    
    default :        
        break;
    }
}

bool TileChart::StartDownLoadFile(int Xtile, int Ytile)
{
    if (m_download_activ) return false;  // No free slot
    //m_pDialog->m_ErrorText->SetLabel(wxString::Format(_T("I:%i  "), i));
    // Make URL
    wxString urlString = OnPrepare(969, Xtile, Ytile, MyZoomLevel);
    if (urlString == wxEmptyString)
        return false;
    // Make DestFile+path
    if (TileFileString != _T(""))
        TileFileString = TileFileString + _T(",");
    TileFileString = TileFileString + wxString::Format(_T("%i-%i.png"), Xtile, Ytile);
    wxString files = wxString::Format(_T("%i-%i.png"), Xtile, Ytile);
    wxFileName fileName;
    fileName.SetFullName(files);
    fileName.SetPath(TileDirectory);
    wxString file_path = fileName.GetFullPath();
    // Start the dowdload now.
    m_download_activ = true;
    Connect(wxEVT_DOWNLOAD_EVENT,
        (wxObjectEventFunction)(wxEventFunction)&TileChart::OnDlEvent);
    auto res = OCPN_downloadFileBackground(
        urlString, file_path, this,
        &m_download_handle);
    return m_download_activ;
}

bool TileChart::DownloadCancel()
{
    DownloadCanceled = true;
    if (DownloadRunning)
    {
        OCPN_cancelDownloadFileBackground(m_download_handle);
        m_download_handle = 0;
        m_download_activ = false;
        Disconnect(wxEVT_DOWNLOAD_EVENT,
            (wxObjectEventFunction)(wxEventFunction)&TileChart::OnDlEvent);
        m_pDialog->m_StatusText->SetForegroundColour(wxColour(255, 0, 0)); // Red
        m_pDialog->m_StatusText->SetLabel(_("Download Canceled"));        
        return true;
    }
    return false;
}

bool TileChart::MergeTile()
{
    wxArrayString Output;
    wxArrayString Error;
    m_pDialog->m_StatusText->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
    m_pDialog->m_StatusText->SetLabel(_("Merge Tile"));
    wxString SRC = TileDirectory.BeforeLast('\\');
    wxString MergeString = _T("\"") + MyInstallPath + _T("\\data\\tile-merger\\");
    MergeString = MergeString + _T("TileMerger.exe\" --src=\"") + SRC + _T("\" --imgs=\"");
    MergeString = MergeString + TileFileString + _T("\" --dest=\"") + TileDirectory;
    MergeString = MergeString + _T("Tilles.png\" --td=lr --cols=");
    MergeString = MergeString + wxString::Format(_T("%i"), XTiles);
    if (LogTile) wxLogMessage(MergeString);
    wxExecute(wxString::Format(MergeString), Output, Error, wxEXEC_SYNC | wxEXEC_HIDE_CONSOLE);
    if (AnalyseTileOutput(Output, Error))
    {
        m_pDialog->m_StatusText->SetLabel(_("Tile Merge ready"));
        return true;
    }
    else
    {
        DeleteTileFiles();
        m_pDialog->m_StatusText->SetForegroundColour(wxColour(255, 0, 0)); // Red
        m_pDialog->m_StatusText->SetLabel(_("Error Tile Merge"));
        return false;
    }    
}

bool TileChart::AnalyseTileOutput(wxArrayString Output, wxArrayString Error)
{
    if (LogTile)
    {
        wxLogMessage(wxString::Format(_("TileMerger Output:")));
        for (size_t i = 0; i < Output.GetCount(); i++)
        {
            wxLogMessage(Output.Item(i));
        }
    }
    if (Error.GetCount())
    {
        wxLogMessage(wxString::Format(_("TileMerger Error Output:")));
        for (size_t i = 0; i < Error.GetCount(); i++)
        {
            wxLogMessage(Error.Item(i));
        }
        return false;
    }
    return true;
}

bool TileChart::DeleteTileFiles()
{
    if (!DeleteTile) return true; // always true
    m_pDialog->m_StatusText->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
    m_pDialog->m_StatusText->SetLabel(_("deleting Tile Files"));
    for (int YR = Ybegin; YR <= YEnd; YR++)
    {
        for (int XR = Xbegin; XR <= XEnd; XR++)
        {
            wxString Re = TileDirectory + wxString::Format(_T("%i-%i.png"), XR, YR);
            wxRemoveFile(Re);
        }
    }
    m_pDialog->m_StatusText->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
    m_pDialog->m_StatusText->SetLabel(_("Tile Files deleted"));
    return true;
}

bool TileChart::GenerateKapFile()
{
    m_pDialog->m_StatusText->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
    m_pDialog->m_StatusText->SetLabel(_("Create Kap File"));

    wxArrayString Output;
    wxArrayString Error;
    KapFile = wxString::Format(_T("Ch-%lu-%lu--%lu-%lu.kap"), Ybegin, Xbegin, YEnd, XEnd);

    double StartLon = tilex2long(Xbegin, MyZoomLevel);
    double StartLat = tiley2lat(Ybegin, MyZoomLevel);
    double EndLon = tilex2long((XEnd + 1), MyZoomLevel);  // Unteres Ende der Ecke !
    double EndLat = tiley2lat((YEnd + 1), MyZoomLevel);

    wxString KapString = _T("\"") + MyInstallPath + _T("\\data\\imgkap\\");
    KapString = KapString +_T("imgkap.exe\" -p KAP ");
    KapString = KapString + TileDirectory + _T("Tilles.png");
    KapString = KapString + wxString::Format(_T(" %f %f %f %f "), StartLat, StartLon, EndLat, EndLon);
    KapString = KapString + KapDirectory + KapFile;
    wxLogMessage(KapString);
    wxExecute(wxString::Format(KapString), Output, Error, wxEXEC_SYNC | wxEXEC_HIDE_CONSOLE);
    if (AnalyseKapOutput(Output, Error))
    {
        m_pDialog->m_StatusText->SetLabel(_("Kap File ready"));
        if (DeleteTile)
             wxRemoveFile((TileDirectory + _T("Tilles.png")));
        if (RenmameChartFile)
        {
            while (!RenameKapChartFile(KapFile)) {}
        }
        AddChartToDBInPlace((KapDirectory + KapFile), true);
        m_pDialog->m_deletegenerated->Enable(true);
        return true;
    }
    else
    {
        DeleteTileFiles();
        m_pDialog->m_StatusText->SetForegroundColour(wxColour(255, 0, 0)); // Red
        m_pDialog->m_StatusText->SetLabel(_("Error creating Kap File"));
        return false;
    }   
}

bool TileChart::AnalyseKapOutput(wxArrayString Output, wxArrayString Error)
{
    if (Output.GetCount())
    {
        wxLogMessage(wxString::Format(_T("ImgKap Output:")));
        for (size_t i = 0; i < Output.GetCount(); i++)
        {
            wxLogMessage(Output.Item(i));
        }
        return false;
    }
    if (Error.GetCount())
    {
        wxLogMessage(wxString::Format(_T("ImgKap Error Output:")));
        for (size_t i = 0; i < Error.GetCount(); i++)
        {
            wxLogMessage(Error.Item(i));
        }
        return false;
    }
    return true;
}

void TileChart::DeleteChart()
{
    RemoveChartFromDBInPlace(KapDirectory + KapFile);
    wxRemoveFile(KapDirectory + KapFile);
    m_pDialog->m_StatusText->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
    m_pDialog->m_StatusText->SetLabel(_("Kap File deleted"));
    m_pDialog->m_deletegenerated->Enable(false);
}

bool TileChart::RenameKapChartFile(wxString &KapFile)
{
    wxString NewName;
    wxString OldName = KapFile;
    RenameDlg *pDialog = new RenameDlg(this, m_pDialog, wxID_ANY, _("Chartfile rename"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    pDialog->Fit();
    KapFile = KapFile.BeforeFirst('.'); 
    pDialog->m_NewchartName->SetValue(KapFile);
    if (pDialog->ShowModal() == wxID_OK)
    {
        NewName = pDialog->m_NewchartName->GetValue();
        if (NewName == wxEmptyString)
        {
            wxMessageBox(_("Enter Filename"));
            delete pDialog;
            return false;
        }
        NewName = NewName.BeforeFirst('.'); // Entered with .kap ?
        NewName = NewName + _T(".kap");
        wxRename(KapDirectory + OldName, KapDirectory + NewName);
        KapFile = NewName;
    }
    delete pDialog;
    return true;
}
