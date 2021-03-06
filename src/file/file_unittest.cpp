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

#include "tests/test.h"

#include "app.h"
#include "document.h"
#include "file/file.h"
#include "file/file_formats_manager.h"
#include "raster/raster.h"
#include "she/she.h"

#include <cstdio>
#include <cstdlib>
#include <vector>

TEST(File, SeveralSizes)
{
  she::ScopedHandle<she::System> system(she::CreateSystem());
  // Register all possible image formats.
  FileFormatsManager::instance().registerAllFormats();
  std::vector<char> fn(256);

  for (int w=10; w<=10+503*2; w+=503) {
    for (int h=10; h<=10+503*2; h+=503) {
      //std::sprintf(&fn[0], "test_%dx%d.ase", w, h);
      std::sprintf(&fn[0], "test.ase");

      {
        UniquePtr<Document> doc(Document::createBasicDocument(IMAGE_INDEXED, w, h, 256));
        doc->setFilename(&fn[0]);

        // Random pixels
        Image* image = doc->getSprite()->getCurrentImage();
        std::srand(w*h);
        int c = std::rand()%256;
        for (int y=0; y<h; y++) {
          for (int x=0; x<w; x++) {
            image_putpixel_fast<IndexedTraits>(image, x, y, c);
            if ((std::rand()&4) == 0)
              c = std::rand()%256;
          }
        }

        save_document(doc);
      }

      {
        UniquePtr<Document> doc(load_document(&fn[0]));
        ASSERT_EQ(w, doc->getSprite()->getWidth());
        ASSERT_EQ(h, doc->getSprite()->getHeight());

        // Same random pixels (see the seed)
        Image* image = doc->getSprite()->getCurrentImage();
        std::srand(w*h);
        int c = std::rand()%256;
        for (int y=0; y<h; y++) {
          for (int x=0; x<w; x++) {
            ASSERT_EQ(c, image_getpixel_fast<IndexedTraits>(image, x, y));
            if ((std::rand()&4) == 0)
              c = std::rand()%256;
          }
        }
      }
    }
  }
}
