#ifndef _WIN32

#include <AtlasSTL.h>
#include "mosdk.h"
#include "mo_common.h"
#import <Foundation/Foundation.h>

@class MOHttpRequest;

struct MOREQUEST
{
    Atlas::String _result;
    FILE* _file;
    MOREQUESTSTATE _state;
	MOHttpRequest* request;
};

@interface MOHttpRequest : NSObject {
    MOREQUEST _struct;
}

- (id)init;
- (void)dealloc;
- (NSURL *)smartURLForString:(NSString *)str;
- (NSMutableURLRequest *)requestGet:(NSString *)aUrl queryString:(NSString *)aQueryString;
- (NSMutableURLRequest *)requestPost:(NSString *)aUrl queryString:(NSString *)aQueryString;
- (void)sendRequest:(NSURLRequest *)request;
- (void)httpGet:(NSString *)aUrl queryString:(NSString *)aQueryString;
- (void)httpPost:(NSString *)aUrl queryString:(NSString *)aQueryString;
- (MOREQUEST*)getStruct;

@end

@implementation MOHttpRequest

- (id)init {
    _struct._result = "";
    _struct._file = NULL;
    _struct._state = MOREQUESTSTATE_PENDING;
    _struct.request = self;
    return self;
}

- (void)dealloc {
	if(_struct._file) fclose(_struct._file);
    [super dealloc];
}

- (NSURL *)smartURLForString:(NSString *)str {
	NSURL *     result;
	NSString *  trimmedStr;
	NSRange     schemeMarkerRange;
	NSString *  scheme;
	
	assert(str != nil);
	
	result = nil;
	
	trimmedStr = [str stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
	if ( (trimmedStr != nil) && (trimmedStr.length != 0) ) {
		schemeMarkerRange = [trimmedStr rangeOfString:@"://"];
		
		if (schemeMarkerRange.location == NSNotFound) {
			result = [NSURL URLWithString:[NSString stringWithFormat:@"http://%@", trimmedStr]];
		} else {
			scheme = [trimmedStr substringWithRange:NSMakeRange(0, schemeMarkerRange.location)];
			assert(scheme != nil);
			
			if ( ([scheme compare:@"http"  options:NSCaseInsensitiveSearch] == NSOrderedSame)
				|| ([scheme compare:@"https" options:NSCaseInsensitiveSearch] == NSOrderedSame) ) {
				result = [NSURL URLWithString:trimmedStr];
			} else {
				// It looks like this is some unsupported URL scheme.
			}
		}
	}
	
	return result;
}

- (NSMutableURLRequest *)requestGet:(NSString *)aUrl queryString:(NSString *)aQueryString {
	NSMutableString *url = [[NSMutableString alloc] initWithString:aUrl];
	if (aQueryString) {
		[url appendFormat:@"?%@", aQueryString];
	}
	
	NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] initWithURL:[self smartURLForString:url]] autorelease];
	[request setHTTPMethod:@"GET"];
	[request setTimeoutInterval:20.0f];
	
	[url release];
	return request;
}

- (NSMutableURLRequest *)requestPost:(NSString *)aUrl queryString:(NSString *)aQueryString {
	
	NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] initWithURL:[self smartURLForString:aUrl]] autorelease];
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
				NSString* newStr = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
				_struct._result = [newStr UTF8String];
				_struct._state = MOREQUESTSTATE_DONE;
        	} else if ([data length] == 0 && error == nil) {
				_struct._result = "";
				_struct._state = MOREQUESTSTATE_DONE;
        	} else if (error != nil) {
				_struct._state = MOREQUESTSTATE_FAILED;
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

- (MOREQUEST*)getStruct {
	return &_struct;
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
	MOHttpRequest* request = [[MOHttpRequest alloc] init];
	NSString* nurl = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
	NSString* nval = [NSString stringWithCString:postdata encoding:NSUTF8StringEncoding];
	if(postdata)
	{
		[request httpPost:nurl queryString:nval];
	}
	else
	{
		[request httpGet:nurl queryString:@""];
	}
	return [request getStruct];
}

MOREQUEST* MODownloadFile(const char* url, const char* postdata, const char* path)
{
	return NULL;
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
	return request->_result.c_str();
}

int MORequestGetResultLength(MOREQUEST* request)
{
	return (int)(request->_result.size());
}

#endif
