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

#include <ctype.h> // toupper()

#include <X11/X.h>
#include <X11/keysym.h>

#include <Inventor/misc/SoBasic.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>

#include <Inventor/Xt/devices/SoXtKeyboard.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

// *************************************************************************

/*!
  \class SoXtKeyboard Inventor/Xt/devices/SoXtKeyboard.h
  \brief The SoXtKeyboard class glues Xt/Motif keyboard interaction together
  with the Open Inventor scene graph.
  \ingroup devices

  All components derived from the SoXtRenderArea have got an SoXtKeyboard 
  device attached by default.
*/

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtKeyboard);

// *************************************************************************

/*!
  \enum SoXtKeyboard::KeyboardEvents

  Enumeration over supported event types.
*/

// *************************************************************************

/*!
  Public constructor.
*/

SoXtKeyboard::SoXtKeyboard(
  int events)
{
  this->events = events;
  this->keyboardEvent = NULL;
} // SoXtKeyboard()

/*!
  Destructor.
*/

SoXtKeyboard::~SoXtKeyboard(// virtual
  void)
{
  delete this->keyboardEvent;
} // ~SoXtKeyboard()

// *************************************************************************

// Doc in superclass.
void
SoXtKeyboard::enable(Widget widget, XtEventHandler handler,
                     XtPointer closure, Window window)
{
  XtAddEventHandler(widget, this->events, FALSE, handler, closure);
}

// Doc in superclass.
void
SoXtKeyboard::disable(Widget widget, XtEventHandler handler, XtPointer closure)
{
  XtRemoveEventHandler(widget, this->events, FALSE, handler, closure);
}

// *************************************************************************

/*!
  This method translates between X events and Open Inventor events.

  If the X event is a keyboard event, an event of the type SoKeyboardEvent
  is returned.  Otherwise, NULL is returned.
*/

const SoEvent *
SoXtKeyboard::translateEvent(// virtual
  XAnyEvent * event)
{
  switch (event->type) {
  case KeyPress:
    return this->makeKeyboardEvent((XKeyEvent *) event, SoButtonEvent::DOWN);
    break;
  case KeyRelease:
    return this->makeKeyboardEvent((XKeyEvent *) event, SoButtonEvent::UP);
    break;
  default:
    break;
  } // switch (event->type)
  return (SoEvent *) NULL;
} // translateEvent()

// *************************************************************************

/*!
  This method creates an SoKeyboardEvent from an X event.
*/

SoKeyboardEvent *
SoXtKeyboard::makeKeyboardEvent(// private
  XKeyEvent * event,
  SoButtonEvent::State state)
{
  delete this->keyboardEvent;
  this->keyboardEvent = new SoKeyboardEvent;
  this->setEventPosition(this->keyboardEvent, event->x, event->y);
  this->keyboardEvent->setState(state);

  char keybuf[8];
  KeySym keysym;

  SoKeyboardEvent::Key key = SoKeyboardEvent::ANY;

  int keybufusage = XLookupString(event, keybuf, 8, &keysym, NULL);
  
  // check these first or they will be set to incorrect values
  switch (keysym) {
  case XK_KP_0:         key = SoKeyboardEvent::PAD_0;          break;
  case XK_KP_1:         key = SoKeyboardEvent::PAD_1;          break;
  case XK_KP_2:         key = SoKeyboardEvent::PAD_2;          break;
  case XK_KP_3:         key = SoKeyboardEvent::PAD_3;          break;
  case XK_KP_4:         key = SoKeyboardEvent::PAD_4;          break;
  case XK_KP_5:         key = SoKeyboardEvent::PAD_5;          break;
  case XK_KP_6:         key = SoKeyboardEvent::PAD_6;          break;
  case XK_KP_7:         key = SoKeyboardEvent::PAD_7;          break;
  case XK_KP_8:         key = SoKeyboardEvent::PAD_8;          break;
  case XK_KP_9:         key = SoKeyboardEvent::PAD_9;          break;
  default:
    key = SoKeyboardEvent::ANY;
  }
  
  if (key == SoKeyboardEvent::ANY && keybufusage == 1) {
    switch (keybuf[0]) {
    case 'a': case 'A':   key = SoKeyboardEvent::A;              break;
    case 'b': case 'B':   key = SoKeyboardEvent::B;              break;
    case 'c': case 'C':   key = SoKeyboardEvent::C;              break;
    case 'd': case 'D':   key = SoKeyboardEvent::D;              break;
    case 'e': case 'E':   key = SoKeyboardEvent::E;              break;
    case 'f': case 'F':   key = SoKeyboardEvent::F;              break;
    case 'g': case 'G':   key = SoKeyboardEvent::G;              break;
    case 'h': case 'H':   key = SoKeyboardEvent::H;              break;
    case 'i': case 'I':   key = SoKeyboardEvent::I;              break;
    case 'j': case 'J':   key = SoKeyboardEvent::J;              break;
    case 'k': case 'K':   key = SoKeyboardEvent::K;              break;
    case 'l': case 'L':   key = SoKeyboardEvent::L;              break;
    case 'm': case 'M':   key = SoKeyboardEvent::M;              break;
    case 'n': case 'N':   key = SoKeyboardEvent::N;              break;
    case 'o': case 'O':   key = SoKeyboardEvent::O;              break;
    case 'p': case 'P':   key = SoKeyboardEvent::P;              break;
    case 'q': case 'Q':   key = SoKeyboardEvent::Q;              break;
    case 'r': case 'R':   key = SoKeyboardEvent::R;              break;
    case 's': case 'S':   key = SoKeyboardEvent::S;              break;
    case 't': case 'T':   key = SoKeyboardEvent::T;              break;
    case 'u': case 'U':   key = SoKeyboardEvent::U;              break;
    case 'v': case 'V':   key = SoKeyboardEvent::V;              break;
    case 'w': case 'W':   key = SoKeyboardEvent::W;              break;
    case 'x': case 'X':   key = SoKeyboardEvent::X;              break;
    case 'y': case 'Y':   key = SoKeyboardEvent::Y;              break;
    case 'z': case 'Z':   key = SoKeyboardEvent::Z;              break;
    case '\\': case '|':  key = SoKeyboardEvent::BACKSLASH;      break;
    case '[': case '{':   key = SoKeyboardEvent::BRACKETLEFT;    break;
    case ']': case '}':   key = SoKeyboardEvent::BRACKETRIGHT;   break;
    case '0': case ')':   key = SoKeyboardEvent::NUMBER_0;       break;
    case '1': case '!':   key = SoKeyboardEvent::NUMBER_1;       break;
    case '2': case '@':   key = SoKeyboardEvent::NUMBER_2;       break;
    case '3': case '#':   key = SoKeyboardEvent::NUMBER_3;       break;
    case '4': case '$':   key = SoKeyboardEvent::NUMBER_4;       break;
    case '5': case '%':   key = SoKeyboardEvent::NUMBER_5;       break;
    case '6': case '^':   key = SoKeyboardEvent::NUMBER_6;       break;
    case '7': case '&':   key = SoKeyboardEvent::NUMBER_7;       break;
    case '8': case '*':   key = SoKeyboardEvent::NUMBER_8;       break;
    case '9': case '(':   key = SoKeyboardEvent::NUMBER_9;       break;
    default:
      break;
    }
  } 

  if (key == SoKeyboardEvent::ANY) {
    switch (keysym) {
    case XK_Shift_L:      key = SoKeyboardEvent::LEFT_SHIFT;     break;
    case XK_Shift_R:      key = SoKeyboardEvent::RIGHT_SHIFT;    break;
    case XK_Control_L:    key = SoKeyboardEvent::LEFT_CONTROL;   break;
    case XK_Control_R:    key = SoKeyboardEvent::RIGHT_CONTROL;  break;
    case XK_Alt_L:        key = SoKeyboardEvent::LEFT_ALT;       break;
    case XK_Alt_R:        key = SoKeyboardEvent::RIGHT_ALT;      break;
    case XK_Home:         key = SoKeyboardEvent::HOME;           break;
    case XK_Left:         key = SoKeyboardEvent::LEFT_ARROW;     break;
    case XK_Up:           key = SoKeyboardEvent::UP_ARROW;       break;
    case XK_Right:        key = SoKeyboardEvent::RIGHT_ARROW;    break;
    case XK_Down:         key = SoKeyboardEvent::DOWN_ARROW;     break;
    case XK_Page_Up:      key = SoKeyboardEvent::PAGE_UP;        break;
    case XK_Page_Down:    key = SoKeyboardEvent::PAGE_DOWN;      break;

// X11 defines XK_Prior and XK_Next to the same as XK_Page_Up and XK_Page_Down
//  case XK_Prior:
//  case XK_Next:

    case XK_End:          key = SoKeyboardEvent::END;            break;
    case XK_KP_Enter:     key = SoKeyboardEvent::PAD_ENTER;      break;
    case XK_KP_F1:        key = SoKeyboardEvent::PAD_F1;         break;
    case XK_KP_F2:        key = SoKeyboardEvent::PAD_F2;         break;
    case XK_KP_F3:        key = SoKeyboardEvent::PAD_F3;         break;
    case XK_KP_F4:        key = SoKeyboardEvent::PAD_F4;         break;
    case XK_KP_Add:       key = SoKeyboardEvent::PAD_SUBTRACT;   break;
    case XK_KP_Subtract:  key = SoKeyboardEvent::PAD_ADD;        break;
    case XK_KP_Multiply:  key = SoKeyboardEvent::PAD_MULTIPLY;   break;
    case XK_KP_Divide:    key = SoKeyboardEvent::PAD_DIVIDE;     break;
    case XK_KP_Space:     key = SoKeyboardEvent::PAD_SPACE;      break;
    case XK_KP_Tab:       key = SoKeyboardEvent::PAD_TAB;        break;
    case XK_KP_Insert:    key = SoKeyboardEvent::PAD_INSERT;     break;
    case XK_KP_Delete:    key = SoKeyboardEvent::PAD_DELETE;     break;
    case XK_KP_Decimal:   key = SoKeyboardEvent::PAD_PERIOD;     break;
    case XK_F1:           key = SoKeyboardEvent::F1;             break;
    case XK_F2:           key = SoKeyboardEvent::F2;             break;
    case XK_F3:           key = SoKeyboardEvent::F3;             break;
    case XK_F4:           key = SoKeyboardEvent::F4;             break;
    case XK_F5:           key = SoKeyboardEvent::F5;             break;
    case XK_F6:           key = SoKeyboardEvent::F6;             break;
    case XK_F7:           key = SoKeyboardEvent::F7;             break;
    case XK_F8:           key = SoKeyboardEvent::F8;             break;
    case XK_F9:           key = SoKeyboardEvent::F9;             break;
    case XK_F10:          key = SoKeyboardEvent::F10;            break;
    case XK_F11:          key = SoKeyboardEvent::F11;            break;
    case XK_F12:          key = SoKeyboardEvent::F12;            break;
    case XK_BackSpace:    key = SoKeyboardEvent::BACKSPACE;      break;
    case XK_Tab:          key = SoKeyboardEvent::TAB;            break;
    case XK_Return:       key = SoKeyboardEvent::RETURN;         break;
    case XK_Linefeed:     key = SoKeyboardEvent::ENTER;          break;
    case XK_Pause:        key = SoKeyboardEvent::PAUSE;          break;
    case XK_Scroll_Lock:  key = SoKeyboardEvent::SCROLL_LOCK;    break;
    case XK_Escape:       key = SoKeyboardEvent::ESCAPE;         break;
#ifdef HAVE_SOKEYBOARDEVENT_DELETE
    case XK_Delete:       key = SoKeyboardEvent::DELETE;         break;
#else /* very strange */
    case XK_Delete:       key = SoKeyboardEvent::KEY_DELETE;     break;
#endif
    case XK_Print:        key = SoKeyboardEvent::PRINT;          break;
    case XK_Insert:       key = SoKeyboardEvent::INSERT;         break;
    case XK_Num_Lock:     key = SoKeyboardEvent::NUM_LOCK;       break;
    case XK_Caps_Lock:    key = SoKeyboardEvent::CAPS_LOCK;      break;
    case XK_Shift_Lock:   key = SoKeyboardEvent::SHIFT_LOCK;     break;
    case XK_space:        key = SoKeyboardEvent::SPACE;          break;
    case XK_apostrophe: case XK_quotedbl: key = SoKeyboardEvent::APOSTROPHE;     break;
    case XK_comma: case XK_less: key = SoKeyboardEvent::COMMA;          break;
    case XK_minus: case XK_underscore: key = SoKeyboardEvent::MINUS;          break;
    case XK_period: case XK_greater: key = SoKeyboardEvent::PERIOD;         break;
    case XK_slash: case XK_question: key = SoKeyboardEvent::SLASH;          break;
    case XK_semicolon: case XK_colon: key = SoKeyboardEvent::SEMICOLON;      break;
    case XK_equal: case XK_plus: key = SoKeyboardEvent::EQUAL;          break;
    case XK_bracketleft: case XK_braceleft: key = SoKeyboardEvent::BRACKETLEFT;    break;
    case XK_bracketright: case XK_braceright: key = SoKeyboardEvent::BRACKETRIGHT;   break;
    case XK_grave: case XK_asciitilde: key = SoKeyboardEvent::GRAVE;          break;

    default:
#if SOXT_DEBUG && 0
      SoDebugError::postWarning("SoXtKeyboard::makeKeyboardEvent",
        "keysym 0x%04x isn't handled", keysym);
#endif // SOXT_DEBUG
      break;
    }
  }

  this->keyboardEvent->setKey(key);

  // modifiers:
  this->keyboardEvent->setShiftDown(
    (event->state & ShiftMask) ? TRUE : FALSE);
  this->keyboardEvent->setCtrlDown(
    (event->state & ControlMask) ? TRUE : FALSE);
  this->keyboardEvent->setAltDown(
    (event->state & Mod1Mask) ? TRUE : FALSE);

  return this->keyboardEvent;
} // makeKeyboardEvent()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtKeyboardRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

