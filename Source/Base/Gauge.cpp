/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Damion Shelton
  All rights reserved. Contributions by Hugo Vincent.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <stdio.h>
#include "GLHeaders.h"

#include "Globals.h"
#include "FontManager.h"
#include "DataSource.h"
#include "GaugeComponent.h"
#include "RenderObject.h"
#include "Gauge.h"
#include "Debug.h"

namespace OpenGC
{

Gauge::Gauge()
{
	m_NumGaugeComponents = 0;
	m_DrawGaugeOutline = false;

	m_Scale.y = 1.0;
	m_Scale.x = 1.0;

	m_PhysicalPosition.x = 0;
	m_PhysicalPosition.y = 0;

	m_PixelPosition.x = 0;
	m_PixelPosition.y = 0;

	m_PixelSize.x = 0;
	m_PixelSize.y = 0;
}

Gauge::~Gauge()
{
	// If there are gauge components, delete them
	if( m_NumGaugeComponents!=0 )
	{
		std::list<GaugeComponent*>::iterator it;
		for (it = m_GaugeComponentList.begin(); it != m_GaugeComponentList.end(); ++it)
		{
			delete *it;
		}
	}
}

void Gauge::InitFromXMLNode(XMLNode gaugeNode)
{
	Check(gaugeNode.IsValid() && gaugeNode.GetName() == "Gauge");

	double scale = globals->m_PrefManager->GetPrefD("DefaultGaugeScale");
	double zoom = globals->m_PrefManager->GetPrefD("Zoom");
	double x, y; // temp variables
	
	// Set the units per pixel
	if (gaugeNode.HasChild("UnitsPerPixel"))
	{
		SetUnitsPerPixel(gaugeNode.GetChild("UnitsPerPixel").GetTextAsDouble());
	}
	else
	{
		SetUnitsPerPixel(globals->m_PrefManager->GetPrefD("UnitsPerPixel"));
	}

	// Set the position
	if (gaugeNode.HasChild("Position"))
	{
		gaugeNode.GetChild("Position").GetTextAsCoord(x, y);
		SetPosition(x * zoom, y * zoom);
	}
	else
	{
		SetPosition(0.0, 0.0);
	}

	// Set the scale
	if (gaugeNode.HasChild("Scale")) {
		gaugeNode.GetChild("Scale").GetTextAsCoord(x, y);
		SetScale(x * zoom * scale, y * zoom * scale);
	}
	else
	{
		SetScale(zoom * scale, zoom * scale);
	}

	// Set the gauge outline
	if (gaugeNode.HasChild("Outline"))
	{
		SetGaugeOutline(gaugeNode.GetChild("Outline").GetTextAsBool());
	}
	
	CustomXMLInit(gaugeNode);
}

void Gauge::AddGaugeComponent(GaugeComponent* pComponent)
{
	m_GaugeComponentList.push_back(pComponent);
	m_NumGaugeComponents++;
}

void Gauge::Render()
{
	// Overload this function in derived classes to render
	// parts of the guage not defined by gauge components

	// BUT!!! you should always call the base class render function
	// as well in order to render the gauge components

	this->ResetGaugeCoordinateSystem();

	if(m_NumGaugeComponents > 0)
	{
		std::list<GaugeComponent*>::iterator it;
		for (it = m_GaugeComponentList.begin(); it != m_GaugeComponentList.end(); ++it)
		{
			(*it)->Render();
		}
	}

	this->ResetGaugeCoordinateSystem();

	if(m_DrawGaugeOutline)
	{
		this->DrawGaugeOutline();
	}
}

// Resets the gauge coordinate system before and after rendering components
void Gauge::ResetGaugeCoordinateSystem()
{
	this->RecalcWindowPlacement();

	// The viewport is established in order to clip things
	// outside the bounds of the gauge
	glViewport(m_PixelPosition.x, m_PixelPosition.y, m_PixelSize.x, m_PixelSize.y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Define the projection so that we're drawing in "real" space
	glOrtho(0,m_PhysicalSize.x,0, m_PhysicalSize.y, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Gauge::RecalcWindowPlacement()
{
	// Figure out where we're drawing in the window
	m_PixelPosition.x = (int) (m_PhysicalPosition.x / m_UnitsPerPixel);
	m_PixelPosition.y = (int) (m_PhysicalPosition.y / m_UnitsPerPixel);

	m_PixelSize.x = (int) (m_PhysicalSize.x / m_UnitsPerPixel * m_Scale.x);
	m_PixelSize.y = (int) (m_PhysicalSize.y / m_UnitsPerPixel * m_Scale.y);
}

void Gauge::SetUnitsPerPixel(double unitsPerPixel)
{
	m_UnitsPerPixel = unitsPerPixel;

	if(m_NumGaugeComponents > 0)
	{
		std::list<GaugeComponent*>::iterator it;
		for (it = m_GaugeComponentList.begin(); it != m_GaugeComponentList.end(); ++it)
		{
			(*it)->SetUnitsPerPixel(m_UnitsPerPixel);
		}
	}
}

void Gauge::SetScale(double xScale, double yScale)
{
	// Set gauge scaling factors, must be greater than 0
	if( (xScale > 0) && (yScale > 0 ) )
	{
		m_Scale.x = xScale;
		m_Scale.y = yScale;

		if(m_NumGaugeComponents > 0)
		{
			std::list<GaugeComponent*>::iterator it;
			for (it = m_GaugeComponentList.begin(); it != m_GaugeComponentList.end(); ++it)
			{
				(*it)->SetScale(xScale, yScale);
			}
		}
	}
}

bool Gauge::ClickTest(int button, int state, int x, int y)
{
	if( (x >= (int)m_PixelPosition.x)&&(x <= (int)(m_PixelPosition.x + m_PixelSize.x))
			&&(y >= (int)m_PixelPosition.y)&&(y <= (int)(m_PixelPosition.y + m_PixelSize.y)) )
	{
		if(m_NumGaugeComponents > 0)
		{
			std::list<GaugeComponent*>::iterator it;
			for (it = m_GaugeComponentList.begin(); it != m_GaugeComponentList.end(); ++it)
			{
				(*it)->HandleMouseButton(button, state, x, y);
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

void Gauge::DrawGaugeOutline()
{
	float vertices[] = {
		0.0, 0.0,
		0.0, m_PhysicalSize.y,
		m_PhysicalSize.x, m_PhysicalSize.y,
		m_PhysicalSize.x, 0.0
	};
	glLineWidth(2.0);
	glColor3ub(0, 190, 190);
	glVertexPointer(2, GL_FLOAT, 0, &vertices);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
}

} // end namespace OpenGC
