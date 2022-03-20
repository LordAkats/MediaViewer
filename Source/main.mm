//
//  main.m
//  GUI Application
//
//  Created by josh on 6/14/21.
//  Copyright © 2021 josh. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#undef main

int main(int argc, const char * argv[]) {
    [NSWindow setAllowsAutomaticWindowTabbing: NO];
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"NSDisabledDictationMenuItem"];
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"NSDisabledCharacterPaletteMenuItem"];
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
    }
    return NSApplicationMain(argc, argv);
}

#define main cppmain
