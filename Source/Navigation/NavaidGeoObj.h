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

#ifndef NavaidGeoObj_h
#define NavaidGeoObj_h

#include "GeographicObject.h"

namespace OpenGC
{

class NavaidGeoObj : public GeographicObject
{
public:

	NavaidGeoObj();
	virtual ~NavaidGeoObj();
  
  /** Accessors for frequency */
  void SetFrequency(float freq) {m_Frequency = freq;}
  float GetFrequency() {return m_Frequency;}
  
  /** Accessors for navaid type */
  void SetNDBType() {m_NavaidType = 0;}
  void SetVORType() {m_NavaidType = 1;}
  void SetDMEType() {m_NavaidType = 2;}
  
  unsigned int GetType() {return m_NavaidType;};

protected:

  /** Frequency of the navaid */
  float m_Frequency;
  
  /** Type of navaid */
  unsigned int m_NavaidType;

};

} // end namespace OpenGC

#endif
