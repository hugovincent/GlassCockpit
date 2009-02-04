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

/**
 * Sound manager class - used for playing warning sounds. 
 * May add text-to-speech annunciations in the future.
 */

#ifndef SoundManager_h
#define SoundManager_h

namespace OpenGC {

typedef enum {
	SND_WARNING,
	SND_ALERT
} sound_t;
	
class SoundManager
{
	public:

		SoundManager();
		~SoundManager();

		void PlaySound(sound_t snd);
};


} // end namespace OpenGC

#endif

