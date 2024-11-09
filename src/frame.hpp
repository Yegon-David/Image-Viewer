#pragma once

#include <pch.hpp>

#include <gallery.hpp> 

class Frame : public wxFrame
{
    public:
    Frame(const wxString & title);
 ~Frame(){
    wxLog::SetActiveTarget(nullptr);
    delete m_logger;
 }
private:
    Gallery* m_gallery;
    wxLog* m_logger;

};