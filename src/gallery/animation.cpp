#include "animation.hpp"

AnimationLogic::AnimationLogic() : 
                                DataLogic(),
                                wxEvtHandler() ,
                                m_timer(this,wxID_ANY),
                                m_duration(2000),
                                a_type(Animation::Single)
{
    
    Bind(wxEVT_MOVE,&AnimationLogic::on_move,this);
    Bind(wxEVT_TIMER,&AnimationLogic::on_timer,this,m_timer.GetId());

}

void AnimationLogic::on_timer(wxTimerEvent &evt)
{
    double elapsed = (wxGetLocalTimeMillis() - start_time).ToDouble();

    if(elapsed < m_duration)
    {
        double t = elapsed / m_duration;

        for(size_t i = 0; i < m_objects.size(); i ++)
        {
            
             m_objects.at(i).move(t,a_type == Animation::Next ? true : false);
        }

    }
    else{
        m_timer.Stop();
        this->update_cordinates();
        
    }

    this->refresh();

   
    evt.Skip();
}

void AnimationLogic::on_move(wxMoveEvent & evt)
{
    auto pos = evt.GetPosition();
    wxLogDebug("pos:(%d,%d)",pos.x,pos.y);
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

void AnimationLogic::animate()
{
    if(m_objects.size() <= 1) {
      wxLogDebug("No images to animate");
      return;
    }

    start_time = wxGetLocalTimeMillis();
    
    m_timer.Start(16);

}
