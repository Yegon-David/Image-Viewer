#pragma once

#include <pch.hpp>

#include "animation.hpp"

class Gallery : public wxWindow, public AnimationLogic {
public:
  Gallery(wxWindow *parent);

void load_images();

private:
  void on_paint(wxPaintEvent &evt);
  void on_size(wxSizeEvent &evt);
  void on_key_down(wxKeyEvent& evt);

  void refresh();
  void f_refresh(){this->Refresh();}


  void update_cordinates();

  wxRect get_banner_rect() const {
    auto rect = f_rect;
    rect.Deflate(m_deflation());
    return rect;
  }

  void draw_image(wxGraphicsContext **gc, const wxBitmap &image,
                  const wxRect &rect, const size_t &roundness = 0);
};