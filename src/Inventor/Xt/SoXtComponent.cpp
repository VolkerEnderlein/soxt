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

// *************************************************************************

// Class documentation in common/SoGuiComponentCommon.cpp.in.

// *************************************************************************

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/misc/SoBasic.h>
#include <Inventor/SoLists.h>
#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>

#include <Inventor/Xt/SoXtComponent.h>
#include <Inventor/Xt/SoXtGLWidget.h>
#include <Inventor/Xt/SoXtRenderArea.h>
#include <Inventor/Xt/viewers/SoXtViewer.h>
#include <Inventor/Xt/viewers/SoXtFullViewer.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/Xt/viewers/SoXtPlaneViewer.h>
#include <Inventor/Xt/viewers/SoXtConstrainedViewer.h>
#include <Inventor/Xt/viewers/SoXtWalkViewer.h>
#include <Inventor/Xt/viewers/SoXtFlyViewer.h>
#include <Inventor/Xt/SoXtCursor.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/cursorfont.h>
#ifdef HAVE_LIBXMU
#include <X11/Xmu/Editres.h>
#endif // HAVE_LIBXMU

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/MessageB.h>

#include <assert.h>
#include <string.h>

// *************************************************************************

// The private data for the SoXtComponent.

class SoXtComponentP {
public:
  // Constructor.
  SoXtComponentP(const SoXtComponent * o)
    : widget(NULL), widgetname(NULL), widgetclass(NULL), title(NULL),
      icontitle(NULL), size(SbVec2s(-1, -1)),
      closecbs(NULL), visibilitycbs(NULL),
      visibilitystate(FALSE), fullscreen(FALSE)
  {
    this->owner = o;
  }


  static Cursor getNativeCursor(Display * d,
                                const SoXtCursor::CustomCursor * cc);

  Widget parent;
  Widget widget;
  char * widgetname;
  char * widgetclass;
  char * title;
  char * icontitle;

  SbBool embedded;

  SbVec2s size;

  static SbPList * widgets;
  static SbPList * components;

  SbPList * closecbs;
  SbPList * visibilitycbs;
  SbBool visibilitystate, fullscreen;

private:
  const SoXtComponent * owner;
  static SbDict * cursordict;
};

SbPList * SoXtComponentP::widgets = NULL;
SbPList * SoXtComponentP::components = NULL;
SbDict * SoXtComponentP::cursordict = NULL;

#define PRIVATE(o) (o->pimpl)

// *************************************************************************

SOXT_OBJECT_ABSTRACT_SOURCE(SoXtComponent);

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoXtComponent::initClasses(void)
{
  SoXtComponent::initClass();
  SoXtGLWidget::initClass();
  SoXtRenderArea::initClass();
  SoXtViewer::initClass();
  SoXtFullViewer::initClass();
  SoXtExaminerViewer::initClass();
  SoXtPlaneViewer::initClass();
  SoXtConstrainedViewer::initClass();
  SoXtWalkViewer::initClass();
  SoXtFlyViewer::initClass();
}

// *************************************************************************

struct SoXtWindowCloseCallbackInfo {
  SoXtComponentCB * callback;
  void * closure;
};

struct SoXtComponentVisibilityCallbackInfo {
  SoXtComponentVisibilityCB * callback;
  void * closure;
};

/*!
  \var SoXtComponent::firstRealize

  This member is used to detect the first realization of the component.
*/

// *************************************************************************

// documented in common/SoGuiComponentCommon.cpp.in.
SoXtComponent::SoXtComponent(const Widget parent,
                             const char * const name, // decides X resources to use
                             const SbBool embed) // protected
{
  PRIVATE(this) = new SoXtComponentP(this);

  this->firstRealize = TRUE;

  if (name && strlen(name) > 0)
    PRIVATE(this)->widgetname = strcpy(new char [ strlen(name) + 1 ], name);

  if ((parent == (Widget) NULL) || ! embed) {
    // create own shell

    Visual * visual = NULL;
    Colormap colormap = 0;
    int depth = 0;
    Display * dpy = SoXt::getDisplay();

    if (parent) {
      Widget shell = parent;
      while (! XtIsShell(shell) && shell != (Widget) NULL)
        shell = XtParent(shell);
      assert(shell != (Widget) NULL);
      dpy = XtDisplay(shell);
      XtVaGetValues(shell,
        XmNvisual, &visual,
        XmNcolormap, &colormap,
        XmNdepth, &depth,
        NULL);
    } else {
      SoXt::selectBestVisual(dpy, visual, colormap, depth);
    }
    assert(dpy != NULL);

    PRIVATE(this)->parent = XtVaAppCreateShell(
      SoXt::getAppName(), // didn't work
      SoXt::getAppClass(),
      topLevelShellWidgetClass,
      dpy,
      XmNvisual, visual,
      XmNcolormap, colormap,
      XmNdepth, depth,
      NULL);

#ifdef HAVE_LIBXMU
#if SOXT_DEBUG
    XtEventHandler editres_hook = (XtEventHandler) _XEditResCheckMessages;
    XtAddEventHandler(PRIVATE(this)->parent, (EventMask) 0, True, editres_hook, NULL);
#endif // SOXT_DEBUG
#endif // HAVE_LIBXMU

    PRIVATE(this)->embedded = FALSE;
  } else {
    PRIVATE(this)->parent = parent;
    PRIVATE(this)->embedded = TRUE;
  }
  if (parent && XtIsShell(parent))
    PRIVATE(this)->embedded = FALSE;

  if (XtIsShell(PRIVATE(this)->parent))
    XtInsertEventHandler(PRIVATE(this)->parent, (EventMask) StructureNotifyMask, False,
      SoXtComponent::event_handler, (XtPointer) this, XtListTail);
}

// documented in common/SoGuiComponentCommon.cpp.in.
SoXtComponent::~SoXtComponent()
{
  delete [] PRIVATE(this)->widgetname;
  delete [] PRIVATE(this)->widgetclass;
  delete [] PRIVATE(this)->title;
  delete [] PRIVATE(this)->icontitle;
  if (PRIVATE(this)->closecbs != NULL) {
    const int num = PRIVATE(this)->closecbs->getLength();
    for (int i = 0; i < num; i++) {
      SoXtWindowCloseCallbackInfo * info =
        (SoXtWindowCloseCallbackInfo *) (*PRIVATE(this)->closecbs)[i];
      delete info;
    }
    delete PRIVATE(this)->closecbs;
  }
  if (PRIVATE(this)->visibilitycbs != NULL) {
    const int num = PRIVATE(this)->visibilitycbs->getLength();
    for (int i = 0; i < num; i++) {
      SoXtComponentVisibilityCallbackInfo * info =
        (SoXtComponentVisibilityCallbackInfo *)
          (*PRIVATE(this)->visibilitycbs)[i];
      delete info;
    }
    delete PRIVATE(this)->visibilitycbs;
  }

  delete PRIVATE(this);
}

// *************************************************************************

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoXtComponent::show(void)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtComponent::show", "[enter]");
#endif // SOXT_DEBUG

  Widget shell = this->getShellWidget();
  if (shell) {
    XtRealizeWidget(shell);
    if (this->firstRealize == TRUE) {
      this->afterRealizeHook();
      this->firstRealize = FALSE;
    }
    XtMapWidget(shell);
  } else {
    XtManageChild(this->getBaseWidget());
  }
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtComponent::show", "[exit]");
#endif // SOXT_DEBUG
}

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoXtComponent::hide(void)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtComponent::hide", "[enter]");
#endif // SOXT_DEBUG
  Widget shell = this->getShellWidget();
  if (shell) {
    XtUnmapWidget(shell);
    XtUnrealizeWidget(shell);
  } else {
    XtUnmanageChild(this->getBaseWidget());
  }
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtComponent::hide", "[exit]");
#endif // SOXT_DEBUG
}

// documented in common/SoGuiComponentCommon.cpp.in.
SbBool
SoXtComponent::isVisible(void)
{
  return PRIVATE(this)->visibilitystate;
}

// *************************************************************************

// documented in common/SoGuiComponentCommon.cpp.in.
Widget
SoXtComponent::getWidget(void) const
{
  return this->getBaseWidget();
}

// documented in common/SoGuiComponentCommon.cpp.in.
Widget
SoXtComponent::getBaseWidget(void) const
{
  return PRIVATE(this)->widget;
}

// documented in common/SoGuiComponentCommon.cpp.in.
SbBool
SoXtComponent::isTopLevelShell(void) const
{
  return PRIVATE(this)->embedded ? FALSE : TRUE;
}

// documented in common/SoGuiComponentCommon.cpp.in.
Widget
SoXtComponent::getParentWidget(void) const
{
  return PRIVATE(this)->parent;
}

// *************************************************************************

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoXtComponent::setSize(const SbVec2s size)
{
  PRIVATE(this)->size = size;
  Widget widget = this->getShellWidget();
  if (!widget) { widget = this->getBaseWidget(); }
  if (!widget) { return; }

  int argc = 0;
  Arg args[2];
  if (size[0] != -1) {
    XtSetArg(args[argc], XmNwidth, size[0]);
    argc++;
  }
  if (size[1] != -1) {
    XtSetArg(args[argc], XmNheight, size[1]);
    argc++;
  }
  XtSetValues(widget, args, argc);
  this->sizeChanged(size);
}

// documented in common/SoGuiComponentCommon.cpp.in.
SbVec2s
SoXtComponent::getSize(void) const
{
  return PRIVATE(this)->size;
}

/*!
  This method tries to resize the component window, using \a size as the
  minimum requirements.

  This method is not part of the Open Inventor SoXt API.
*/
void
SoXtComponent::fitSize(const SbVec2s size)
{
  if (this->isTopLevelShell() || (PRIVATE(this)->parent && XtIsShell(PRIVATE(this)->parent))) {
    XtWidgetGeometry geometry;
    XtQueryGeometry(this->getBaseWidget(), NULL, &geometry);
    PRIVATE(this)->size[0] = SoXtMax((short) geometry.width, size[0]);
    PRIVATE(this)->size[1] = SoXtMax((short) geometry.height, size[1]);
    XtVaSetValues(this->getShellWidget(),
                  XmNwidth, PRIVATE(this)->size[0],
                  XmNheight, PRIVATE(this)->size[1],
                  NULL);
  }
}

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoXtComponent::sizeChanged(const SbVec2s & size)
{
  // Since SoXtComponent doesn't manage any internal widgets, this
  // default implementation does nothing.
  //
  // Derived components should implement this to resize internal
  // widgets and pass the [modified] size down to the parent class
  // afterwards.
}

// *************************************************************************

/*!
  This method returns the display the component is sent to.
*/
Display *
SoXtComponent::getDisplay(void)
{
#if SOXT_DEBUG
  if (! this->getBaseWidget())
    SoDebugError::postInfo("SoXtComponent::getDisplay",
      "component has no base widget");
#endif // SOXT_DEBUG
  return this->getBaseWidget() ?
    XtDisplay(this->getBaseWidget()) : (Display *) NULL;
}

// *************************************************************************

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoXtComponent::setTitle(const char * const title)
{
  if (PRIVATE(this)->title && strlen(PRIVATE(this)->title) >= strlen(title)) {
    strcpy(PRIVATE(this)->title, (char *) title);
  } else {
    delete [] PRIVATE(this)->title;
    PRIVATE(this)->title = strcpy(new char [strlen(title)+1], title);
  }

  Widget shell = this->getShellWidget();
  if (! shell)
    return;
  XtVaSetValues(shell,
                XmNtitle, PRIVATE(this)->title,
                NULL);
}

// documented in common/SoGuiComponentCommon.cpp.in.
const char *
SoXtComponent::getTitle(void) const
{
  // FIXME: use SoXtResource to see if title is set?
  return PRIVATE(this)->title ? PRIVATE(this)->title : this->getDefaultTitle();
}

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoXtComponent::setIconTitle(const char * const title)
{
  if (PRIVATE(this)->icontitle && strlen(PRIVATE(this)->icontitle) >= strlen(title)) {
    strcpy(PRIVATE(this)->icontitle, (char *) title);
  } else {
    delete [] PRIVATE(this)->icontitle;
    PRIVATE(this)->icontitle = strcpy(new char [strlen(title)+1], title);
  }
  Widget shell = this->getShellWidget();
  if (! shell)
    return;
  XtVaSetValues(shell,
                XtNiconName, PRIVATE(this)->icontitle,
                NULL);
}

// documented in common/SoGuiComponentCommon.cpp.in.
const char *
SoXtComponent::getIconTitle(void) const
{
  // FIXME: use SoXtResource to see if iconName is set?
  return PRIVATE(this)->icontitle ? PRIVATE(this)->icontitle : this->getDefaultIconTitle();
}

// *************************************************************************

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoXtComponent::setWindowCloseCallback(
  SoXtComponentCB * const callback,
  void * const closure)
{
  this->addWindowCloseCallback(callback, closure);
}

/*!
  This method adds a close callback for the component window.

  This method is not standard for Open Inventor.

  \sa SoXtComponent::setWindowCloseCallback()
  \sa SoXtComponent::removeWindowCloseCallback()
  \sa SoXtComponent::invokeWindowCloseCallback()
*/

void
SoXtComponent::addWindowCloseCallback(
  SoXtComponentCB * callback,
  void * closure)
{
  if (PRIVATE(this)->closecbs == NULL)
    PRIVATE(this)->closecbs = new SbPList;
  SoXtWindowCloseCallbackInfo * info = new SoXtWindowCloseCallbackInfo;
  info->callback = callback;
  info->closure = closure;
  PRIVATE(this)->closecbs->append(info);
}

/*!
  This method removes a close callback for the component window.

  This method is not standard for Open Inventor.

  \sa SoXtComponent::addWindowCloseCallback()
  \sa SoXtComponent::setWindowCloseCallback()
  \sa SoXtComponent::invokeWindowCloseCallback()
*/

void
SoXtComponent::removeWindowCloseCallback(
  SoXtComponentCB * callback,
  void * closure)
{
  if (PRIVATE(this)->closecbs != NULL) {
    const int num = PRIVATE(this)->closecbs->getLength();
    for (int i = 0; i < num; i++) {
      SoXtWindowCloseCallbackInfo * info =
        (SoXtWindowCloseCallbackInfo *) (*PRIVATE(this)->closecbs)[i];
      if (info->callback == callback && info->closure == closure) {
        PRIVATE(this)->closecbs->remove(i);
        delete info;
        return;
      }
    }
  }
#if SOXT_DEBUG
  SoDebugError::post("SoXtComponent::removeWindowCloseCallback",
    "trying to remove nonexisting callback");
#endif // SOXT_DEBUG
}

/*!
  This method invokes the close callbacks for the component window.

  This method is not standard for Open Inventor.

  \sa SoXtComponent::addWindowCloseCallback()
  \sa SoXtComponent::setWindowCloseCallback()
  \sa SoXtComponent::removeWindowCloseCallback()
*/

void
SoXtComponent::invokeWindowCloseCallbacks(// protected
  void) const
{
  // FIXME: close callbacks never actually invoked from anywhere?
  // 20020503 mortene.

  if (PRIVATE(this)->closecbs == NULL)
    return;
  const int num = PRIVATE(this)->closecbs->getLength();
  for (int i = 0; i < num; i++) {
    SoXtWindowCloseCallbackInfo * info =
      (SoXtWindowCloseCallbackInfo *) (*PRIVATE(this)->closecbs)[i];
    // Cast required for AIX
    info->callback(info->closure, (SoXtComponent *) this);
  }
}

// *************************************************************************

/*!
  This method returns a pointer to the SoXtComponent object which the
  \a widget argument is registered for.
*/

SoXtComponent *
SoXtComponent::getComponent(// static
  Widget const widget)
{
  assert(SoXtComponentP::widgets != NULL);
  int pos = SoXtComponentP::widgets->find((void *) widget);
  if (pos == -1)
    return NULL;
  return (SoXtComponent *) (*SoXtComponentP::components)[pos];
}

/*!
  This method returns the name of the component.
*/

const char *
SoXtComponent::getWidgetName(
  void) const
{
  return PRIVATE(this)->widgetname ? PRIVATE(this)->widgetname : this->getDefaultWidgetName();
}

/*!
  This method returns the class name of the component.
*/

const char *
SoXtComponent::getClassName(
  void) const
{
  return PRIVATE(this)->widgetclass;
}

/*!
  This method sets the base widget of the component.
*/
void         // protected
SoXtComponent::setBaseWidget(Widget widget)
{
  const EventMask events = StructureNotifyMask | VisibilityChangeMask;

//  ExposureMask | VisibilityChangeMask | ResizeRedirectMask |
//    FocusChangeMask;
//    EnterWindowMask | LeaveWindowMask |

  if (PRIVATE(this)->widget) {
    // remove event handler
  }

  PRIVATE(this)->widget = widget;

  // really resize widget?  after all, size has been touched...
  if (PRIVATE(this)->size[0] != -1)
    XtVaSetValues(PRIVATE(this)->widget, XtNwidth, PRIVATE(this)->size[0], NULL);
  if (PRIVATE(this)->size[1] != -1)
    XtVaSetValues(PRIVATE(this)->widget, XtNheight, PRIVATE(this)->size[1], NULL);
  // register widget?

  XtInsertEventHandler(PRIVATE(this)->widget, events, False,
    SoXtComponent::event_handler, (XtPointer) this, XtListTail);
}

/*!
  This method sets the class name of the widget.
*/

void
SoXtComponent::setClassName(// protected
  const char * const name)
{
  if (PRIVATE(this)->widgetclass && strlen(PRIVATE(this)->widgetclass) >= strlen(name)) {
    strcpy(PRIVATE(this)->widgetclass, (char *) name);
  } else {
    delete [] PRIVATE(this)->widgetclass;
    PRIVATE(this)->widgetclass = strcpy(new char [strlen(name)+1], name);
  }
}

/*!
  This method should be hooked up to window close events.
  If the main window is closed, the program exits.  If a sub-window is
  closed, it is just hidden.
*/

void
SoXtComponent::windowCloseAction(// virtual, protected
  void)
{
  if (this->getShellWidget() == SoXt::getTopLevelWidget()) {
    XtAppSetExitFlag(SoXt::getAppContext());
  } else {
    this->hide();
  }
}

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoXtComponent::afterRealizeHook(void)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtComponent::afterRealizeHook", "invoked");
#endif // SOXT_DEBUG
  if (this->isTopLevelShell()) {

    XtVaSetValues(this->getShellWidget(),
                  XmNtitle, this->getTitle(),
                  XmNiconName, this->getIconTitle(),
                  NULL);

    if (PRIVATE(this)->size[0] > 0) {
      XtVaSetValues(this->getShellWidget(),
                    XmNwidth, PRIVATE(this)->size[0],
                    XmNheight, PRIVATE(this)->size[1],
                    NULL);
    }
  }
}

// *************************************************************************

/*!
  This method registers the widget as part of the component.

  All components should at least register it's base widget.  This database
  is used by the SoXtResource class.

  \sa SoXtComponent::unregisterWidget()
*/

// FIXME: Should base widgets get registered when setBaseWidget is called?

void
SoXtComponent::registerWidget(// protected
  Widget widget)
{
  if (SoXtComponentP::widgets == NULL) {
    SoXtComponentP::widgets = new SbPList;
    SoXtComponentP::components = new SbPList;
  }
  SoXtComponentP::widgets->append((void *) widget);
  SoXtComponentP::components->append((void *) this);
}

/*!
  This method unregisters \a widget.

  \sa SoXtComponent::registerWidget()
*/

void
SoXtComponent::unregisterWidget(// protected
  Widget widget)
{
  assert(SoXtComponentP::widgets != NULL);
  assert(widget != NULL);
  int pos = SoXtComponentP::widgets->find((void *) widget);
  if (pos == -1) {
    SoDebugError::post("SoXtComponent::unregisterWidget",
      "widget (%s) not registered", XtName(widget));
  }
  assert(SoXtComponentP::components != NULL);
  SoXtComponentP::widgets->remove(pos);
  SoXtComponentP::components->remove(pos);
}

// *************************************************************************

/*!
  This method adds a callback that will be invoked when the components
  visibility changes.

  Callback invocation is not implemented yet.

  \sa SoXtComponent::removeVisibilityChangeCallback()
*/

void
SoXtComponent::addVisibilityChangeCallback(// protected
  SoXtComponentVisibilityCB * const callback,
  void * const closure)
{
  if (PRIVATE(this)->visibilitycbs == NULL)
    PRIVATE(this)->visibilitycbs = new SbPList;
  SoXtComponentVisibilityCallbackInfo * info =
    new SoXtComponentVisibilityCallbackInfo;
  PRIVATE(this)->visibilitycbs->append(info);
}

/*!
  This method removes a callback from the list of callbacks that are to be
  invoked when the component visibility changes.

  \sa SoXtComponent::addVisibilityChangeCallback()
*/

void
SoXtComponent::removeVisibilityChangeCallback(// protected
  SoXtComponentVisibilityCB * const callback,
  void * const closure)
{
  if (PRIVATE(this)->visibilitycbs != NULL) {
    const int num = PRIVATE(this)->visibilitycbs->getLength();
    for (int i = 0; i < num; i++) {
      SoXtComponentVisibilityCallbackInfo * info =
        (SoXtComponentVisibilityCallbackInfo *)
          (*PRIVATE(this)->visibilitycbs)[i];
      if (info->callback == callback && info->closure == closure) {
        PRIVATE(this)->visibilitycbs->remove(i);
        delete info;
        return;
      }
    }
  }
#if SOXT_DEBUG
  SoDebugError::post("SoXtComponent::removeVisibilityChangeCallback",
    "Tried to remove nonexistent callback.");
#endif // SOXT_DEBUG
}

/*!
  This method invokes all the visibility-change callbacks.

  This method is not part of the Open Inventor API.

  \sa SoXtComponent::addVisibilityChangeCallback()
  \sa SoXtComponent::removeVisibilityChangeCallback()
*/

void
SoXtComponent::invokeVisibilityChangeCallbacks(// protected
  const SbBool enable) const
{
  if (PRIVATE(this)->visibilitycbs == NULL)
    return;
  const int num = PRIVATE(this)->visibilitycbs->getLength();
  for (int i = 0; i < num; i++) {
    SoXtComponentVisibilityCallbackInfo * info =
      (SoXtComponentVisibilityCallbackInfo *)
        (*PRIVATE(this)->visibilitycbs)[i];
    info->callback(info->closure, enable);
  }
}

// *************************************************************************

/*!
  This method is used to open component help cards.  \a name is the name of
  a file that will be searched for in the following path:

  .:$SO_HELP_DIR:/{prefix}/share/Coin/help

  If no card is found, an error dialog will appear.

  This method is not implemented yet.

  \sa SoXtViewer::openViewerHelpCard()
*/

void         // protected
SoXtComponent::openHelpCard(const char * const name)
{
  SoXt::createSimpleErrorDialog(this->getWidget(),
                                "Not Implemented",
                                "Help Card functionality is not implemented yet.");
}

// *************************************************************************

/*!
  This function is not implemented yet.
*/

char *
SoXtComponent::getlabel(// static, protected
  unsigned int what)
{
  SOXT_STUB();
  return "(null)";
}

// *************************************************************************

/*!
  FIXME: write doc
*/
Boolean      // protected, virtual
SoXtComponent::sysEventHandler(Widget widget, XEvent * event)
{
  if (widget == PRIVATE(this)->widget) { // base widget
#if SOXT_DEBUG && 0
    SoDebugError::postInfo("SoXtComponent::sysEventHandler",
                           "base widget event (%d)", event->type);
#endif // SOXT_DEBUG
    if (event->type == ConfigureNotify) {
      XConfigureEvent * conf = (XConfigureEvent *) event;
      if (PRIVATE(this)->size != SbVec2s(conf->width, conf->height)) {
        PRIVATE(this)->size = SbVec2s(conf->width, conf->height);
        this->sizeChanged(PRIVATE(this)->size);
      }
    } else if (event->type == MapNotify) {
      Dimension width = 0, height = 0;
      XtVaGetValues(this->getBaseWidget(),
        XmNwidth, &width,
        XmNheight, &height,
        NULL);
      PRIVATE(this)->size = SbVec2s(width, height);
      this->sizeChanged(PRIVATE(this)->size);
    } else if (event->type == VisibilityNotify) {
//    SoDebugError::postInfo("SoXtComponent::sysEventHandler", "Visibility");
      XVisibilityEvent * visibility = (XVisibilityEvent *) event;
      SbBool newvisibility = TRUE;
      if (visibility->state == VisibilityFullyObscured)
        newvisibility = FALSE;
      if (PRIVATE(this)->visibilitystate != newvisibility) {
        PRIVATE(this)->visibilitystate = newvisibility;
        this->invokeVisibilityChangeCallbacks(PRIVATE(this)->visibilitystate);
      }
    }
  } else if (this->isTopLevelShell() && widget == this->getShellWidget()) {
#if SOXT_DEBUG && 0
    SoDebugError::postInfo("SoXtComponent::sysEventHandler",
                           "shell widget event (%d)", event->type);
#endif // SOXT_DEBUG

    if (event->type == ConfigureNotify) {
      XConfigureEvent * conf = (XConfigureEvent *) event;
      if (PRIVATE(this)->size != SbVec2s(conf->width, conf->height)) {
        PRIVATE(this)->size = SbVec2s(conf->width, conf->height);
        XtVaSetValues(this->getBaseWidget(),
          XmNwidth, PRIVATE(this)->size[0],
          XmNheight, PRIVATE(this)->size[1],
          NULL);
        this->sizeChanged(PRIVATE(this)->size);
      }
    }
  } else {
#if SOXT_DEBUG && 0
    SoDebugError::postInfo("SoXtComponent::sysEventHandler",
                           "[removing] event handler for unknown widget");
#endif // SOXT_DEBUG
  }
  return True;
}

/*!
  This static callback invokes SoXtComponent::sysEventHandler.

  \sa sysEventHandler
*/
void
SoXtComponent::event_handler(Widget widget,
                             XtPointer closure,
                             XEvent * event,
                             Boolean * dispatch)
{
  assert(closure != NULL);
  SoXtComponent * component = (SoXtComponent *) closure;
  *dispatch = component->sysEventHandler(widget, event);
}

/*!
  Toggle full screen mode for this component, if possible.

  Returns \c FALSE if operation failed.  This might happen if the
  toolkit doesn't support attempts at making the component cover the
  complete screen or if the component is not a toplevel window.
*/
SbBool
SoXtComponent::setFullScreen(const SbBool onoff)
{
  if (onoff == PRIVATE(this)->fullscreen) { return TRUE; }
  SOXT_STUB();
  return FALSE;
}

/*!
  Returns if this widget/component is in full screen mode.
*/
SbBool
SoXtComponent::isFullScreen(void) const
{
  return PRIVATE(this)->fullscreen;
}

// Converts from the common generic cursor format to a X11 Cursor
// instance.
Cursor
SoXtComponentP::getNativeCursor(Display * d,
                                const SoXtCursor::CustomCursor * cc)
{
  if (SoXtComponentP::cursordict == NULL) { // first call, initialize
    SoXtComponentP::cursordict = new SbDict; // FIXME: mem leak. 20011121 mortene.
  }

  void * qc;
  SbBool b = SoXtComponentP::cursordict->find((unsigned long)cc, qc);
  if (b) { return (Cursor)qc; }

  // FIXME: translate from bitmap to native format. 20011127 mortene.

  // FIXME: currently a memory leak here. 20011121 mortene.
  Cursor c = XCreateFontCursor(d, XC_hand2);
  // FIXME: use a better dict class -- the void* cast is ugly.
  // 20011127 mortene.
  assert(sizeof(Cursor) <= sizeof(void*));
  SoXtComponentP::cursordict->enter((unsigned long)cc, (void *)c);
  return c;
}

/*!
  Sets the cursor for this component.
*/
void 
SoXtComponent::setComponentCursor(const SoXtCursor & cursor)
{
  SoXtComponent::setWidgetCursor(this->getWidget(), cursor);
}

/*!
  Set cursor for a native widget in the underlying toolkit.
*/
void
SoXtComponent::setWidgetCursor(Widget w, const SoXtCursor & cursor)
{
  Window win = XtWindow(w);
  if (win == (Window)NULL) { return; } // widget probably not realized yet

  Display * d = SoXt::getDisplay();

  if (cursor.getShape() == SoXtCursor::CUSTOM_BITMAP) {
    const SoXtCursor::CustomCursor * cc = &cursor.getCustomCursor();
    XDefineCursor(d, win, SoXtComponentP::getNativeCursor(d, cc));
  }
  else {
    Cursor c;
    switch (cursor.getShape()) {
    case SoXtCursor::DEFAULT:
      XUndefineCursor(d, win);
      break;

    case SoXtCursor::BUSY:
      // FIXME: plug memory leak. 20011127 mortene.
      c = XCreateFontCursor(d, XC_clock);
      XDefineCursor(d, win, c);
      break;

    case SoXtCursor::CROSSHAIR:
      // FIXME: plug memory leak. 20011127 mortene.
      c = XCreateFontCursor(d, XC_crosshair);
      XDefineCursor(d, win, c);
      break;

    case SoXtCursor::UPARROW:
      // FIXME: plug memory leak. 20011127 mortene.
      c = XCreateFontCursor(d, XC_based_arrow_up);
      // FIXME: perhaps this one is better?:  20011127 mortene.
//        Cursor c = XCreateFontCursor(d, XC_sb_up_arrow);
      XDefineCursor(d, win, c);
      break;

    default:
      assert(FALSE && "unsupported cursor shape type");
      break;
    }
  }
}

// *************************************************************************
