/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  TileChartPlugin
 * Author:   Dipl-Ing. Bernd Cirotzki
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
 ***************************************************************************
 */

#ifndef _CALCULATOR_PI_H_
#define _CALCULATOR_PI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif //precompiled headers

#include <wx/fileconf.h>
#include <wx/event.h>

#include "ocpn_plugin.h" //Required for OCPN plugin functions
#include "TileChartgui_impl.h"
#include "SatOverlayFactory.h"
#include "version.h"

class Dlg;

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define CALCULATOR_TOOL_POSITION    -1          // Request default positioning of toolbar tool

class Tile
{
public:
    long long xtile;
    long long ytile;
};

class TileChart : public wxEvtHandler, public opencpn_plugin_116
{	
public:
      TileChart(void *ppimgr);
	   ~TileChart(void);

	  //    The required PlugIn Methods
      int Init(void);
      bool DeInit(void);

      int GetAPIVersionMajor();
      int GetAPIVersionMinor();
      int GetPlugInVersionMajor();
      int GetPlugInVersionMinor();
      int GetPlugInVersionPatch();
      int GetPlugInVersionPost();
      wxBitmap *GetPlugInBitmap();
      wxString GetCommonName();
      wxString GetShortDescription();
      wxString GetLongDescription();
      
//    The required override PlugIn Methods
      int GetToolbarToolCount(void);
	  void ShowPreferencesDialog(wxWindow* parent);
      void OnToolbarToolCallback(int id);
      bool RenderOverlay(wxDC& dc, PlugIn_ViewPort* vp);
      bool RenderGLOverlay(wxGLContext* pcontext, PlugIn_ViewPort* vp);
      Tile GetTilefromPosition(double Lat, double Lon, long ZoomLevel);
      long GetTileCount(double Lat1, double Lon1, double Lat2, double Lon2, long ZoomLevel);
      void GenerateCharts();
      void DeleteChart();
      bool DownloadTile();
      bool DownloadCancel();

//    Optional plugin overrides
      //void SetColorScheme(PI_ColorScheme cs);
      bool             m_bShowTileChart;

//    The override PlugIn Methods

//    Other public methods
      void SetCalculatorDialogX         (int x){ m_route_dialog_x = x;};
      void SetCalculatorDialogY         (int x){ m_route_dialog_y = x;};
      void SetCalculatorDialogWidth     (int x){ m_route_dialog_width = x;};
      void SetCalculatorDialogHeight    (int x){ m_route_dialog_height = x;};
      void OnTileChartDialogClose();
      //void OnContextMenuItemCallback(int id);
      void SetCursorLatLon(double lat, double lon);
      bool MouseEventHook(wxMouseEvent& event);
      void RefreshOpenCPNCanvas(void) { RequestRefresh(m_parent_window); } // refresh main window
      void SetTileCount();
      void reduceTile();
	  Dlg			    *m_pDialog;
      SatOverlayFactory *m_pOverlayFactory;
      PlugIn_ViewPort   *m_vp;
      // Dialog Style
      int              m_route_dialog_x, m_route_dialog_y;
      bool             m_bTileCharthowIcon;
      bool             GetArea;
      bool             MustSaveArea;
      int              MyZoomLevel;
      int              DownloadURL;
      bool             ShowChartSelection;
private:     
	  
	  wxFileConfig      *m_pconfig;
      wxWindow          *m_parent_window;
      bool              LoadConfig(void);
      bool              SaveConfig(void);      
      int               m_route_dialog_width,m_route_dialog_height;
      int               m_display_width, m_display_height;
      int               m_leftclick_tool_id;      
      //int               contextMenuId;        // Id of the plug-in item in context menu
      //wxMenuItem*       contextMenu;          // Pointer to plug-in context menu item object
      wxString          OnPrepare(long v, long x, long y, long z);
      wxString          OnPrepareGoogle(long v, long x, long y, long z, wxString Server);
      wxString          OnPrepareOpenStreetmap(long x, long y, long z);
      wxString          OnPrepareUserdef(long v,long x, long y, long z);
      wxString          TileDirectory;
      wxString          KapDirectory;
      wxString          UserURL;
      wxString          ServerName;
      wxString          KapFile;
      long              m_download_handle;
      bool              m_download_activ;
      bool              DownloadError;
      wxString          TileFileString;
      int               XTiles;
      long              Xbegin;
      long              Ybegin;
      int               XEnd;
      int               YEnd;
      bool              LogTile;
      bool              DeleteTile;
      long              MaxTile;
      long              TileCount;
      bool              RenmameChartFile;
      bool              DrawRealTile;      
      bool              DownloadCanceled;
      bool              DownloadRunning;
      bool              MoveArea;
      double            LatStartDifftoMouse;
      double            LonStartDifftoMouse;
      double            LatStopDifftoMouse;
      double            LonStopDifftoMouse;
      void              SetViewPort(PlugIn_ViewPort* vp);
      bool              RenameKapChartFile(wxString& KapFile);
      bool              StartDownLoadFile(int Xtile, int Ytile);
      bool              AnalyseKapOutput(wxArrayString Output, wxArrayString Error);
      bool              AnalyseTileOutput(wxArrayString Output, wxArrayString Error);
      bool              MergeTile();
      bool              DeleteTileFiles();
      bool              GenerateKapFile();
      void              MakeTilePosition();
      bool              IsMouseInArea();
      wxString          MyInstallPath;
      // Events
      void OnDlEvent(OCPN_downloadEvent& ev);
};

#endif
