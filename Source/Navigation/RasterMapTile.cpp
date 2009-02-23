/*
 *  RasterMapTile.cpp
 *  GlassCockpit
 *
 *  Created by Hugo Vincent on 22/02/09.
 *  Copyright 2009 Hugo Vincent. All rights reserved.
 *
 */

#include "RasterMapTile.h"

namespace OpenGC {

RasterMapTile::RasterMapTile(unsigned char *image, unsigned int width, unsigned int height)
	: m_Image(image), m_Width(width), m_Height(height)
{

}

} // end namespace OpenGC
