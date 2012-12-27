{
method : "aaaaa",
argements : {
	name : value
		 }
}
{
	code : 11,
message : {
	name : value
		 }
}


Json::Value call;

_U8 data[10*1024];
_U32 datalen=sizeof(data);

const DDLReflect::CLASS_INFO* cls;
_U16 fid;
if(!Atlas::[GetServerFunctionStub](name, cls, fid))
{
	wxMessageBox(wxT("unknown command"), wxT("error"));
	return;
}


Json2Call(const FUNCTION_INFO*, call["argements"], datalen, data);

DDLStub::SGGAME_C2S<CSGNodeClient, CSGGameServerComponent>	SGGAME_C2S_STUB;
SGGAME_C2S_STUB.Dispatcher(caller, fid, memstream(data, datalen);


CSGGameServerComponent::Ping()
{
}

----------------
std::map<std::string, CLASS_INFO*> json_class_table;

1. class method 
2. json_class_table[class]
3. find method in class_info

------

1. json string To JsonValue
2. JsonValue.method find Class Fid
3. JsonValue.body to Buf(Data, Len);
4. (Data, Len) function call
