
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

function A_CONFUSED_NUMBER() {
	this.value = 0;
	this.mask = 0;
	this.__proto__ = A_CONFUSED_NUMBER;
}
A_CONFUSED_NUMBER.serialize = function(v) {
	this.value = parseInt(v['value']);
	if(this.value<0) return false;
	this.mask = parseInt(v['mask']);
	if(this.mask<0) return false;
	return true;
}
module.exports.A_CONFUSED_NUMBER = A_CONFUSED_NUMBER;

function A_CONFUSED_INT() {
	A_CONFUSED_NUMBER.call(this);
	this.__proto__ = A_CONFUSED_INT;
}
A_CONFUSED_INT.serialize = function(v) {
	if(!A_CONFUSED_NUMBER.serialize.call(this)) return false;
	return true;
}
A_CONFUSED_INT.prototype = A_CONFUSED_NUMBER;
module.exports.A_CONFUSED_INT = A_CONFUSED_INT;

function A_CONFUSED_FLOAT() {
	A_CONFUSED_NUMBER.call(this);
	this.__proto__ = A_CONFUSED_FLOAT;
}
A_CONFUSED_FLOAT.serialize = function(v) {
	if(!A_CONFUSED_NUMBER.serialize.call(this)) return false;
	return true;
}
A_CONFUSED_FLOAT.prototype = A_CONFUSED_NUMBER;
module.exports.A_CONFUSED_FLOAT = A_CONFUSED_FLOAT;

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
