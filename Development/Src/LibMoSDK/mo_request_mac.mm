#ifndef _WIN32

#include <string>
#include <map>
#include "mosdk.h"
#include "mo_common.h"
#import <Foundation/Foundation.h>

@class MOAsyncHttp;

struct MOREQUEST
{
    char* _result;
    int _result_bsize;
    int _result_size;
    FILE* _file;
    MOREQUESTSTATE _state;
	MOAsyncHttp* request;
};

@interface NSURL (QAdditions)

+ (NSDictionary *)parseURLQueryString:(NSString *)queryString;

+ (NSURL *)smartURLForString:(NSString *)str;

@end

@implementation NSURL (QAdditions)

#pragma mark -
#pragma mark Class methods

+ (NSDictionary *)parseURLQueryString:(NSString *)queryString {
	
	NSMutableDictionary *dict = [NSMutableDictionary dictionary];
	NSArray *pairs = [queryString componentsSeparatedByString:@"&"];
	for(NSString *pair in pairs) {
		NSArray *keyValue = [pair componentsSeparatedByString:@"="];
		if([keyValue count] == 2) {
			NSString *key = [keyValue objectAtIndex:0];
			NSString *value = [keyValue objectAtIndex:1];
			value = [value stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
			if(key && value)
				[dict setObject:value forKey:key];
		}
	}
	return [NSDictionary dictionaryWithDictionary:dict];
}

+ (NSURL *)smartURLForString:(NSString *)str {
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

@end

@interface MOMutableURLRequest : NSObject {

}

//Return a request for http get method
+ (NSMutableURLRequest *)requestGet:(NSString *)aUrl queryString:(NSString *)aQueryString;

//Return a request for http post method
+ (NSMutableURLRequest *)requestPost:(NSString *)aUrl queryString:(NSString *)aQueryString;

//Return a request for http post with multi-part method
+ (NSMutableURLRequest *)requestPostWithFile:(NSDictionary *)files url:(NSString *)aUrl queryString:(NSString *)aQueryString;

@end

@implementation MOMutableURLRequest


#pragma mark -
#pragma mark calss methods

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

+ (NSMutableURLRequest *)requestPostWithFile:(NSDictionary *)files url:(NSString *)aUrl queryString:(NSString *)aQueryString {
	
	NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] initWithURL:[NSURL smartURLForString:aUrl]] autorelease];
	[request setHTTPMethod:@"POST"];
	[request setTimeoutInterval:20.0f];
	
	//generate boundary string
	CFUUIDRef       uuid;
    CFStringRef     uuidStr;
    uuid = CFUUIDCreate(NULL);
    assert(uuid != NULL);
    uuidStr = CFUUIDCreateString(NULL, uuid);
    assert(uuidStr != NULL);
    NSString *boundary = [NSString stringWithFormat:@"Boundary-%@", uuidStr];
    CFRelease(uuidStr);
    CFRelease(uuid);
	
	NSData *boundaryBytes = [[NSString stringWithFormat:@"\r\n--%@\r\n", boundary] dataUsingEncoding:NSUTF8StringEncoding];
	[request setValue:[NSString stringWithFormat:@"multipart/form-data; boundary=%@", boundary] forHTTPHeaderField:@"Content-Type"];
	
	NSMutableData *bodyData = [NSMutableData data];
	NSString *formDataTemplate = @"\r\n--%@\r\nContent-Disposition: form-data; name=\"%@\"\r\n\r\n%@";
	
	NSDictionary *listParams = [NSURL parseURLQueryString:aQueryString];
	for (NSString *key in listParams) {
		
		NSString *value = [listParams valueForKey:key];
		NSString *formItem = [NSString stringWithFormat:formDataTemplate, boundary, key, value];
		[bodyData appendData:[formItem dataUsingEncoding:NSUTF8StringEncoding]];
	}
	[bodyData appendData:boundaryBytes];
	 
	NSString *headerTemplate = @"Content-Disposition: form-data; name=\"%@\"; filename=\"%@\"\r\nContent-Type: \"application/octet-stream\"\r\n\r\n";
	for (NSString *key in files) {
		
		NSString *filePath = [files objectForKey:key];
		NSData *fileData = [NSData dataWithContentsOfFile:filePath];
		NSString *header = [NSString stringWithFormat:headerTemplate, key, [[filePath componentsSeparatedByString:@"/"] lastObject]];
		[bodyData appendData:[header dataUsingEncoding:NSUTF8StringEncoding]];
		[bodyData appendData:fileData];
		[bodyData appendData:boundaryBytes];
	}
    [request setValue:[NSString stringWithFormat:@"%d", [bodyData length]] forHTTPHeaderField:@"Content-Length"];
	[request setHTTPBody:bodyData];
	
	return request;
}

@end

@interface MOSyncHttp : NSObject {

}

//Do http get method and return the data received.
- (NSString *)httpGet:(NSString *)aUrl queryString:(NSString *)aQueryString;

//Do http post method and return the data received.
- (NSString *)httpPost:(NSString *)aUrl queryString:(NSString *)aQueryString;

//do http multi-part method and return the data received.
- (NSString *)httpPostWithFile:(NSDictionary *)files url:(NSString *)aUrl queryString:(NSString *)aQueryString;

@end

@implementation MOSyncHttp

#pragma mark -
#pragma mark private methods

- (NSString *)getResponseWithRequest:(NSURLRequest *)request {
	
	NSURLResponse *response = nil;
	NSData *responseData = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:nil];
	NSString *retString = [[[NSString alloc] initWithData:responseData encoding:NSUTF8StringEncoding] autorelease];
	NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)response;
	
	NSLog(@"response code:%d string:%@", httpResponse.statusCode, retString);
	
	return retString;
}

#pragma mark -
#pragma mark instance methods

- (NSString *)httpGet:(NSString *)aUrl queryString:(NSString *)aQueryString {
	
	NSMutableURLRequest *request = [MOMutableURLRequest requestGet:aUrl queryString:aQueryString];
	return [self getResponseWithRequest:request];
}

- (NSString *)httpPost:(NSString *)aUrl queryString:(NSString *)aQueryString {
	
	NSMutableURLRequest *request = [MOMutableURLRequest requestPost:aUrl queryString:aQueryString];
	return [self getResponseWithRequest:request];
}

- (NSString *)httpPostWithFile:(NSDictionary *)files url:(NSString *)aUrl queryString:(NSString *)aQueryString {
	
	NSMutableURLRequest *request = [MOMutableURLRequest requestPostWithFile:files url:aUrl queryString:aQueryString];
	return [self getResponseWithRequest:request];
}

@end

@interface MOAsyncHttp : NSObject
{
    NSMutableURLRequest* _request;
    NSURLConnection* _connect;

    MOREQUEST _struct;
}

- (id)init;
- (void)dealloc;
- (void)appendData:(const void*) data length:(int) len;

- (void)httpGet:(NSString*)url savefile:(FILE*)file;
- (void)httpPost:(NSString*)url postData:(NSString*)data savefile:(FILE*)file;

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response;
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data;
- (void)connectionDidFinishLoading:(NSURLConnection *)connection;
- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error;

- (MOREQUEST*)getStruct;

@end

@implementation MOAsyncHttp

- (id)init
{
    _request = NULL;
    _connect = NULL;
    _struct._result = NULL;
    _struct._result_size = 0;
    _struct._file = NULL;
    _struct._state = MOREQUESTSTATE_PENDING;
    _struct.request = self;
    return self;
}

- (void)dealloc
{
	if(_struct._result) free(_struct._result);
	if(_struct._file) fclose(_struct._file);
    [super dealloc];
}

- (void) appendData:(const void*) data length:(int) len;
{
	if(_struct._file)
	{
		fwrite(data, 1, len, _struct._file);
		return;
	}

    if(!_struct._result)
    {
        _struct._result_bsize = 1024;
        _struct._result = (char*)malloc(_struct._result_bsize);
        _struct._result_size = 0;
    }
    
    if(_struct._result_bsize-_struct._result_size<=len)
    {
        _struct._result_bsize = _struct._result_size + len + 1024;
        _struct._result = (char*)realloc(_struct._result, _struct._result_bsize);
    }
    
    memcpy(_struct._result+_struct._result_size, data, len);
    _struct._result_size += len;
}

- (void) httpGet:(NSString*)aUrl savefile:(FILE*)file
{
	NSMutableString *url = [[NSMutableString alloc] initWithString:aUrl];
	_request = [[[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:url]] autorelease];
	[_request setHTTPMethod:@"GET"];
	[_request setTimeoutInterval:20.0f];
	[url release];
	_struct._file = file;
	_connect = [NSURLConnection connectionWithRequest:_request delegate:self]; 
}

- (void) httpPost:(NSString*)aUrl postData:(NSString*)data savefile:(FILE*)file
{
	NSMutableString *url = [[NSMutableString alloc] initWithString:aUrl];
	_request = [[[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:url]] autorelease];
	[_request setHTTPMethod:@"POST"];
	[_request setTimeoutInterval:20.0f];
	[_request setValue:@"application/x-www-form-urlencoded" forHTTPHeaderField:@"Content-Type"];
	[_request setHTTPBody:[data dataUsingEncoding:NSUTF8StringEncoding]];
 	_struct._file = file;
	_connect = [NSURLConnection connectionWithRequest:_request delegate:self]; 
   [url release];
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{

}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
	[self appendData:[data bytes] length:[data length]];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    _struct._result[_struct._result_size] = '\0';
	_struct._state = MOREQUESTSTATE_DONE;
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
	_struct._state = MOREQUESTSTATE_FAILED;
}

- (MOREQUEST*)getStruct
{
	return &_struct;
}

@end

MOREQUEST* MORequestString(const char* url, const std::map<std::string, std::string>& params)
{
	std::string val;
	build_http_param(val, params);
	return MORequestString(url, val.c_str());
}

MOREQUEST* MODownloadFile(const char* url, const std::map<std::string, std::string>& params, const char* path)
{
	std::string val;
	build_http_param(val, params);
	return MODownloadFile(url, val.c_str(), path);
}

MOREQUEST* MOUploadFiles(const char* url, const std::map<std::string, std::string>& params, const std::map<std::string, std::string>& files)
{
	std::string val;
	build_http_param(val, params);
	return MOUploadFiles(url, val.c_str(), files);
}

MOREQUEST* MORequestString(const char* url, const char* postdata)
{

	if(postdata)
	{
		NSString* nurl = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
		NSString* nval = [NSString stringWithCString:postdata encoding:NSUTF8StringEncoding];
		MOSyncHttp* SyncRequest = [[MOSyncHttp alloc] init];
		NSString* nret = [SyncRequest httpGet:nurl queryString:nval];
		std::string ret = [nret UTF8String];

		MOREQUEST* request = (MOREQUEST*)malloc(sizeof(MOREQUEST)+ret.size()+1);
		request->_result = (char*)(request+1);
		strcpy(request->_result, ret.c_str());
		request->_result_bsize = 0;
		request->_result_size = (int)ret.size();
		request->_file = NULL;
		request->_state = MOREQUESTSTATE_DONE;
		request->request = NULL;
		return request;
//		[request httpPost:nurl postData:nval savefile:NULL];
	}
	else
	{
		MOAsyncHttp* request = [[MOAsyncHttp alloc] init];
		NSString* nurl = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
		[request httpGet:nurl savefile:NULL];
		return [request getStruct];
	}
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

MOREQUEST* MOUploadFiles(const char* url, const char* postdata, const std::map<std::string, std::string>& files)
{
	return NULL;
}

void MORequestDestory(MOREQUEST* request)
{
	if(request->request)
	{
		[request->request release];
	}
	else
	{
		free(request);
	}
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
