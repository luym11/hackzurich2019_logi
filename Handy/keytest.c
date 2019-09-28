#include <stdio.h>
#include <ApplicationServices/ApplicationServices.h>

int main()
{
    ProcessSerialNumber psn;
    GetFrontProcess(&psn);
    CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);//CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    CGEventRef pressB = CGEventCreateKeyboardEvent(source, (CGKeyCode)11, true);
    CGEventRef releaseB = CGEventCreateKeyboardEvent(source, (CGKeyCode)11, false);

    // simulate capslock down
    CGEventPost(kCGHIDEventTap, pressB);
    // simulate capslock up
    CGEventPost(kCGHIDEventTap, releaseB);

    CFRelease(pressB);
    CFRelease(releaseB);
    CFRelease(source);
    
    return 0;
}
