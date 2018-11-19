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
    XCUIElement* _btnFullscreen;
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

-(int)makeRand:(int)min Max:(int)max
{
    unsigned int Min = min;
    unsigned int Max = max;
    return (int)((double)rand()/(double)RAND_MAX*(Max-Min+1)+Min);
}

- (void)testExample {
    // Use recording to get started writing UI tests.
    // Use XCTAssert and related functions to verify your tests produce the correct results.

#if 1 //测试app切换前后台
    while(true)
    {
        [_device pressButton:XCUIDeviceButtonHome];
        usleep(1000*[self makeRand:5000 Max:1000*10]);
        [_app activate];
        usleep(1000*[self makeRand:5000 Max:1000*10]);
        [[_device siriService] activateWithVoiceRecognitionText:@"你认识乔布斯吗"];
        usleep(1000*[self makeRand:5000 Max:1000*10]);
    }
#else //测试动画效果切换全屏/非全屏
    _btnFullscreen = _app.buttons[@"FULL SCREEN"];
    while(true)
    {
        if(_btnFullscreen)
            [_btnFullscreen tap];
        usleep(1000*[self makeRand:1000 Max:5000]);
    }
#endif
}

@end
