/**************************************************************************\
 *
 *  This file is part of the Coin GUI toolkit libraries.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation.  See the
 *  file LICENSE.LGPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you want to use this library with software that is incompatible
 *  licensewise with the LGPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#ifndef SOXTFULLVIEWERP_H
#define SOXTFULLVIEWERP_H

#include <Inventor/SbBasic.h>
#include <SoGuiFullViewerP.h>

class SoXtFullViewer;

// ************************************************************************

// This class contains private data and methods used within the
// SoXtFullViewer class.

class SoXtFullViewerP : public SoGuiFullViewerP
{
public:
  SoXtFullViewerP(SoXtFullViewer * publ);
  ~SoXtFullViewerP();
};

// ************************************************************************

#endif // ! SOXTFULLVIEWERP_H
