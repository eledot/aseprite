// ASEPRITE gui library
// Copyright (C) 2001-2012  David Capello
//
// This source file is distributed under a BSD-like license, please
// read LICENSE.txt for more information.

#ifndef UI_WINDOW_H_INCLUDED
#define UI_WINDOW_H_INCLUDED

#include "base/compiler_specific.h"
#include "base/signal.h"
#include "gfx/point.h"
#include "ui/close_event.h"
#include "ui/event.h"
#include "ui/hit_test_event.h"
#include "ui/widget.h"

namespace ui {

  class Window : public Widget
  {
  public:
    Window(bool is_desktop, const char* text);
    ~Window();

    Widget* get_killer();

    void set_autoremap(bool state);
    void set_moveable(bool state);
    void set_sizeable(bool state);
    void set_ontop(bool state);
    void set_wantfocus(bool state);

    void remap_window();
    void center_window();
    void position_window(int x, int y);
    void move_window(JRect rect);

    void openWindow();
    void openWindowInForeground();
    void closeWindow(Widget* killer);

    bool is_toplevel();
    bool is_foreground() const { return m_is_foreground; }
    bool is_desktop() const { return m_is_desktop; }
    bool is_ontop() const { return m_is_ontop; }
    bool is_wantfocus() const { return m_is_wantfocus; }
    bool is_moveable() const { return m_is_moveable; }

    HitTest hitTest(const gfx::Point& point);

    void removeDecorativeWidgets();

    // Signals
    Signal1<void, CloseEvent&> Close;

  protected:
    virtual bool onProcessMessage(Message* msg) OVERRIDE;
    virtual void onPreferredSize(PreferredSizeEvent& ev) OVERRIDE;
    virtual void onPaint(PaintEvent& ev) OVERRIDE;
    virtual void onBroadcastMouseMessage(WidgetsList& targets) OVERRIDE;
    virtual void onSetText() OVERRIDE;

    // New events
    virtual void onClose(CloseEvent& ev);
    virtual void onHitTest(HitTestEvent& ev);

  private:
    void window_set_position(JRect rect);
    int get_action(int x, int y);
    void limit_size(int* w, int* h);
    void move_window(JRect rect, bool use_blit);

    Widget* m_killer;
    bool m_is_desktop : 1;
    bool m_is_moveable : 1;
    bool m_is_sizeable : 1;
    bool m_is_ontop : 1;
    bool m_is_wantfocus : 1;
    bool m_is_foreground : 1;
    bool m_is_autoremap : 1;
    int m_hitTest;
  };

} // namespace ui

#endif
