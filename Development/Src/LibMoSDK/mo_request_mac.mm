#ifndef _WIN32

#include <ZionSTL.h>
#include "mosdk.h"
#include "mo_common.h"
#import <Foundation/Foundation.h>

@class MOHttpRequest;

struct MOREQUEST
{
    Zion::String _result;
    FILE* _file;
	int _length;
    MOREQUESTSTATE _state;
	MOHttpRequest* request;
};

@interface MOHttpRequest : NSObject {
    MOREQUEST _struct;
}

- (id)init;
- (void)dealloc;
- (MOREQUEST*)getStruct;
- (void)stringRequest:(NSString *)aUrl queryString:(NSString *)aQueryString;
- (void)streamRequest:(NSString *)aUrl queryString:(NSString *)aQueryString;

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

- (MOREQUEST*)getStruct {
	return &_struct;
}

- (void)stringRequest:(NSString *)aUrl queryString:(NSString *)aQueryString {
	NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:aUrl]] autorelease];
	[request setTimeoutInterval:100.0f];
	if (aQueryString) {
		[request setHTTPMethod:@"POST"];
		[request setValue:@"application/x-www-form-urlencoded" forHTTPHeaderField:@"Content-Type"];
		[request setHTTPBody:[aQueryString dataUsingEncoding:NSUTF8StringEncoding]];
	} else {
		[request setHTTPMethod:@"GET"];
	}
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

- (bool)streamRequest:(NSString *)aUrl queryString:(NSString *)aQueryString {
	NSURLRequest *request=[NSURLRequest requestWithURL:[NSURL URLWithString:aUrl] autorelease];
	[request setTimeoutInterval:100.0f];
	if (aQueryString) {
		[request setHTTPMethod:@"POST"];
		[request setValue:@"application/x-www-form-urlencoded" forHTTPHeaderField:@"Content-Type"];
		[request setHTTPBody:[aQueryString dataUsingEncoding:NSUTF8StringEncoding]];
	} else {
		[request setHTTPMethod:@"GET"];
	}
	NSURLConnection *connection=[[NSURLConnection alloc] initWithRequest:theRequest delegate:self];
}

- (void)connection:(NSURLConnection __unused *)connection didReceiveResponse:(NSURLResponse *)response
{
}

- (void)connection:(NSURLConnection __unused *)connection didReceiveData:(NSData *)data
{
	fwrite([data bytes], 1, [data length], _struct._file);
	_struct._length += (int)[data length];
}

- (void)connectionDidFinishLoading:(NSURLConnection __unused *)connection
{
	_struct._state = MOREQUESTSTATE_DONE;
}

- (void)connection:(NSURLConnection __unused *)connection didFailWithError:(NSError *)error
{
	_struct._state = MOREQUESTSTATE_FAILED;
}

@end

MOREQUEST* MORequestString(const char* url, const Zion::Map<Zion::String, Zion::String>& params)
{
	Zion::String val;
	build_http_param(val, params);
	return MORequestString(url, val.c_str());
}

MOREQUEST* MODownloadFile(const char* url, const Zion::Map<Zion::String, Zion::String>& params, const char* path, bool append)
{
	Zion::String val;
	build_http_param(val, params);
	return MODownloadFile(url, val.c_str(), path);
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

MOREQUEST* MODownloadFile(const char* url, const char* postdata, const char* path, bool append)
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

int MORequestContentSize(MOREQUEST* request)
{
	return 0;
}

const char* MORequestGetResult(MOREQUEST* request)
{
	return request->_result.c_str();
}

int MORequestGetResultLength(MOREQUEST* request)
{
	return request->_file?request->_length:(int)(request->_result.size());
}

#endif
