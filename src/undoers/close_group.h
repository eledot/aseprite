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

#ifndef UNDOERS_CLOSE_GROUP_H_INCLUDED
#define UNDOERS_CLOSE_GROUP_H_INCLUDED

#include "base/compiler_specific.h"
#include "raster/sprite_position.h"
#include "undo/object_id.h"
#include "undo/undoer.h"

class Sprite;

namespace undoers {

class CloseGroup : public undo::Undoer
{
public:
  CloseGroup(undo::ObjectsContainer* objects,
             const char* label,
             undo::Modification modification,
             Sprite* sprite);

  void dispose() OVERRIDE;
  size_t getMemSize() const OVERRIDE { return sizeof(*this); }
  undo::Modification getModification() const { return m_modification; }
  bool isOpenGroup() const OVERRIDE { return false; }
  bool isCloseGroup() const OVERRIDE { return true; }
  void revert(undo::ObjectsContainer* objects, undo::UndoersCollector* redoers) OVERRIDE;

  const char* getLabel() { return m_label; }
  const SpritePosition& getSpritePosition() { return m_spritePosition; }

private:
  const char* m_label;
  undo::Modification m_modification;
  undo::ObjectId m_spriteId;
  SpritePosition m_spritePosition;
};

} // namespace undoers

#endif  // UNDOERS_CLOSE_GROUP_H_INCLUDED
