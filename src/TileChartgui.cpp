///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.0.0-0-g0efcecf)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "TileChartgui.h"

///////////////////////////////////////////////////////////////////////////

m_dialog::m_dialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxDialog( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_startbutton = new wxButton( this, wxID_ANY, _("Start"), wxDefaultPosition, wxSize( 140,-1 ), 0 );
	fgSizer3->Add( m_startbutton, 0, wxALL|wxEXPAND, 5 );

	m_StatusText = new wxStaticText( this, wxID_ANY, _("---"), wxDefaultPosition, wxDefaultSize, 0 );
	m_StatusText->Wrap( -1 );
	m_StatusText->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial") ) );
	m_StatusText->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

	fgSizer3->Add( m_StatusText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer1->Add( fgSizer3, 0, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 6, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_ZoomText = new wxStaticText( this, wxID_ANY, _("Zoomlevel :"), wxDefaultPosition, wxSize( 70,-1 ), 0 );
	m_ZoomText->Wrap( -1 );
	fgSizer1->Add( m_ZoomText, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceZoomChoices[] = { _("Chart"), _("9"), _("10"), _("11"), _("12"), _("13"), _("14"), _("15"), _("16"), _("17"), _("18"), _("19"), _("(20)"), _("(21)") };
	int m_choiceZoomNChoices = sizeof( m_choiceZoomChoices ) / sizeof( wxString );
	m_choiceZoom = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 60,-1 ), m_choiceZoomNChoices, m_choiceZoomChoices, 0 );
	m_choiceZoom->SetSelection( 0 );
	fgSizer1->Add( m_choiceZoom, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	fgSizer1->Add( 3, 0, 0, wxEXPAND, 5 );

	m_count_tile = new wxStaticText( this, wxID_ANY, _("Tile count :"), wxDefaultPosition, wxSize( 60,-1 ), 0 );
	m_count_tile->Wrap( -1 );
	fgSizer1->Add( m_count_tile, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

	m_TileValue = new wxStaticText( this, wxID_ANY, _("---"), wxDefaultPosition, wxSize( 80,-1 ), wxALIGN_CENTER_HORIZONTAL );
	m_TileValue->Wrap( -1 );
	m_TileValue->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	fgSizer1->Add( m_TileValue, 0, wxALIGN_CENTER, 5 );

	m_ErrorText = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_ErrorText->Wrap( -1 );
	m_ErrorText->SetFont( wxFont( 9, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false, wxT("Arial") ) );
	m_ErrorText->SetForegroundColour( wxColour( 255, 0, 0 ) );

	fgSizer1->Add( m_ErrorText, 0, wxALIGN_CENTER, 5 );


	bSizer1->Add( fgSizer1, 0, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_generateChart = new wxButton( this, wxID_ANY, _("Generate Chart"), wxDefaultPosition, wxSize( 140,-1 ), 0 );
	m_generateChart->Enable( false );

	fgSizer2->Add( m_generateChart, 0, wxALL, 5 );

	m_deletegenerated = new wxButton( this, wxID_ANY, _("Delete Chart"), wxDefaultPosition, wxSize( 132,-1 ), 0 );
	m_deletegenerated->Enable( false );

	fgSizer2->Add( m_deletegenerated, 0, wxALL, 5 );

	m_cancel = new wxButton( this, wxID_ANY, _("Cancel"), wxDefaultPosition, wxSize( 85,-1 ), 0 );
	m_cancel->Enable( false );

	fgSizer2->Add( m_cancel, 0, wxALL, 5 );


	bSizer1->Add( fgSizer2, 1, wxEXPAND, 5 );

	m_ServerName = new wxStaticText( this, wxID_ANY, _("------"), wxDefaultPosition, wxSize( 397,-1 ), wxALIGN_CENTER_HORIZONTAL );
	m_ServerName->Wrap( -1 );
	m_ServerName->SetFont( wxFont( 8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial") ) );
	m_ServerName->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

	bSizer1->Add( m_ServerName, 0, 0, 0 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_startbutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_dialog::OnClickStartButton ), NULL, this );
	m_choiceZoom->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( m_dialog::OnChooseZoomLevel ), NULL, this );
	m_generateChart->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_dialog::OnClickGenerateChart ), NULL, this );
	m_deletegenerated->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_dialog::OnDeleteChart ), NULL, this );
	m_cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_dialog::OnClickCancel ), NULL, this );
}

m_dialog::~m_dialog()
{
	// Disconnect Events
	m_startbutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_dialog::OnClickStartButton ), NULL, this );
	m_choiceZoom->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( m_dialog::OnChooseZoomLevel ), NULL, this );
	m_generateChart->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_dialog::OnClickGenerateChart ), NULL, this );
	m_deletegenerated->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_dialog::OnDeleteChart ), NULL, this );
	m_cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( m_dialog::OnClickCancel ), NULL, this );

}

m_Parameterdialog::m_Parameterdialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxDialog( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxSize( 387,444 ), wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );


	bSizer1->Add( 0, 5, 0, 0, 1 );

	m_staticText9 = new wxStaticText( this, wxID_ANY, _("  Downloadsource (URL) : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer1->Add( m_staticText9, 0, wxALL, 2 );

	m_DownloadURL = new wxComboBox( this, wxID_ANY, _("Google Sattalite (khms1)"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_DownloadURL->Append( _("Google Sattalite (khms1) v=1001") );
	m_DownloadURL->Append( _("Google Sattalite (khms2) v=1001") );
	m_DownloadURL->Append( _("Google Sattalite (khms3) v=1001") );
	m_DownloadURL->Append( _("OpenStreetmap") );
	m_DownloadURL->Append( _("User defined") );
	m_DownloadURL->SetSelection( 0 );
	bSizer1->Add( m_DownloadURL, 0, wxALL|wxEXPAND, 2 );

	m_UserURL = new wxTextCtrl( this, wxID_ANY, _("https://khms3.google.com/kh/v=[v]?x=[x]&y=[y]&z=[z]"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_UserURL, 0, wxALL|wxEXPAND, 5 );

	m_staticText10 = new wxStaticText( this, wxID_ANY, _("  example : http(s)://Server.com?[x]?[y]?[z]"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	m_staticText10->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );

	bSizer1->Add( m_staticText10, 0, wxALIGN_CENTER|wxALL, 1 );

	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );

	m_staticText1 = new wxStaticText( this, wxID_ANY, _("Output Chartdirectory (.kap) :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer1->Add( m_staticText1, 0, wxALL, 5 );

	m_chartdirectory = new wxDirPickerCtrl( this, wxID_ANY, wxT("c:\\charts\\"), _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST|wxDIRP_SMALL );
	bSizer1->Add( m_chartdirectory, 0, wxALL|wxEXPAND, 1 );

	m_staticText11 = new wxStaticText( this, wxID_ANY, _("tmp tile dir :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer1->Add( m_staticText11, 0, wxALL, 5 );

	m_tiledirectory = new wxDirPickerCtrl( this, wxID_ANY, wxT("c:\\tile\\"), _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST|wxDIRP_SMALL );
	bSizer1->Add( m_tiledirectory, 0, wxALL|wxEXPAND, 1 );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText111 = new wxStaticText( this, wxID_ANY, _("max tile download "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	fgSizer4->Add( m_staticText111, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_MaxTile = new wxTextCtrl( this, wxID_ANY, _("2000"), wxDefaultPosition, wxSize( 40,-1 ), wxTE_CENTER );
	#ifdef __WXGTK__
	if ( !m_MaxTile->HasFlag( wxTE_MULTILINE ) )
	{
	m_MaxTile->SetMaxLength( 4 );
	}
	#else
	m_MaxTile->SetMaxLength( 4 );
	#endif
	fgSizer4->Add( m_MaxTile, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	bSizer1->Add( fgSizer4, 1, wxEXPAND, 5 );

	m_LogTile = new wxCheckBox( this, wxID_ANY, _("Log Tilemerger output to OpenCPN Logfile"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_LogTile, 0, wxALL, 5 );

	m_deleteTile = new wxCheckBox( this, wxID_ANY, _("delete tile after download"), wxDefaultPosition, wxDefaultSize, 0 );
	m_deleteTile->SetValue(true);
	bSizer1->Add( m_deleteTile, 0, wxALL, 5 );

	m_RenameChartfile = new wxCheckBox( this, wxID_ANY, _("Rename Chartfile (.kap) after download"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_RenameChartfile, 0, wxALL, 5 );

	m_ShowChartSelection = new wxCheckBox( this, wxID_ANY, _("Show chartselection area"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ShowChartSelection->SetValue(true);
	bSizer1->Add( m_ShowChartSelection, 0, wxALL, 5 );

	m_DrawRealTile = new wxCheckBox( this, wxID_ANY, _("Show real Tilechartarea"), wxDefaultPosition, wxDefaultSize, 0 );
	m_DrawRealTile->SetValue(true);
	bSizer1->Add( m_DrawRealTile, 0, wxALL, 5 );

	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();

	bSizer1->Add( m_sdbSizer1, 1, wxEXPAND|wxRIGHT, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_DownloadURL->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( m_Parameterdialog::OnChooseSource ), NULL, this );
	m_MaxTile->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( m_Parameterdialog::OnEnterText ), NULL, this );
	m_ShowChartSelection->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( m_Parameterdialog::OnShowChartselectionArea ), NULL, this );
	m_DrawRealTile->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( m_Parameterdialog::OnShowRealTileChartArea ), NULL, this );
}

m_Parameterdialog::~m_Parameterdialog()
{
	// Disconnect Events
	m_DownloadURL->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( m_Parameterdialog::OnChooseSource ), NULL, this );
	m_MaxTile->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( m_Parameterdialog::OnEnterText ), NULL, this );
	m_ShowChartSelection->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( m_Parameterdialog::OnShowChartselectionArea ), NULL, this );
	m_DrawRealTile->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( m_Parameterdialog::OnShowRealTileChartArea ), NULL, this );

}

RenameDialog::RenameDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_staticText12 = new wxStaticText( this, wxID_ANY, _("Enter Chart Filename (without .kap ending)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer3->Add( m_staticText12, 0, wxALL, 5 );

	m_NewchartName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 230,-1 ), 0 );
	bSizer3->Add( m_NewchartName, 0, wxALL, 5 );

	m_sdbSizer3 = new wxStdDialogButtonSizer();
	m_sdbSizer3OK = new wxButton( this, wxID_OK );
	m_sdbSizer3->AddButton( m_sdbSizer3OK );
	m_sdbSizer3Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer3->AddButton( m_sdbSizer3Cancel );
	m_sdbSizer3->Realize();

	bSizer3->Add( m_sdbSizer3, 1, 0, 5 );


	this->SetSizer( bSizer3 );
	this->Layout();

	this->Centre( wxBOTH );
}

RenameDialog::~RenameDialog()
{
}
