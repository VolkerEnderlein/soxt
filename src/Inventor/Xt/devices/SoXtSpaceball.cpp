/**************************************************************************
 *
 *  This file is part of the Coin SoXt GUI binding library.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License version
 *  2.1 as published by the Free Software Foundation.  See the file
 *  LICENSE.LGPL at the root directory of the distribution for all the
 *  details.
 *
 *  If you want to use Coin SoXt for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

#include <X11/extensions/XInput.h>

#include <Inventor/misc/SoBasic.h>

// FIXME: really ugly, but necessary to avoid XInput.h/Print.h conflict
#define _XpPrint_H_
#define _XP_PRINT_SERVER_
typedef void * XPContext;

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>

#include <Inventor/Xt/devices/SoXtSpaceball.h>

// *************************************************************************

/*!
  \class SoXtSpaceball Inventor/Xt/devices/SoXtSpaceball.h
  \brief The SoXtSpaceball class is the glue between Xt/Motif spaceball
  handling and Inventor spaceball handling.
  \ingroup devices

  This class is not implemented yet.  Do not purchase a spaceball for use
  with Coin unless you intend to use it through the Linux Joystick driver
  (SoXtLinuxJoystick) at this moment.
*/

// *************************************************************************

class SoXtSpaceballP {
public:
  int events;
  float rotationFactor;
  float translationFactor;
};

#define PRIVATE(p) (p->pimpl)

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtSpaceball);

// *************************************************************************

/*!
  \enum SoXtSpaceball::DeviceType

  Enumeration of unsupported Spaceball input device types.
*/

/*!
  \enum SoXtSpaceball::Mask

  Enumeration of unsupported Spaceball input device event types.
*/

// *************************************************************************

/*!
  Public constructor.
*/

SoXtSpaceball::SoXtSpaceball(int events)
{
  PRIVATE(this) = new SoXtSpaceballP;
  PRIVATE(this)->events = events;
  PRIVATE(this)->rotationFactor = 1.0f;
  PRIVATE(this)->translationFactor = 1.0f;
}

/*!
  Destructor.
*/

SoXtSpaceball::~SoXtSpaceball()
{
  delete PRIVATE(this);
}

// *************************************************************************

// Documented in superclass.
void
SoXtSpaceball::enable(Widget, // widget,
                      SoXtEventHandler *, // func,
                      XtPointer)
{
  SOXT_STUB();
}

// Documented in superclass.
void
SoXtSpaceball::disable(Widget, // widget,
                       SoXtEventHandler *, // func,
                       XtPointer) // data)
{
  SOXT_STUB();
}

// *************************************************************************

/*!
  This method is not implemented.
*/

const SoEvent *
SoXtSpaceball::translateEvent(XAnyEvent * event)
{
  SOXT_STUB_ONCE();
  return (SoEvent *) NULL;
}

// *************************************************************************

/*!
  This method sets the scale factor for the spaceball rotation vectors.
*/

void
SoXtSpaceball::setRotationScaleFactor(float factor)
{
  PRIVATE(this)->rotationFactor = factor;
}

/*!
  This method returns the scale factor for the spaceball rotation vectors.
*/

float
SoXtSpaceball::getRotationScaleFactor(void) const
{
  return PRIVATE(this)->rotationFactor;
}

/*!
  This method sets the scale factor for the spaceball translation vectors.
*/

void
SoXtSpaceball::setTranslationScaleFactor(float factor)
{
  PRIVATE(this)->translationFactor = factor;
}

/*!
  This method returns the scale factor for the spaceball translation vectors.
*/

float
SoXtSpaceball::getTranslationScaleFactor(void) const
{
  return PRIVATE(this)->translationFactor;
}

// *************************************************************************

/*!
  This method checks to see if a spaceball device is available.
*/
SbBool
SoXtSpaceball::exists(void)
{
  Display * display = SoXt::getDisplay();
  assert(display != NULL);
  Atom SpaceballAtom = XInternAtom(display, XI_SPACEBALL, True);
  if (SpaceballAtom == None)
    return FALSE;
  int numDevices = 0;
  XDeviceInfo * devices = XListInputDevices(display, &numDevices);
  for (int i = 0; i < numDevices; i++) {
    if (devices[i].type == SpaceballAtom) {
      XFreeDeviceList(devices);
      return TRUE;
    }
  }
  XFreeDeviceList(devices);
  return FALSE;
}

// *************************************************************************

/*!
  This method is not implemented.
*/

void
SoXtSpaceball::setFocusToWindow(SbBool)
{
  SOXT_STUB();
}

/*!
  This method is not implemented.
*/

SbBool
SoXtSpaceball::isFocusToWindow(void) const
{
  SOXT_STUB();
  return FALSE;
}

// *************************************************************************
