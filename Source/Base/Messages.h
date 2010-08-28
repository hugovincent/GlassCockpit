/*=========================================================================

  Copyright (c) 2005-2010 Hugo Vincent <hugo.vincent@gmail.com>
  All rights reserved.
  
  This project is distributed under the terms of the GNU General Public License
  Version 3 <http://www.gnu.org/licenses/gpl.html>.
  
      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, specifically version 3 of the License.
  
      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.
  
      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <http://www.gnu.org/licenses/>.

=========================================================================*/

/**
 * Defines messages which can be triggered by mouse clicks, key presses, etc.
 */

#ifndef Messages_h
#define Messages_h

namespace OpenGC
{

enum Message
{
	/** Nav zoom in and out */
	MSG_NAV_ZOOM_DECREASE,
	MSG_NAV_ZOOM_INCREASE,

	/** Application control messages */
	MSG_APP_QUIT
};

} // end namespace OpenGC

#endif
