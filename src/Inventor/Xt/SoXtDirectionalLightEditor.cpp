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

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

// *************************************************************************

/*!
  \class SoXtDirectionalLightEditor Inventor/Xt/SoXtDirectionalLightEditor.h
  \brief The SoXtDirectionalLightEditor class is yet to be documented.
  \ingroup components editors

  This class is not implemented.
*/

// *************************************************************************

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoDirectionalLight.h>

#include <soxtdefs.h>

#include <Inventor/Xt/SoXtDirectionalLightEditor.h>

// *************************************************************************

/*!
  \var SoDirectionalLight * SoXtDirectionalLightEditor::dirLight
  FIXME: write doc
*/
/*!
  \var SoSeparator * SoXtDirectionalLightEditor::root
  FIXME: write doc
*/
/*!
  \var SoSeparator * SoXtDirectionalLightEditor::litStuff
  FIXME: write doc
*/
/*!
  \var SoPerspectiveCamera * SoXtDirectionalLightEditor::myCamera
  FIXME: write doc
*/
/*!
  \var SoCamera * SoXtDirectionalLightEditor::cameraToWatch
  FIXME: write doc
*/
/*!
  \var _SoXtColorEditor * SoXtDirectionalLightEditor::colorEditor
  FIXME: write doc
*/
/*!
  \var _SoXtColorSlider * SoXtDirectionalLightEditor::intensitySlider
  FIXME: write doc
*/
/*!
  \var SoXtRenderArea * SoXtDirectionalLightEditor::renderArea
  FIXME: write doc
*/
/*!
  \var SoNodeSensor * SoXtDirectionalLightEditor::lightSensor
  FIXME: write doc
*/
/*!
  \var SoNodeSensor * SoXtDirectionalLightEditor::cameraSensor
  FIXME: write doc
*/
/*!
  \var SoDirectionalLightManip * SoXtDirectionalLightEditor::dirLightManip
  FIXME: write doc
*/
/*!
  \var static char * SoXtDirectionalLightEditor::geomBuffer
  FIXME: write doc
*/
/*!
  \var SbBool SoXtDirectionalLightEditor::ignoreCallback
  FIXME: write doc
*/
/*!
  \var SoCallbackList * SoXtDirectionalLightEditor::callbackList
  FIXME: write doc
*/
/*!
  \var SoXtClipboard * SoXtDirectionalLightEditor::clipboard
  FIXME: write doc
*/

// *************************************************************************

/*!
  Constructor.
*/

SoXtDirectionalLightEditor::SoXtDirectionalLightEditor(
  Widget parent,
  const char * const name,
  SbBool embed )
: inherited( parent, name, embed )
{
  SOXT_STUB();
  this->constructor( TRUE );
} // SoXtDirectionalLightEditor()

/*!
  Constructor.
*/

SoXtDirectionalLightEditor::SoXtDirectionalLightEditor( // protected
  Widget parent,
  const char * const name,
  SbBool embed,
  SbBool build )
: inherited( parent, name, embed )
{
  SOXT_STUB();
  this->constructor( build );
} // SoXtDirectionalLightEditor()

/*!
  Destructor.
*/

SoXtDirectionalLightEditor::~SoXtDirectionalLightEditor(
  void )
{
  SOXT_STUB();
} // ~SoXtDirectionalLightEditor()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtDirectionalLightEditor::attach(
  SoPath * pathToLight )
{
  SOXT_STUB();
} // attach()

/*!
  FIXME: write doc
*/

void
SoXtDirectionalLightEditor::detach(
  void )
{
  SOXT_STUB();
} // detach()

/*!
  FIXME: write doc
*/

SbBool
SoXtDirectionalLightEditor::isAttached(
  void )
{
  SOXT_STUB();
  return FALSE;
} // isAttached()

/*!
  FIXME: write doc
*/

void
SoXtDirectionalLightEditor::setLight(
  const SoDirectionalLight & light )
{
  SOXT_STUB();
} // setLight()

/*!
  FIXME: write doc
*/

const SoDirectionalLight &
SoXtDirectionalLightEditor::getLight(
  void ) const
{
  SOXT_STUB();
  return *((SoDirectionalLight *) SoDirectionalLight::getClassTypeId().createInstance());
} // getLight()

/*!
  FIXME: write doc
*/

void
SoXtDirectionalLightEditor::addLightChangedCallback(
  SoXtDirectionalLightEditorCB * callback,
  void * user )
{
  SOXT_STUB();
} // addLightChangedCallback()

/*!
  FIXME: write doc
*/

void
SoXtDirectionalLightEditor::removeLightChangedCallback(
  SoXtDirectionalLightEditorCB * callback,
  void * user )
{
  SOXT_STUB();
} // removeLightChangedCallback()

/*!
  FIXME: write doc
*/

void
SoXtDirectionalLightEditor::show( // virtual
  void )
{
  SOXT_STUB();
} // show()

/*!
  FIXME: write doc
*/

void
SoXtDirectionalLightEditor::hide( // virtual
  void )
{
  SOXT_STUB();
} // hide()

// *************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoXtDirectionalLightEditor::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "SoXtDirectionalLightEditor";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  FIXME: write doc
*/

const char *
SoXtDirectionalLightEditor::getDefaultTitle( // virtual, protected
  void ) const
{
  static const char defaultTitle[] = "Directional Light";
  return defaultTitle;
} // getDefaultTitle()

/*!
  FIXME: write doc
*/

const char *
SoXtDirectionalLightEditor::getDefaultIconTitle( // virtual, protected
  void ) const
{
  static const char defaultIconTitle[] = "Directional Light";
  return defaultIconTitle;
} // getDefaultIconTitle()

/*!
  FIXME: write doc
*/

void
SoXtDirectionalLightEditor::copyLight( // protected
  SoDirectionalLight * destination,
  const SoDirectionalLight * source )
{
  SOXT_STUB();
} // copyLight()

/*!
  FIXME: write doc
*/

void
SoXtDirectionalLightEditor::updateLocalComponents( // protected
  void )
{
  SOXT_STUB();
} // updateLocalComponents()

// *************************************************************************

/*!
  FIXME: write doc
*/

// this method will be moved to protected - no point otherwise...

Widget
SoXtDirectionalLightEditor::buildWidget( // private
  Widget parent )
{
  SOXT_STUB();
  return (Widget) NULL;
} // buildWidget()

/*!
  FIXME: write doc
*/

void
SoXtDirectionalLightEditor::constructor( // private
  SbBool build )
{
  SOXT_STUB();
} // constructor()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtDirectionalLightEditorRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

