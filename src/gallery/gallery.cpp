#include "gallery.hpp"

Gallery::Gallery(wxWindow *parent)
    : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
               wxFULL_REPAINT_ON_RESIZE),
      AnimationLogic() 
{

  this->SetBackgroundStyle(wxBG_STYLE_PAINT);

  wxEvtHandler *handler = wxWindow::GetEventHandler();
  handler->Bind(wxEVT_PAINT, &Gallery::on_paint, this);
  handler->Bind(wxEVT_SIZE, &Gallery::on_size, this);
  handler->Bind(wxEVT_KEY_DOWN, &Gallery::on_key_down, this);

  this->load_images();
}

void Gallery::load_images() {
  m_objects.push_back(
      {{"D:/pjojects/images/New folder/a.jpg", wxBITMAP_TYPE_ANY}, wxRect(),1});
  m_objects.push_back(
      {{"D:/pjojects/images/New folder/n1.jpg", wxBITMAP_TYPE_ANY}, wxRect(),0});

  m_objects.push_back(
      {{"D:/pjojects/images/New folder/n2.jpg", wxBITMAP_TYPE_ANY}, wxRect(),2});
  m_objects.push_back(
      {{"D:/pjojects/images/New folder/n3.jpg", wxBITMAP_TYPE_ANY}, wxRect(),3});
  m_objects.push_back(
      {{"D:/pjojects/images/New folder/n4.jpg", wxBITMAP_TYPE_ANY}, wxRect(),4});
  m_objects.push_back(
      {{"D:/pjojects/images/New folder/n5.jpg", wxBITMAP_TYPE_ANY}, wxRect(),5});

  m_object = pop();
 
}

void Gallery::on_paint(wxPaintEvent &evt) {
  wxAutoBufferedPaintDC dc(this);
  dc.SetBackground(wxBrush("#1e1e1e"));
  dc.Clear();

  auto gc = wxGraphicsContext::Create(dc);

  if (gc && !m_objects.empty()) {
    auto rect = get_banner_rect();
    gc->PushState();
    gc->SetBrush(wxBrush("#0001"));
    gc->DrawRoundedRectangle(rect.x-3, rect.y+7, rect.width, rect.height, 1);
    gc->PopState();
    
    if (m_object.image.IsOk())

    {
      this->draw_image(&gc, m_object.image,
                       m_object.rect);
      // this->draw_text(&gc,m_object.rect);
    }
    if (isrunning() && m_object_front.image.IsOk())

    {
      this->draw_image(&gc, m_object_front.image,
                       m_object_front.rect);
    }
    
    for (size_t i = 0;i < m_objects.size();i++) {

      if(i == 0 && isrunning()) continue;
 
      if (m_objects.at(i).image.IsOk()) {
        this->draw_image(&gc, m_objects.at(i).image,m_objects.at(i).rect, 2);
        // this->draw_text(&gc,m_objects.at(i).rect);

      } else {
        wxLogDebug("Image at index %zu not ok!", i);
      }
    }

    delete gc;
  }
}

void Gallery::draw_image(wxGraphicsContext **gc, const wxBitmap &image,
                         const wxRect &rect, const size_t &roundness)
{

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
        else
        (*gc)->Clip(rect);


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

    (*gc)->DrawBitmap(image, x, y, image_width, image_height);


        (*gc)->ResetClip();
}

void Gallery::draw_text(wxGraphicsContext **gc,wxRect rect)
{   
   double padding = 10;
   
    (*gc)->SetBrush(wxBrush("#0007"));
    (*gc)->SetPen(wxPen("#fff"));
    (*gc)->SetFont(FONT(12,false),"#fff");

    rect.Deflate(static_cast<int>(m_object.rect.GetHeight() / 5));
    rect.SetX(m_object.rect.x + padding);
    (*gc)->DrawRoundedRectangle(rect.x-3, rect.y+7, rect.width, rect.height, 1);

    double header_height = rect.height * 0.2;
    double date_height = rect.height * 0.15;
    double description_height = rect.height -(date_height+header_height);
    
    
      
      wxString text= "Filename";
      wxString ext= ".jpg";
      wxString date= "2023/09/04";
      wxString resolution= "1650 * 728";
      wxString path= "D:/pjojects/c++/projects/wx/Viewer";
      
      double th,tw;

      (*gc)->GetTextExtent(text,&tw,&th);
      (*gc)->DrawText(text,rect.x + padding,rect.y  + th /2); 

      (*gc)->GetTextExtent(ext,&tw,&th);
      (*gc)->DrawText(ext,rect.x + rect.width - padding - tw,rect.y  + th /2); 
      
      (*gc)->SetFont(FONT(10,false),"#fff4");

      (*gc)->GetTextExtent(resolution,&tw,&th);
      (*gc)->DrawText(resolution,rect.x + padding,rect.y + header_height + th / 2); 

      (*gc)->GetTextExtent(date,&tw,&th);
      (*gc)->DrawText(date,rect.x + rect.width - padding - tw,rect.y + header_height + th / 2);

      (*gc)->SetFont(FONT(12,false),"#fff8");
      (*gc)->GetTextExtent(path,&tw,&th);
      (*gc)->DrawText(path,rect.x + padding,rect.y + header_height + date_height+20); 

    (*gc)->StrokeLine(rect.x,rect.y+header_height,rect.x+rect.width,rect.y+header_height);
    

}

void Gallery::on_size(wxSizeEvent &evt) {
  wxRect rect = this->GetClientRect();
  if (f_rect.IsEmpty() || f_rect != rect ||
      f_rect.GetPosition() != rect.GetPosition()) {
    f_rect = rect;
  }
  this->update_cordinates();
  evt.Skip();
}

void Gallery::update_cordinates() {
  m_object.rect = get_banner_rect();
  
  auto init_rect = get_next_rect();
  for (size_t i = 0;i<m_objects.size();i++)    
  {
      const double x = init_rect.x + static_cast<double>(i) * (m_gap + init_rect.width);
      m_objects.at(i).rect = wxRect2DDouble_to_wxRect(wxRect2DDouble(x, init_rect.y, init_rect.width, init_rect.height));
  }
   for (size_t i = 0;i<m_objects.size();i++)    
   {
    const size_t pindex = i <= 0 ? m_objects.size() - 1 : i - 1;
    const size_t nindex = i < m_objects.size() - 1 ? i + 1 : 0;
   
    m_objects.at(i).prev_rect =(i==0)? m_object.rect : m_objects.at(pindex).rect;
    m_objects.at(i).future_rect =(i==m_objects.size()-1)? m_object.rect : m_objects.at(nindex).rect;
     m_objects.at(i).log_all();
   }

    
}

void Gallery::on_key_down(wxKeyEvent &evt) {

  if (evt.GetKeyCode() == WXK_RIGHT) {
    this->next();
  } else if (evt.GetKeyCode() == WXK_LEFT) {
    this->prev();
  }
  if(evt.GetUnicodeKey() == 'R')
  {
     this->update_cordinates();
     this->f_refresh();
     
  }
  else if(evt.GetUnicodeKey() == 'S')
  {
     a_type = Animation::Single;
     this->animate();
  }

  evt.Skip();
}

void Gallery::refresh() {
  for (size_t i = 0; i < m_objects.size(); i++) {
    this->RefreshRect(m_objects.at(i).rect);
    this->RefreshRect(m_objects.at(i).prev_rect);
  }
}
