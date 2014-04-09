"use strict";

function isArray(o) {
	return Object.prototype.toString.call(o) === '[object Array]';
}

var exports_struct = {};
var exports_stub = {};
var exports_proxy = {};

function A_CONTENT_OBJECT() {
	this.__typename = 'A_CONTENT_OBJECT';
	this._uuid = '{00000000-0000-0000-0000-000000000000}';
	this._name = '';
	this.__proto__ = A_CONTENT_OBJECT;
}
A_CONTENT_OBJECT.serialize = function(v) {
	this._uuid = v['_uuid'].toString();
	this._name = v['_name'].toString();
	return true;
}
exports_struct.A_CONTENT_OBJECT = A_CONTENT_OBJECT;

function A_LIVE_OBJECT() {
	this.__typename = 'A_LIVE_OBJECT';
	this._uuid = '{00000000-0000-0000-0000-000000000000}';
	this.__proto__ = A_LIVE_OBJECT;
}
A_LIVE_OBJECT.serialize = function(v) {
	this._uuid = v['_uuid'].toString();
	return true;
}
exports_struct.A_LIVE_OBJECT = A_LIVE_OBJECT;

function STUB_DATASYNC_JSON_C2S(_this)
{
	this._this = _this;
}
STUB_DATASYNC_JSON_C2S.prototype.Dispatch = function (cmd, args) {
	var _fun;
	switch(cmd) {
	case 'DS_CreateObject':
		if(args.length!=2) break;
		_fun = this._this.DS_CreateObject;
		_fun.apply(this._this, args);
		return true;
	case 'DS_UpdateObject':
		if(args.length!=2) break;
		_fun = this._this.DS_UpdateObject;
		_fun.apply(this._this, args);
		return true;
	case 'DS_DeleteObject':
		if(args.length!=2) break;
		_fun = this._this.DS_DeleteObject;
		_fun.apply(this._this, args);
		return true;
	}
	return false;
}
exports_stub.DATASYNC_JSON_C2S = STUB_DATASYNC_JSON_C2S

function PROXY_DATASYNC_JSON_S2C()
{
}
PROXY_DATASYNC_JSON_S2C.prototype.DS_SyncOpen = function () {
	if(arguments.length!=1) return undefined;
	var args = [];
	args.push(arguments[0]);
	return '{"method":"DATASYNC_JSON_S2C.DS_SyncOpen","args":' + JSON.stringify(args) + '}';
}
PROXY_DATASYNC_JSON_S2C.prototype.DS_SyncReady = function () {
	if(arguments.length!=0) return undefined;
	var args = [];
	return '{"method":"DATASYNC_JSON_S2C.DS_SyncReady","args":' + JSON.stringify(args) + '}';
}
PROXY_DATASYNC_JSON_S2C.prototype.DS_SyncClose = function () {
	if(arguments.length!=0) return undefined;
	var args = [];
	return '{"method":"DATASYNC_JSON_S2C.DS_SyncClose","args":' + JSON.stringify(args) + '}';
}
PROXY_DATASYNC_JSON_S2C.prototype.DS_CreateObjectDone = function () {
	if(arguments.length!=1) return undefined;
	var args = [];
	args.push(arguments[0]);
	return '{"method":"DATASYNC_JSON_S2C.DS_CreateObjectDone","args":' + JSON.stringify(args) + '}';
}
PROXY_DATASYNC_JSON_S2C.prototype.DS_CreateObject = function () {
	if(arguments.length!=2) return undefined;
	var args = [];
	args.push(arguments[0]);
	args.push(arguments[1]);
	return '{"method":"DATASYNC_JSON_S2C.DS_CreateObject","args":' + JSON.stringify(args) + '}';
}
PROXY_DATASYNC_JSON_S2C.prototype.DS_UpdateObject = function () {
	if(arguments.length!=2) return undefined;
	var args = [];
	args.push(arguments[0]);
	args.push(arguments[1]);
	return '{"method":"DATASYNC_JSON_S2C.DS_UpdateObject","args":' + JSON.stringify(args) + '}';
}
PROXY_DATASYNC_JSON_S2C.prototype.DS_DeleteObject = function () {
	if(arguments.length!=2) return undefined;
	var args = [];
	args.push(arguments[0]);
	args.push(arguments[1]);
	return '{"method":"DATASYNC_JSON_S2C.DS_DeleteObject","args":' + JSON.stringify(args) + '}';
}
exports_proxy.DATASYNC_JSON_S2C = PROXY_DATASYNC_JSON_S2C

function CARD_AVATAR_OPERATOR() {
	A_CONTENT_OBJECT.call(this);
	this.__typename = 'CARD_AVATAR_OPERATOR';
	this.v1 = 0;
	this.v2 = 0;
	this.v3 = 0.0;
	this.__proto__ = CARD_AVATAR_OPERATOR;
}
CARD_AVATAR_OPERATOR.serialize = function(v) {
	if(!A_CONTENT_OBJECT.serialize.call(this)) return false;
	this.v1 = parseInt(v['v1']);
	if(this.v1<0) return false;
	this.v2 = parseInt(v['v2']);
	this.v3 = parseFloat(v['v3']);
	return true;
}
CARD_AVATAR_OPERATOR.prototype = A_CONTENT_OBJECT;
exports_struct.CARD_AVATAR_OPERATOR = CARD_AVATAR_OPERATOR;

function CARD_AVATAR_DESC() {
	A_LIVE_OBJECT.call(this);
	this.__typename = 'CARD_AVATAR_DESC';
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
exports_struct.CARD_AVATAR_DESC = CARD_AVATAR_DESC;

function CARD_AVATAR() {
	A_LIVE_OBJECT.call(this);
	this.__typename = 'CARD_AVATAR';
	this.avatar_name = '';
	this.__proto__ = CARD_AVATAR;
}
CARD_AVATAR.serialize = function(v) {
	if(!A_LIVE_OBJECT.serialize.call(this)) return false;
	this.avatar_name = v['avatar_name'].toString();
	return true;
}
CARD_AVATAR.prototype = A_LIVE_OBJECT;
exports_struct.CARD_AVATAR = CARD_AVATAR;

function CARD_AVATAR_OWNOBJ() {
	A_LIVE_OBJECT.call(this);
	this.__typename = 'CARD_AVATAR_OWNOBJ';
	this.obj_name = '';
	this.obj_value = 0.0;
	this.__proto__ = CARD_AVATAR_OWNOBJ;
}
CARD_AVATAR_OWNOBJ.serialize = function(v) {
	if(!A_LIVE_OBJECT.serialize.call(this)) return false;
	this.obj_name = v['obj_name'].toString();
	this.obj_value = parseFloat(v['obj_value']);
	return true;
}
CARD_AVATAR_OWNOBJ.prototype = A_LIVE_OBJECT;
exports_struct.CARD_AVATAR_OWNOBJ = CARD_AVATAR_OWNOBJ;

function STUB_CARDGAME_C2S(_this)
{
	this._this = _this;
}
STUB_CARDGAME_C2S.prototype.Dispatch = function (cmd, args) {
	var _fun;
	switch(cmd) {
	case 'Ping':
		if(args.length!=0) break;
		_fun = this._this.Ping;
		_fun.apply(this._this, args);
		return true;
	case 'SelectServer':
		if(args.length!=1) break;
		_fun = this._this.SelectServer;
		_fun.apply(this._this, args);
		return true;
	case 'GetAvatarList':
		if(args.length!=0) break;
		_fun = this._this.GetAvatarList;
		_fun.apply(this._this, args);
		return true;
	case 'CreateAvatar':
		if(args.length!=2) break;
		_fun = this._this.CreateAvatar;
		_fun.apply(this._this, args);
		return true;
	case 'EnterGame':
		if(args.length!=1) break;
		_fun = this._this.EnterGame;
		_fun.apply(this._this, args);
		return true;
	case 'LeaveGame':
		if(args.length!=0) break;
		_fun = this._this.LeaveGame;
		_fun.apply(this._this, args);
		return true;
	}
	return false;
}
exports_stub.CARDGAME_C2S = STUB_CARDGAME_C2S

function PROXY_CARDGAME_S2C()
{
}
PROXY_CARDGAME_S2C.prototype.Pong = function () {
	if(arguments.length!=0) return undefined;
	var args = [];
	return '{"method":"CARDGAME_S2C.Pong","args":' + JSON.stringify(args) + '}';
}
PROXY_CARDGAME_S2C.prototype.GetAvatarListCallback = function () {
	if(arguments.length!=3) return undefined;
	var args = [];
	args.push(arguments[0]);
	args.push(arguments[1]);
	args.push(arguments[2]);
	return '{"method":"CARDGAME_S2C.GetAvatarListCallback","args":' + JSON.stringify(args) + '}';
}
PROXY_CARDGAME_S2C.prototype.CreateAvatarCallback = function () {
	if(arguments.length!=1) return undefined;
	var args = [];
	args.push(arguments[0]);
	return '{"method":"CARDGAME_S2C.CreateAvatarCallback","args":' + JSON.stringify(args) + '}';
}
PROXY_CARDGAME_S2C.prototype.EnterGameCallback = function () {
	if(arguments.length!=1) return undefined;
	var args = [];
	args.push(arguments[0]);
	return '{"method":"CARDGAME_S2C.EnterGameCallback","args":' + JSON.stringify(args) + '}';
}
PROXY_CARDGAME_S2C.prototype.LeaveGameCallback = function () {
	if(arguments.length!=1) return undefined;
	var args = [];
	args.push(arguments[0]);
	return '{"method":"CARDGAME_S2C.LeaveGameCallback","args":' + JSON.stringify(args) + '}';
}
exports_proxy.CARDGAME_S2C = PROXY_CARDGAME_S2C

module.exports.struct = exports_struct;
module.exports.stub = exports_stub;
module.exports.proxy = exports_proxy;

module.exports.isLiveObject = function(val) {
	try {
		if(!exports_struct.hasOwnProperty(val.__typename)) {
			return false;
		}
		if(val.__proto__!=exports_struct[val.__typename]) {
			return false;
		}
		var proto = this.__proto;
		while(proto) {
			if(proto==A_LIVE_OBJECT) {
				return true;
			}
		}
	} catch(err) {
	}
	return false;
}

module.exports.isContentObject = function(val) {
	try {
		if(!exports_struct.hasOwnProperty(val.__typename)) {
			return false;
		}
		if(val.__proto__!=exports_struct[val.__typename]) {
			return false;
		}
		var proto = this.__proto;
		while(proto) {
			if(proto==A_CONTENT_OBJECT) {
				return true;
			}
		}
	} catch(err) {
	}
	return false;
}

module.exports.createObject = function(typename) {
	if(!exports_struct.hasOwnProperty(typename)) {
		return undefined;
	}
	return new exports_struct[typename];
}

module.exports.checkObjectType = function(obj, typename) {
	try {
		if(obj.__typename!=typename) {
			return false;
		}
		if(!exports_struct.hasOwnProperty(typename)) {
			return false;
		}
		if(obj.__proto__!=exports_struct[typename]) {
			return false;
		}
		return true;
	} catch(err) {
	}
	return false;
}

