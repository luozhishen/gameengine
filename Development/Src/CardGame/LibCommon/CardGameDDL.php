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
		$__result .= '"_uuid":'.json_encode($this->_uuid).'';
		// _name
		if(!is_string($this->_name)) return '';
		$__result .= ',"_name":'.json_encode($this->_name).'';
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
		$__result .= '"_uuid":'.json_encode($this->_uuid).'';
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

class CG_AVATAR_DESC extends A_LIVE_OBJECT
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
		if($__result!='') $__result .= ',';
		// avatar_id
		if(!is_int($this->avatar_id)) return '';
		if($this->avatar_id<0 || $this->avatar_id>4294967295) return false;
		$__result .= '"avatar_id":'.$this->avatar_id;
		// avatar_name
		if(!is_string($this->avatar_name)) return '';
		$__result .= ',"avatar_name":'.json_encode($this->avatar_name).'';
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

class CG_AVATAR extends A_LIVE_OBJECT
{
	public $avatar_name; // string
	public $money; // float

	public function __construct()
	{
		parent::__construct();
		$this->avatar_name = '';
		$this->money = 0.0;
	}
	public function ToStringInternal()
	{
		$__result = parent::ToStringInternal();
		if($__result!='') $__result .= ',';
		// avatar_name
		if(!is_string($this->avatar_name)) return '';
		$__result .= '"avatar_name":'.json_encode($this->avatar_name).'';
		// money
		if(!is_numeric($this->money)) return '';
		$__result .= ',"money":'.$this->money;
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
		// money
		$_array['money'] = $this->money;
		return $_array;
	}
	public function FromArray($_array)
	{
		if(!is_array($_array)) return false;
		if(!parent::FromArray($_array)) return false;
		// avatar_name
		if(!is_string($_array['avatar_name'])) return false;
		$this->avatar_name = $_array['avatar_name'];
		// money
		if(!is_numeric($_array['money'])) return false;
		$this->money = $_array['money'];
		return true;
	}
};

class CG_CARD extends A_LIVE_OBJECT
{
	public $content_uuid; // string
	public $win_count; // int
	public $lost_count; // int

	public function __construct()
	{
		parent::__construct();
		$this->content_uuid = '';
		$this->win_count = 0;
		$this->lost_count = 0;
	}
	public function ToStringInternal()
	{
		$__result = parent::ToStringInternal();
		if($__result!='') $__result .= ',';
		// content_uuid
		if(!is_string($this->content_uuid)) return '';
		$__result .= '"content_uuid":'.json_encode($this->content_uuid).'';
		// win_count
		if(!is_int($this->win_count)) return '';
		if($this->win_count<0 || $this->win_count>4294967295) return false;
		$__result .= ',"win_count":'.$this->win_count;
		// lost_count
		if(!is_int($this->lost_count)) return '';
		if($this->lost_count<0 || $this->lost_count>4294967295) return false;
		$__result .= ',"lost_count":'.$this->lost_count;
		return $__result;
	}
	public function ToString()
	{
		return '{'.$this->ToStringInternal().'}';
	}
	public function ToArray($_array=array())
	{
		$_array = parent::ToArray($_array);
		// content_uuid
		$_array['content_uuid'] = $this->content_uuid;
		// win_count
		$_array['win_count'] = $this->win_count;
		// lost_count
		$_array['lost_count'] = $this->lost_count;
		return $_array;
	}
	public function FromArray($_array)
	{
		if(!is_array($_array)) return false;
		if(!parent::FromArray($_array)) return false;
		// content_uuid
		if(!is_string($_array['content_uuid'])) return false;
		$this->content_uuid = $_array['content_uuid'];
		// win_count
		if(!is_int($_array['win_count'])) return false;
		if($_array['win_count']<0 || $_array['win_count']>4294967295) return false;
		$this->win_count = $_array['win_count'];
		// lost_count
		if(!is_int($_array['lost_count'])) return false;
		if($_array['lost_count']<0 || $_array['lost_count']>4294967295) return false;
		$this->lost_count = $_array['lost_count'];
		return true;
	}
};

class CG_SHOPITEM extends A_CONTENT_OBJECT
{
	public $shopitem_id; // string
	public $price; // int
	public $content_uuid; // string

	public function __construct()
	{
		parent::__construct();
		$this->shopitem_id = '';
		$this->price = 0;
		$this->content_uuid = '';
	}
	public function ToStringInternal()
	{
		$__result = parent::ToStringInternal();
		if($__result!='') $__result .= ',';
		// shopitem_id
		if(!is_string($this->shopitem_id)) return '';
		$__result .= '"shopitem_id":'.json_encode($this->shopitem_id).'';
		// price
		if(!is_int($this->price)) return '';
		if($this->price<0 || $this->price>4294967295) return false;
		$__result .= ',"price":'.$this->price;
		// content_uuid
		if(!is_string($this->content_uuid)) return '';
		$__result .= ',"content_uuid":'.json_encode($this->content_uuid).'';
		return $__result;
	}
	public function ToString()
	{
		return '{'.$this->ToStringInternal().'}';
	}
	public function ToArray($_array=array())
	{
		$_array = parent::ToArray($_array);
		// shopitem_id
		$_array['shopitem_id'] = $this->shopitem_id;
		// price
		$_array['price'] = $this->price;
		// content_uuid
		$_array['content_uuid'] = $this->content_uuid;
		return $_array;
	}
	public function FromArray($_array)
	{
		if(!is_array($_array)) return false;
		if(!parent::FromArray($_array)) return false;
		// shopitem_id
		if(!is_string($_array['shopitem_id'])) return false;
		$this->shopitem_id = $_array['shopitem_id'];
		// price
		if(!is_int($_array['price'])) return false;
		if($_array['price']<0 || $_array['price']>4294967295) return false;
		$this->price = $_array['price'];
		// content_uuid
		if(!is_string($_array['content_uuid'])) return false;
		$this->content_uuid = $_array['content_uuid'];
		return true;
	}
};

class CG_CARD_CONFIG extends A_CONTENT_OBJECT
{
	public $disp_name; // string
	public $point; // int

	public function __construct()
	{
		parent::__construct();
		$this->disp_name = '';
		$this->point = 0;
	}
	public function ToStringInternal()
	{
		$__result = parent::ToStringInternal();
		if($__result!='') $__result .= ',';
		// disp_name
		if(!is_string($this->disp_name)) return '';
		$__result .= '"disp_name":'.json_encode($this->disp_name).'';
		// point
		if(!is_int($this->point)) return '';
		if($this->point<0 || $this->point>4294967295) return false;
		$__result .= ',"point":'.$this->point;
		return $__result;
	}
	public function ToString()
	{
		return '{'.$this->ToStringInternal().'}';
	}
	public function ToArray($_array=array())
	{
		$_array = parent::ToArray($_array);
		// disp_name
		$_array['disp_name'] = $this->disp_name;
		// point
		$_array['point'] = $this->point;
		return $_array;
	}
	public function FromArray($_array)
	{
		if(!is_array($_array)) return false;
		if(!parent::FromArray($_array)) return false;
		// disp_name
		if(!is_string($_array['disp_name'])) return false;
		$this->disp_name = $_array['disp_name'];
		// point
		if(!is_int($_array['point'])) return false;
		if($_array['point']<0 || $_array['point']>4294967295) return false;
		$this->point = $_array['point'];
		return true;
	}
};

class DATASYNC_JSON_S2C
{
	public function DS_SyncOpen($flag)
	{
		$__result = '';
		if(!is_int($flag)) return false;
		if($flag<0 || $flag>4294967295) return false;
		$__result .= $flag;
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
		$__result .= json_encode($_uuid);
		ZionSession::Get()->Send('{"method":"DATASYNC_JSON_S2C.DS_CreateObjectDone","args":['.$__result.']}');
		return true;
	}
	public function DS_CreateObject($type, $json)
	{
		$__result = '';
		if(!is_string($type)) return false;
		$__result .= json_encode($type);
		$__result .= ',';
		if(!is_string($json)) return false;
		$__result .= json_encode($json);
		ZionSession::Get()->Send('{"method":"DATASYNC_JSON_S2C.DS_CreateObject","args":['.$__result.']}');
		return true;
	}
	public function DS_UpdateObject($_uuid, $json)
	{
		$__result = '';
		if(!is_string($_uuid)) return false;
		$__result .= json_encode($_uuid);
		$__result .= ',';
		if(!is_string($json)) return false;
		$__result .= json_encode($json);
		ZionSession::Get()->Send('{"method":"DATASYNC_JSON_S2C.DS_UpdateObject","args":['.$__result.']}');
		return true;
	}
	public function DS_DeleteObject($_uuid, $count)
	{
		$__result = '';
		if(!is_array($_uuid)) return false;
		$__result .= '[';
		for($__i=0; $__i<count($_uuid); $__i++)
		{
			if($__i>0) $__result .= ',';
			if(!is_string($_uuid[$__i])) return false;
			$__result .= json_encode($_uuid[$__i]);
		}
		$__result .= ']';
		$__result .= ',';
		if(!is_int($count)) return false;
		if($count<0 || $count>4294967295) return false;
		$__result .= $count;
		ZionSession::Get()->Send('{"method":"DATASYNC_JSON_S2C.DS_DeleteObject","args":['.$__result.']}');
		return true;
	}
}

function CGSERVER_BASE_Dispatcher($fname, $_array, $_this)
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
		if(count($_array)!=1) return false;
		if(!is_string($_array[0])) return false;
		$__avatar_name = $_array[0];
		$_this->CreateAvatar($__avatar_name);
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

function CGSERVER_GAME_Dispatcher($fname, $_array, $_this)
{
	if($fname=='Buy')
	{
		if(count($_array)!=1) return false;
		if(!is_string($_array[0])) return false;
		$__shopitem_id = $_array[0];
		$_this->Buy($__shopitem_id);
		return true;
	}
	if($fname=='Discard')
	{
		if(count($_array)!=1) return false;
		if(!is_string($_array[0])) return false;
		$__uuid = $_array[0];
		$_this->Discard($__uuid);
		return true;
	}
	if($fname=='Beg')
	{
		if(count($_array)!=1) return false;
		if(!is_int($_array[0])) return false;
		if($_array[0]<0 || $_array[0]>4294967295) return false;
		$__money = $_array[0];
		$_this->Beg($__money);
		return true;
	}
	if($fname=='Gamble')
	{
		if(count($_array)!=2) return false;
		if(!is_string($_array[0])) return false;
		$__card = $_array[0];
		if(!is_int($_array[1])) return false;
		$__mode = $_array[1];
		$_this->Gamble($__card, $__mode);
		return true;
	}
	return false;
}

class CGCALLBACK_BASE
{
	public function Pong()
	{
		ZionSession::Get()->Send('{"method":"CGCALLBACK_BASE.Pong","args":[]}');
		return true;
	}
	public function GetAvatarListCallback($arr, $count)
	{
		$__result = '';
		if(!is_array($arr)) return false;
		$__result .= '[';
		for($__i=0; $__i<count($arr); $__i++)
		{
			if($__i>0) $__result .= ',';
			$__result .= $arr[$__i]->ToString();
		}
		$__result .= ']';
		$__result .= ',';
		if(!is_int($count)) return false;
		if($count<0 || $count>4294967295) return false;
		$__result .= $count;
		ZionSession::Get()->Send('{"method":"CGCALLBACK_BASE.GetAvatarListCallback","args":['.$__result.']}');
		return true;
	}
	public function CreateAvatarCallback($errcode)
	{
		$__result = '';
		if(!is_int($errcode)) return false;
		if($errcode<0 || $errcode>4294967295) return false;
		$__result .= $errcode;
		ZionSession::Get()->Send('{"method":"CGCALLBACK_BASE.CreateAvatarCallback","args":['.$__result.']}');
		return true;
	}
	public function EnterGameCallback($errcode)
	{
		$__result = '';
		if(!is_int($errcode)) return false;
		if($errcode<0 || $errcode>4294967295) return false;
		$__result .= $errcode;
		ZionSession::Get()->Send('{"method":"CGCALLBACK_BASE.EnterGameCallback","args":['.$__result.']}');
		return true;
	}
	public function LeaveGameCallback($errcode)
	{
		$__result = '';
		if(!is_int($errcode)) return false;
		if($errcode<0 || $errcode>4294967295) return false;
		$__result .= $errcode;
		ZionSession::Get()->Send('{"method":"CGCALLBACK_BASE.LeaveGameCallback","args":['.$__result.']}');
		return true;
	}
}

class CGCALLBACK_GAME
{
	public function Ready()
	{
		ZionSession::Get()->Send('{"method":"CGCALLBACK_GAME.Ready","args":[]}');
		return true;
	}
	public function BegResult($money)
	{
		$__result = '';
		if(!is_int($money)) return false;
		if($money<0 || $money>4294967295) return false;
		$__result .= $money;
		ZionSession::Get()->Send('{"method":"CGCALLBACK_GAME.BegResult","args":['.$__result.']}');
		return true;
	}
	public function GambleResult($point)
	{
		$__result = '';
		if(!is_int($point)) return false;
		if($point<0 || $point>4294967295) return false;
		$__result .= $point;
		ZionSession::Get()->Send('{"method":"CGCALLBACK_GAME.GambleResult","args":['.$__result.']}');
		return true;
	}
}

?>