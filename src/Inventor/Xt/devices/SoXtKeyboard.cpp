/**************************************************************************
 *
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
 **************************************************************************/

static const char rcsid[] =
  "$Id$";

#include <X11/X.h>

#include <Inventor/misc/SoBasic.h>
#include <Inventor/events/SoKeyboardEvent.h>

#include <Inventor/Xt/common/soguidefs.h> // SOGUI_STUB()
#include <Inventor/Xt/devices/SoXtKeyboard.h>

// *************************************************************************

/*!
*/

SoXtKeyboard::SoXtKeyboard(
  int events )
{
  this->events = events;
  this->keyboardEvent = NULL;
} // SoXtKeyboard()

/*!
*/

SoXtKeyboard::~SoXtKeyboard(
  void )
{
  delete this->keyboardEvent;
} // ~SoXtKeyboard()

/*!
*/

void
SoXtKeyboard::enable( // virtual
  Widget, // widget,
  XtEventHandler, // func,
  XtPointer, // data,
  Window ) // window )
{
  SOGUI_STUB();
} // enable()

/*!
*/

void
SoXtKeyboard::disable( // virtual
  Widget, // widget,
  XtEventHandler, // func,
  XtPointer ) // data )
{
  SOGUI_STUB();
} // disable()

/*!
*/

const SoEvent *
SoXtKeyboard::translateEvent( // virtual
  XAnyEvent * event )
{
  switch ( event->type ) {

  // events we should catch:
  case KeyPress:
  case KeyRelease:
    do {
      SOGUI_STUB();
    } while ( FALSE );
    // return SoEvent * here
    // return this->makeKeyboardEvent( event ); ??
    break;

  // events we should ignore:
  default:
    break;

  } // switch ( event->type )

  return (SoEvent *) NULL;
} // translateEvent()

/*!
*/

SoKeyboardEvent *
SoXtKeyboard::makeKeyboardEvent( // private
  XKeyEvent *, //  event,
  SoButtonEvent::State ) // state )
{
  SOGUI_STUB();
  return (SoKeyboardEvent *) NULL;
} // makeKeyboardEvent()

// *************************************************************************

// To shut up nagging compilers...
const char *
getSoXtKeyboardRCSId(void)
{
  return rcsid;
} // getSoXtKeyboardRCSId()

