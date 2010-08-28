/*=========================================================================

  Copyright (c) 2001-2004 Damion Shelton
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

#ifndef DataSource_h
#define DataSource_h

#include "AirframeDataContainer.h"

namespace OpenGC
{

class DataSource
{
	public:
		
		DataSource();
		virtual ~DataSource();

		/** Initialize the data to a default value*/
		void InitializeData();

		/** Called by the base AppObject after all the init parameters
		 * have been complete. This should open the connection to the sim
		 */
		virtual bool Open();

		/** Called by the render window during idle processing
		 * This function is the one and only place where OpenGC
		 * should acquire data from the sim
		 */
		virtual bool OnIdle();

		/** Get access to airframe data */
		AirframeDataContainer* GetAirframe() { return m_Airframe; }

	protected:

		/** Data that describes the airframe (alt, heading, control surfaces, etc.) */
		AirframeDataContainer* m_Airframe;
};

} // end namespace OpenGC

#endif

