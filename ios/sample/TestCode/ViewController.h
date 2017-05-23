//
//  ViewController.h
//  TestCode
//
//  Created by Jun Lin on 3/02/17.
//  Copyright © 2017 qiniu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController
{
    IBOutlet UIButton* _btnStart;
    //IBOutlet UIButton* _btnStart;
}

-(void)onAppActive:(BOOL)active;

-(IBAction)onStart:(id)sender;
-(IBAction)onStop:(id)sender;
-(IBAction)onFullScreen:(id)sender;
-(IBAction)onSelectStream:(id)sender;
@end

