///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/button.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/filepicker.h>
#include <wx/checkbox.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class m_dialog
///////////////////////////////////////////////////////////////////////////////
class m_dialog : public wxDialog
{
	private:

	protected:
		wxStaticText* m_ZoomText;
		wxStaticText* m_count_tile;

		// Virtual event handlers, override them in your derived class
		virtual void OnClickStartButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChooseZoomLevel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClickGenerateChart( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeleteChart( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClickCancel( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxButton* m_startbutton;
		wxStaticText* m_StatusText;
		wxChoice* m_choiceZoom;
		wxStaticText* m_TileValue;
		wxStaticText* m_ErrorText;
		wxButton* m_generateChart;
		wxButton* m_deletegenerated;
		wxButton* m_cancel;
		wxStaticText* m_ServerName;

		m_dialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("TileChart"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 388,140 ), long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxT("TileChart") );

		~m_dialog();

};

///////////////////////////////////////////////////////////////////////////////
/// Class m_Parameterdialog
///////////////////////////////////////////////////////////////////////////////
class m_Parameterdialog : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText9;
		wxStaticText* m_staticText13;
		wxStaticText* m_staticText10;
		wxStaticLine* m_staticline2;
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText11;
		wxStaticText* m_staticText111;
		wxStaticLine* m_staticline1;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;

		// Virtual event handlers, override them in your derived class
		virtual void OnChooseSource( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEnterText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShowChartselectionArea( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShowRealTileChartArea( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxComboBox* m_DownloadURL;
		wxTextCtrl* m_Google_V;
		wxTextCtrl* m_UserURL;
		wxDirPickerCtrl* m_chartdirectory;
		wxDirPickerCtrl* m_tiledirectory;
		wxTextCtrl* m_MaxTile;
		wxCheckBox* m_LogTile;
		wxCheckBox* m_deleteTile;
		wxCheckBox* m_RenameChartfile;
		wxCheckBox* m_ShowChartSelection;
		wxCheckBox* m_DrawRealTile;

		m_Parameterdialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Preferences TileChart"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 387,444 ), long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxT("TileChart") );

		~m_Parameterdialog();

};

///////////////////////////////////////////////////////////////////////////////
/// Class RenameDialog
///////////////////////////////////////////////////////////////////////////////
class RenameDialog : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText12;
		wxStdDialogButtonSizer* m_sdbSizer3;
		wxButton* m_sdbSizer3OK;
		wxButton* m_sdbSizer3Cancel;

	public:
		wxTextCtrl* m_NewchartName;

		RenameDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("ChartFile"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 256,117 ), long style = wxDEFAULT_DIALOG_STYLE );

		~RenameDialog();

};

