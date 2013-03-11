#ifndef _WIN32

#include <AtlasSTL.h>
#include "mosdk.h"
#include "mo_common.h"
#import <Foundation/Foundation.h>

@class MOHttpRequest;

struct MOREQUEST
{
    char* _result;
    int _result_bsize;
    int _result_size;
    FILE* _file;
	MOHttpRequest* request;
};

@interface MOHttpRequest : NSObject {
    MOREQUEST _struct;
}

- (id)init;
- (void)dealloc;
+ (NSMutableURLRequest *)requestGet:(NSString *)aUrl queryString:(NSString *)aQueryString;
+ (NSMutableURLRequest *)requestPost:(NSString *)aUrl queryString:(NSString *)aQueryString;
- (void)sendRequest:(NSURLRequest *)request;
- (void)httpGet:(NSString *)aUrl queryString:(NSString *)aQueryString;
- (void)httpPost:(NSString *)aUrl queryString:(NSString *)aQueryString;

@end

@implementation MOHttpRequest

- (id)init {
    _struct._result = NULL;
    _struct._result_size = 0;
    _struct._file = NULL;
    _struct._state = MOREQUESTSTATE_PENDING;
    _struct.request = self;
    return self;
}

- (void)dealloc {
	if(_struct._result) free(_struct._result);
	if(_struct._file) fclose(_struct._file);
    [super dealloc];
}

+ (NSMutableURLRequest *)requestGet:(NSString *)aUrl queryString:(NSString *)aQueryString {
	NSMutableString *url = [[NSMutableString alloc] initWithString:aUrl];
	if (aQueryString) {
		[url appendFormat:@"?%@", aQueryString];
	}
	
	NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] initWithURL:[NSURL smartURLForString:url]] autorelease];
	[request setHTTPMethod:@"GET"];
	[request setTimeoutInterval:20.0f];
	
	[url release];
	return request;
}

+ (NSMutableURLRequest *)requestPost:(NSString *)aUrl queryString:(NSString *)aQueryString {
	
	NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] initWithURL:[NSURL smartURLForString:aUrl]] autorelease];
	[request setHTTPMethod:@"POST"];
	[request setTimeoutInterval:20.0f];
	[request setValue:@"application/x-www-form-urlencoded" forHTTPHeaderField:@"Content-Type"];
	[request setHTTPBody:[aQueryString dataUsingEncoding:NSUTF8StringEncoding]];
	
	return request;
}

- (void)sendRequest:(NSURLRequest *)request {
	[NSURLConnection
		sendAsynchronousRequest:request
		queue:[[NSOperationQueue alloc] init]
		completionHandler:^(NSURLResponse *response,
							NSData *data,
							NSError *error) 
        {
        	if ([data length] >0 && error == nil) {
        	} else if ([data length] == 0 && error == nil) {
        	} else if (error != nil) {
        	}
        }];
}

- (void)httpGet:(NSString *)aUrl queryString:(NSString *)aQueryString {
	NSMutableURLRequest *request = [self requestGet:aUrl queryString:aQueryString];
	[self sendRequest:request];
}

- (void)httpPost:(NSString *)aUrl queryString:(NSString *)aQueryString {
	NSMutableURLRequest *request = [self requestPost:aUrl queryString:aQueryString];
	[self sendRequest:request];
}

@end

MOREQUEST* MORequestString(const char* url, const Atlas::Map<Atlas::String, Atlas::String>& params)
{
	Atlas::String val;
	build_http_param(val, params);
	return MORequestString(url, val.c_str());
}

MOREQUEST* MODownloadFile(const char* url, const Atlas::Map<Atlas::String, Atlas::String>& params, const char* path)
{
	Atlas::String val;
	build_http_param(val, params);
	return MODownloadFile(url, val.c_str(), path);
}

MOREQUEST* MOUploadFiles(const char* url, const Atlas::Map<Atlas::String, Atlas::String>& params, const Atlas::Map<Atlas::String, Atlas::String>& files)
{
	Atlas::String val;
	build_http_param(val, params);
	return MOUploadFiles(url, val.c_str(), files);
}

MOREQUEST* MORequestString(const char* url, const char* postdata)
{
	MOAsyncHttp* request = [[MOAsyncHttp alloc] init];
	NSString* nurl = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
	NSString* nval = [NSString stringWithCString:postdata encoding:NSUTF8StringEncoding];
	if(postdata)
	{
		[request httpPost:nurl postData:nval savefile:NULL];
	}
	else
	{
		[request httpGet:nurl savefile:NULL];
	}
	return [request getStruct];
}

MOREQUEST* MODownloadFile(const char* url, const char* postdata, const char* path)
{
	FILE* fp = fopen(path, "wb");
	if(!fp) return NULL;

	MOAsyncHttp* request = [[MOAsyncHttp alloc] init];
	if(postdata)
	{
		NSString* nurl = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
		NSString* nval = [NSString stringWithCString:postdata encoding:NSUTF8StringEncoding];
		[request httpPost:nurl postData:nval savefile:fp];
	}
	else
	{
		NSString* nurl = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
		[request httpGet:nurl savefile:fp];
	}
	return [request getStruct];
}

MOREQUEST* MOUploadFiles(const char* url, const char* postdata, const Atlas::Map<Atlas::String, Atlas::String>& files)
{
	return NULL;
}

void MORequestDestory(MOREQUEST* request)
{
	[request->request release];
}

MOREQUESTSTATE MORequestStatus(MOREQUEST* request)
{
	return request->_state;
}

const char* MORequestGetResult(MOREQUEST* request)
{
	return request->_result;
}

int MORequestGetResultLength(MOREQUEST* request)
{
	return request->_result_size;
}

#endif
