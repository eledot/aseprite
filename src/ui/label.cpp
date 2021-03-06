// ASEPRITE gui library
// Copyright (C) 2001-2012  David Capello
//
// This source file is distributed under a BSD-like license, please
// read LICENSE.txt for more information.

#include "config.h"

#include "ui/label.h"
#include "ui/message.h"
#include "ui/theme.h"

namespace ui {

Label::Label(const char *text)
  : Widget(JI_LABEL)
{
  setAlign(JI_LEFT | JI_MIDDLE);
  setText(text);
  initTheme();

  m_textColor = ji_color_foreground();
}

int Label::getTextColor() const
{
  return m_textColor;
}

void Label::setTextColor(int color)
{
  m_textColor = color;
}

bool Label::onProcessMessage(Message* msg)
{
  switch (msg->type) {

    case JM_REQSIZE:
      if (this->hasText()) {
        msg->reqsize.w = jwidget_get_text_length(this);
        msg->reqsize.h = jwidget_get_text_height(this);
      }
      else
        msg->reqsize.w = msg->reqsize.h = 0;

      msg->reqsize.w += this->border_width.l + this->border_width.r;
      msg->reqsize.h += this->border_width.t + this->border_width.b;
      return true;

  }

  return Widget::onProcessMessage(msg);
}

void Label::onPaint(PaintEvent& ev)
{
  getTheme()->paintLabel(ev);
}

} // namespace ui
