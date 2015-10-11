#import "FilePluginCordova.h"

#import <SvnSdk/svn_file_api.h>
#import <Svnsdk/PreviewView.h>

@implementation FilePluginCordova

- (void)listFile:(CDVInvokedUrlCommand*)command
{

    NSString* callbackId = [command callbackId];
    NSString* directory = [[command arguments] objectAtIndex:0];


    
    
    NSMutableArray * resultArray = [[NSMutableArray alloc] init];
    /*open directory*/
    SVN_DIR_S     *dir     = NULL;
    dir = svn_opendir([directory UTF8String]);
    if(NULL != dir)
    {
        SVN_DIRINFO_S* subdir = NULL;
        subdir = svn_readdir(dir);
        while(subdir != NULL)
        {
            if((strcmp(subdir->acDirName,".") == 0) ||(strcmp(subdir->acDirName,"..") == 0))
            {
                subdir = svn_readdir(dir);
                continue;
            }
            
            NSString * name = [NSString stringWithUTF8String:subdir->acDirName];
           
            
            
            
            if(name)
            {
                NSString * fullPath = [directory stringByAppendingPathComponent:name];
                //BOOL isDirectory =  (subdir->enType == SVN_TYPE_DIR);
               
                
                BOOL isDir = NO;
                BOOL isEncFile = NO;
                if ([[NSFileManager defaultManager] fileExistsAtPath:fullPath isDirectory:&isDir] && isDir) {
                    isDir = YES;
                    isEncFile = NO;
                }
                else if ([[NSFileManager defaultManager] fileExistsAtPath:fullPath isDirectory:&isDir] && !isDir) {
                    isEncFile = NO;
                }
                else
                {
                    isEncFile = YES;
                }
                
                NSMutableDictionary *obj = [[NSMutableDictionary alloc] init];
                [obj setValue:name forKey:@"name"];
                [obj setValue:fullPath forKey:@"fullPath"];
                [obj setValue:[NSNumber numberWithBool:isDir] forKey:@"isDirectory"];
                [obj setValue:directory forKey:@"parentPath"];
                [obj setValue:[NSNumber numberWithInt:isEncFile] forKey:@"isEnryptedFile"];
                [resultArray addObject:obj];
                
                NSLog(@"file info:%@", obj);
            }
            
            subdir = svn_readdir(dir);
        }
        
        svn_closedir(dir);
    }
    
    CDVPluginResult* result = [CDVPluginResult
                               resultWithStatus:CDVCommandStatus_OK
                               messageAsArray:resultArray];
    [self.commandDelegate sendPluginResult:result callbackId:callbackId];
    
   
    
    
}


- (void)readFile:(CDVInvokedUrlCommand*)command
{

    NSString* callbackId = [command callbackId];
    NSString* name = [[command arguments] objectAtIndex:0];
    //NSString* msg = [NSString stringWithFormat: @"Hello, %@", name];
    
    
    PreviewView *prewView = [[PreviewView alloc] init];
    [prewView previewDocument:name];
    
    [self.viewController presentViewController:prewView animated:YES completion:nil];

    CDVPluginResult* result = [CDVPluginResult
                               resultWithStatus:CDVCommandStatus_OK
                               ];
    [self.commandDelegate sendPluginResult:result callbackId:callbackId];
}

@end