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
    return wxString::Format("%s rect(x:%d,y:%d,w:%d,h:%d))",
                            GetFilename(), rect.x, rect.y, rect.width,
                            rect.height);
  }
  void move(const double& t,const bool& forward = true)
  {
    if(forward){
      rect.SetX(static_cast<int>(prev_rect.x + t*t* (future_rect.x-prev_rect.x)));
      rect.SetY(static_cast<int>(prev_rect.y + t*t* (future_rect.y-prev_rect.y)));
      rect.SetWidth(static_cast<int>(prev_rect.width + t*t* (future_rect.width-prev_rect.width)));
      rect.SetHeight(static_cast<int>(prev_rect.height + t*t* (future_rect.height-prev_rect.height)));
    }
    else
    {
      rect.SetX(static_cast<int>(future_rect.x + t*t* (prev_rect.x-future_rect.x)));
      rect.SetY(static_cast<int>(future_rect.y + t*t* (prev_rect.y-future_rect.y)));
      rect.SetWidth(static_cast<int>(future_rect.width + t*t* (prev_rect.width-future_rect.width)));
      rect.SetHeight(static_cast<int>(future_rect.height + t*t* (prev_rect.height-future_rect.height)));
    }
  }
};

class DataLogic {
public:
  DataLogic() : active_index(0), deflation(30), m_gap(20) {}

  void next() {
    if (m_objects.empty() || m_objects.size() <= 1)
      return;

    if (isrunning()) {
      wxLogDebug("animation in progress");
      return;
    }

    if ((active_index + 1) >= (m_objects.size() - 1)) {
      active_index = 0;

    } else {
      active_index++;
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

    if (active_index == 0) {
      active_index = m_objects.size() - 1;

    } else {
      active_index--;
    }
    this->animate_prev();
  }

private:
  size_t active_index, deflation;

protected:
  std::vector<ImageObject> m_objects;
  double m_gap;

  const size_t m_index() const { return active_index; }
  size_t m_index() { return active_index; }

  const size_t prev_index() const {
    return m_index() <= 0 ? m_objects.size() - 1 : m_index() - 1;
  }
  size_t prev_index() {
    return m_index() <= 0 ? m_objects.size() - 1 : m_index() - 1;
  }

  const size_t future_index() const {
    return m_index() > m_objects.size() - 1 ? 0 : m_index() + 1;
  }
  size_t future_index() {
    return m_index() > m_objects.size() - 1 ? 0 : m_index() + 1;
  }

  const size_t m_deflation() const { return deflation; }
  size_t m_deflation() { return deflation; }

  virtual void animate_next() = 0;
  virtual void animate_prev() = 0;
  virtual void animate_pick_at() = 0;
  virtual const bool isrunning() const = 0;
};