/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#include "SoXtLinuxMagellan.h"

SoXtLinuxMagellan::SoXtLinuxMagellan()
{
    // these are empirically good default values
    rotScale = .010;
    transScale = .001;
}

////////////////////////////////////////////////////////////////////////
//
// This is where the real constructor work gets done.
//
// private
//
void SoXtLinuxMagellan::init(Display *display, SoXtMagellan::Mask)
//
////////////////////////////////////////////////////////////////////////
{
    if (display == NULL)
    {
        return;
    }
    MagellanWindow = InputFocus;
    /* Define the Magellan Event types */
    MagellanMotionEvent = XInternAtom(display, "MotionEvent", TRUE);
    MagellanButtonPressEvent = XInternAtom(display, "ButtonPressEvent", TRUE);
    MagellanButtonReleaseEvent = XInternAtom(display, "ButtonReleaseEvent", TRUE);
    MagellanCommandEvent = XInternAtom(display, "CommandEvent", TRUE);

    /* TRUE if Driver exist */
    MagellanExist = (MagellanMotionEvent != 0) && (MagellanButtonPressEvent != 0) && (MagellanButtonReleaseEvent != 0) && (MagellanCommandEvent != 0);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   Constructor.
//
// public
//
SoXtLinuxMagellan::~SoXtLinuxMagellan()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   This selects input for spaceball device events which occur in w.
// The callback routine is proc, and the callback data is clientData.
//
// virtual public
//
void
SoXtLinuxMagellan::enable(
    Widget w,
    XtEventHandler proc,
    XtPointer clientData,
    Window window)
//
////////////////////////////////////////////////////////////////////////
{
    XtAddEventHandler(w, 0, TRUE, proc, clientData);
    MagellanExist = MagellanSetWindow(SoXt::getDisplay(), window);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   This unselects input for spaceball device events which occur in w,
// i.e. spaceball events will no longer be recognized.
//
// virtual public
//
void
SoXtLinuxMagellan::disable(
    Widget w,
    XtEventHandler proc,
    XtPointer clientData)
//
////////////////////////////////////////////////////////////////////////
{
    (void)w;
    (void)clientData;
    (void)proc;
    MagellanSetWindow(SoXt::getDisplay(), (Window)InputFocus);
    MagellanExist = FALSE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   This returns an SoEvent for the passed X event, if the event
// was generated by the mouse device.
//
// virtual public
//
const SoEvent *SoXtLinuxMagellan::translateEvent(XAnyEvent *xevent)
//
////////////////////////////////////////////////////////////////////////
{
    MagellanFloatEvent MagellanEvent;
    SoButtonEvent::State whichState = SoButtonEvent::UP;

    if (xevent->type != ClientMessage)
        return NULL;

    switch (MagellanTranslateEvent(SoXt::getDisplay(), (XEvent *)xevent, &MagellanEvent, 1.0, 1.0))
    {
    case MagellanInputMotionEvent:
    {

        //fprintf(stderr,"x=%+5.0lf y=%+5.0lf z=%+5.0lf a=%+5.0lf b=%+5.0lf c=%+5.0lf\n",
        //   MagellanEvent.MagellanData[ MagellanX ],
        //   MagellanEvent.MagellanData[ MagellanY ],
        //   MagellanEvent.MagellanData[ MagellanZ ],
        //   MagellanEvent.MagellanData[ MagellanA ],
        //   MagellanEvent.MagellanData[ MagellanB ],
        //   MagellanEvent.MagellanData[ MagellanC ] );

        motionEvent->setTime(SbTime::getTimeOfDay());

        SbVec3f trans(MagellanEvent.MagellanData[MagellanX], MagellanEvent.MagellanData[MagellanY], -MagellanEvent.MagellanData[MagellanZ]);
        trans *= transScale;
        motionEvent->setTranslation(trans);

        float quat[4], q[3], cy, cp, cr, sy, sp, sr;

        const int ROLL = 0, PITCH = 1, YAW = 2;
        const int X = 0, Y = 1, Z = 2, W = 3;

        // halve and convert to radians
        q[ROLL] = M_PI / 180 * rotScale * MagellanEvent.MagellanData[MagellanA];
        q[PITCH] = M_PI / 180 * rotScale * MagellanEvent.MagellanData[MagellanB];
        q[YAW] = -M_PI / 180 * rotScale * MagellanEvent.MagellanData[MagellanC];

        cr = cos(q[ROLL]); //roll  = psi   = x-axis
        sr = sin(q[ROLL]);
        cp = cos(q[PITCH]); //pitch = theta = y-axis
        sp = sin(q[PITCH]);
        cy = cos(q[YAW]); //yaw   = phi   = z-axis
        sy = sin(q[YAW]);

        quat[W] = cr * cp * cy + sr * sp * sy;
        quat[X] = sr * cp * cy - cr * sp * sy;
        quat[Y] = cr * sp * cy + sr * cp * sy;
        quat[Z] = cr * cp * sy - sr * sp * cy;

        motionEvent->setRotation(SbRotation(quat));
        return motionEvent;
    }

    case MagellanInputButtonPressEvent:
        whichState = SoButtonEvent::DOWN;
    // fall through!
    case MagellanInputButtonReleaseEvent:
    {
        SoSpaceballButtonEvent::Button button;
        //fprintf(stderr," [%c] ",
        //   MagellanEvent.MagellanButton == 9 ? '*' : '0'+MagellanEvent.MagellanButton );
        buttonEvent->setTime(SbTime::getTimeOfDay());
        buttonEvent->setState(whichState);
        switch (MagellanEvent.MagellanButton)
        {
        case 1:
            button = SoSpaceballButtonEvent::BUTTON1;
            break;
        case 2:
            button = SoSpaceballButtonEvent::BUTTON2;
            break;
        case 3:
            button = SoSpaceballButtonEvent::BUTTON3;
            break;
        case 4:
            button = SoSpaceballButtonEvent::BUTTON4;
            break;
        case 5:
            button = SoSpaceballButtonEvent::BUTTON5;
            break;
        case 6:
            button = SoSpaceballButtonEvent::BUTTON6;
            break;
        case 7:
            button = SoSpaceballButtonEvent::BUTTON7;
            break;
        case 8:
            button = SoSpaceballButtonEvent::BUTTON8;
            break;
        case 9:
            button = SoSpaceballButtonEvent::PICK;
            break;
        default:
            button = SoSpaceballButtonEvent::BUTTON1;
            fprintf(stderr, "unhandled SoXtLinuxMagellan::translateEvent(): MagellanButton\n");
            break;
        }
        buttonEvent->setButton(button);
        return buttonEvent;
    }
    default: /* another ClientMessage event */
        break;
    }

    return NULL;
}

int SoXtLinuxMagellan::MagellanErrorHandler(Display *display, XErrorEvent *Error)
{
    char Buffer[128];

    switch (Error->error_code)
    {
    case BadWindow:
        break;

    default:
        XGetErrorText(display, Error->error_code, Buffer, sizeof(Buffer));
        fprintf(stderr, "Magellan: Error = %s. Exit ... \n", Buffer);
        break;
    };
    return 0;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   This returns an SoMotion3Event for the passed X event.
//
// private
//
SoMotion3Event *
SoXtLinuxMagellan::translateMotionEvent(XDeviceMotionEvent *me)
//
////////////////////////////////////////////////////////////////////////
{
    (void)me;
    return motionEvent;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   This returns an SoSpaceballButtonEvent for the passed X event.
//
// private
//
SoSpaceballButtonEvent *
SoXtLinuxMagellan::translateButtonEvent(XDeviceButtonEvent *be,
                                        SoButtonEvent::State whichState)
//
////////////////////////////////////////////////////////////////////////
{
    (void)be;
    (void)whichState;
    return buttonEvent;
}

int SoXtLinuxMagellan::MagellanSetWindow(Display *display, Window window)
{

    XTextProperty MagellanWindowName;
    XEvent CommandMessage;
    Atom ActualType;
    int ActualFormat;
    unsigned long NItems, BytesReturn;
    unsigned char *PropReturn;
    Window root;
    int result;
    int (*ApplicationErrorHandler)(
        Display *,
        XErrorEvent *);

    result = TRUE;
    ApplicationErrorHandler = XSetErrorHandler(MagellanErrorHandler);

    /* Read the window of the Magellan Driver */
    root = RootWindow(display, DefaultScreen(display));

    PropReturn = NULL;
    XGetWindowProperty(display, root, MagellanCommandEvent, 0, 1, FALSE,
                       AnyPropertyType, &ActualType, &ActualFormat, &NItems,
                       &BytesReturn, &PropReturn);

    MagellanWindow = InputFocus;
    if (PropReturn != NULL)
    {
        MagellanWindow = *(Window *)PropReturn;
        XFree(PropReturn);
    }
    else
        return result = FALSE;

    if (XGetWMName(display, MagellanWindow, &MagellanWindowName) == 0)
        return result = FALSE;

    if (strcmp((char *)"Magellan Window", (char *)MagellanWindowName.value) != 0)
        return result = FALSE;

    /* Send the application window to the Magellan X Window Driver */
    CommandMessage.type = ClientMessage;
    CommandMessage.xclient.format = 16;
    CommandMessage.xclient.send_event = FALSE;
    CommandMessage.xclient.display = display;
    CommandMessage.xclient.window = MagellanWindow;
    CommandMessage.xclient.message_type = MagellanCommandEvent;

    CommandMessage.xclient.data.s[0] = (short)XHigh32(window);
    CommandMessage.xclient.data.s[1] = (short)XLow32(window);
    CommandMessage.xclient.data.s[2] = CommandMessageApplicationWindow;

    if (XSendEvent(display, MagellanWindow, FALSE, 0x0000, &CommandMessage) == 0)
        return result = FALSE;

    XFlush(display);

    XSetErrorHandler(ApplicationErrorHandler);
    return result;
}

int SoXtLinuxMagellan::MagellanTranslateEvent(Display *display,
                                              XEvent *Event, MagellanFloatEvent *MagellanEvent,
                                              double MagellanTranslationScale, double MagellanRotationScale)
{
    if (MagellanExist == FALSE)
        return FALSE;

    if (Event->type == ClientMessage)
    {
        if (Event->xclient.message_type == MagellanMotionEvent)
        {
            MagellanEvent->MagellanType = MagellanInputMotionEvent;
            MagellanEvent->MagellanData[MagellanX] = Event->xclient.data.s[2] * MagellanTranslationScale;
            MagellanEvent->MagellanData[MagellanY] = Event->xclient.data.s[3] * MagellanTranslationScale;
            MagellanEvent->MagellanData[MagellanZ] = Event->xclient.data.s[4] * MagellanTranslationScale;
            MagellanEvent->MagellanData[MagellanA] = Event->xclient.data.s[5] * MagellanRotationScale;
            MagellanEvent->MagellanData[MagellanB] = Event->xclient.data.s[6] * MagellanRotationScale;
            MagellanEvent->MagellanData[MagellanC] = Event->xclient.data.s[7] * MagellanRotationScale;
            MagellanEvent->MagellanPeriod = Event->xclient.data.s[8];
            return MagellanInputMotionEvent;
        }
        else if (Event->xclient.message_type == MagellanButtonPressEvent)
        {
            MagellanEvent->MagellanType = MagellanInputButtonPressEvent;
            MagellanEvent->MagellanButton = Event->xclient.data.s[2];
            return MagellanInputButtonPressEvent;
        }
        else if (Event->xclient.message_type == MagellanButtonReleaseEvent)
        {
            MagellanEvent->MagellanType = MagellanInputButtonReleaseEvent;
            MagellanEvent->MagellanButton = Event->xclient.data.s[2];
            return MagellanInputButtonReleaseEvent;
        };
    };
    return display == NULL;
}
