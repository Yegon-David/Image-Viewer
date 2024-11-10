#pragma once


#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

#include <vector>
#include <filesystem>


inline wxRect wxRect2DDouble_to_wxRect(const wxRect2DDouble& rect)
{
    return wxRect(static_cast<int>(rect.m_x),static_cast<int>(rect.m_y),static_cast<int>(rect.m_width),static_cast<int>(rect.m_height));
}


inline void log_rect(const wxRect& rect)
{
    wxLogDebug(wxString::Format("rect(x:%d,y:%d,w:%d,h:%d))", rect.x, rect.y, rect.width,
                            rect.height));
}
inline void log_rect(const wxString& str,const wxRect& rect)
{
    wxLogDebug(wxString::Format("%s rect(x:%d,y:%d,w:%d,h:%d))", str,rect.x, rect.y, rect.width,
                            rect.height));
}

inline wxFont FONT(const size_t& size,const bool& bold = true)
{
    return wxFont(size, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL,
                           bold ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL);
}