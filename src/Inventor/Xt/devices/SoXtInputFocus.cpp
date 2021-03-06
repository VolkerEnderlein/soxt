/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include <X11/X.h>

#include <soxtdefs.h>
#include <Inventor/Xt/devices/SoXtInputFocus.h>
#include <Inventor/Xt/devices/SoGuiInputFocusP.h>

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->pub)

// *************************************************************************

class SoXtInputFocusP : public SoGuiInputFocusP {
public:
  SoXtInputFocusP(SoXtInputFocus * p) : SoGuiInputFocusP(p) { }
};

// *************************************************************************

SoXtInputFocus::SoXtInputFocus(int events)
{
  PRIVATE(this) = new SoXtInputFocusP(this);
  PRIVATE(this)->eventmask = events;
}

SoXtInputFocus::~SoXtInputFocus()
{
  delete PRIVATE(this);
}

// *************************************************************************

// Documented in superclass.
void
SoXtInputFocus::enable(Widget, // widget,
                       SoXtEventHandler *, // func,
                       XtPointer)
{
  SOXT_STUB();
}

// Documented in superclass.
void
SoXtInputFocus::disable(Widget, // widget,
                        SoXtEventHandler *, // func,
                        XtPointer) // data)
{
  SOXT_STUB();
}

// *************************************************************************

const SoEvent *
SoXtInputFocus::translateEvent(XAnyEvent * event)
{
  switch (event->type) {

  // events we should handle:
  case FocusIn:
  case FocusOut:
    do {
      SOXT_STUB();
    } while (FALSE);
    // return SoEvent here
    break;

  // events we should ignore:
  default:
    break;
  }

  return (SoEvent *) NULL;
}

// *************************************************************************

#undef PRIVATE
#undef PUBLIC

