//
//  ViewController.m
//  SkiaTextDemo
//
//  Created by lvpengwei on 2019/5/18.
//  Copyright © 2019 lvpengwei. All rights reserved.
//

#import "ViewController.h"
#import "SkiaView.h"
#import "SkiaLayer.h"
#import "SkiaTextLayer.h"

@interface ViewController ()
@property (nonatomic, weak) SkiaTextLayer *skiaTextLayer;
@property (nonatomic, weak) SkiaLayer *skiaLayer;
@end

@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    SkiaTextLayer *skiaTextLayer = [[SkiaTextLayer alloc] init];
    skiaTextLayer.frame = CGRectMake(100, 100, 200, 200);
    [self.view.layer addSublayer:skiaTextLayer];
    self.skiaTextLayer = skiaTextLayer;
//    SkiaLayer *skiaLayer = [[SkiaLayer alloc] init];
//    skiaLayer.frame = CGRectMake(100, 400, 200, 200);
//    [self.view.layer addSublayer:skiaLayer];
//    self.skiaLayer = skiaLayer;
}
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
//    [self.skiaLayer draw];
    [self.skiaTextLayer flush];
}
- (IBAction)buttonAction:(id)sender {
//    [self.skiaLayer draw];
    [self.skiaTextLayer flush];
}
@end
