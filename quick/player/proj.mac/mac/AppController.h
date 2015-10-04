
#import <Cocoa/Cocoa.h>

#import "ConsoleWindowController.h"
#include "ProjectConfig/ProjectConfig.h"
#include "AppDelegate.h"

@interface AppController : NSObject <NSApplicationDelegate, NSWindowDelegate, NSFileManagerDelegate>
{
    NSWindow *_window;
    BOOL _isAlwaysOnTop;
    AppDelegate *_app;
    ProjectConfig _project;
    int _debugLogFile;

    //log file
    ConsoleWindowController *_consoleController;
    NSFileHandle *_fileHandle;
    //console pipe
    NSPipe *_pipe;
    NSFileHandle *_pipeReadHandle;

    // build task
    NSTask *_buildTask;
    bool _isBuildingFinished;
    NSAlert *_buildAlert;
}

@property (nonatomic, assign) IBOutlet NSMenu* menu;

- (void) relaunch:(NSArray*)args;

@end
