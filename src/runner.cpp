#include <pch.hpp>

#include "frame.hpp"


class App : public wxApp
{
    public:
    bool OnInit();
};

bool App::OnInit()
{
    wxFrame* frame = new Frame("Image Viewer");

    frame->SetSize(600,400);
    frame->Centre();
    frame->Show(true);

    return true;
}

wxIMPLEMENT_APP(App);