/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Hugo Vincent.
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <stdio.h>
#include "SoundManager.h"

namespace OpenGC {

SoundManager::SoundManager() {}
SoundManager::~SoundManager() {}

void SoundManager::PlaySound(sound_t snd)
{
	printf("FIXME: play sound %d\n", snd);
}

} // end namespace OpenGC


