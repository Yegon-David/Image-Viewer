#pragma once

#include <pch.hpp>

struct ImageObject {
  wxBitmap image;

  wxRect rect;
  wxRect prev_rect = wxRect();
  wxRect future_rect = wxRect();

  wxString name = GetFilename();

  static wxString GetFilename() { return wxString::Format("image:"); }

  operator wxString() {
    return wxString::Format("%s rect(x:%d,y:%d,w:%d,h:%d))", GetFilename(),
                            rect.x, rect.y, rect.width, rect.height);
  }
  
  void move(const double &t, const bool &forward = true) {

    if (forward) {
      rect.SetX(static_cast<int>(prev_rect.x +
                                 t * t * (future_rect.x - prev_rect.x)));
      rect.SetY(static_cast<int>(prev_rect.y +
                                 t * t * (future_rect.y - prev_rect.y)));
      rect.SetWidth(static_cast<int>(
          prev_rect.width + t * t * (future_rect.width - prev_rect.width)));
      rect.SetHeight(static_cast<int>(
          prev_rect.height + t * t * (future_rect.height - prev_rect.height)));
    } else {
      rect.SetX(static_cast<int>(future_rect.x +
                                 t * t * (prev_rect.x - future_rect.x)));
      rect.SetY(static_cast<int>(future_rect.y +
                                 t * t * (prev_rect.y - future_rect.y)));
      rect.SetWidth(static_cast<int>(
          future_rect.width + t * t * (prev_rect.width - future_rect.width)));
      rect.SetHeight(
          static_cast<int>(future_rect.height +
                           t * t * (prev_rect.height - future_rect.height)));
    }
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
  DataLogic() : active_index(0), deflation(30), m_gap(20),index(m_index()) {}

  void next() {
   
    if (m_objects.empty() || m_objects.size() <= 1)
      return;

    if (isrunning()) {
      wxLogDebug("animation in progress");
      return;
    }

    this->animate_next();


  }

  void prev() {
    if (m_objects.empty() || m_objects.size() <= 1)
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

private:
  size_t active_index, deflation;

protected:
  std::vector<ImageObject> m_objects;
  double m_gap;
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

  void log_order() {
    if (!(m_objects.size() > 1))
      return;

    wxString str = "";
    for (size_t i = 0; i < m_objects.size(); i++) {
      char c = (i == m_objects.size() - 1) ? '\0' : ',';

      if (i == m_index()) {
        str += wxString::Format(">%zu<%c", i, c);
      } else {
        str += wxString::Format("%zu%c", i, c);
      }
    }
    wxLogDebug("lists [%s]", str.c_str());
  }
  void log_display_order() {
    if (!(m_objects.size() > 1))
      return;

    wxString str = "";
    for (size_t i = (m_index() + 1) % m_objects.size(); i != m_index();
         i = (i + 1) % m_objects.size()) {
      char c = (i == m_objects.size() - 1) ? '\0' : ',';

      if (i == m_index()) {
        str += wxString::Format(">%zu<%c", i, c);
      } else {
        str += wxString::Format("%zu%c", i, c);
      }
    }
    wxLogDebug("lists [%s]", str.c_str());
  }
  void log_args()
  {
  wxLogDebug("prev %zu",prev_index());
  wxLogDebug(m_objects.at(prev_index()).prev());
  wxLogDebug(m_objects.at(prev_index()).srect());
  wxLogDebug(m_objects.at(prev_index()).future());
  wxLogDebug("now %zu", m_index());
  wxLogDebug(m_objects.at(m_index()).prev());
  wxLogDebug(m_objects.at(m_index()).srect());
  wxLogDebug(m_objects.at(m_index()).future());
  wxLogDebug("future %zu",future_index());
  wxLogDebug(m_objects.at(future_index()).prev());
  wxLogDebug(m_objects.at(future_index()).srect());
  wxLogDebug(m_objects.at(future_index()).future());
  wxLogDebug("\n");
  }
};