//
//  corePlayerUITests.m
//  corePlayerUITests
//
//  Created by Jun Lin on 18/11/2018.
//  Copyright © 2018 qiniu. All rights reserved.
//

#import <XCTest/XCTest.h>

@interface corePlayerUITests : XCTestCase
{
    XCUIApplication* 	_app;
    XCUIDevice*			_device;
}
@end

@implementation corePlayerUITests

- (void)setUp {
    [super setUp];
    
    // Put setup code here. This method is called before the invocation of each test method in the class.
    
    // In UI tests it is usually best to stop immediately when a failure occurs.
    self.continueAfterFailure = NO;
    // UI tests must launch the application that they test. Doing this in setup will make sure it happens for each test method.
    _device = [[XCUIDevice alloc] init];
    _app = [[XCUIApplication alloc] init];
    [_app launch];

    
    // In UI tests it’s important to set the initial state - such as interface orientation - required for your tests before they run. The setUp method is a good place to do this.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
    _app = nil;
    _device = nil;
}

-(int) getSysTime
{
    return ((long long)[[NSProcessInfo processInfo] systemUptime] * 1000) & 0x7FFFFFFF;
}

-(int) makeRand
{
    // 100 ms ~ 15 secs
#if 1
    unsigned int Min = 5000;
    unsigned int Max = 1000*10;
#else
    unsigned int Min = 1000;
    unsigned int Max = 1000;
#endif
    return (int)((double)rand()/(double)RAND_MAX*(Max-Min+1)+Min);
}

- (void)testExample {
    // Use recording to get started writing UI tests.
    // Use XCTAssert and related functions to verify your tests produce the correct results.

    while(true)
    {
        [_device pressButton:XCUIDeviceButtonHome];
        usleep(1000*[self makeRand]);
        [_app activate];
        usleep(1000*[self makeRand]);
        [[_device siriService] activateWithVoiceRecognitionText:@"你认识乔布斯吗"];
        usleep(1000*[self makeRand]);
    }
}




@end
