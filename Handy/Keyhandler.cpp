#include <ApplicationServices/ApplicationServices.h>

#include "Keyhandler.h"

void Keyhandler::pressKey(int keycode, bool shift=false, bool ctrl=false, bool option=false, bool cmd=false) {
	static const CGKeyCode COMMAND = 0x37;
	static const CGKeyCode SHIFT = 0x38;
	static const CGKeyCode OPTION = 0x3A;
	static const CGKeyCode CONTROL = 0x3B;
    ProcessSerialNumber psn;
    GetFrontProcess(&psn);
    CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
    CGEventRef press = CGEventCreateKeyboardEvent(source, (CGKeyCode)keycode, true);
    CGEventRef release = CGEventCreateKeyboardEvent(source, (CGKeyCode)keycode, false);

    CGEventRef pcmd, rcmd, pshift, rshift, poption, roption, pctrl, rctrl;

    if (cmd) {
    	pcmd = CGEventCreateKeyboardEvent(source, COMMAND, true);
	rcmd = CGEventCreateKeyboardEvent(source, COMMAND, false);
	CGEventPost(kCGHIDEventTap, pcmd);
    }
    if (shift) {
    	pshift = CGEventCreateKeyboardEvent(source, SHIFT, true);
	rshift = CGEventCreateKeyboardEvent(source, SHIFT, false);
	CGEventPost(kCGHIDEventTap, pcmd);
    }
    if (option) {
    	poption = CGEventCreateKeyboardEvent(source, OPTION, true);
	roption = CGEventCreateKeyboardEvent(source, OPTION, false);
	CGEventPost(kCGHIDEventTap, pcmd);
    }
    if (ctrl) {
    	pctrl = CGEventCreateKeyboardEvent(source, CONTROL, true);
	rctrl = CGEventCreateKeyboardEvent(source, CONTROL, false);
	CGEventPost(kCGHIDEventTap, pcmd);
    }

    CGEventPost(kCGHIDEventTap, press);
    CGEventPost(kCGHIDEventTap, release);

    if (cmd) {
    	CGEventPost(kCGHIDEventTap, rcmd);
	CFRelease(pcmd);
	CFRelease(rcmd);
    }
    if (shift) {
    	CGEventPost(kCGHIDEventTap, rshift);
	CFRelease(pshift);
	CFRelease(rshift);
    }
    if (option) {
    	CGEventPost(kCGHIDEventTap, roption);
	CFRelease(poption);
	CFRelease(roption);
    }
    if (ctrl) {
    	CGEventPost(kCGHIDEventTap, rctrl);
	CFRelease(pctrl);
	CFRelease(rctrl);
    }

    CFRelease(press);
    CFRelease(release);
    CFRelease(source);
}
