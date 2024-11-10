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
  wxRect get_next_rect() const {
    const double card_width = f_rect.GetWidth() * 0.2;
    const double card_height = f_rect.GetHeight() * 0.4;

    const double y = f_rect.GetY() + (f_rect.GetHeight() - card_height) / 2;
    double ix = f_rect.GetX() + f_rect.GetWidth() / 2;

    return wxRect(wxRect2DDouble_to_wxRect(wxRect2DDouble(ix,y,card_width,card_height)));
  }


  void draw_image(wxGraphicsContext **gc, const wxBitmap &image,
                  const wxRect &rect, const size_t &roundness = 0);
};