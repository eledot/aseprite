/* Jinete - a GUI library
 * Copyright (c) 2003, 2004, 2005, 2007, David A. Capello
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of the Jinete nor the names of its contributors may
 *     be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* #define REPORT_EVENTS */
/* #define LIMIT_DISPATCH_TIME */

#ifdef REPORT_EVENTS
#include <stdio.h>
#endif

#include <allegro.h>

#include "jinete/jinete.h"
#include "jinete/jintern.h"

#define TOPWND(manager) (jlist_first_data((manager)->children))

#define ACCEPT_FOCUS(widget)				\
  (((widget)->flags & (JI_FOCUSREST |			\
		       JI_DISABLED |			\
		       JI_HIDDEN |			\
		       JI_DECORATIVE)) == JI_FOCUSREST)

enum {
  STAGE_NORMAL,
  STAGE_WANT_CLOSE,
  STAGE_WAITING_REPLY,
  STAGE_CLOSE_ALL,
};

static int want_close_stage;	   /* variable to handle the external
				      close button in some Windows
				      enviroments */

static JWidget default_manager = NULL;

static JList new_windows;	/* windows that we should show */
static JList old_windows;	/* windows that we should destroy */
static JList proc_windows_list; /* current window's list in process */
static JList msg_queue;		/* messages queue */

static JWidget focus_widget;	/* the widget with the focus */
static JWidget mouse_widget;	/* the widget with the mouse */
static JWidget capture_widget;	/* the widget that captures the
				   mouse */

static bool first_time_poll;    /* TRUE when we don't enter in poll yet */

static char old_readed_key[KEY_MAX]; /* keyboard status of previous
					poll */

static bool manager_msg_proc(JWidget widget, JMessage msg);
static void manager_request_size(JWidget widget, int *w, int *h);
static void manager_set_position(JWidget widget, JRect rect);
static void manager_redraw_region(JWidget widget, JRegion region);

static void dispatch_msgs(void);
static void destroy_window(JWidget window);
static void remove_msgs_for(JWidget widget, JMessage msg);
static void generate_proc_windows_list(void);
static void generate_proc_windows_list2(JWidget manager);
static int some_parent_is_focusrest(JWidget widget);
static JWidget find_magnetic_widget(JWidget widget);
static JMessage new_mouse_msg(int type);
static JMessage new_key_msg(int type, int readkey_value);
static void broadcast_key_msg(JWidget manager, JMessage msg);
/* keyboard focus movement stuff */
static bool move_focus(JWidget manager, JMessage msg);
static int count_widgets_accept_focus(JWidget widget);
static bool childs_accept_focus(JWidget widget, bool first);
static JWidget next_widget(JWidget widget);
static int cmp_left(JWidget widget, int x, int y);
static int cmp_right(JWidget widget, int x, int y);
static int cmp_up(JWidget widget, int x, int y);
static int cmp_down(JWidget widget, int x, int y);

/* hooks the close-button in some platform with window support */
static void allegro_window_close_hook(void)
{
  if (want_close_stage == STAGE_NORMAL)
    want_close_stage = STAGE_WANT_CLOSE;
}

JWidget ji_get_default_manager(void)
{
  return default_manager;
}

JWidget jmanager_new(void)
{
  JWidget widget;
  int c;

  if (!default_manager) {
    /* initialize system */
    _ji_system_init();
    _ji_font_init();
    _ji_theme_init();

    /* hook the window close message */
    want_close_stage = STAGE_NORMAL;
    set_window_close_hook(allegro_window_close_hook);

    /* empty lists */
    msg_queue = jlist_new();
    new_windows = jlist_new();
    old_windows = jlist_new();
    proc_windows_list = jlist_new();

    /* reset variables */
    focus_widget = NULL;
    mouse_widget = NULL;
    capture_widget = NULL;

    first_time_poll = TRUE;

    /* reset keyboard */
    for (c=0; c<KEY_MAX; c++)
      old_readed_key[c] = 0;
  }

  widget = jwidget_new(JI_MANAGER);

  jwidget_add_hook(widget, JI_MANAGER, manager_msg_proc, NULL);

  jrect_replace(widget->rc, 0, 0, JI_SCREEN_W, JI_SCREEN_H);
  jwidget_show(widget);

  /* default manager is the first one (and is ever visible) */
  if (!default_manager)
    default_manager = widget;

  return widget;
}

void jmanager_free(JWidget widget)
{
  /* there are some messages in queue? */
  jmanager_dispatch_messages();

  /* finish with main manager */
  if (default_manager == widget) {
    /* no more cursor */
    jmouse_set_cursor(JI_CURSOR_NULL);

    /* TODO destroy the AUTODESTROY windows in these lists */
    jlist_free(new_windows);
    jlist_free(old_windows);

    /* finish theme */
    ji_set_theme(NULL);

    /* destroy clipboard */
    jclipboard_set_text(NULL);

    /* destroy this widget */
    jwidget_free(widget);

    /* no more default manager */
    default_manager = NULL;

    /* shutdown system */
    _ji_theme_exit();
    _ji_font_exit();
    _ji_system_exit();
    _ji_free_all_widgets();
  }
  else {
    /* destroy this widget */
    jwidget_free(widget);
  }
}

void jmanager_run(JWidget widget)
{
  do {
  } while (jmanager_poll(widget, TRUE));
}

bool jmanager_poll(JWidget manager, bool all_windows)
{
/*   JList old_proc_windows_list; */
  JWidget widget;
  JWidget window;
  bool ret = TRUE;
  int mousemove;
  JMessage msg;
  JLink link;
  int c;

  /* poll keyboard */
  poll_keyboard();

  /* TODO check for STAGE */

  if (first_time_poll) {
    first_time_poll = FALSE;

    jmanager_refresh_screen();
    jmouse_set_cursor(JI_CURSOR_NORMAL);
  }

  /* first check */
  if (jlist_empty(manager->children))
    return FALSE;

  /* new windows to show? */
  if (!jlist_empty(new_windows)) {
    JI_LIST_FOR_EACH(new_windows, link) {
      window = link->data;

      /* dirty the entire window and show it */
      jwidget_dirty(window);
      jwidget_show(window);

      /* attract the focus to the magnetic widget */
      /* jmanager_attract_focus(window); */
      jmanager_focus_first_child(window);

      /* redraw all */
      /* jwidget_flush_redraw(window); */
    }

    jlist_clear(new_windows);

    generate_proc_windows_list();
  }

  /* create the window's list in process */
/*   old_proc_windows_list = proc_windows_list; */
/*   proc_windows_list = NULL; */

/*   generate_proc_windows_list (manager); */
  if (jlist_empty(proc_windows_list))
    generate_proc_windows_list();

  /* update mouse status */
  mousemove = jmouse_poll();

  /* get the widget under the mouse */
  widget = NULL;

  JI_LIST_FOR_EACH(proc_windows_list, link) {
    window = link->data;
    widget = jwidget_pick(window, jmouse_x(0), jmouse_y(0));
    if (widget)
      break;
  }

  /* fixup "mouse" flag */
  if (widget != mouse_widget) {
    if (!widget)
      jmanager_free_mouse();
    else
      jmanager_set_mouse(widget);
  }

  /* mouse movement */
  if (mousemove) {
    msg = new_mouse_msg(JM_MOTION);
    /* TODO rigid marshal */

    /* reset double click status */
    jmouse_set_click_level(JI_CLICK_NOT);

    /* send the mouse movement message */
    if (capture_widget)
      jmessage_broadcast_to_parents(msg, capture_widget);
    else if (mouse_widget)
      jmessage_broadcast_to_parents(msg, mouse_widget);

    jmanager_send_message(msg);
    jmessage_free(msg);
  }

  /* mouse wheel */
  if (jmouse_z(0) != jmouse_z(1)) {
    msg = new_mouse_msg(JM_WHEEL);
    /* TODO rigid marshal */

    /* send the mouse wheel message */
    if (capture_widget)
      jmessage_broadcast_to_parents(msg, capture_widget);
    else if (mouse_widget)
      jmessage_broadcast_to_parents(msg, mouse_widget);

    jmanager_send_message(msg);
    jmessage_free(msg);
  }

  /* mouse clicks */
  if ((jmouse_b(0) != jmouse_b(1)) &&
      ((!jmouse_b(0)) || (!jmouse_b(1)))) {
    /* press and release button messages */
    msg = new_mouse_msg((!jmouse_b(1))? JM_BUTTONPRESSED:
					JM_BUTTONRELEASED);
    /* TODO rigid marshal */

    if (msg->type == JM_BUTTONPRESSED)
      if (jmouse_get_click_level() == JI_CLICK_NOT)
	jmouse_set_click_level(JI_CLICK_START);

    if (capture_widget)
      jmessage_broadcast_to_parents (msg, capture_widget);
    else if (mouse_widget) {
      /* Z-Order:
	 Send the window to top (only when you click in a window
	 that aren't the desktop) */
      if (msg->type == JM_BUTTONPRESSED) {
	JWidget window = jwidget_get_window(mouse_widget);
	JWidget win_manager = window ? jwindow_get_manager(window): NULL;

	if ((window) &&
	    (!jwindow_is_desktop (window)) &&
	    (window != TOPWND(win_manager))) {
	  /* put it in the top of the list */
	  jlist_remove(win_manager->children, window);

	  if (jwindow_is_ontop (window))
	    jlist_prepend(win_manager->children, window);
	  else {
	    int pos = jlist_length(win_manager->children);
	    JI_LIST_FOR_EACH_BACK(win_manager->children, link) {
	      if (jwindow_is_ontop((JWidget)link->data))
		break;
	      pos--;
	    }
	    jlist_insert(win_manager->children, window, pos);
	  }

	  generate_proc_windows_list();
	  jwidget_dirty(window);
	}

	/* put the focus */
	jmanager_set_focus(mouse_widget);
      }

      jmessage_broadcast_to_parents(msg, mouse_widget);
    }
    else {
      jmouse_set_click_level(JI_CLICK_NOT);

      /* maybe someone want catch this lost click (menus use this to
	 down to parents) */
      msg->type = JM_IDLE;

      JI_LIST_FOR_EACH(proc_windows_list, link) {
	window = link->data;
	jmessage_broadcast_to_children(msg, window);
      }
    }

    jmanager_send_message(msg);
    jmessage_free(msg);
  }

  /* double clicks */
  if (jmouse_get_click_level() == JI_CLICK_AGAIN) {
    msg = new_mouse_msg(JM_DOUBLECLICK);
    /* TODO rigid marshal */

    jmouse_set_click_level(JI_CLICK_NOT);

    if (capture_widget)
      jmessage_broadcast_to_parents(msg, capture_widget);

    if (mouse_widget)
      jmessage_broadcast_to_parents(msg, mouse_widget);

    jmanager_send_message(msg);
    jmessage_free(msg);
  }

  /* generate JM_CHAR/JM_KEYPRESSED messages */
  while (keypressed()) {
    int readkey_value = readkey();

    /* char message first */
    msg = new_key_msg(JM_CHAR, readkey_value);
    broadcast_key_msg(manager, msg);

    /* key-pressed message is dependent from char message (if char
       message isn't used, we send the key-pressed message) */
    c = readkey_value >> 8;
    if (old_readed_key[c] != key[c] && !old_readed_key[c]) {
      JMessage sub_msg = new_key_msg(JM_KEYPRESSED, readkey_value);
      old_readed_key[c] = key[c];

      /* same addressee */
      sub_msg->any.widgets = jlist_copy(msg->any.widgets);

      jmessage_set_sub_msg(msg, sub_msg);
      jmessage_free(sub_msg);
    }

    jmanager_send_message (msg);
    jmessage_free (msg);
  }

  /* generate JM_KEYRELEASED messages */
  for (c=0; c<KEY_MAX; c++) {
    if (old_readed_key[c] != key[c] && old_readed_key[c]) {
      /* press/release key interface */
      msg = new_key_msg (JM_KEYRELEASED, (c << 8) | scancode_to_ascii (c));
      old_readed_key[c] = key[c];
      broadcast_key_msg (manager, msg);
      jmanager_send_message (msg);
      jmessage_free (msg);
    }
  }

  /* broadcast the IDLE message */
  msg = new_mouse_msg(JM_IDLE);
  JI_LIST_FOR_EACH(proc_windows_list, link) {
    window = link->data;
    jmessage_broadcast_to_children (msg, window);
  }
  jmessage_add_dest(msg, manager);
  jmanager_send_message(msg);
  jmessage_free(msg);

  /* process messages queue */
  jmanager_dispatch_messages();

  /* redraw dirty widgets */
  jwidget_flush_redraw(manager);
  jmanager_dispatch_messages();

  /* make some OSes happy */
  yield_timeslice();

  /* old windows to close? */
  if (!jlist_empty(old_windows)) {
    bool final_close = FALSE;
    bool break_loop = FALSE;

    JI_LIST_FOR_EACH(old_windows, link) {
      window = link->data;

      if (jwindow_is_desktop(window))
	final_close = TRUE;

      if (jwindow_is_foreground(window))
	break_loop = TRUE;

      destroy_window(window);
    }

    jlist_clear(old_windows);

    if ((!all_windows) && (break_loop))
      ret = FALSE;
  }

  /* destroy the window's list in process */
/*   jlist_free (proc_windows_list); */
/*   proc_windows_list = old_proc_windows_list; */

  /* return */
  return ret;
}

void jmanager_send_message(const JMessage msg)
{
  jlist_append(msg_queue, jmessage_new_copy(msg));
}

void jmanager_dispatch_messages(void)
{
  dispatch_msgs();
}

JWidget jmanager_get_focus(void)
{
  return focus_widget;
}

JWidget jmanager_get_mouse(void)
{
  return mouse_widget;
}

JWidget jmanager_get_capture(void)
{
  return capture_widget;
}

void jmanager_set_focus(JWidget widget)
{
  if ((focus_widget != widget)
      && (!(widget)
	  || (!(widget->flags & JI_DISABLED)
	      && !(widget->flags & JI_HIDDEN)
	      && !(widget->flags & JI_DECORATIVE)
	      && some_parent_is_focusrest (widget)))) {
    JList widget_parents = NULL;
    JWidget common_parent = NULL;
    JLink link, link2;
    JMessage msg;

    if (widget)
      widget_parents = jwidget_get_parents(widget, FALSE);
    else
      widget_parents = jlist_new();

    /* fetch the focus */

    if (focus_widget) {
      JList focus_parents = jwidget_get_parents (focus_widget, TRUE);
      msg = jmessage_new (JM_FOCUSLEAVE);

      JI_LIST_FOR_EACH(focus_parents, link) {
	if (widget) {
	  JI_LIST_FOR_EACH(widget_parents, link2) {
	    if (link->data == link2->data) {
	      common_parent = link->data;
	      break;
	    }
	  }
	  if (common_parent)
	    break;
	}

	if (jwidget_has_focus(link->data)) {
	  ((JWidget)link->data)->flags &= ~JI_HASFOCUS;
	  jmessage_add_dest(msg, link->data);
	}
      }

      jmanager_send_message(msg);
      jmessage_free(msg);
      jlist_free(focus_parents);
    }

    /* put the focus */

    focus_widget = widget;

    if (widget) {
      if (common_parent)
	link = jlist_find(widget_parents, common_parent)->next;
      else
	link = jlist_first(widget_parents);

      msg = jmessage_new (JM_FOCUSENTER);

      for (; link != widget_parents->end; link=link->next) {
	JWidget w = (JWidget)link->data;

	if (w->flags & JI_FOCUSREST) {
	  w->flags |= JI_HASFOCUS;

	  jmessage_add_dest(msg, w);
	}
      }

      jmanager_send_message (msg);
      jmessage_free (msg);
    }

    jlist_free(widget_parents);
  }
}

void jmanager_set_mouse(JWidget widget)
{
  if ((mouse_widget != widget)
      && (!capture_widget || !(capture_widget->flags & JI_HARDCAPTURE))) {
    JList widget_parents = NULL;
    JWidget common_parent = NULL;
    JLink link, link2;
    JMessage msg;

    if (widget)
      widget_parents = jwidget_get_parents(widget, FALSE);
    else
      widget_parents = jlist_new();

    /* fetch the mouse */

    if (mouse_widget) {
      JList mouse_parents = jwidget_get_parents(mouse_widget, TRUE);
      msg = jmessage_new(JM_MOUSELEAVE);

      JI_LIST_FOR_EACH(mouse_parents, link) {
	if (widget) {
	  JI_LIST_FOR_EACH(widget_parents, link2) {
	    if (link->data == link2->data) {
	      common_parent = link->data;
	      break;
	    }
	  }
	  if (common_parent)
	    break;
	}

	if (jwidget_has_mouse(link->data)) {
	  ((JWidget)link->data)->flags &= ~JI_HASMOUSE;
	  jmessage_add_dest(msg, link->data);
	}
      }

      jmanager_send_message(msg);
      jmessage_free(msg);
      jlist_free(mouse_parents);
    }

    /* put the mouse */

    mouse_widget = widget;

    if (widget) {
      if (common_parent)
	link = jlist_find(widget_parents, common_parent)->next;
      else
	link = jlist_first(widget_parents);

      msg = jmessage_new(JM_MOUSEENTER);

      for (; link != widget_parents->end; link=link->next) {
	((JWidget)link->data)->flags |= JI_HASMOUSE;
	jmessage_add_dest(msg, link->data);
      }

      jmanager_send_message(msg);
      jmessage_free(msg);
    }

    jlist_free(widget_parents);
  }
}

void jmanager_set_capture(JWidget widget)
{
  widget->flags |= JI_HASCAPTURE;
  capture_widget = widget;
}

/* sets the focus to the "magnetic" widget inside the window */
void jmanager_attract_focus(JWidget widget)
{
  /* get the magnetic widget */
  JWidget magnet = find_magnetic_widget(jwidget_get_window(widget));

  /* if magnetic widget exists and it doesn't have the focus */
  if (magnet && !jwidget_has_focus(magnet))
    jmanager_set_focus(magnet);
}

void jmanager_focus_first_child(JWidget widget)
{
  JWidget it;

  for (it=jwidget_get_window(widget); it; it=next_widget(it)) {
    if (ACCEPT_FOCUS(it) && !(childs_accept_focus(it, TRUE))) {
      jmanager_set_focus(it);
      break;
    }
  }
}

void jmanager_free_focus(void)
{
  jmanager_set_focus(NULL);
}

void jmanager_free_mouse(void)
{
  jmanager_set_mouse(NULL);
}

void jmanager_free_capture(void)
{
  if (capture_widget) {
    capture_widget->flags &= ~JI_HASCAPTURE;
    capture_widget = NULL;
  }
}

void jmanager_free_widget(JWidget widget)
{
  /* break any relationship with the GUI manager */
  if (jwidget_has_capture(widget))
    jmanager_free_capture();

  if (jwidget_has_mouse(widget))
    jmanager_free_mouse();

  if (jwidget_has_focus(widget) || (widget == focus_widget))
    jmanager_free_focus();
}

void jmanager_remove_message(JMessage msg)
{
  jlist_remove(msg_queue, msg);
}

void jmanager_remove_messages_for(JWidget widget)
{
  JLink link;
  JI_LIST_FOR_EACH(msg_queue, link)
    remove_msgs_for(widget, link->data);
}

void jmanager_refresh_screen(void)
{
  if (default_manager)
    jwidget_invalidate(default_manager);
}

/* configures the window for begin the loop */
void _jmanager_open_window(JWidget manager, JWidget window)
{
  JMessage msg;

  /* free all widgets of special states */
  jmanager_free_capture();
  jmanager_free_mouse();
  jmanager_free_focus();

  /* add the window to manager */
  jlist_prepend(manager->children, window);
  window->parent = manager;

  /* append signal */
  jwidget_emit_signal(manager, JI_SIGNAL_MANAGER_ADD_WINDOW);

  /* broadcast the open message */
  msg = jmessage_new(JM_OPEN);
  jmessage_broadcast_to_children(msg, window);
  jmanager_send_message(msg);
  jmessage_free(msg);

  /* update the new windows list to show */
  jlist_append(new_windows, window);
}

void _jmanager_close_window(JWidget manager, JWidget window,
			    bool sendtokill, bool redraw_background)
{
  JMessage msg;
  JRegion reg1;

  if (!jwidget_has_child(manager, window))
    return;

  if (redraw_background)
    reg1 = jwidget_get_region(window);
  else
    reg1 = NULL;

/*   jmanager_dispatch_messages (); /\* TODO WARNING!!! *\/ */

  /* printf ("			%d CLOSED BY MANAGER\n", window_id); */

  /* close all windows to this desktop */
  if (jwindow_is_desktop(window)) {
    JLink link, next;

    JI_LIST_FOR_EACH_SAFE(manager->children, link, next) {
      if (link->data == window)
	break;
      else {
	JRegion reg2 = jwidget_get_region(window);
	jregion_union(reg1, reg1, reg2);
	jregion_free(reg2);

	_jmanager_close_window(manager, link->data, sendtokill, FALSE);
      }
    }
  }

  /* free all widgets of special states */
  jmanager_free_capture();
  jmanager_free_mouse();

  jmanager_set_focus(manager);
  /* jmanager_free_focus(); */

  /* hide window */
  jwidget_hide(window);

/*   jmanager_dispatch_messages(); /\* TODO WARNING!!! *\/ */

  /* close message */
  msg = jmessage_new(JM_CLOSE);
  jmessage_broadcast_to_children(msg, window);
  jmanager_send_message(msg);
  jmessage_free(msg);

  jmanager_dispatch_messages(); /* TODO WARNING!!! */

  /* update manager list stuff */
  jlist_remove(manager->children, window);
  window->parent = NULL;
  generate_proc_windows_list();

  /* remove signal */
  jwidget_emit_signal(manager, JI_SIGNAL_MANAGER_REMOVE_WINDOW);

  /* redraw background */
  if (reg1) {
    jwidget_invalidate_region(manager, reg1);
    jregion_free(reg1);
  }

  /* update the list of windows to close */
  if (sendtokill) {
    jlist_remove(new_windows, window); /* maybe the window is in the "new_windows" list */
    jlist_append(old_windows, window);
  }
}


/**********************************************************************
				Manager
 **********************************************************************/

static bool manager_msg_proc(JWidget widget, JMessage msg)
{
  switch (msg->type) {

    case JM_REQSIZE:
      manager_request_size(widget, &msg->reqsize.w, &msg->reqsize.h);
      return TRUE;

    case JM_SETPOS:
      manager_set_position(widget, &msg->setpos.rect);
      return TRUE;

    case JM_DRAWRGN:
      manager_redraw_region(widget, msg->drawrgn.region);
      return TRUE;

    case JM_DRAW:
      jdraw_rectfill(&msg->draw.rect, widget->theme->desktop_color);
      return TRUE;

    case JM_CHAR:
#if 0				/* TODO do this */
      /* close desktop? */
      if (msg->key.scancode == KEY_ESC) {
	JWidget window;
	JLink link;

	JI_LIST_FOR_EACH(widget->children, link) {
	  window = link->data;
	  if (jwindow_is_desktop(window)) {
	    jwindow_close(window, widget);
	    return TRUE;
	  }
	}
      }
#endif
      /* check the focus movement */
      return move_focus(widget, msg);
  }

  return FALSE;
}

static void manager_request_size(JWidget widget, int *w, int *h)
{
  if (!widget->parent) {	/* hasn't parent? */
    *w = jrect_w(widget->rc);
    *h = jrect_h(widget->rc);
  }
  else {
    JRect cpos, pos = jwidget_get_child_rect(widget->parent);
    JLink link;

    JI_LIST_FOR_EACH(widget->children, link) {
      cpos = jwidget_get_rect(link->data);
      jrect_union(pos, cpos);
      jrect_free(cpos);
    }

    *w = jrect_w(pos);
    *h = jrect_h(pos);
    jrect_free(pos);
  }
}

static void manager_set_position(JWidget widget, JRect rect)
{
  JRect cpos, old_pos;
  JWidget child;
  JLink link;
  int x, y;

  old_pos = jrect_new_copy(widget->rc);
  jrect_copy(widget->rc, rect);

  /* offset for all windows */
  x = widget->rc->x1 - old_pos->x1;
  y = widget->rc->y1 - old_pos->y1;

  JI_LIST_FOR_EACH(widget->children, link) {
    child = (JWidget)link->data;

    cpos = jwidget_get_rect(child);
    jrect_displace(cpos, x, y);
    jwidget_set_rect(child, cpos);
    jrect_free(cpos);
  }

  jrect_free(old_pos);
}

static void manager_redraw_region(JWidget widget, JRegion region)
{
  JWidget window;
  JRegion reg1 = jregion_new(NULL, 0);
  JRegion reg2 = jregion_new(widget->rc, 0);
  JRegion reg3;
  JLink link;

  /* TODO intersect with jwidget_get_drawable_region ()??? */
  jregion_intersect(reg1, region, reg2);

  /* redraw windows from top to background */
  JI_LIST_FOR_EACH(widget->children, link) {
    window = (JWidget)link->data;

    /* redraw this window */
    jwidget_redraw_region(window, reg1);

    /* there is desktop? */
    if (jwindow_is_desktop(window))
      break;					/* work done */

    /* clip this window area for the next window */
    reg3 = jwidget_get_region(window);
    jregion_copy(reg2, reg1);
    jregion_subtract(reg1, reg2, reg3);
    jregion_free(reg3);
  }

  /* clean areas outside windows (only for non-desktop enviroments) */
  if (link == widget->children->end) {
    JMessage msg = jmessage_new(JM_DRAWRGN);
    msg->drawrgn.region = reg1;
    jwidget_send_message_after_type(widget, msg, JI_MANAGER);
    jmessage_free(msg);
  }

  jregion_free(reg1);
  jregion_free(reg2);
}


/**********************************************************************
			Internal routines
 **********************************************************************/

static void dispatch_msgs(void)
{
  JMessage msg, first_msg;
  JLink link, link2, next;
  JWidget widget;
  bool done;
#ifdef LIMIT_DISPATCH_TIME
  int t = ji_clock;
#endif

  link = jlist_first(msg_queue);
  while (link != msg_queue->end) {
    msg = link->data;

#ifdef LIMIT_DISPATCH_TIME
    if (ji_clock-t > JI_TICKS_PER_SEC/4)
      break;
#endif

    /* go to next message */
    if (msg->any.used) {
      link = link->next;
      continue;
    }

    /* this message is in use */
    msg->any.used = TRUE;
    first_msg = msg;

    done = FALSE;
    do {
      JI_LIST_FOR_EACH(msg->any.widgets, link2) {
	widget = link2->data;

#ifdef REPORT_EVENTS
	if (msg->type != JM_IDLE) { /* don't report IDLE message */
	  static char *msg_name[] = {
	    "Open",
	    "Close",
	    "Destroy",
	    "Draw",
	    "Idle",
	    "Signal",
	    "ReqSize",
	    "SetPos",
	    "WinMove",
	    "DrawRgn",
	    "DirtyChildren",
	    "Char",
	    "KeyPressed",
	    "KeyReleased",
	    "FocusEnter",
	    "FocusLeave",
	    "ButtonPressed",
	    "ButtonReleased",
	    "DoubleClick",
	    "MouseEnter",
	    "MouseLeave",
	    "Motion",
	    "Wheel",
	  };
	  const char *string =
	    (msg->type >= JM_OPEN &&
	     msg->type <= JM_WHEEL) ? msg_name[msg->type]:
				      "Unknown";

	  printf("Event: %s (%d)\n", string, widget->id);
	}
#endif

	/* draw message? */
	if (msg->type == JM_DRAW) {
	  /* hidden? */
	  if (widget->flags & JI_HIDDEN)
	    continue;

	  jmouse_hide();
	  acquire_bitmap(ji_screen);

	  /* set clip */
	  set_clip(ji_screen,
		   msg->draw.rect.x1, msg->draw.rect.y1,
		   msg->draw.rect.x2-1, msg->draw.rect.y2-1);
#ifdef REPORT_EVENTS
	  printf("set_clip (%d, %d, %d, %d)\n",
		 msg->draw.rect.x1, msg->draw.rect.y1,
		 msg->draw.rect.x2-1, msg->draw.rect.y2-1);
#endif
/* 	  	rectfill (ji_screen, 0, 0, JI_SCREEN_W-1, JI_SCREEN_H-1, makecol (255, 0, 0)); */
/* 	  	vsync (); vsync (); vsync (); vsync (); */
	}

	/* call message handler */
	done = jwidget_send_message(widget, msg);

	/* restore clip */
	if (msg->type == JM_DRAW) {
	  set_clip(ji_screen, 0, 0, JI_SCREEN_W-1, JI_SCREEN_H-1);

	  release_bitmap(ji_screen);
	  jmouse_show();
	}

	if (done)		/* TODO use marshal? */
	  break;
      }

      /* done? */
      if (done)			/* TODO use marshal? */
	/* don't go to sub-msg */
	msg = NULL;
      else
	/* use sub-msg */
	msg = msg->any.sub_msg;

    } while (msg);

    /* remove the message from the msg_queue */
    next = link->next;
    jlist_delete_link(msg_queue, link);
    link = next;

    /* destroy the message */
    jmessage_free(first_msg);
  }
}

static void destroy_window (JWidget window)
{
  /* printf ("			%d DESTROYED BY MANAGER\n", window->id); */

  /* delete the window */
  if (jwidget_is_autodestroy (window))
    jwidget_free (window);
}

static void remove_msgs_for(JWidget widget, JMessage msg)
{
  JLink link, next;

  JI_LIST_FOR_EACH_SAFE(msg->any.widgets, link, next) {
    if (link->data == widget) 
      jlist_delete_link(msg->any.widgets, link);
  }

  if (msg->any.sub_msg)
    remove_msgs_for(widget, msg->any.sub_msg);
}

static void generate_proc_windows_list(void)
{
  jlist_clear(proc_windows_list);
  generate_proc_windows_list2(default_manager);
}

static void generate_proc_windows_list2(JWidget widget)
{
  JWidget window;
  JLink link;

  if (widget->type == JI_MANAGER) {
    JI_LIST_FOR_EACH(widget->children, link) {
      window = link->data;
      jlist_append(proc_windows_list, window);
      if (jwindow_is_foreground(window) || jwindow_is_desktop(window))
	break;
    }
  }

  JI_LIST_FOR_EACH(widget->children, link)
    generate_proc_windows_list2(link->data);
}

static int some_parent_is_focusrest (JWidget widget)
{
  if (jwidget_is_focusrest(widget))
    return TRUE;

  if (widget->parent)
    return some_parent_is_focusrest(widget->parent);
  else
    return FALSE;
}

static JWidget find_magnetic_widget(JWidget widget)
{
  JWidget found;
  JLink link;

  JI_LIST_FOR_EACH(widget->children, link) {
    found = find_magnetic_widget(link->data);
    if (found)
      return found;
  }

  if (jwidget_is_magnetic(widget))
    return widget;
  else
    return NULL;
}

static JMessage new_mouse_msg(int type)
{
  JMessage msg = jmessage_new(type);

  msg->mouse.x = jmouse_x(0);
  msg->mouse.y = jmouse_y(0);
  msg->mouse.flags =
    type == JM_DOUBLECLICK    ? jmouse_get_click_button():
    type == JM_BUTTONRELEASED ? jmouse_b(1):
				jmouse_b(0);
  msg->mouse.left = msg->mouse.flags & 1 ? TRUE: FALSE;
  msg->mouse.right = msg->mouse.flags & 2 ? TRUE: FALSE;
  msg->mouse.middle = msg->mouse.flags & 4 ? TRUE: FALSE;

  return msg;
}

static JMessage new_key_msg(int type, int readkey_value)
{
  JMessage msg = jmessage_new(type);

  msg->key.scancode = (readkey_value >> 8) & 0xff;
  msg->key.ascii = readkey_value & 0xff;
#if 0
  printf("%i: %i %i [%c]\n", type, msg->key.scancode,
	 msg->key.ascii, msg->key.ascii);
#endif
  return msg;
}

static void broadcast_key_msg(JWidget manager, JMessage msg)
{
  JWidget window;
  JLink link;

  /* send the message to the widget with capture */
  if (capture_widget)
    jmessage_broadcast_to_parents(msg, capture_widget);
  else {
    /* send the msg to the focused widget */
    if (focus_widget)
      jmessage_broadcast_to_parents(msg, focus_widget);

    /* send to top window */
    if (!jlist_empty(manager->children))
      jmessage_broadcast_to_children(msg, TOPWND(manager));

    /* send to more closest desktop (if the window didn't in foreground) */
    JI_LIST_FOR_EACH(manager->children, link) {
      window = (JWidget)link->data;

      if (jwindow_is_foreground(window))
	break;

      if (jwindow_is_desktop(window)) {
	jmessage_broadcast_to_children(msg, window);
	break;
      }
    }

    /* finally, send the message to the manager, it'll know what to do */
    jmessage_add_dest(msg, manager);
  }
}


/***********************************************************************
			    Focus Movement
 ***********************************************************************/

static bool move_focus(JWidget manager, JMessage msg)
{
  int (*cmp)(JWidget, int, int) = NULL;
  JWidget focus = NULL;
  JWidget it, *list;
  bool ret = FALSE;
  JWidget window;
  int c, count;

  /* who have the focus */
  if (focus_widget)
    window = jwidget_get_window(focus_widget);
  else if (!jlist_empty(manager->children))
    window = TOPWND(manager);
  else
    return ret;

  /* how many child-widget want the focus in this widget? */
  count = count_widgets_accept_focus(window);

  /* one at least */
  if (count > 0) {
    /* list of widgets */
    list = jmalloc (sizeof (JWidget) * count);
    if (!list)
      return ret;

    c = 0;

    /* list's 1st element is the focused widget */
    for (it=focus_widget; it; it=next_widget(it)) {
      if (ACCEPT_FOCUS(it) && !(childs_accept_focus(it, TRUE)))
	list[c++] = it;
    }

    for (it=window; it != focus_widget; it=next_widget(it)) {
      if (ACCEPT_FOCUS(it) && !(childs_accept_focus(it, TRUE)))
	list[c++] = it;
    }

    /* depend of the pressed key */
    switch (msg->key.scancode) {

      case KEY_TAB:
        /* reverse tab */
        if (msg->any.shifts & (KB_SHIFT_FLAG | KB_CTRL_FLAG | KB_ALT_FLAG)) {
          focus = list[count-1];
        }
        /* normal tab */
        else if (count > 1) {
          focus = list[1];
        }
        ret = TRUE;
        break;

      /* arrow keys */
      case KEY_LEFT:  if (!cmp) cmp = cmp_left;
      case KEY_RIGHT: if (!cmp) cmp = cmp_right;
      case KEY_UP:    if (!cmp) cmp = cmp_up;
      case KEY_DOWN:  if (!cmp) cmp = cmp_down;

        /* more than one widget */
        if (count > 1) {
          int i, j, x, y;

          /* position where the focus come */
          x = ((focus_widget) ? focus_widget->rc->x1+focus_widget->rc->x2:
				window->rc->x1+window->rc->x2)
	    / 2;
          y = ((focus_widget) ? focus_widget->rc->y1+focus_widget->rc->y2:
				window->rc->y1+window->rc->y2)
	    / 2;

          c = focus_widget ? 1: 0;

          /* rearrange the list */
          for (i=c; i<count-1; i++) {
            for (j=i+1; j<count; j++) {
              /* sort the list in ascending order */
              if ((*cmp) (list[i], x, y) > (*cmp) (list[j], x, y)) {
		JWidget tmp = list[i];
                list[i] = list[j];
                list[j] = tmp;
              }
            }
          }

          /* check if the new widget to put the focus is not
             in the wrong way */
          if ((*cmp) (list[c], x, y) < INT_MAX)
            focus = list[c];
        }
        /* if only there are one widget, put the focus in this */
        else
          focus = list[0];

        ret = TRUE;
        break;
    }

    jfree (list);

    if ((focus) && (focus != focus_widget))
      jmanager_set_focus (focus);
  }

  return ret;
}

static int count_widgets_accept_focus(JWidget widget)
{
  int count = 0;
  JLink link;

  JI_LIST_FOR_EACH(widget->children, link)
    count += count_widgets_accept_focus(link->data);

  if ((count == 0) && (ACCEPT_FOCUS(widget)))
    count++;

  return count;
}

static bool childs_accept_focus(JWidget widget, bool first)
{
  JLink link;

  JI_LIST_FOR_EACH(widget->children, link)
    if (childs_accept_focus(link->data, FALSE))
      return TRUE;

  return first ? FALSE: ACCEPT_FOCUS(widget);
}

static JWidget next_widget(JWidget widget)
{
  if (!jlist_empty(widget->children))
    return jlist_first(widget->children)->data;

  while (widget->parent->type != JI_MANAGER) {
    JLink link = jlist_find(widget->parent->children, widget);
    if (link->next != widget->parent->children->end)
      return link->next->data;
    else
      widget = widget->parent;
  }

  return NULL;
}

static int cmp_left(JWidget widget, int x, int y)
{
  int z = x - (widget->rc->x1+widget->rc->x2)/2;
  if (z <= 0)
    return INT_MAX;
  return z + ABS((widget->rc->y1+widget->rc->y2)/2 - y) * 8;
}

static int cmp_right(JWidget widget, int x, int y)
{
  int z = (widget->rc->x1+widget->rc->x2)/2 - x;
  if (z <= 0)
    return INT_MAX;
  return z + ABS((widget->rc->y1+widget->rc->y2)/2 - y) * 8;
}

static int cmp_up(JWidget widget, int x, int y)
{
  int z = y - (widget->rc->y1+widget->rc->y2)/2;
  if (z <= 0)
    return INT_MAX;
  return z + ABS((widget->rc->x1+widget->rc->x2)/2 - x) * 8;
}

static int cmp_down(JWidget widget, int x, int y)
{
  int z = (widget->rc->y1+widget->rc->y2)/2 - y;
  if (z <= 0)
    return INT_MAX;
  return z + ABS((widget->rc->x1+widget->rc->x2)/2 - x) * 8;
}
