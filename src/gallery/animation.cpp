#include "animation.hpp"

AnimationLogic::AnimationLogic()
    : DataLogic(), wxEvtHandler(), m_timer(this, wxID_ANY), m_duration(1000),
      a_type(Animation::Single) {

  Bind(wxEVT_MOVE, &AnimationLogic::on_move, this);
  Bind(wxEVT_TIMER, &AnimationLogic::on_timer, this, m_timer.GetId());
}

void AnimationLogic::on_timer(wxTimerEvent &evt) {
  double elapsed = (wxGetLocalTimeMillis() - start_time).ToDouble();

  if (elapsed <= m_duration) {
    double t = elapsed / m_duration;

    for (size_t i = (m_index() + 1) % m_objects.size(); i != m_index();
         i = (i + 1) % m_objects.size()) {
         auto initial_rect = m_objects.at(i).rect;
         auto target_rect = m_objects.at(i).future_rect;

            if (a_type==Animation::Next) {
                 m_objects.at(i).rect.SetX(static_cast<int>(initial_rect.x +
                                            t * t * (target_rect.x - initial_rect.x)));
                 m_objects.at(i).rect.SetY(static_cast<int>(initial_rect.y +
                                            t * t * (target_rect.y - initial_rect.y)));
                 m_objects.at(i).rect.SetWidth(static_cast<int>(
                    initial_rect.width + t * t * (target_rect.width - initial_rect.width)));
                 m_objects.at(i).rect.SetHeight(static_cast<int>(
                    initial_rect.height + t * t * (target_rect.height - initial_rect.height)));
                } else {
                 m_objects.at(i).rect.SetX(static_cast<int>(target_rect.x +
                                            t * t * (initial_rect.x - target_rect.x)));
                 m_objects.at(i).rect.SetY(static_cast<int>(target_rect.y +
                                            t * t * (initial_rect.y - target_rect.y)));
                 m_objects.at(i).rect.SetWidth(static_cast<int>(
                    target_rect.width + t * t * (initial_rect.width - target_rect.width)));
                 m_objects.at(i).rect.SetHeight(
                    static_cast<int>(target_rect.height +
                                    t * t * (initial_rect.height - target_rect.height)));
                }
    }

  } else {
    set(index);
    m_timer.Stop();
    // this->update_cordinates();
  }
  this->f_refresh();
  evt.Skip();
}

void AnimationLogic::on_move(wxMoveEvent &evt) {
  auto pos = evt.GetPosition();
  wxLogDebug("pos:(%d,%d)", pos.x, pos.y);
  this->update_cordinates();
  evt.Skip();
}

void AnimationLogic::animate_next() {
  a_type = Animation::Next;
  index = future_index();
  this->animate();
}

void AnimationLogic::animate_prev() {
  a_type = Animation::Prev;
  index = prev_index();

  this->animate();
}

void AnimationLogic::animate_pick_at() {
  wxLogDebug("Animate to pick at triggered!");
}

void AnimationLogic::animate() {
  if (m_objects.size() <= 1) {
    wxLogDebug("No images to animate");
    return;
  }

  start_time = wxGetLocalTimeMillis();

  m_timer.Start(16);
  wxLogDebug("index %zu", index);
}
