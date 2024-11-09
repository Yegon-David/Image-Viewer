#include "gallery.hpp"

Gallery::Gallery(wxWindow *parent)
    : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
               wxFULL_REPAINT_ON_RESIZE),
      AnimationLogic() {

  this->SetBackgroundStyle(wxBG_STYLE_PAINT);

  wxEvtHandler *handler = wxWindow::GetEventHandler();
  handler->Bind(wxEVT_PAINT, &Gallery::on_paint, this);
  handler->Bind(wxEVT_SIZE, &Gallery::on_size, this);
  handler->Bind(wxEVT_KEY_DOWN, &Gallery::on_key_down, this);

  this->load_images();
}

void Gallery::load_images() {
  m_objects.push_back({{"D:/pjojects/images/New folder/n1.jpg", wxBITMAP_TYPE_ANY}, wxRect()});
  m_objects.push_back({{"D:/pjojects/images/New folder/a.jpg", wxBITMAP_TYPE_ANY}, wxRect()});
  m_objects.push_back({{"D:/pjojects/images/New folder/n2.jpg", wxBITMAP_TYPE_ANY}, wxRect()});
  m_objects.push_back({{"D:/pjojects/images/New folder/n3.jpg",wxBITMAP_TYPE_ANY},wxRect()});
  m_objects.push_back({{"D:/pjojects/images/New folder/n4.jpg",wxBITMAP_TYPE_ANY},wxRect()});
  m_objects.push_back({{"D:/pjojects/images/New folder/n5.jpg",wxBITMAP_TYPE_ANY},wxRect()});
}

void Gallery::on_paint(wxPaintEvent &evt) {
  wxAutoBufferedPaintDC dc(this);
  dc.SetBackground(wxBrush("white"));
  dc.Clear();

  auto gc = wxGraphicsContext::Create(dc);

  if (gc && !m_objects.empty()) {
    if (m_objects.at(m_index()).image.IsOk())

    {

      this->draw_image(&gc, m_objects.at(m_index()).image,
                       m_objects.at(m_index()).rect, 3);
    }

    for (auto i = 0; i < m_objects.size(); i++) {
      if (i == m_index())
        continue;

      if (m_objects.at(i).image.IsOk()) {

        this->draw_image(&gc, m_objects.at(i).image, m_objects.at(i).rect, 2);

      } else {
        wxLogDebug("Image at index %zu not ok!", i);
      }
    }

    delete gc;
  }
}

void Gallery::draw_image(wxGraphicsContext **gc, const wxBitmap &image,
                         const wxRect &rect, const size_t &roundness) {

  wxPoint points[] = {
      wxPoint(rect.GetX() + roundness, rect.GetY()),     // Top-left corner
      wxPoint(rect.GetRight() - roundness, rect.GetY()), // Top-right corner
      wxPoint(rect.GetRight(),
              rect.GetY() + roundness), // Right-top corner curve
      wxPoint(rect.GetRight(),
              rect.GetBottom() - roundness), // Right-bottom corner
      wxPoint(rect.GetRight() - roundness,
              rect.GetBottom()), // Bottom-right corner curve
      wxPoint(rect.GetX() + roundness,
              rect.GetBottom()), // Bottom-left corner
      wxPoint(rect.GetX(),
              rect.GetBottom() - roundness),        // Left-bottom corner curve
      wxPoint(rect.GetX(), rect.GetY() + roundness) // Left-top corner curve
  };
  wxRegion region(8, points, wxODDEVEN_RULE);

  if (roundness)
    (*gc)->Clip(region);

  double image_width =
      static_cast<double>(image.GetWidth()); // is initially int
  double image_height =
      static_cast<double>(image.GetHeight()); // is initially int // cover
  double scalex = static_cast<double>(rect.GetWidth()) / image.GetWidth();
  double scaley = static_cast<double>(rect.GetHeight()) / image.GetHeight();
  double scale = std::max(scalex, scaley);
  image_width *= scale;
  image_height *= scale;
  double x = rect.GetX() + rect.GetWidth() / 2 - image_width / 2;
  double y = rect.GetY() + rect.GetHeight() / 2 - image_height / 2;

  // wxLogMessage("image: (x:%.2f,y:%.2f,w:%.2f,h:%.2f)", x, y, image_width,
  //              image_height);

  (*gc)->DrawBitmap(image, x, y, image_width, image_height);

  if (roundness)
    (*gc)->ResetClip();
}

void Gallery::on_size(wxSizeEvent &evt) {
  wxRect rect = this->GetClientRect();
  if (f_rect.IsEmpty() || f_rect != rect ||
      f_rect.GetPosition() != rect.GetPosition()) {
    f_rect = rect;
    m_objects.at(m_index()).rect = rect;
    m_objects.at(m_index()).rect.Deflate(FromDIP(m_deflation()));
  }
  this->update_cordinates();
  wxLogDebug("calling size");
  evt.Skip();
}

void Gallery::update_cordinates() {
  m_objects.at(m_index()).rect = get_banner_rect();

  double card_width = f_rect.GetWidth() * 0.2;
  double card_height = f_rect.GetHeight() * 0.4;

  double y = f_rect.GetY() + (f_rect.GetHeight() - card_height) / 2;

  double ix = f_rect.GetX() + f_rect.GetWidth() / 2;

  size_t offset = 0;

  for (size_t i = 0; i < m_objects.size(); i++) {
    if (i == m_index()) {
      offset++;
      continue;
    }
    const size_t index = i;

    double x = ix + static_cast<double>(index-offset) * (m_gap + card_width);

    m_objects.at(index).rect =
        wxRect2DDouble_to_wxRect(wxRect2DDouble(x, y, card_width, card_height));
    
  }
  for (size_t i = 0; i < m_objects.size(); i++) {

    const size_t index = i;
    const size_t pindex = index <= 0 ? m_objects.size() - 1 : index - 1;
    const size_t nindex = index < m_objects.size() - 1 ? index + 1 : 0;

   m_objects.at(index).prev_rect =  m_objects.at(pindex).rect;
   m_objects.at(index).future_rect =  m_objects.at(nindex).rect;
  }
   for (size_t i = 0; i < m_objects.size(); i++) 
   {
      wxLogDebug("%zu",i);
      log_rect(m_objects.at(i).rect);
      log_rect("prev",m_objects.at(i).prev_rect);
      log_rect("fiture",m_objects.at(i).future_rect);
   }
  // wxLogDebug("%zu",m_index());
  // log_rect(m_objects.at(m_index()).rect);
  // log_rect("prev",m_objects.at(m_index()).rect);
  // log_rect("fiture",m_objects.at(m_index()).rect);

  this->f_refresh();

}

void Gallery::on_key_down(wxKeyEvent &evt) {

  if (evt.GetKeyCode() == WXK_RIGHT) {
    this->next();
  } else if (evt.GetKeyCode() == WXK_LEFT) {
    this->prev();
  }

  evt.Skip();
}

void Gallery::refresh() {
  for (size_t i = 0; i < m_objects.size(); i++) {
    this->RefreshRect(m_objects.at(i).rect);
    this->RefreshRect(m_objects.at(i).prev_rect);
  }
}