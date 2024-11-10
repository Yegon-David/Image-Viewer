#include "animation.hpp"

AnimationLogic::AnimationLogic()
    : DataLogic(), wxEvtHandler(), m_timer(this, wxID_ANY), m_duration(400),
      a_type(Animation::Single) {

  Bind(wxEVT_MOVE, &AnimationLogic::on_move, this);
  Bind(wxEVT_TIMER, &AnimationLogic::on_timer, this, m_timer.GetId());
}

void AnimationLogic::on_timer(wxTimerEvent &evt) {
  double elapsed = (wxGetLocalTimeMillis() - start_time).ToDouble();

  if (elapsed <= m_duration) {
    double t = elapsed / m_duration;

    m_object_front.move(t);

    offset = offset_start + t * t * (offset_end - offset_start);
    for (size_t i = 0; i < m_objects.size(); i++) {
      m_objects.at(i).move(t);
    }

  } else {
    m_object_front.move(1);
    for (size_t i = 0; i < m_objects.size(); i++) {
      m_objects.at(i).move(1);
    }
    m_timer.Stop();
    if (a_type == Animation::Next)
      set_next();

    this->update_cordinates();
  }
  this->refresh();
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
  this->animate();
}

void AnimationLogic::animate_prev() {
  a_type = Animation::Prev;
  this->animate();
}

void AnimationLogic::animate_pick_at() {
  wxLogDebug("Animate to pick at triggered!");
}

void AnimationLogic::animate() {
  if (m_objects.size() < 1) {
    wxLogDebug("No images to animate");
    return;
  }
  this->displace();
  log_data();
  start_time = wxGetLocalTimeMillis();

  m_timer.Start(16);
}

void AnimationLogic::displace() {
  auto start_rect = get_next_rect();
  switch (a_type) {

  case Animation::Next:
    m_object_front = m_objects.at(0);
    m_object_front.future_rect = m_object_front.prev_rect;
    m_object_front.prev_rect = m_object_front.rect;



    for (size_t i = 0; i < m_objects.size(); i++) {
      m_objects.at(i).future_rect = m_objects.at(i).prev_rect;
      m_objects.at(i).prev_rect = m_objects.at(i).rect;
     }

    offset_start = start_rect.width + m_gap;
    m_object_front.log_all();
    break;

  case Animation::Prev:
    m_object_front = m_object;
    m_object_front.prev_rect = m_object.rect;
    m_object_front.future_rect = get_next_rect();
    set_prev();
    m_object.rect = get_banner_rect();

    for (size_t i = 0; i < m_objects.size(); i++) {
      //  m_objects.at(i).future_rect = m_objects.at(i).prev_rect;
      m_objects.at(i).prev_rect = m_objects.at(i).rect;
      m_objects.at(i).log_all();
    }

    break;

  case Animation::Single:
    offset_start = 100;
    break;

  default:
    break;
  }
}

void AnimationLogic::animate_rect(wxRect &rect, const wxRect &rect_old,
                                  const wxRect &rect_new, const double &t) {

  rect.SetX(static_cast<int>(rect_old.x + t * t * (rect_new.x - rect_old.x)));

  rect.SetY(static_cast<int>(rect_old.y + t * t * (rect_new.y - rect_old.y)));
  rect.SetWidth(static_cast<int>(rect_old.width +
                                 t * t * (rect_new.width - rect_old.width)));
  rect.SetHeight(static_cast<int>(rect_old.height +
                                  t * t * (rect_new.height - rect_old.height)));
}
