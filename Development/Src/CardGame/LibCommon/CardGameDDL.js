
function isArray(o) {
	return Object.prototype.toString.call(o) === '[object Array]';
}

function A_CONTENT_OBJECT() {
	this._uuid = '{00000000-0000-0000-0000-000000000000}';
	this._name = '';
	this.__proto__ = A_CONTENT_OBJECT;
}
A_CONTENT_OBJECT.serialize = function(v) {
	this._uuid = v['_uuid'].toString();
	this._name = v['_name'].toString();
	return true;
}
module.exports.A_CONTENT_OBJECT = A_CONTENT_OBJECT;

function A_LIVE_OBJECT() {
	this._uuid = '{00000000-0000-0000-0000-000000000000}';
	this.__proto__ = A_LIVE_OBJECT;
}
A_LIVE_OBJECT.serialize = function(v) {
	this._uuid = v['_uuid'].toString();
	return true;
}
module.exports.A_LIVE_OBJECT = A_LIVE_OBJECT;

function CARD_AVATAR_OPERATOR() {
	A_CONTENT_OBJECT.call(this);
	this.__proto__ = CARD_AVATAR_OPERATOR;
}
CARD_AVATAR_OPERATOR.serialize = function(v) {
	if(!A_CONTENT_OBJECT.serialize.call(this)) return false;
	return true;
}
CARD_AVATAR_OPERATOR.prototype = A_CONTENT_OBJECT;
module.exports.CARD_AVATAR_OPERATOR = CARD_AVATAR_OPERATOR;

function CARD_AVATAR_DESC() {
	A_LIVE_OBJECT.call(this);
	this.avatar_id = 0;
	this.avatar_name = '';
	this.__proto__ = CARD_AVATAR_DESC;
}
CARD_AVATAR_DESC.serialize = function(v) {
	if(!A_LIVE_OBJECT.serialize.call(this)) return false;
	this.avatar_id = parseInt(v['avatar_id']);
	if(this.avatar_id<0) return false;
	this.avatar_name = v['avatar_name'].toString();
	return true;
}
CARD_AVATAR_DESC.prototype = A_LIVE_OBJECT;
module.exports.CARD_AVATAR_DESC = CARD_AVATAR_DESC;

function CARD_AVATAR() {
	A_LIVE_OBJECT.call(this);
	this.avatar_name = '';
	this.__proto__ = CARD_AVATAR;
}
CARD_AVATAR.serialize = function(v) {
	if(!A_LIVE_OBJECT.serialize.call(this)) return false;
	this.avatar_name = v['avatar_name'].toString();
	return true;
}
CARD_AVATAR.prototype = A_LIVE_OBJECT;
module.exports.CARD_AVATAR = CARD_AVATAR;

function CARD_AVATAR_OWNOBJ() {
	A_LIVE_OBJECT.call(this);
	this.obj_name = '';
	this.obj_value = 0;
	this.__proto__ = CARD_AVATAR_OWNOBJ;
}
CARD_AVATAR_OWNOBJ.serialize = function(v) {
	if(!A_LIVE_OBJECT.serialize.call(this)) return false;
	this.obj_name = v['obj_name'].toString();
	this.obj_value = parseInt(v['obj_value']);
	return true;
}
CARD_AVATAR_OWNOBJ.prototype = A_LIVE_OBJECT;
module.exports.CARD_AVATAR_OWNOBJ = CARD_AVATAR_OWNOBJ;
