<?php

class A_CONTENT_OBJECT
{
	public $_uuid; // string
	public $_name; // string

	public function __construct()
	{
		$this->_uuid = '';
		$this->_name = '';
	}
	public function ToStringInternal()
	{
		$__result = '';
		// _uuid
		if(!is_string($this->_uuid)) return '';
		$__result = $__result.'"_uuid":"'.$this->_uuid.'"';
		// _name
		if(!is_string($this->_name)) return '';
		$__result = $__result.',"_name":"'.$this->_name.'"';
		return $__result;
	}
	public function ToString()
	{
		return '{'.$this->ToStringInternal().'}';
	}
	public function ToArray($_array=array())
	{
		// _uuid
		$_array['_uuid'] = $this->_uuid;
		// _name
		$_array['_name'] = $this->_name;
		return $_array;
	}
	public function FromArray($_array)
	{
		if(!is_array($_array)) return false;
		// _uuid
		if(!is_string($_array['_uuid'])) return false;
		$this->_uuid = $_array['_uuid'];
		// _name
		if(!is_string($_array['_name'])) return false;
		$this->_name = $_array['_name'];
		return true;
	}
};

class A_LIVE_OBJECT
{
	public $_uuid; // string

	public function __construct()
	{
		$this->_uuid = '';
	}
	public function ToStringInternal()
	{
		$__result = '';
		// _uuid
		if(!is_string($this->_uuid)) return '';
		$__result = $__result.'"_uuid":"'.$this->_uuid.'"';
		return $__result;
	}
	public function ToString()
	{
		return '{'.$this->ToStringInternal().'}';
	}
	public function ToArray($_array=array())
	{
		// _uuid
		$_array['_uuid'] = $this->_uuid;
		return $_array;
	}
	public function FromArray($_array)
	{
		if(!is_array($_array)) return false;
		// _uuid
		if(!is_string($_array['_uuid'])) return false;
		$this->_uuid = $_array['_uuid'];
		return true;
	}
};

class CARD_AVATAR_OPERATOR extends A_CONTENT_OBJECT
{
	public $v1; // int
	public $v2; // int
	public $v3; // float

	public function __construct()
	{
		parent::__construct();
		$this->v1 = 0;
		$this->v2 = 0;
		$this->v3 = 0.0;
	}
	public function ToStringInternal()
	{
		$__result = parent::ToStringInternal();
		if($__result!='') $__result = $__result.',';
		// v1
		if(!is_int($this->v1)) return '';
		if($this->v1<0 || $this->v1>4294967295) return false;
		$__result = $__result.'"v1":'.$this->v1;
		// v2
		if(!is_int($this->v2)) return '';
		$__result = $__result.',"v2":'.$this->v2;
		// v3
		if(!is_numeric($this->v3)) return '';
		$__result = $__result.',"v3":'.$this->v3;
		return $__result;
	}
	public function ToString()
	{
		return '{'.$this->ToStringInternal().'}';
	}
	public function ToArray($_array=array())
	{
		$_array = parent::ToArray($_array);
		// v1
		$_array['v1'] = $this->v1;
		// v2
		$_array['v2'] = $this->v2;
		// v3
		$_array['v3'] = $this->v3;
		return $_array;
	}
	public function FromArray($_array)
	{
		if(!is_array($_array)) return false;
		if(!parent::FromArray($_array)) return false;
		// v1
		if(!is_int($_array['v1'])) return false;
		if($_array['v1']<0 || $_array['v1']>4294967295) return false;
		$this->v1 = $_array['v1'];
		// v2
		if(!is_int($_array['v2'])) return false;
		$this->v2 = $_array['v2'];
		// v3
		if(!is_numeric($_array['v3'])) return false;
		$this->v3 = $_array['v3'];
		return true;
	}
};

class CARD_AVATAR_DESC extends A_LIVE_OBJECT
{
	public $avatar_id; // int
	public $avatar_name; // string

	public function __construct()
	{
		parent::__construct();
		$this->avatar_id = 0;
		$this->avatar_name = '';
	}
	public function ToStringInternal()
	{
		$__result = parent::ToStringInternal();
		if($__result!='') $__result = $__result.',';
		// avatar_id
		if(!is_int($this->avatar_id)) return '';
		if($this->avatar_id<0 || $this->avatar_id>4294967295) return false;
		$__result = $__result.'"avatar_id":'.$this->avatar_id;
		// avatar_name
		if(!is_string($this->avatar_name)) return '';
		$__result = $__result.',"avatar_name":"'.$this->avatar_name.'"';
		return $__result;
	}
	public function ToString()
	{
		return '{'.$this->ToStringInternal().'}';
	}
	public function ToArray($_array=array())
	{
		$_array = parent::ToArray($_array);
		// avatar_id
		$_array['avatar_id'] = $this->avatar_id;
		// avatar_name
		$_array['avatar_name'] = $this->avatar_name;
		return $_array;
	}
	public function FromArray($_array)
	{
		if(!is_array($_array)) return false;
		if(!parent::FromArray($_array)) return false;
		// avatar_id
		if(!is_int($_array['avatar_id'])) return false;
		if($_array['avatar_id']<0 || $_array['avatar_id']>4294967295) return false;
		$this->avatar_id = $_array['avatar_id'];
		// avatar_name
		if(!is_string($_array['avatar_name'])) return false;
		$this->avatar_name = $_array['avatar_name'];
		return true;
	}
};

class CARD_AVATAR extends A_LIVE_OBJECT
{
	public $avatar_name; // string

	public function __construct()
	{
		parent::__construct();
		$this->avatar_name = '';
	}
	public function ToStringInternal()
	{
		$__result = parent::ToStringInternal();
		if($__result!='') $__result = $__result.',';
		// avatar_name
		if(!is_string($this->avatar_name)) return '';
		$__result = $__result.'"avatar_name":"'.$this->avatar_name.'"';
		return $__result;
	}
	public function ToString()
	{
		return '{'.$this->ToStringInternal().'}';
	}
	public function ToArray($_array=array())
	{
		$_array = parent::ToArray($_array);
		// avatar_name
		$_array['avatar_name'] = $this->avatar_name;
		return $_array;
	}
	public function FromArray($_array)
	{
		if(!is_array($_array)) return false;
		if(!parent::FromArray($_array)) return false;
		// avatar_name
		if(!is_string($_array['avatar_name'])) return false;
		$this->avatar_name = $_array['avatar_name'];
		return true;
	}
};

class CARD_AVATAR_OWNOBJ extends A_LIVE_OBJECT
{
	public $obj_name; // string
	public $obj_value; // float

	public function __construct()
	{
		parent::__construct();
		$this->obj_name = '';
		$this->obj_value = 0.0;
	}
	public function ToStringInternal()
	{
		$__result = parent::ToStringInternal();
		if($__result!='') $__result = $__result.',';
		// obj_name
		if(!is_string($this->obj_name)) return '';
		$__result = $__result.'"obj_name":"'.$this->obj_name.'"';
		// obj_value
		if(!is_numeric($this->obj_value)) return '';
		$__result = $__result.',"obj_value":'.$this->obj_value;
		return $__result;
	}
	public function ToString()
	{
		return '{'.$this->ToStringInternal().'}';
	}
	public function ToArray($_array=array())
	{
		$_array = parent::ToArray($_array);
		// obj_name
		$_array['obj_name'] = $this->obj_name;
		// obj_value
		$_array['obj_value'] = $this->obj_value;
		return $_array;
	}
	public function FromArray($_array)
	{
		if(!is_array($_array)) return false;
		if(!parent::FromArray($_array)) return false;
		// obj_name
		if(!is_string($_array['obj_name'])) return false;
		$this->obj_name = $_array['obj_name'];
		// obj_value
		if(!is_numeric($_array['obj_value'])) return false;
		$this->obj_value = $_array['obj_value'];
		return true;
	}
};

function DATASYNC_JSON_C2S_Dispatcher($fname, $_array, $_this)
{
	if($fname=='DS_CreateObject')
	{
		if(count($_array)!=2) return false;
		if(!is_string($_array[0])) return false;
		$__type = $_array[0];
		if(!is_string($_array[1])) return false;
		$__data = $_array[1];
		$_this->DS_CreateObject($__type, $__data);
		return true;
	}
	if($fname=='DS_UpdateObject')
	{
		if(count($_array)!=2) return false;
		if(!is_string($_array[0])) return false;
		$___uuid = $_array[0];
		if(!is_string($_array[1])) return false;
		$__data = $_array[1];
		$_this->DS_UpdateObject($___uuid, $__data);
		return true;
	}
	if($fname=='DS_DeleteObject')
	{
		if(count($_array)!=2) return false;
		if(!is_array($_array[0])) return false;
		$___uuid = array();
		$_earray = $_array[0];
		for($__i=0; $__i<count($_earray); $__i++)
		{
			if(!is_string($_earray[$__i])) return false;
			$___uuid[$__i] = $_earray[$__i];
		}
		if(!is_int($_array[1])) return false;
		if($_array[1]<0 || $_array[1]>4294967295) return false;
		$__count = $_array[1];
		$_this->DS_DeleteObject($___uuid, $__count);
		return true;
	}
	return false;
}

class DATASYNC_JSON_S2C
{
	public function DS_SyncOpen($flag)
	{
		$__result = '';
		if(!is_int($flag)) return false;
		if($flag<0 || $flag>4294967295) return false;
		$__result += $flag;
		ZionSession::Get()->Send('{"method":"DATASYNC_JSON_S2C.DS_SyncOpen","args":['.$__result.']}');
		return true;
	}
	public function DS_SyncReady()
	{
		ZionSession::Get()->Send('{"method":"DATASYNC_JSON_S2C.DS_SyncReady","args":[]}');
		return true;
	}
	public function DS_SyncClose()
	{
		ZionSession::Get()->Send('{"method":"DATASYNC_JSON_S2C.DS_SyncClose","args":[]}');
		return true;
	}
	public function DS_CreateObjectDone($_uuid)
	{
		$__result = '';
		if(!is_string($_uuid)) return false;
		$__result += '"'.$_uuid.'"';
		ZionSession::Get()->Send('{"method":"DATASYNC_JSON_S2C.DS_CreateObjectDone","args":['.$__result.']}');
		return true;
	}
	public function DS_CreateObject($type, $json)
	{
		$__result = '';
		if(!is_string($type)) return false;
		$__result += '"'.$type.'"';
		$__result += ','
		if(!is_string($json)) return false;
		$__result += '"'.$json.'"';
		ZionSession::Get()->Send('{"method":"DATASYNC_JSON_S2C.DS_CreateObject","args":['.$__result.']}');
		return true;
	}
	public function DS_UpdateObject($_uuid, $json)
	{
		$__result = '';
		if(!is_string($_uuid)) return false;
		$__result += '"'.$_uuid.'"';
		$__result += ','
		if(!is_string($json)) return false;
		$__result += '"'.$json.'"';
		ZionSession::Get()->Send('{"method":"DATASYNC_JSON_S2C.DS_UpdateObject","args":['.$__result.']}');
		return true;
	}
	public function DS_DeleteObject($_uuid, $count)
	{
		$__result = '';
		if(!is_array($_uuid)) return false;
		$__result += '[';
		for($__i=0; $__i<count($_uuid); $__i++)
		{
			if($__i>0) $__result += ',';
			if(!is_string($_uuid[$__i])) return false;
			$__result += '"'.$_uuid[$__i].'"';
		}
		$__result += ']';
		$__result += ','
		if(!is_int($count)) return false;
		if($count<0 || $count>4294967295) return false;
		$__result += $count;
		ZionSession::Get()->Send('{"method":"DATASYNC_JSON_S2C.DS_DeleteObject","args":['.$__result.']}');
		return true;
	}
}

function CARDGAME_C2S_Dispatcher($fname, $_array, $_this)
{
	if($fname=='Ping')
	{
		if(count($_array)!=0) return false;
		$_this->Ping();
		return true;
	}
	if($fname=='SelectServer')
	{
		if(count($_array)!=1) return false;
		if(!is_int($_array[0])) return false;
		if($_array[0]<0 || $_array[0]>4294967295) return false;
		$__server_id = $_array[0];
		$_this->SelectServer($__server_id);
		return true;
	}
	if($fname=='GetAvatarList')
	{
		if(count($_array)!=0) return false;
		$_this->GetAvatarList();
		return true;
	}
	if($fname=='CreateAvatar')
	{
		if(count($_array)!=2) return false;
		if(!is_string($_array[0])) return false;
		$__avatar_name = $_array[0];
		if(!is_int($_array[1])) return false;
		if($_array[1]<0 || $_array[1]>4294967295) return false;
		$__type = $_array[1];
		$_this->CreateAvatar($__avatar_name, $__type);
		return true;
	}
	if($fname=='EnterGame')
	{
		if(count($_array)!=1) return false;
		if(!is_int($_array[0])) return false;
		if($_array[0]<0 || $_array[0]>4294967295) return false;
		$__avatar_id = $_array[0];
		$_this->EnterGame($__avatar_id);
		return true;
	}
	if($fname=='LeaveGame')
	{
		if(count($_array)!=0) return false;
		$_this->LeaveGame();
		return true;
	}
	return false;
}

class CARDGAME_S2C
{
	public function Pong()
	{
		ZionSession::Get()->Send('{"method":"CARDGAME_S2C.Pong","args":[]}');
		return true;
	}
	public function GetAvatarListCallback($errcode, $arr, $count)
	{
		$__result = '';
		if(!is_int($errcode)) return false;
		if($errcode<0 || $errcode>4294967295) return false;
		$__result += $errcode;
		$__result += ','
		if(!is_array($arr)) return false;
		$__result += '[';
		for($__i=0; $__i<count($arr); $__i++)
		{
			if($__i>0) $__result += ',';
			$__result += $arr[$__i]->ToString();
		}
		$__result += ']';
		$__result += ','
		if(!is_int($count)) return false;
		if($count<0 || $count>4294967295) return false;
		$__result += $count;
		ZionSession::Get()->Send('{"method":"CARDGAME_S2C.GetAvatarListCallback","args":['.$__result.']}');
		return true;
	}
	public function CreateAvatarCallback($errcode)
	{
		$__result = '';
		if(!is_int($errcode)) return false;
		if($errcode<0 || $errcode>4294967295) return false;
		$__result += $errcode;
		ZionSession::Get()->Send('{"method":"CARDGAME_S2C.CreateAvatarCallback","args":['.$__result.']}');
		return true;
	}
	public function EnterGameCallback($errcode)
	{
		$__result = '';
		if(!is_int($errcode)) return false;
		if($errcode<0 || $errcode>4294967295) return false;
		$__result += $errcode;
		ZionSession::Get()->Send('{"method":"CARDGAME_S2C.EnterGameCallback","args":['.$__result.']}');
		return true;
	}
	public function LeaveGameCallback($errcode)
	{
		$__result = '';
		if(!is_int($errcode)) return false;
		if($errcode<0 || $errcode>4294967295) return false;
		$__result += $errcode;
		ZionSession::Get()->Send('{"method":"CARDGAME_S2C.LeaveGameCallback","args":['.$__result.']}');
		return true;
	}
}

?>