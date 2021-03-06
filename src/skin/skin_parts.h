/* ASEPRITE
 * Copyright (C) 2001-2012  David Capello
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef SKIN_PARTS_H_INCLUDED
#define SKIN_PARTS_H_INCLUDED

// Available parts in the skin sheet
enum {

  PART_RADIO_NORMAL,
  PART_RADIO_SELECTED,
  PART_RADIO_DISABLED,

  PART_CHECK_NORMAL,
  PART_CHECK_SELECTED,
  PART_CHECK_DISABLED,

  PART_CHECK_FOCUS_NW,
  PART_CHECK_FOCUS_N,
  PART_CHECK_FOCUS_NE,
  PART_CHECK_FOCUS_E,
  PART_CHECK_FOCUS_SE,
  PART_CHECK_FOCUS_S,
  PART_CHECK_FOCUS_SW,
  PART_CHECK_FOCUS_W,

  PART_RADIO_FOCUS_NW,
  PART_RADIO_FOCUS_N,
  PART_RADIO_FOCUS_NE,
  PART_RADIO_FOCUS_E,
  PART_RADIO_FOCUS_SE,
  PART_RADIO_FOCUS_S,
  PART_RADIO_FOCUS_SW,
  PART_RADIO_FOCUS_W,

  PART_BUTTON_NORMAL_NW,
  PART_BUTTON_NORMAL_N,
  PART_BUTTON_NORMAL_NE,
  PART_BUTTON_NORMAL_E,
  PART_BUTTON_NORMAL_SE,
  PART_BUTTON_NORMAL_S,
  PART_BUTTON_NORMAL_SW,
  PART_BUTTON_NORMAL_W,

  PART_BUTTON_HOT_NW,
  PART_BUTTON_HOT_N,
  PART_BUTTON_HOT_NE,
  PART_BUTTON_HOT_E,
  PART_BUTTON_HOT_SE,
  PART_BUTTON_HOT_S,
  PART_BUTTON_HOT_SW,
  PART_BUTTON_HOT_W,

  PART_BUTTON_FOCUSED_NW,
  PART_BUTTON_FOCUSED_N,
  PART_BUTTON_FOCUSED_NE,
  PART_BUTTON_FOCUSED_E,
  PART_BUTTON_FOCUSED_SE,
  PART_BUTTON_FOCUSED_S,
  PART_BUTTON_FOCUSED_SW,
  PART_BUTTON_FOCUSED_W,

  PART_BUTTON_SELECTED_NW,
  PART_BUTTON_SELECTED_N,
  PART_BUTTON_SELECTED_NE,
  PART_BUTTON_SELECTED_E,
  PART_BUTTON_SELECTED_SE,
  PART_BUTTON_SELECTED_S,
  PART_BUTTON_SELECTED_SW,
  PART_BUTTON_SELECTED_W,

  PART_SUNKEN_NORMAL_NW,
  PART_SUNKEN_NORMAL_N,
  PART_SUNKEN_NORMAL_NE,
  PART_SUNKEN_NORMAL_E,
  PART_SUNKEN_NORMAL_SE,
  PART_SUNKEN_NORMAL_S,
  PART_SUNKEN_NORMAL_SW,
  PART_SUNKEN_NORMAL_W,

  PART_SUNKEN_FOCUSED_NW,
  PART_SUNKEN_FOCUSED_N,
  PART_SUNKEN_FOCUSED_NE,
  PART_SUNKEN_FOCUSED_E,
  PART_SUNKEN_FOCUSED_SE,
  PART_SUNKEN_FOCUSED_S,
  PART_SUNKEN_FOCUSED_SW,
  PART_SUNKEN_FOCUSED_W,

  PART_SUNKEN2_NORMAL_NW,
  PART_SUNKEN2_NORMAL_N,
  PART_SUNKEN2_NORMAL_NE,
  PART_SUNKEN2_NORMAL_E,
  PART_SUNKEN2_NORMAL_SE,
  PART_SUNKEN2_NORMAL_S,
  PART_SUNKEN2_NORMAL_SW,
  PART_SUNKEN2_NORMAL_W,

  PART_SUNKEN2_FOCUSED_NW,
  PART_SUNKEN2_FOCUSED_N,
  PART_SUNKEN2_FOCUSED_NE,
  PART_SUNKEN2_FOCUSED_E,
  PART_SUNKEN2_FOCUSED_SE,
  PART_SUNKEN2_FOCUSED_S,
  PART_SUNKEN2_FOCUSED_SW,
  PART_SUNKEN2_FOCUSED_W,

  PART_SUNKEN_MINI_NORMAL_NW,
  PART_SUNKEN_MINI_NORMAL_N,
  PART_SUNKEN_MINI_NORMAL_NE,
  PART_SUNKEN_MINI_NORMAL_E,
  PART_SUNKEN_MINI_NORMAL_SE,
  PART_SUNKEN_MINI_NORMAL_S,
  PART_SUNKEN_MINI_NORMAL_SW,
  PART_SUNKEN_MINI_NORMAL_W,

  PART_SUNKEN_MINI_FOCUSED_NW,
  PART_SUNKEN_MINI_FOCUSED_N,
  PART_SUNKEN_MINI_FOCUSED_NE,
  PART_SUNKEN_MINI_FOCUSED_E,
  PART_SUNKEN_MINI_FOCUSED_SE,
  PART_SUNKEN_MINI_FOCUSED_S,
  PART_SUNKEN_MINI_FOCUSED_SW,
  PART_SUNKEN_MINI_FOCUSED_W,

  PART_WINDOW_NW,
  PART_WINDOW_N,
  PART_WINDOW_NE,
  PART_WINDOW_E,
  PART_WINDOW_SE,
  PART_WINDOW_S,
  PART_WINDOW_SW,
  PART_WINDOW_W,

  PART_MENU_NW,
  PART_MENU_N,
  PART_MENU_NE,
  PART_MENU_E,
  PART_MENU_SE,
  PART_MENU_S,
  PART_MENU_SW,
  PART_MENU_W,

  PART_WINDOW_CLOSE_BUTTON_NORMAL,
  PART_WINDOW_CLOSE_BUTTON_HOT,
  PART_WINDOW_CLOSE_BUTTON_SELECTED,

  PART_SLIDER_FULL_NW,
  PART_SLIDER_FULL_N,
  PART_SLIDER_FULL_NE,
  PART_SLIDER_FULL_E,
  PART_SLIDER_FULL_SE,
  PART_SLIDER_FULL_S,
  PART_SLIDER_FULL_SW,
  PART_SLIDER_FULL_W,

  PART_SLIDER_EMPTY_NW,
  PART_SLIDER_EMPTY_N,
  PART_SLIDER_EMPTY_NE,
  PART_SLIDER_EMPTY_E,
  PART_SLIDER_EMPTY_SE,
  PART_SLIDER_EMPTY_S,
  PART_SLIDER_EMPTY_SW,
  PART_SLIDER_EMPTY_W,

  PART_SLIDER_FULL_FOCUSED_NW,
  PART_SLIDER_FULL_FOCUSED_N,
  PART_SLIDER_FULL_FOCUSED_NE,
  PART_SLIDER_FULL_FOCUSED_E,
  PART_SLIDER_FULL_FOCUSED_SE,
  PART_SLIDER_FULL_FOCUSED_S,
  PART_SLIDER_FULL_FOCUSED_SW,
  PART_SLIDER_FULL_FOCUSED_W,

  PART_SLIDER_EMPTY_FOCUSED_NW,
  PART_SLIDER_EMPTY_FOCUSED_N,
  PART_SLIDER_EMPTY_FOCUSED_NE,
  PART_SLIDER_EMPTY_FOCUSED_E,
  PART_SLIDER_EMPTY_FOCUSED_SE,
  PART_SLIDER_EMPTY_FOCUSED_S,
  PART_SLIDER_EMPTY_FOCUSED_SW,
  PART_SLIDER_EMPTY_FOCUSED_W,

  PART_MINI_SLIDER_FULL_NW,
  PART_MINI_SLIDER_FULL_N,
  PART_MINI_SLIDER_FULL_NE,
  PART_MINI_SLIDER_FULL_E,
  PART_MINI_SLIDER_FULL_SE,
  PART_MINI_SLIDER_FULL_S,
  PART_MINI_SLIDER_FULL_SW,
  PART_MINI_SLIDER_FULL_W,

  PART_MINI_SLIDER_EMPTY_NW,
  PART_MINI_SLIDER_EMPTY_N,
  PART_MINI_SLIDER_EMPTY_NE,
  PART_MINI_SLIDER_EMPTY_E,
  PART_MINI_SLIDER_EMPTY_SE,
  PART_MINI_SLIDER_EMPTY_S,
  PART_MINI_SLIDER_EMPTY_SW,
  PART_MINI_SLIDER_EMPTY_W,

  PART_MINI_SLIDER_FULL_FOCUSED_NW,
  PART_MINI_SLIDER_FULL_FOCUSED_N,
  PART_MINI_SLIDER_FULL_FOCUSED_NE,
  PART_MINI_SLIDER_FULL_FOCUSED_E,
  PART_MINI_SLIDER_FULL_FOCUSED_SE,
  PART_MINI_SLIDER_FULL_FOCUSED_S,
  PART_MINI_SLIDER_FULL_FOCUSED_SW,
  PART_MINI_SLIDER_FULL_FOCUSED_W,

  PART_MINI_SLIDER_EMPTY_FOCUSED_NW,
  PART_MINI_SLIDER_EMPTY_FOCUSED_N,
  PART_MINI_SLIDER_EMPTY_FOCUSED_NE,
  PART_MINI_SLIDER_EMPTY_FOCUSED_E,
  PART_MINI_SLIDER_EMPTY_FOCUSED_SE,
  PART_MINI_SLIDER_EMPTY_FOCUSED_S,
  PART_MINI_SLIDER_EMPTY_FOCUSED_SW,
  PART_MINI_SLIDER_EMPTY_FOCUSED_W,

  PART_MINI_SLIDER_THUMB,
  PART_MINI_SLIDER_THUMB_FOCUSED,

  PART_SEPARATOR_HORZ,
  PART_SEPARATOR_VERT,

  PART_COMBOBOX_ARROW_DOWN,
  PART_COMBOBOX_ARROW_DOWN_SELECTED,
  PART_COMBOBOX_ARROW_DOWN_DISABLED,
  PART_COMBOBOX_ARROW_UP,
  PART_COMBOBOX_ARROW_UP_SELECTED,
  PART_COMBOBOX_ARROW_UP_DISABLED,
  PART_COMBOBOX_ARROW_LEFT,
  PART_COMBOBOX_ARROW_LEFT_SELECTED,
  PART_COMBOBOX_ARROW_LEFT_DISABLED,
  PART_COMBOBOX_ARROW_RIGHT,
  PART_COMBOBOX_ARROW_RIGHT_SELECTED,
  PART_COMBOBOX_ARROW_RIGHT_DISABLED,

  PART_TOOLBUTTON_NORMAL_NW,
  PART_TOOLBUTTON_NORMAL_N,
  PART_TOOLBUTTON_NORMAL_NE,
  PART_TOOLBUTTON_NORMAL_E,
  PART_TOOLBUTTON_NORMAL_SE,
  PART_TOOLBUTTON_NORMAL_S,
  PART_TOOLBUTTON_NORMAL_SW,
  PART_TOOLBUTTON_NORMAL_W,

  PART_TOOLBUTTON_HOT_NW,
  PART_TOOLBUTTON_HOT_N,
  PART_TOOLBUTTON_HOT_NE,
  PART_TOOLBUTTON_HOT_E,
  PART_TOOLBUTTON_HOT_SE,
  PART_TOOLBUTTON_HOT_S,
  PART_TOOLBUTTON_HOT_SW,
  PART_TOOLBUTTON_HOT_W,

  PART_TOOLBUTTON_LAST_NW,
  PART_TOOLBUTTON_LAST_N,
  PART_TOOLBUTTON_LAST_NE,
  PART_TOOLBUTTON_LAST_E,
  PART_TOOLBUTTON_LAST_SE,
  PART_TOOLBUTTON_LAST_S,
  PART_TOOLBUTTON_LAST_SW,
  PART_TOOLBUTTON_LAST_W,

  PART_TOOLBUTTON_PUSHED_NW,
  PART_TOOLBUTTON_PUSHED_N,
  PART_TOOLBUTTON_PUSHED_NE,
  PART_TOOLBUTTON_PUSHED_E,
  PART_TOOLBUTTON_PUSHED_SE,
  PART_TOOLBUTTON_PUSHED_S,
  PART_TOOLBUTTON_PUSHED_SW,
  PART_TOOLBUTTON_PUSHED_W,

  PART_TAB_NORMAL_NW,
  PART_TAB_NORMAL_N,
  PART_TAB_NORMAL_NE,
  PART_TAB_NORMAL_E,
  PART_TAB_NORMAL_SE,
  PART_TAB_NORMAL_S,
  PART_TAB_NORMAL_SW,
  PART_TAB_NORMAL_W,

  PART_TAB_SELECTED_NW,
  PART_TAB_SELECTED_N,
  PART_TAB_SELECTED_NE,
  PART_TAB_SELECTED_E,
  PART_TAB_SELECTED_SE,
  PART_TAB_SELECTED_S,
  PART_TAB_SELECTED_SW,
  PART_TAB_SELECTED_W,

  PART_TAB_BOTTOM_SELECTED_NW,
  PART_TAB_BOTTOM_SELECTED_N,
  PART_TAB_BOTTOM_SELECTED_NE,
  PART_TAB_BOTTOM_SELECTED_E,
  PART_TAB_BOTTOM_SELECTED_SE,
  PART_TAB_BOTTOM_SELECTED_S,
  PART_TAB_BOTTOM_SELECTED_SW,
  PART_TAB_BOTTOM_SELECTED_W,

  PART_TAB_BOTTOM_NORMAL,

  PART_TAB_FILLER,

  PART_EDITOR_NORMAL_NW,
  PART_EDITOR_NORMAL_N,
  PART_EDITOR_NORMAL_NE,
  PART_EDITOR_NORMAL_E,
  PART_EDITOR_NORMAL_SE,
  PART_EDITOR_NORMAL_S,
  PART_EDITOR_NORMAL_SW,
  PART_EDITOR_NORMAL_W,

  PART_EDITOR_SELECTED_NW,
  PART_EDITOR_SELECTED_N,
  PART_EDITOR_SELECTED_NE,
  PART_EDITOR_SELECTED_E,
  PART_EDITOR_SELECTED_SE,
  PART_EDITOR_SELECTED_S,
  PART_EDITOR_SELECTED_SW,
  PART_EDITOR_SELECTED_W,

  PART_COLORBAR_0_NW,
  PART_COLORBAR_0_N,
  PART_COLORBAR_0_NE,
  PART_COLORBAR_0_E,
  PART_COLORBAR_0_SE,
  PART_COLORBAR_0_S,
  PART_COLORBAR_0_SW,
  PART_COLORBAR_0_W,

  PART_COLORBAR_1_NW,
  PART_COLORBAR_1_N,
  PART_COLORBAR_1_NE,
  PART_COLORBAR_1_E,
  PART_COLORBAR_1_SE,
  PART_COLORBAR_1_S,
  PART_COLORBAR_1_SW,
  PART_COLORBAR_1_W,

  PART_COLORBAR_2_NW,
  PART_COLORBAR_2_N,
  PART_COLORBAR_2_NE,
  PART_COLORBAR_2_E,
  PART_COLORBAR_2_SE,
  PART_COLORBAR_2_S,
  PART_COLORBAR_2_SW,
  PART_COLORBAR_2_W,

  PART_COLORBAR_3_NW,
  PART_COLORBAR_3_N,
  PART_COLORBAR_3_NE,
  PART_COLORBAR_3_E,
  PART_COLORBAR_3_SE,
  PART_COLORBAR_3_S,
  PART_COLORBAR_3_SW,
  PART_COLORBAR_3_W,

  PART_COLORBAR_BORDER_FG_NW,
  PART_COLORBAR_BORDER_FG_N,
  PART_COLORBAR_BORDER_FG_NE,
  PART_COLORBAR_BORDER_FG_E,
  PART_COLORBAR_BORDER_FG_SE,
  PART_COLORBAR_BORDER_FG_S,
  PART_COLORBAR_BORDER_FG_SW,
  PART_COLORBAR_BORDER_FG_W,

  PART_COLORBAR_BORDER_BG_NW,
  PART_COLORBAR_BORDER_BG_N,
  PART_COLORBAR_BORDER_BG_NE,
  PART_COLORBAR_BORDER_BG_E,
  PART_COLORBAR_BORDER_BG_SE,
  PART_COLORBAR_BORDER_BG_S,
  PART_COLORBAR_BORDER_BG_SW,
  PART_COLORBAR_BORDER_BG_W,

  PART_COLORBAR_BORDER_HOTFG_NW,
  PART_COLORBAR_BORDER_HOTFG_N,
  PART_COLORBAR_BORDER_HOTFG_NE,
  PART_COLORBAR_BORDER_HOTFG_E,
  PART_COLORBAR_BORDER_HOTFG_SE,
  PART_COLORBAR_BORDER_HOTFG_S,
  PART_COLORBAR_BORDER_HOTFG_SW,
  PART_COLORBAR_BORDER_HOTFG_W,

  PART_SCROLLBAR_BG_NW,
  PART_SCROLLBAR_BG_N,
  PART_SCROLLBAR_BG_NE,
  PART_SCROLLBAR_BG_E,
  PART_SCROLLBAR_BG_SE,
  PART_SCROLLBAR_BG_S,
  PART_SCROLLBAR_BG_SW,
  PART_SCROLLBAR_BG_W,

  PART_SCROLLBAR_THUMB_NW,
  PART_SCROLLBAR_THUMB_N,
  PART_SCROLLBAR_THUMB_NE,
  PART_SCROLLBAR_THUMB_E,
  PART_SCROLLBAR_THUMB_SE,
  PART_SCROLLBAR_THUMB_S,
  PART_SCROLLBAR_THUMB_SW,
  PART_SCROLLBAR_THUMB_W,

  PART_TOOLTIP_NW,
  PART_TOOLTIP_N,
  PART_TOOLTIP_NE,
  PART_TOOLTIP_E,
  PART_TOOLTIP_SE,
  PART_TOOLTIP_S,
  PART_TOOLTIP_SW,
  PART_TOOLTIP_W,

  PART_TOOLTIP_ARROW_NW,
  PART_TOOLTIP_ARROW_N,
  PART_TOOLTIP_ARROW_NE,
  PART_TOOLTIP_ARROW_E,
  PART_TOOLTIP_ARROW_SE,
  PART_TOOLTIP_ARROW_S,
  PART_TOOLTIP_ARROW_SW,
  PART_TOOLTIP_ARROW_W,

  PART_ANI_FIRST,
  PART_ANI_FIRST_SELECTED,
  PART_ANI_FIRST_DISABLED,
  PART_ANI_PREVIOUS,
  PART_ANI_PREVIOUS_SELECTED,
  PART_ANI_PREVIOUS_DISABLED,
  PART_ANI_PLAY,
  PART_ANI_PLAY_SELECTED,
  PART_ANI_PLAY_DISABLED,
  PART_ANI_NEXT,
  PART_ANI_NEXT_SELECTED,
  PART_ANI_NEXT_DISABLED,
  PART_ANI_LAST,
  PART_ANI_LAST_SELECTED,
  PART_ANI_LAST_DISABLED,

  PART_TARGET_ONE,
  PART_TARGET_ONE_SELECTED,
  PART_TARGET_FRAMES,
  PART_TARGET_FRAMES_SELECTED,
  PART_TARGET_LAYERS,
  PART_TARGET_LAYERS_SELECTED,
  PART_TARGET_FRAMES_LAYERS,
  PART_TARGET_FRAMES_LAYERS_SELECTED,

  PART_BRUSH_CIRCLE,
  PART_BRUSH_CIRCLE_SELECTED,
  PART_BRUSH_SQUARE,
  PART_BRUSH_SQUARE_SELECTED,
  PART_BRUSH_LINE,
  PART_BRUSH_LINE_SELECTED,

  PART_SCALE_ARROW_1,
  PART_SCALE_ARROW_2,
  PART_SCALE_ARROW_3,
  PART_ROTATE_ARROW_1,
  PART_ROTATE_ARROW_2,
  PART_ROTATE_ARROW_3,

  PART_LAYER_VISIBLE,
  PART_LAYER_VISIBLE_SELECTED,
  PART_LAYER_HIDDEN,
  PART_LAYER_HIDDEN_SELECTED,
  PART_LAYER_EDITABLE,
  PART_LAYER_EDITABLE_SELECTED,
  PART_LAYER_LOCKED,
  PART_LAYER_LOCKED_SELECTED,

  PART_UNPINNED,
  PART_PINNED,

  PART_DROP_DOWN_BUTTON_LEFT_NORMAL_NW,
  PART_DROP_DOWN_BUTTON_LEFT_NORMAL_N,
  PART_DROP_DOWN_BUTTON_LEFT_NORMAL_NE,
  PART_DROP_DOWN_BUTTON_LEFT_NORMAL_E,
  PART_DROP_DOWN_BUTTON_LEFT_NORMAL_SE,
  PART_DROP_DOWN_BUTTON_LEFT_NORMAL_S,
  PART_DROP_DOWN_BUTTON_LEFT_NORMAL_SW,
  PART_DROP_DOWN_BUTTON_LEFT_NORMAL_W,

  PART_DROP_DOWN_BUTTON_LEFT_HOT_NW,
  PART_DROP_DOWN_BUTTON_LEFT_HOT_N,
  PART_DROP_DOWN_BUTTON_LEFT_HOT_NE,
  PART_DROP_DOWN_BUTTON_LEFT_HOT_E,
  PART_DROP_DOWN_BUTTON_LEFT_HOT_SE,
  PART_DROP_DOWN_BUTTON_LEFT_HOT_S,
  PART_DROP_DOWN_BUTTON_LEFT_HOT_SW,
  PART_DROP_DOWN_BUTTON_LEFT_HOT_W,

  PART_DROP_DOWN_BUTTON_LEFT_FOCUSED_NW,
  PART_DROP_DOWN_BUTTON_LEFT_FOCUSED_N,
  PART_DROP_DOWN_BUTTON_LEFT_FOCUSED_NE,
  PART_DROP_DOWN_BUTTON_LEFT_FOCUSED_E,
  PART_DROP_DOWN_BUTTON_LEFT_FOCUSED_SE,
  PART_DROP_DOWN_BUTTON_LEFT_FOCUSED_S,
  PART_DROP_DOWN_BUTTON_LEFT_FOCUSED_SW,
  PART_DROP_DOWN_BUTTON_LEFT_FOCUSED_W,

  PART_DROP_DOWN_BUTTON_LEFT_SELECTED_NW,
  PART_DROP_DOWN_BUTTON_LEFT_SELECTED_N,
  PART_DROP_DOWN_BUTTON_LEFT_SELECTED_NE,
  PART_DROP_DOWN_BUTTON_LEFT_SELECTED_E,
  PART_DROP_DOWN_BUTTON_LEFT_SELECTED_SE,
  PART_DROP_DOWN_BUTTON_LEFT_SELECTED_S,
  PART_DROP_DOWN_BUTTON_LEFT_SELECTED_SW,
  PART_DROP_DOWN_BUTTON_LEFT_SELECTED_W,

  PART_DROP_DOWN_BUTTON_RIGHT_NORMAL_NW,
  PART_DROP_DOWN_BUTTON_RIGHT_NORMAL_N,
  PART_DROP_DOWN_BUTTON_RIGHT_NORMAL_NE,
  PART_DROP_DOWN_BUTTON_RIGHT_NORMAL_E,
  PART_DROP_DOWN_BUTTON_RIGHT_NORMAL_SE,
  PART_DROP_DOWN_BUTTON_RIGHT_NORMAL_S,
  PART_DROP_DOWN_BUTTON_RIGHT_NORMAL_SW,
  PART_DROP_DOWN_BUTTON_RIGHT_NORMAL_W,

  PART_DROP_DOWN_BUTTON_RIGHT_HOT_NW,
  PART_DROP_DOWN_BUTTON_RIGHT_HOT_N,
  PART_DROP_DOWN_BUTTON_RIGHT_HOT_NE,
  PART_DROP_DOWN_BUTTON_RIGHT_HOT_E,
  PART_DROP_DOWN_BUTTON_RIGHT_HOT_SE,
  PART_DROP_DOWN_BUTTON_RIGHT_HOT_S,
  PART_DROP_DOWN_BUTTON_RIGHT_HOT_SW,
  PART_DROP_DOWN_BUTTON_RIGHT_HOT_W,

  PART_DROP_DOWN_BUTTON_RIGHT_FOCUSED_NW,
  PART_DROP_DOWN_BUTTON_RIGHT_FOCUSED_N,
  PART_DROP_DOWN_BUTTON_RIGHT_FOCUSED_NE,
  PART_DROP_DOWN_BUTTON_RIGHT_FOCUSED_E,
  PART_DROP_DOWN_BUTTON_RIGHT_FOCUSED_SE,
  PART_DROP_DOWN_BUTTON_RIGHT_FOCUSED_S,
  PART_DROP_DOWN_BUTTON_RIGHT_FOCUSED_SW,
  PART_DROP_DOWN_BUTTON_RIGHT_FOCUSED_W,

  PART_DROP_DOWN_BUTTON_RIGHT_SELECTED_NW,
  PART_DROP_DOWN_BUTTON_RIGHT_SELECTED_N,
  PART_DROP_DOWN_BUTTON_RIGHT_SELECTED_NE,
  PART_DROP_DOWN_BUTTON_RIGHT_SELECTED_E,
  PART_DROP_DOWN_BUTTON_RIGHT_SELECTED_SE,
  PART_DROP_DOWN_BUTTON_RIGHT_SELECTED_S,
  PART_DROP_DOWN_BUTTON_RIGHT_SELECTED_SW,
  PART_DROP_DOWN_BUTTON_RIGHT_SELECTED_W,

  PART_TRANSFORMATION_HANDLE,
  PART_PIVOT_HANDLE,

  PARTS
};

#endif
