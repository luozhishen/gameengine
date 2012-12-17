#ifndef _WIN32

#include <string>
#include <map>
#include "mosdk.h"
#include "mo_common.h"
#import <Foundation/Foundation.h>

@class MORequest;

struct MOREQUEST
{
	MORequest* request;
};

@interface MORequest : NSObject
{
    char* _result;
    int _result_bsize;
    int _result_size;
    FILE* _file;

    NSMutableURLRequest* _request;
    NSURLConnection* _connect;

    MOREQUESTSTATE _state;
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

- (const char*)getResult;
- (int)getResultLength;
- (MOREQUESTSTATE)getState;

@end

@implementation MORequest

- (id)init
{
    _result = NULL;
    _result_size = 0;
    _file = NULL;
    _request = NULL;
    _connect = NULL;
    _state = MOREQUESTSTATE_PENDING;
    _struct.request = self;
    return self;
}

- (void)dealloc
{
	if(_result) free(_result);
	if(_file) fclose(_file);
    [super dealloc];
}

- (void) appendData:(const void*) data length:(int) len;
{
	if(_file)
	{
		fwrite(data, 1, len, _file);
		return;
	}

    if(!_result)
    {
        _result_bsize = 1024;
        _result = (char*)malloc(_result_bsize);
        _result_size = 0;
    }
    
    if(_result_bsize-_result_size<=len)
    {
        _result_bsize = _result_size + len + 1024;
        _result = (char*)realloc(_result, _result_bsize);
    }
    
    memcpy(_result+_result_size, data, len);
    _result_size += len;
}

- (void) httpGet:(NSString*)aUrl savefile:(FILE*)file
{
	NSMutableString *url = [[NSMutableString alloc] initWithString:aUrl];
	_request = [[[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:url]] autorelease];
	[_request setHTTPMethod:@"GET"];
	[_request setTimeoutInterval:20.0f];
	[url release];
	_file = file;
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
    [url release];
	_file = file;
	_connect = [NSURLConnection connectionWithRequest:_request delegate:self]; 
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
    _result[_result_size] = '\0';
	_state = MOREQUESTSTATE_DONE;
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
	_state = MOREQUESTSTATE_FAILED;
}

- (const char*)getResult
{
    return _result;
}

- (int)getResultLength
{
    return _result_size;
}

- (MOREQUESTSTATE)getState
{
    return _state;
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
	MORequest* request = [[MORequest alloc] init];
	if(postdata)
	{
		NSString* nurl = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
		NSString* nval = [NSString stringWithCString:postdata encoding:NSUTF8StringEncoding];
		[request httpPost:nurl postData:nval savefile:NULL];
	}
	else
	{
		NSString* nurl = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
		[request httpGet:nurl savefile:NULL];
	}
	return (MOREQUEST*)request;
}

MOREQUEST* MODownloadFile(const char* url, const char* postdata, const char* path)
{
	FILE* fp = fopen(path, "wb");
	if(!fp) return NULL;

	MORequest* request = [[MORequest alloc] init];
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
	return (MOREQUEST*)request;
}

MOREQUEST* MOUploadFiles(const char* url, const char* postdata, const std::map<std::string, std::string>& files)
{
	return NULL;
}

void MORequestDestory(MOREQUEST* request)
{
	[request->request release];
}

MOREQUESTSTATE MORequestStatus(MOREQUEST* request)
{
	return [request->request getState];
}

const char* MORequestGetResult(MOREQUEST* request)
{
	return [request->request getResult];
}

int MORequestGetResultLength(MOREQUEST* request)
{
	return [request->request getResultLength];
}

#endif
