// FIXME header

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
