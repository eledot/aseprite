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

#include "config.h"

#include "app.h"
#include "commands/command.h"
#include "document_wrappers.h"
#include "skin/skin_theme.h"
#include "ui/system.h"
#include "ui/theme.h"
#include "widgets/status_bar.h"

#include <allegro.h>

#if defined ALLEGRO_WINDOWS && defined DEBUGMODE
  #include <winalleg.h>
  #include <psapi.h>
#endif

//////////////////////////////////////////////////////////////////////
// refresh

class RefreshCommand : public Command
{
public:
  RefreshCommand();
  Command* clone() { return new RefreshCommand(*this); }

protected:
  void onExecute(Context* context);
};

RefreshCommand::RefreshCommand()
  : Command("Refresh",
            "Refresh",
            CmdUIOnlyFlag)
{
}

void RefreshCommand::onExecute(Context* context)
{
  ui::jmouse_hide();
  clear_to_color(screen, makecol(0, 0, 0));
  ui::jmouse_show();

  // Reload skin
  {
    SkinTheme* theme = (SkinTheme*)ui::CurrentTheme::get();
    theme->reload_skin();
    theme->regenerate();
  }

  {
    const ActiveDocumentReader document(context);
    app_refresh_screen(document);
  }

  // Print memory information
#if defined ALLEGRO_WINDOWS && defined DEBUGMODE
  {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
      StatusBar::instance()
        ->showTip(1000,
                  "Current memory: %.16g KB (%lu)\n"
                  "Peak of memory: %.16g KB (%lu)",
                  pmc.WorkingSetSize / 1024.0, pmc.WorkingSetSize,
                  pmc.PeakWorkingSetSize / 1024.0, pmc.PeakWorkingSetSize);
    }
  }
#endif
}

//////////////////////////////////////////////////////////////////////
// CommandFactory

Command* CommandFactory::createRefreshCommand()
{
  return new RefreshCommand;
}
