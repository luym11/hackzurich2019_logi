#include <ApplicationServices/ApplicationServices.h>

#include "Keyhandler.h"

void Keyhandler::pressKey(int keycode, bool shift, bool ctrl, bool option, bool cmd) {
	static const CGKeyCode COMMAND = 0x37;
	static const CGKeyCode SHIFT = 0x38;
	static const CGKeyCode OPTION = 0x3A;
	static const CGKeyCode CONTROL = 0x3B;
    ProcessSerialNumber psn;
    GetFrontProcess(&psn);
    CGEventRef press = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)keycode, true);
    CGEventRef release = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)keycode, false);

	CGEventFlags flags = 0;
	if (cmd) flags |= kCGEventFlagMaskCommand;
	if (shift) flags |= kCGEventFlagMaskShift;
	if (option) flags |= kCGEventFlagMaskAlternate;
	if (ctrl) flags |= kCGEventFlagMaskControl;

	CGEventSetFlags(press, flags);

    CGEventPost(kCGHIDEventTap, press);
    CGEventPost(kCGHIDEventTap, release);

    CFRelease(press);
    CFRelease(release);
}
