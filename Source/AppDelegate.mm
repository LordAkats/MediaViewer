//
//  AppDelegate.m
//  GUI Application
//
//  Created by josh on 6/14/21.
//  Copyright © 2021 josh. All rights reserved.
//

//
//  AppDelegate.m
//  AppKit
//
//  Created by Josh on 6/4/21.
//

#import "AppDelegate.h"
#import <vector>
#import <crt_externs.h>
#import "CocoaInterface.h"

extern int cppmain(int argc, const char * argv[]);

@interface AppDelegate ()

@property (strong) IBOutlet NSWindow *window;
@property NSApplicationTerminateReply terminationReply;

@end

NSApplicationTerminateReply terminationReply = NSTerminateCancel;

@implementation AppDelegate

-(BOOL) validateMenuItem:(NSMenuItem*)item {
    return YES;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    self.terminationReply = NSTerminateCancel;
    int argc = *_NSGetArgc();
    char** argv = *_NSGetArgv();
    cppmain(argc, (const char**)argv);
    self.terminationReply = NSTerminateNow;
    [NSApp terminate:self];
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
    CocoaInterface_Event ev = {};
    ev.type = COCOAEVENT_APPTERMINATE;
    ev.source = (__bridge void*) self;
    if (CocoaInterface_EmitEventCallback != NULL) CocoaInterface_EmitEventCallback(ev);
    //CocoaInterface_Event::queue->push_back(ev);
    NSEvent* dummyEvent = [NSEvent
        otherEventWithType: NSEventTypeApplicationDefined
                  location: NSZeroPoint
             modifierFlags: 0
                 timestamp: 0
              windowNumber: 0
                   context: nil
                   subtype:0
                     data1:0
                     data2:0];
    [NSApp postEvent: dummyEvent atStart: TRUE];
    return self.terminationReply;
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

@end
