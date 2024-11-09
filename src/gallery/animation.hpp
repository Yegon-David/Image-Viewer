#pragma once

#include "datamanager.hpp"

enum struct Animation : int 
{
  Prev=1,Next,AT,Single
};

class AnimationLogic : public DataLogic , public wxEvtHandler
{
public:
    AnimationLogic();

private:

    wxLongLong start_time;
    wxTimer m_timer;
    double m_duration;


    void on_timer(wxTimerEvent& evt);
    void on_move(wxMoveEvent& evt);

    virtual wxRect get_banner_rect() const = 0;
    

protected:
    wxRect f_rect,o_rect,prev_old_rect;
    
    Animation a_type;
    
    virtual void animate_next();
    virtual void animate_prev();
    virtual void animate_pick_at();

    virtual void refresh()=0;
    virtual void f_refresh()=0;

    virtual void update_cordinates()=0;

    void animate();

    const bool isrunning() const {return m_timer.IsRunning();}

};