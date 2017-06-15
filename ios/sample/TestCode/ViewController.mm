//
//  ViewController.m
//  TestCode
//
//  Created by Jun Lin on 3/02/17.
//  Copyright © 2017 qiniu. All rights reserved.
//

#import "ViewController.h"
#include "qcPlayer.h"
#include "qcData.h"
#import <AVFoundation/AVFoundation.h>

@interface ViewController () <UITableViewDataSource, UITableViewDelegate>
{
    QCM_Player      _player;
    BOOL            _isFullScreen;
    BOOL            _isDragSlider;
    
    NSMutableArray* _urlList;
    NSInteger       _currURL;
    
    UIView*         _viewVideo;
    CGRect          _rectSmallScreen;
    CGRect          _rectFullScreen;
    
    UITableView*    _tableViewURL;
    UITableView*    _tableViewStreamInfo;
    UISlider*       _sliderPosition;
    UILabel*        _labelPlayingTime;
    UISwitch*       _switchHW;
    UILabel*        _labelHW;
    UILabel*        _labelVersion;
    UIButton*       _btnCancelSelectStream;
    
    NSTimer*        _timer;
    
    UITapGestureRecognizer* _tapGesture;
    
    NSInteger		_networkConnectionErrorTime;
    NSString*		_clipboardURL;
}
@end

@implementation ViewController

#pragma mark Player event callback
void NotifyEvent (void * pUserData, int nID, void * pValue1)
{
    ViewController* vc = (ViewController*)pUserData;
    [vc onPlayerEvent:nID withParam:pValue1];
}

- (void)onPlayerEvent:(int)nID withParam:(void*)pParam
{
    if (nID == QC_MSG_PLAY_OPEN_DONE)
    {
        NSLog(@"Run\n");
        if(_player.hPlayer)
            _player.Run(_player.hPlayer);
        _btnStart.enabled = ![self isLive];
        _sliderPosition.enabled = ![self isLive];
    }
    else if(nID == QC_MSG_PLAY_OPEN_FAILED)
    {
        [[NSOperationQueue mainQueue] addOperationWithBlock:^{
            [self showMessage:@"Open fail" duration:2.0];
        }];
    }
    else if (nID == QC_MSG_PLAY_COMPLETE)
    {
        [[NSOperationQueue mainQueue] addOperationWithBlock:^{
            [self onStop: _btnStart];
        }];
    }
    else if (nID == QC_MSG_PLAY_SEEK_DONE)
    {
        NSLog(@"Seek done\n");
    }
    else if (nID == QC_MSG_HTTP_DISCONNECTED || nID == QC_MSG_HTTP_RECONNECT_FAILED)
    {
        if(_networkConnectionErrorTime == -1)
        {
            NSLog(@"Connect lost, %x\n", nID);
            _networkConnectionErrorTime = [self getSysTime];
            
            [[NSOperationQueue mainQueue] addOperationWithBlock:^{
                [self showMessage:@"Connection loss" duration:5.0];
            }];
        }
        
        //
        if(([self getSysTime]-_networkConnectionErrorTime) > 300*1000)
        {
            [[NSOperationQueue mainQueue] addOperationWithBlock:^{
                [[NSOperationQueue mainQueue] addOperationWithBlock:^{
                    [self showMessage:@"Stop player" duration:5.0];
                }];

                [self onStop: _btnStart];
                _networkConnectionErrorTime = -1;
            }];
        }
    }
    else if (nID == QC_MSG_HTTP_RECONNECT_SUCESS)
    {
        NSLog(@"Connect recovered\n");
        _networkConnectionErrorTime = -1;
        
        [[NSOperationQueue mainQueue] addOperationWithBlock:^{
            [self showMessage:@"Connection recovered" duration:5.0];
        }];
    }
}

-(void) createPlayer
{
    if(_player.hPlayer)
        return;
    
    qcCreatePlayer(&_player, NULL);
    _player.SetNotify(_player.hPlayer, NotifyEvent, self);
    _player.SetView(_player.hPlayer, (void*)_viewVideo, NULL);
}

-(void) destroyPlayer
{
    if(!_player.hPlayer)
        return;
    qcDestroyPlayer(&_player);
    _player.hPlayer = NULL;
}

#pragma mark setup UI
- (void)parseStream:(NSString *)html
{
    NSScanner* theScanner;
    NSString* text = nil;
    NSString* keyString = @"?stream=";
    
    [_urlList insertObject:@"" atIndex:0];
    theScanner = [NSScanner scannerWithString:html];
    
    while ([theScanner isAtEnd] == NO)
    {
        BOOL res = [theScanner scanUpToString:keyString intoString:nil] ;
        if(res && [theScanner isAtEnd] == NO)
        {
            theScanner.scanLocation += [keyString length];
            res = [theScanner scanUpToString:@"\"" intoString:&text] ;
            
            if(text && res)
            {
                [_urlList insertObject:[NSString stringWithFormat:@"http://pili-live-hls.pili2test.qbox.net/pili2test/%@.m3u8", text] atIndex:0];
                [_urlList insertObject:[NSString stringWithFormat:@"http://pili-live-hdl.pili2test.qbox.net/pili2test/%@.flv", text] atIndex:0];
                [_urlList insertObject:[NSString stringWithFormat:@"rtmp://pili-live-rtmp.pili2test.qbox.net/pili2test/%@", text] atIndex:0];
            }
        }
    }
}

-(void)parseDemoLive
{
    NSString *host = @"http://pili2-demo.qiniu.com";
    NSString *method = @"GET";
    
    NSString *url = [NSString stringWithFormat:@"%@",  host];
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString: url]  cachePolicy:NSURLRequestReloadIgnoringLocalCacheData  timeoutInterval:  5];
    request.HTTPMethod  =  method;
    NSURLSession *requestSession = [NSURLSession sessionWithConfiguration:[NSURLSessionConfiguration defaultSessionConfiguration]];
    NSURLSessionDataTask *task = [requestSession dataTaskWithRequest:request
                                                   completionHandler:^(NSData * _Nullable body , NSURLResponse * _Nullable response, NSError * _Nullable error) {
                                                       if(response && body)
                                                       {
                                                           NSString *bodyString = [[NSString alloc] initWithData:body encoding:NSUTF8StringEncoding];
                                                           //NSLog(@"Response body: %@" , bodyString);
                                                           [self parseStream:bodyString];
                                                           [[NSOperationQueue mainQueue] addOperationWithBlock:^{
                                                               [_tableViewURL reloadData];
                                                           }];
                                                           [bodyString release];
                                                       }
                                                       else
                                                       {
                                                           [self parseDemoLive];
                                                       }
                                                   }];
    [task resume];
}

-(void)prepareURL
{
    if(_urlList)
    	[_urlList removeAllObjects];
    else
    	_urlList = [[NSMutableArray alloc] init];
    
    _currURL = 0;
    _clipboardURL = nil;
    
#if 0
    [_urlList addObject:@"https://ofmw8vyd3.qnssl.com/1461562925fetch/111.mp4"];
    [_urlList addObject:@"https://devimages.apple.com.edgekey.net/streaming/examples/bipbop_4x3/bipbop_4x3_variant.m3u8"];
    [_urlList addObject:@"rtmp://ftv.sun0769.com/dgrtv1/mp4:b1"];
    [_urlList addObject:@"http://devimages.apple.com/iphone/samples/bipbop/bipbopall.m3u8"];
    [_urlList addObject:@"http://devimages.apple.com/iphone/samples/bipbop/gear4/prog_index.m3u8"];
#endif
    
    NSString* docPathDir = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSLog(@"%@", docPathDir);
    NSArray* fileList = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:docPathDir error:nil];
    
    for (NSString *fileName in fileList)
    {
        if ([fileName hasSuffix:@".txt"])
        {
            NSArray* URLs = [[NSString stringWithContentsOfFile:[NSString stringWithFormat:@"%@/%@", docPathDir, fileName] encoding:NSUTF8StringEncoding error:nil] componentsSeparatedByString:@"\n"];
            
            for(NSString* url in URLs)
            {
                [_urlList addObject:[url stringByReplacingOccurrencesOfString:@"\r" withString:@""]];
            }
        }
        else if(![fileName hasSuffix:@".log"])
            [_urlList addObject:[NSString stringWithFormat:@"%@/%@", docPathDir, fileName]];
    }
    
    [self parseDemoLive];
}

-(void)setupUI
{
#if DEBUG
#if __LP64__
    NSLog(@"App is running as arm64");
#else
    NSLog(@"App is running as armv7/v7s");
#endif
#endif

    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
    [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleLightContent];
    [self.view setTranslatesAutoresizingMaskIntoConstraints:NO];
    
    
    // Video view
    _rectFullScreen = self.view.bounds;
    _rectFullScreen.size.width = self.view.bounds.size.height;
    _rectFullScreen.size.height = self.view.bounds.size.width;
    
    _rectSmallScreen = self.view.bounds;
    _rectSmallScreen.size.height /= 3;
    _rectSmallScreen.origin.y = 0;//[[UIApplication sharedApplication] statusBarFrame].size.height;
    _viewVideo = [[UIView alloc] initWithFrame:_rectSmallScreen];
    _viewVideo.backgroundColor = [UIColor blackColor];
    [self.view insertSubview:_viewVideo atIndex:0];
    [_viewVideo release];
    
    // Position slider
    _sliderPosition = [[UISlider alloc] initWithFrame:CGRectMake(0, _rectSmallScreen.origin.y+_rectSmallScreen.size.height - 40, _rectSmallScreen.size.width, 20)];
    [_sliderPosition addTarget:self action:@selector(onPositionChange:) forControlEvents:UIControlEventTouchUpInside];
    [_sliderPosition addTarget:self action:@selector(onPositionChangeBegin:) forControlEvents:UIControlEventTouchDown];
    _sliderPosition.minimumValue = 0.0;
    _sliderPosition.maximumValue = 1.0;
    [_sliderPosition setThumbImage:[UIImage imageNamed:@"seekbar.png"] forState:UIControlStateNormal];
    [_viewVideo addSubview:_sliderPosition];
    [_sliderPosition release];
    // layout contraits
    [_sliderPosition setTranslatesAutoresizingMaskIntoConstraints:NO];
    NSLayoutConstraint *contraint2 = [NSLayoutConstraint constraintWithItem:_sliderPosition attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:_viewVideo attribute:NSLayoutAttributeLeft multiplier:1.0 constant:5.0];
    NSLayoutConstraint *contraint3 = [NSLayoutConstraint constraintWithItem:_sliderPosition attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:_viewVideo attribute:NSLayoutAttributeBottom multiplier:1.0 constant:-10.0];
    NSLayoutConstraint *contraint4 = [NSLayoutConstraint constraintWithItem:_sliderPosition attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:_viewVideo attribute:NSLayoutAttributeRight multiplier:1.0 constant:-5.0];
    NSArray* array = [NSArray arrayWithObjects:contraint2, contraint3, contraint4, nil, nil, nil];
    [_viewVideo addConstraints:array];
    
    _tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(actionTapGesture:)];
    [_sliderPosition addGestureRecognizer:_tapGesture];
    
    
    // Playing time label
    int width = 80;
    _labelPlayingTime = [[UILabel alloc] initWithFrame:CGRectMake(_rectSmallScreen.size.width - width, _rectSmallScreen.origin.y+_rectSmallScreen.size.height - 50, width, 20)];
    _labelPlayingTime.text = @"00:00:00 / 00:00:00";
    _labelPlayingTime.font = [UIFont systemFontOfSize:8];
    _labelPlayingTime.textColor = [UIColor redColor];
    [_viewVideo addSubview:_labelPlayingTime];
    [_labelPlayingTime release];
    // layout contraits
    [_labelPlayingTime setTranslatesAutoresizingMaskIntoConstraints:NO];
    contraint3 = [NSLayoutConstraint constraintWithItem:_labelPlayingTime attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:_viewVideo attribute:NSLayoutAttributeBottom multiplier:1.0 constant:-30.0];
    contraint4 = [NSLayoutConstraint constraintWithItem:_labelPlayingTime attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:_viewVideo attribute:NSLayoutAttributeRight multiplier:1.0 constant:-5.0];
    array = [NSArray arrayWithObjects:contraint3, contraint4, nil, nil, nil, nil];
    [_viewVideo addConstraints:array];
    
    //Switch HW and SW
    width = 80;
    _switchHW = [[UISwitch alloc] initWithFrame:CGRectMake(_rectSmallScreen.size.width - _labelPlayingTime.frame.size.width - width, _rectSmallScreen.origin.y + 50, width, 20)];
    _switchHW.on = NO;
    [_viewVideo addSubview:_switchHW];
    [_switchHW release];
    // layout contraits
    [_switchHW setTranslatesAutoresizingMaskIntoConstraints:NO];
//    contraint2 = [NSLayoutConstraint constraintWithItem:_switchHW attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:_viewVideo attribute:NSLayoutAttributeLeft multiplier:1.0 constant:5.0];
    contraint3 = [NSLayoutConstraint constraintWithItem:_switchHW attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:_viewVideo attribute:NSLayoutAttributeTop multiplier:1.0 constant:80.0];
    contraint4 = [NSLayoutConstraint constraintWithItem:_switchHW attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:_viewVideo attribute:NSLayoutAttributeRight multiplier:1.0 constant:-10.0];
    array = [NSArray arrayWithObjects:contraint3, contraint4, nil, nil, nil, nil];
    [_viewVideo addConstraints:array];
    
    //Lable HW
    width = 80;
    _labelHW = [[UILabel alloc] initWithFrame:CGRectMake(_rectSmallScreen.size.width - width, _rectSmallScreen.origin.y+_rectSmallScreen.size.height + 50, width, 20)];
    _labelHW.text = @"HW Enable:";
    _labelHW.font = [UIFont systemFontOfSize:15];
    _labelHW.textColor = [UIColor redColor];
    [_viewVideo addSubview:_labelHW];
    [_labelHW release];
    // layout contraits
    [_labelHW setTranslatesAutoresizingMaskIntoConstraints:NO];
    contraint3 = [NSLayoutConstraint constraintWithItem:_labelHW attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:_viewVideo attribute:NSLayoutAttributeTop multiplier:1.0 constant:80.0];
    contraint4 = [NSLayoutConstraint constraintWithItem:_labelHW attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:_viewVideo attribute:NSLayoutAttributeRight multiplier:1.0 constant:-80.0];
    array = [NSArray arrayWithObjects:contraint3, contraint4, nil, nil, nil, nil];
    [_viewVideo addConstraints:array];

    //Lable version
    width = 80;
    _labelVersion = [[UILabel alloc] initWithFrame:CGRectMake(_rectSmallScreen.size.width - width, _rectSmallScreen.origin.y+_rectSmallScreen.size.height + 50, width, 20)];
    _labelVersion.text = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];//@"V1.0.0.0 B1";
    _labelVersion.font = [UIFont systemFontOfSize:8];
    _labelVersion.textColor = [UIColor redColor];
    [_viewVideo addSubview:_labelVersion];
    [_labelVersion release];
    // layout contraits
    [_labelVersion setTranslatesAutoresizingMaskIntoConstraints:NO];
    contraint3 = [NSLayoutConstraint constraintWithItem:_labelVersion attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:_viewVideo attribute:NSLayoutAttributeTop multiplier:1.0 constant:30.0];
    contraint4 = [NSLayoutConstraint constraintWithItem:_labelVersion attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:_viewVideo attribute:NSLayoutAttributeRight multiplier:1.0 constant:-5.0];
    array = [NSArray arrayWithObjects:contraint3, contraint4, nil, nil, nil, nil];
    [_viewVideo addConstraints:array];


    // URL list view
    CGRect r = self.view.bounds;
    r.size.height -= _viewVideo.bounds.size.height;
    r.origin.y = _viewVideo.bounds.size.height;
    _tableViewURL = [[UITableView alloc]initWithFrame:r style:UITableViewStylePlain];
    _tableViewURL.delegate = self;
    _tableViewURL.dataSource = self;
    [_tableViewURL setBackgroundColor:[UIColor clearColor]];
    _tableViewURL.separatorInset = UIEdgeInsetsMake(0,10, 0, 10);
    _tableViewURL.separatorColor = [UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:0.2];
    [self.view addSubview:_tableViewURL];

    //
    UIRefreshControl *refreshControl = [[UIRefreshControl alloc] init];
    [refreshControl addTarget:self action:@selector(refreshClick:) forControlEvents:UIControlEventValueChanged];
    [_tableViewURL addSubview:refreshControl];
}

- (void)refreshClick:(UIRefreshControl *)refreshControl
{
    [self prepareURL];
    [refreshControl endRefreshing];
    [_tableViewURL reloadData];
}

- (void)actionTapGesture:(UITapGestureRecognizer *)sender
{
    [_sliderPosition setValue:([sender locationInView:_sliderPosition].x / _sliderPosition.frame.size.width) animated:YES];
    [self onPositionChange:_sliderPosition];
}

-(void) updateStreamInfo
{
    int		i;
    char	szItem[32];
    int     nCount = 0;
    
    int nRC = _player.GetParam (_player.hPlayer, QCPLAY_PID_StreamNum, &nCount);
    
    if (nCount > 1)
    {
        QC_STREAM_FORMAT stmInfo;
        memset (&stmInfo, 0, sizeof (stmInfo));
        for (i = 1; i <= nCount; i++)
        {
            stmInfo.nID = i - 1;
            _player.GetParam (_player.hPlayer, QCPLAY_PID_StreamInfo, &stmInfo);
            sprintf (szItem, "Stream %d - %d", i, stmInfo.nBitrate);
            NSLog(@"Stream info: %s\n", szItem);
        }
    }
    
    nRC = _player.GetParam (_player.hPlayer, QCPLAY_PID_AudioTrackNum, &nCount);
    if (nCount > 1)
    {
        for (i = 1; i < nCount; i++)
        {
            sprintf (szItem, "Audio%d", i+1);
            NSLog(@"Audio track: %s\n", szItem);
        }
    }
    
    nRC = _player.GetParam (_player.hPlayer, QCPLAY_PID_VideoTrackNum, &nCount);
    if (nCount > 1)
    {
        for (i = 1; i < nCount; i++)
        {
            sprintf (szItem, "Video%d", i+1);
            NSLog(@"Video track: %s\n", szItem);
        }
    }
    
    nRC = _player.GetParam (_player.hPlayer, QCPLAY_PID_SubttTrackNum, &nCount);
    if (nCount > 1)
    {
        for (i = 1; i < nCount; i++)
        {
            sprintf (szItem, "Subtt%d", i+1);
            NSLog(@"Sub track: %s\n", szItem);
        }
    }
}


- (void)viewDidLoad
{
    [super viewDidLoad];
    
    //
    [self setupUI];
    [self prepareURL];
    
    _isFullScreen = NO;
}

#pragma mark UI action
-(IBAction)onStart:(id)sender
{
    [self createPlayer];
    
    UIButton* btn = (UIButton*)sender;
    NSLog(@"+Start, %s", [_urlList count]<=0?"":[_urlList[_currURL] UTF8String]);
    QCPLAY_STATUS status = _player.GetStatus(_player.hPlayer);
    
    if(status == QC_PLAY_Pause)
    {
        [_switchHW setHidden:YES];
        [_labelHW setHidden:YES];
        [btn setTitle:@"PAUSE" forState:UIControlStateNormal];
        _player.Run(_player.hPlayer);
    }
    else if(status == QC_PLAY_Run)
    {
        [_switchHW setHidden:NO];
        [_labelHW setHidden:NO];
        [btn setTitle:@"START" forState:UIControlStateNormal];
        _player.Pause(_player.hPlayer);
    }
    else
    {
        [btn setTitle:@"PAUSE" forState:UIControlStateNormal];
        _timer = [NSTimer scheduledTimerWithTimeInterval:1.0/100.0 target:self selector:@selector(onTimer:) userInfo:nil repeats:YES];
        
        _networkConnectionErrorTime = -1;
        const char* url = [_urlList[_currURL] UTF8String];
        if(_clipboardURL)
            url = [_clipboardURL UTF8String];
        //_player.SetParam(_player.hPlayer, QCPLAY_PID_DRM_KeyText, (void*)"XXXXXXXXXXXX");
        _player.Open(_player.hPlayer, url, _switchHW.on?QCPLAY_OPEN_VIDDEC_HW:0);
        if(_clipboardURL)
        {
            [_clipboardURL release];
            _clipboardURL = nil;
        }
        [_switchHW setHidden:YES];
        [_labelHW setHidden:YES];
    }
    
    NSLog(@"-Start");
}

-(IBAction)onStop:(id)sender
{
    NSLog(@"+Stop");
    
    if(!_player.hPlayer)
        return;

    [((UIButton*)sender) setTitle:@"START" forState:UIControlStateNormal];
    
    [_timer invalidate];
    _timer = nil;
    _player.Stop(_player.hPlayer);
    _player.Close(_player.hPlayer);
    [self destroyPlayer];
    
    //
    [_switchHW setHidden:NO];
    [_labelHW setHidden:NO];
    [_sliderPosition setValue:0.0];
    [_tableViewStreamInfo removeFromSuperview];
    _tableViewStreamInfo = nil;
    NSLog(@"-Stop");
}

- (IBAction)onPositionChangeBegin:(id)sender
{
    _isDragSlider = true;
}

- (IBAction)onPositionChange:(id)sender
{
    if(!_player.hPlayer)
        return;
    
    _isDragSlider = false;
    UISlider* slider = (UISlider *)sender;
    NSLog(@"Set pos %lld", (long long)((float)_player.GetDur(_player.hPlayer)*slider.value));
    _player.SetPos(_player.hPlayer, (long long)((float)_player.GetDur(_player.hPlayer)*slider.value));
}

- (IBAction)onTimer:(id)sender
{
    if(!_player.hPlayer)
        return;
    
    long long pos = _player.GetPos(_player.hPlayer) / 1000;
    long long dur = _player.GetDur(_player.hPlayer) / 1000;
    //NSLog(@"Pos %lld, duration %lld", _player.GetPos(_player.hPlayer), _player.GetDur(_player.hPlayer));
    if(!_isDragSlider)
    {
        if(dur <= 0)
            _sliderPosition.value = 1.0;
        else
            _sliderPosition.value = (float)pos/(float)dur;
    }
    
    NSString* strPos = [NSString stringWithFormat:@"%02lld:%02lld:%02lld", pos / 3600, pos % 3600 / 60, pos % 3600 % 60];
    NSString* strDur = [NSString stringWithFormat:@"%02lld:%02lld:%02lld", dur / 3600, dur % 3600 / 60, dur % 3600 % 60];

    _labelPlayingTime.text = [NSString stringWithFormat: @"%@%@%@", strPos, @" / " , strDur];
}

-(void)onAppActive:(BOOL)active
{
    NSLog(@"Active %d", active?1:0);
    
    if(!_player.hPlayer)
    	return;
    
    NSString* url = _urlList[_currURL];
    if(!url)
        return;
    
    bool isLive = [self isLive];
    
    if(active)
    {
        if(isLive)
        {
            int nVal = QC_PLAY_VideoEnable;
            _player.SetParam(_player.hPlayer, QCPLAY_PID_Disable_Video, &nVal);
        }
        else
        	_player.Run(_player.hPlayer);
    }
    else
    {
        if(isLive)
        {
            int nVal = _switchHW.enabled?QC_PLAY_VideoDisable_Decoder|QC_PLAY_VideoDisable_Render:QC_PLAY_VideoDisable_Render;
            _player.SetParam(_player.hPlayer, QCPLAY_PID_Disable_Video, &nVal);
        }
        else
        	_player.Pause(_player.hPlayer);
    }
}

-(IBAction)onFullScreen:(id)sender
{
    if(!_isFullScreen)
    {
        _isFullScreen = YES;
        //[[UIDevice currentDevice]setValue:[NSNumber numberWithInteger:UIDeviceOrientationPortrait]  forKey:@"orientation"];
        [[UIDevice currentDevice]setValue:[NSNumber numberWithInteger:UIDeviceOrientationLandscapeLeft] forKey:@"orientation"];
    }
    else
    {
        _isFullScreen = NO;
        //[[UIDevice currentDevice]setValue:[NSNumber numberWithInteger:UIDeviceOrientationLandscapeLeft]  forKey:@"orientation"];
        [[UIDevice currentDevice] setValue:[NSNumber numberWithInteger:UIDeviceOrientationPortrait] forKey:@"orientation"];
    }
    
    [_tableViewURL setHidden:_isFullScreen?YES:NO];
    _viewVideo.frame = _isFullScreen?_rectFullScreen:_rectSmallScreen;
    
    [UIView animateWithDuration:1.0 animations:^{
        
        if(_player.hPlayer)
            _player.SetView(_player.hPlayer, _viewVideo, NULL);
        
    } completion:^(BOOL finished) {
    }];
}

-(IBAction)onSelectStreamEnd:(id)sender
{
    if(_tableViewStreamInfo)
        [_tableViewStreamInfo setHidden:YES];
}

-(IBAction)onSelectStream:(id)sender
{
    QCPLAY_STATUS status = _player.GetStatus(_player.hPlayer);
    
    if(status == QC_PLAY_Run)
    {
        CGRect r = self.view.bounds;
        
        if(!_tableViewStreamInfo)
        {
            _tableViewStreamInfo = [[UITableView alloc]initWithFrame:r style:UITableViewStylePlain];
            _tableViewStreamInfo.delegate = self;
            _tableViewStreamInfo.dataSource = self;
            [_tableViewStreamInfo setBackgroundColor:[UIColor whiteColor]];
            _tableViewStreamInfo.separatorInset = UIEdgeInsetsMake(0,10, 0, 10);
            _tableViewStreamInfo.separatorColor = [UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:0.2];
            [self.view addSubview:_tableViewStreamInfo];
        }
        else
        {
            _tableViewStreamInfo.frame = r;
        }
        
        [_tableViewStreamInfo setHidden:NO];
    }
}


#pragma mark UI rotate
- (BOOL)shouldAutorotate
{
    return YES;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations
{
    if (!_isFullScreen)
    {
        return UIInterfaceOrientationMaskPortrait;
    }
    else
    {
        return UIInterfaceOrientationMaskLandscape;
    }
}


#pragma mark Table view processing
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if(_tableViewURL == tableView)
    {
        [tableView deselectRowAtIndexPath:[NSIndexPath indexPathWithIndex:_currURL] animated:YES];
        
        _currURL = indexPath.row;
        if([self fastOpen:[[_urlList objectAtIndex:indexPath.row] UTF8String]])
			return;
        
        [self onStop:_btnStart];
        [self onStart:_btnStart];
    }
    else if(_tableViewStreamInfo == tableView)
    {
        int idx = (int)indexPath.row;
        
        if (_player.hPlayer != NULL)
            _player.SetParam (_player.hPlayer, QCPLAY_PID_StreamPlay, &idx);
        
        [_tableViewStreamInfo setHidden:YES];
    }
}

- (void)tableView:(UITableView *)tableView willDisplayHeaderView:(UIView *)view forSection:(NSInteger)section
{
    view.tintColor = [UIColor clearColor];
    
    UITableViewHeaderFooterView* header = (UITableViewHeaderFooterView*)view;
    
    header.textLabel.font = [UIFont systemFontOfSize:12];
    [header.textLabel setTextColor:[UIColor blackColor]];
    header.textLabel.textAlignment = NSTextAlignmentCenter;
    
    if(_tableViewURL == tableView)
        header.textLabel.text = @"SELECT URL";
    else if(_tableViewStreamInfo == tableView)
    {
        header.textLabel.textAlignment = NSTextAlignmentCenter;
        
        if(!_btnCancelSelectStream)
        {
            _btnCancelSelectStream = [UIButton buttonWithType:UIButtonTypeSystem];
            [_btnCancelSelectStream setTitle:[NSString stringWithFormat:@"%s", "CANCEL"] forState:UIControlStateNormal];
            [_btnCancelSelectStream addTarget:self action:@selector(onSelectStreamEnd:) forControlEvents:UIControlEventTouchUpInside];
            [_btnCancelSelectStream setFrame:CGRectMake(10, view.frame.size.height/2, 80, 30)];
            [view addSubview:_btnCancelSelectStream];
            [_btnCancelSelectStream release];
        }
        //[view.textLabel setText:[NSString stringWithFormat:@"Section: %ld",(long)section]];
        
        header.textLabel.text = @"SELECT STREAM";
    }
}

-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    if(_tableViewURL == tableView)
        return 30;
    else if(_tableViewStreamInfo == tableView)
        return 50;

    return 30;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 30;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    if(_tableViewURL == tableView)
        return 1;
    
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if(_tableViewURL == tableView)
        return [_urlList count];
    else if(_tableViewStreamInfo== tableView)
    {
        int     nCount = 0;
        _player.GetParam (_player.hPlayer, QCPLAY_PID_StreamNum, &nCount);
        return nCount;
    }
    
    return 0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellIdentifier = @"UITableViewCellIdentifierBase";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];
    
    if(!cell)
    {
        cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleValue2 reuseIdentifier:cellIdentifier];
    }
    
    if(_tableViewURL == tableView)
    {
        cell.detailTextLabel.font = [UIFont systemFontOfSize:12];
        cell.detailTextLabel.textColor = [UIColor blackColor];
        [cell setBackgroundColor:[UIColor clearColor]];
        
        cell.detailTextLabel.text = _urlList[indexPath.row];
        cell.detailTextLabel.lineBreakMode = NSLineBreakByTruncatingMiddle;
    }
    else if(_tableViewStreamInfo == tableView)
    {
        cell.detailTextLabel.font = [UIFont systemFontOfSize:12];
        cell.detailTextLabel.textColor = [UIColor blackColor];
        [cell setBackgroundColor:[UIColor clearColor]];
        
        char	szItem[32];
        int     nCount = 0;
        
        if(_player.hPlayer)
        {
            _player.GetParam (_player.hPlayer, QCPLAY_PID_StreamNum, &nCount);
            
            if (nCount > 1 && indexPath.row<nCount)
            {
                QC_STREAM_FORMAT stmInfo;
                memset (&stmInfo, 0, sizeof (stmInfo));
                
                stmInfo.nID = (int)indexPath.row;
                _player.GetParam (_player.hPlayer, QCPLAY_PID_StreamInfo, &stmInfo);
                sprintf (szItem, "Stream %d - %d", (int)indexPath.row+1, stmInfo.nBitrate);
                NSLog(@"Stream info: %s\n", szItem);
                cell.detailTextLabel.text = [NSString stringWithFormat:@"%s", szItem];
            }
        }
        else
            cell.detailTextLabel.text = [NSString stringWithFormat:@"%s", "ERROR"];
    }
    
    return cell;
}

- (BOOL)tableView:(UITableView *)tableView shouldShowMenuForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return YES;
}

- (BOOL)tableView:(UITableView *)tableView canPerformAction:(SEL)action forRowAtIndexPath:(NSIndexPath *)indexPath withSender:(id)sender
{
    if (action == @selector(paste:) || action == @selector(copy:))
    {
        return YES;
    }
    return NO;
}

- (void)tableView:(UITableView *)tableView performAction:(SEL)action forRowAtIndexPath:(NSIndexPath *)indexPath withSender:(id)sender
{
    if (action == @selector(copy:))
    {
        [UIPasteboard generalPasteboard].string = [_urlList objectAtIndex:indexPath.row];
    }
    if (action == @selector(paste:))
    {
        if(_clipboardURL)
            [_clipboardURL release];
        _clipboardURL = [[UIPasteboard generalPasteboard].string retain];
        if([self fastOpen:[_clipboardURL UTF8String]])
            return;
        
        [self onStop:_btnStart];
        [self onStart:_btnStart];
    }
}

#pragma mark show warning message
-(void) showMessage:(NSString *)message duration:(NSTimeInterval)time
{
    CGSize screenSize = [[UIScreen mainScreen] bounds].size;
    
    UIWindow* window = [UIApplication sharedApplication].keyWindow;
    UIView* showView =  [[UIView alloc]init];
    showView.backgroundColor = [UIColor darkGrayColor];
    showView.frame = CGRectMake(1, 1, 1, 1);
    showView.alpha = 1.0f;
    showView.layer.cornerRadius = 5.0f;
    showView.layer.masksToBounds = YES;
    [window addSubview:showView];
    
    UILabel *label = [[UILabel alloc]init];
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc]init];
    paragraphStyle.lineBreakMode = NSLineBreakByWordWrapping;
    
    NSDictionary *attributes = @{NSFontAttributeName:[UIFont systemFontOfSize:15.f],
                                 NSParagraphStyleAttributeName:paragraphStyle.copy};
    
    CGSize labelSize = [message boundingRectWithSize:CGSizeMake(207, 999)
                                             options:NSStringDrawingUsesLineFragmentOrigin
                                          attributes:attributes context:nil].size;
    
    label.frame = CGRectMake(10, 5, labelSize.width +20, labelSize.height);
    label.text = message;
    label.textColor = [UIColor whiteColor];
    label.textAlignment = NSTextAlignmentCenter;
    label.backgroundColor = [UIColor clearColor];
    label.font = [UIFont boldSystemFontOfSize:15];
    [showView addSubview:label];
    
    CGSize videoViewSize = _viewVideo.bounds.size;
    
    showView.frame = CGRectMake((screenSize.width - labelSize.width - 20)/2,
                                videoViewSize.height/2 - labelSize.height/2,
                                labelSize.width+40,
                                labelSize.height+10);
    
    [UIView animateWithDuration:time animations:^{
        showView.alpha = 0;
    } completion:^(BOOL finished) {
        [showView removeFromSuperview];
    }];
}

-(int) getSysTime
{
    return [[NSProcessInfo processInfo] systemUptime] * 1000;
}

-(bool)isLive
{
    if(_player.hPlayer)
    {
        if(_player.GetDur(_player.hPlayer) > 0)
            return NO;
    }

    return YES;
}

-(BOOL)fastOpen:(const char*)newURL
{
    return NO;
    if(_player.hPlayer)
    {
        QCPLAY_STATUS status = _player.GetStatus(_player.hPlayer);
        if(status == QC_PLAY_Run)
        {
            const char* oldURL = [_urlList[_currURL] UTF8String];
            char* end = strchr(oldURL, ':');
            if(end)
            {
                if(!strncmp(newURL, oldURL, end-oldURL))
                {
                    NSLog(@"+Fast open, %s", newURL);
                    int flag = _switchHW.on?QCPLAY_OPEN_VIDDEC_HW:0;
                    _player.Open(_player.hPlayer, newURL, (flag|QCPLAY_OPEN_SAME_SOURCE));
                    NSLog(@"-Fast open");
                    return YES;
                }
            }
        }
    }
    
    return NO;
}


#pragma mark Other
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)dealloc
{
    [self destroyPlayer];
    [super dealloc];
    
    [_urlList release];
    _urlList = nil;
}

- (UIStatusBarStyle)preferredStatusBarStyle
{
    return UIStatusBarStyleLightContent;
}

@end
