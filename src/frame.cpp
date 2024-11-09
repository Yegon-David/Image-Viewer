#include "frame.hpp"



Frame::Frame(const wxString & title) : 
wxFrame(nullptr,wxID_ANY,title)
{
    wxInitAllImageHandlers();
    this->SetPosition({1600,300});
       
    m_gallery = new Gallery(this);
    
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_gallery,1,wxEXPAND);

    this->SetSizer(sizer);
    this->Layout();

    m_logger = new wxLogWindow(this,"Logger",true,false);
    wxLog::SetActiveTarget(m_logger);

  
}
