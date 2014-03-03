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

class A_CONFUSED_NUMBER
{
	public $value; // int
	public $mask; // int

	public function __construct()
	{
		$this->value = 0;
		$this->mask = 0;
	}
	public function ToStringInternal()
	{
		$__result = '';
		// value
		if(!is_int($this->value)) return '';
		if($this->value<0 || $this->value>4294967295) return false;
		$__result = $__result.'"value":'.$this->value;
		// mask
		if(!is_int($this->mask)) return '';
		if($this->mask<0 || $this->mask>4294967295) return false;
		$__result = $__result.',"mask":'.$this->mask;
		return $__result;
	}
	public function ToString()
	{
		return '{'.$this->ToStringInternal().'}';
	}
	public function ToArray($_array=array())
	{
		// value
		$_array['value'] = $this->value;
		// mask
		$_array['mask'] = $this->mask;
		return $_array;
	}
	public function FromArray($_array)
	{
		if(!is_array($_array)) return false;
		// value
		if(!is_int($_array['value'])) return false;
		if($_array['value']<0 || $_array['value']>4294967295) return false;
		$this->value = $_array['value'];
		// mask
		if(!is_int($_array['mask'])) return false;
		if($_array['mask']<0 || $_array['mask']>4294967295) return false;
		$this->mask = $_array['mask'];
		return true;
	}
};

class A_CONFUSED_INT extends A_CONFUSED_NUMBER
{
	public function __construct()
	{
		parent::__construct();
	}
	public function ToStringInternal()
	{
		$__result = parent::ToStringInternal();
		return $__result;
	}
	public function ToString()
	{
		return '{'.$this->ToStringInternal().'}';
	}
	public function ToArray($_array=array())
	{
		$_array = parent::ToArray($_array);
		return $_array;
	}
	public function FromArray($_array)
	{
		if(!is_array($_array)) return false;
		if(!parent::FromArray($_array)) return false;
		return true;
	}
};

class A_CONFUSED_FLOAT extends A_CONFUSED_NUMBER
{
	public function __construct()
	{
		parent::__construct();
	}
	public function ToStringInternal()
	{
		$__result = parent::ToStringInternal();
		return $__result;
	}
	public function ToString()
	{
		return '{'.$this->ToStringInternal().'}';
	}
	public function ToArray($_array=array())
	{
		$_array = parent::ToArray($_array);
		return $_array;
	}
	public function FromArray($_array)
	{
		if(!is_array($_array)) return false;
		if(!parent::FromArray($_array)) return false;
		return true;
	}
};

class CARD_AVATAR_OPERATOR extends A_CONTENT_OBJECT
{
	public function __construct()
	{
		parent::__construct();
	}
	public function ToStringInternal()
	{
		$__result = parent::ToStringInternal();
		return $__result;
	}
	public function ToString()
	{
		return '{'.$this->ToStringInternal().'}';
	}
	public function ToArray($_array=array())
	{
		$_array = parent::ToArray($_array);
		return $_array;
	}
	public function FromArray($_array)
	{
		if(!is_array($_array)) return false;
		if(!parent::FromArray($_array)) return false;
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
	public $obj_value; // int

	public function __construct()
	{
		parent::__construct();
		$this->obj_name = '';
		$this->obj_value = 0;
	}
	public function ToStringInternal()
	{
		$__result = parent::ToStringInternal();
		if($__result!='') $__result = $__result.',';
		// obj_name
		if(!is_string($this->obj_name)) return '';
		$__result = $__result.'"obj_name":"'.$this->obj_name.'"';
		// obj_value
		if(!is_int($this->obj_value)) return '';
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
		if(!is_int($_array['obj_value'])) return false;
		$this->obj_value = $_array['obj_value'];
		return true;
	}
};

function CARDGAME_C2S_Dispatcher($fname, $_array, $_this)
{
	if($fname=='Ping')
	{
		$_this->Ping();
		return true;
	}
	if($fname=='SelectServer')
	{
		if(!is_int($_array['server_id'])) return false;
		if($_array['server_id']<0 || $_array['server_id']>4294967295) return false;
		$__server_id = $_array['server_id'];
		$_this->SelectServer($__server_id);
		return true;
	}
	if($fname=='GetAvatarList')
	{
		$_this->GetAvatarList();
		return true;
	}
	if($fname=='CreateAvatar')
	{
		if(!is_string($_array['avatar_name'])) return false;
		$__avatar_name = $_array['avatar_name'];
		if(!is_int($_array['type'])) return false;
		if($_array['type']<0 || $_array['type']>4294967295) return false;
		$__type = $_array['type'];
		$_this->CreateAvatar($__avatar_name, $__type);
		return true;
	}
	if($fname=='EnterGame')
	{
		if(!is_int($_array['avatar_id'])) return false;
		if($_array['avatar_id']<0 || $_array['avatar_id']>4294967295) return false;
		$__avatar_id = $_array['avatar_id'];
		$_this->EnterGame($__avatar_id);
		return true;
	}
	if($fname=='Change')
	{
		if(!is_string($_array['obj_uuid'])) return false;
		$__obj_uuid = $_array['obj_uuid'];
		if(!is_int($_array['value'])) return false;
		$__value = $_array['value'];
		$_this->Change($__obj_uuid, $__value);
		return true;
	}
	if($fname=='LeaveGame')
	{
		$_this->LeaveGame();
		return true;
	}
	return false;
}

class CARDGAME_S2C
{
	public function Pong()
	{
		ZionSession::Get()->Send('{"method":"CARDGAME_S2C.Pong","message":{}}');
		return true;
	}
	public function GetAvatarListCallback($errcode, $arr, $count)
	{
		if(!is_int($errcode)) return false;
		if($errcode<0 || $errcode>4294967295) return false;
		$__result = '"errcode":'.$errcode;
		if(!is_array($arr)) return false;
		$__result = $__result.',"arr":[';
		for($__i=0; $__i<count($arr); $__i++)
		{
			if($__i>0) $__result = $__result.',';
			$__result = $__result.$arr[$__i]->ToString();
		}
		$__result = $__result.']';
		if(!is_int($count)) return false;
		if($count<0 || $count>4294967295) return false;
		$__result = $__result.',"count":'.$count;
		ZionSession::Get()->Send('{"method":"CARDGAME_S2C.GetAvatarListCallback","message":{'.$__result.'}}');
		return true;
	}
	public function CreateAvatarCallback($errcode)
	{
		if(!is_int($errcode)) return false;
		if($errcode<0 || $errcode>4294967295) return false;
		$__result = '"errcode":'.$errcode;
		ZionSession::Get()->Send('{"method":"CARDGAME_S2C.CreateAvatarCallback","message":{'.$__result.'}}');
		return true;
	}
	public function EnterGameCallback($errcode)
	{
		if(!is_int($errcode)) return false;
		if($errcode<0 || $errcode>4294967295) return false;
		$__result = '"errcode":'.$errcode;
		ZionSession::Get()->Send('{"method":"CARDGAME_S2C.EnterGameCallback","message":{'.$__result.'}}');
		return true;
	}
	public function SyncAvatar($avatar)
	{
		if(!is_object($avatar) || get_class($avatar)!='CARD_AVATAR') return false;
		$__result = '"avatar":'.$avatar->ToString();
		ZionSession::Get()->Send('{"method":"CARDGAME_S2C.SyncAvatar","message":{'.$__result.'}}');
		return true;
	}
	public function SyncAvatarOwnObj($ownobj)
	{
		if(!is_object($ownobj) || get_class($ownobj)!='CARD_AVATAR_OWNOBJ') return false;
		$__result = '"ownobj":'.$ownobj->ToString();
		ZionSession::Get()->Send('{"method":"CARDGAME_S2C.SyncAvatarOwnObj","message":{'.$__result.'}}');
		return true;
	}
	public function SyncCompleted()
	{
		ZionSession::Get()->Send('{"method":"CARDGAME_S2C.SyncCompleted","message":{}}');
		return true;
	}
	public function LeaveGameCallback($errcode)
	{
		if(!is_int($errcode)) return false;
		if($errcode<0 || $errcode>4294967295) return false;
		$__result = '"errcode":'.$errcode;
		ZionSession::Get()->Send('{"method":"CARDGAME_S2C.LeaveGameCallback","message":{'.$__result.'}}');
		return true;
	}
}

?>