#pragma once

#include <pch.hpp>

struct ImageObject {
  wxBitmap image;

  wxRect rect;
  size_t id;
  wxRect prev_rect = wxRect();
  wxRect future_rect = wxRect();

  wxString name = GetFilename();

  static wxString GetFilename() { return wxString::Format("image:"); }

  operator wxString() {
    return wxString::Format("%s rect(x:%d,y:%d,w:%d,h:%d))", GetFilename(),
                            rect.x, rect.y, rect.width, rect.height);
  }
  
  void move(const double &t) {

      rect.SetX(static_cast<int>(prev_rect.x +
                                 t * t * (future_rect.x - prev_rect.x)));
      rect.SetY(static_cast<int>(prev_rect.y +
                                 t * t * (future_rect.y - prev_rect.y)));
      rect.SetWidth(static_cast<int>(
          prev_rect.width + t * t * (future_rect.width - prev_rect.width)));
      rect.SetHeight(static_cast<int>(
          prev_rect.height + t * t * (future_rect.height - prev_rect.height)));
    
  }
  
  inline wxString srect()
  {
    return wxString::Format("current:(x:%d,y:%d,w:%d,h:%d)",rect.x, rect.y, rect.width, rect.height);
  }
  inline wxString prev()
  {
     return wxString::Format("prev:(x:%d,y:%d,w:%d,h:%d)",prev_rect.x, prev_rect.y, prev_rect.width, prev_rect.height);
  }
  inline wxString future()
  {
     return wxString::Format("future:(x:%d,y:%d,w:%d,h:%d)",future_rect.x, future_rect.y, future_rect.width, future_rect.height);
  }

  void log_all()
  {
    wxLogDebug("%s\n%s\n%s\n",prev(),srect(),future());
  }
};

class DataLogic {
public:
  DataLogic() : active_index(0), deflation(30), m_gap(20),index(m_index()) {
    offset=0;
    offset_start=offset;
    offset_end=0;
  }

  void next() {
   
    if (m_objects.empty())
      return;

    if (isrunning()) {
      wxLogDebug("animation in progress");
      return;
    }

    this->animate_next();


  }

  void prev() {
    if (m_objects.empty())
      return;
    if (isrunning()) {
      wxLogDebug("animation in progress");
      return;
    }
    this->animate_prev();
  }

  void set(const size_t& value)
  {
    active_index = value;
  }
  
  ImageObject pop()
  {
    ImageObject temp;
    auto it = m_objects.begin()+m_index();
    temp = *it;
    m_objects.erase(it);
    return temp;
  }
  ImageObject pop_back()
  {
    ImageObject temp = m_objects.back();
    m_objects.pop_back();
    return temp;
  }
  
  void add_back()
  {
    m_objects.insert(m_objects.end(),m_object);
  }
  
  void add_front()
  {
    m_objects.insert(m_objects.begin(),m_object);
  }
  
  void set_next()
  {
    ImageObject n_temp = pop();
    add_back();
    m_object = n_temp;
  }

  void set_prev()
  {
    ImageObject n_temp = pop_back();
    add_front();
    m_object = n_temp;
  }

private:
  size_t active_index, deflation;



protected:
  std::vector<ImageObject> m_objects;
  ImageObject m_object,m_object_front;
  double m_gap,offset,offset_start,offset_end;
  size_t index;

  const size_t m_index() const { return active_index; }
  size_t m_index() { return active_index; }

  const size_t prev_index() const {
    return m_index() <= 0 ? m_objects.size() - 1 : m_index() - 1;
  }
  size_t prev_index() {
    return m_index() <= 0 ? m_objects.size() - 1 : m_index() - 1;
  }

  const size_t future_index() const {
    return m_index() < m_objects.size() - 1 ? m_index() + 1 : 0;
  }
  size_t future_index() {
    return m_index() < m_objects.size() - 1 ? m_index() + 1 : 0;
  }

  const size_t m_deflation() const { return deflation; }
  size_t m_deflation() { return deflation; }

  virtual void animate_next() = 0;
  virtual void animate_prev() = 0;
  virtual void animate_pick_at() = 0;
  virtual const bool isrunning() const = 0;
 
  void log_data()
  {
    wxString str="";
    for(size_t i=0;i<m_objects.size();i++)
    {
     str += wxString::Format("(l:%zu,id:%zu)",i,m_objects.at(i).id);
    }
    wxLogDebug("<object> %s out(%zu)",str.c_str(),m_object.id);
  }
};