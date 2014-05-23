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

function CG_AVATAR_DESC() {
	this.__typename = 'CG_AVATAR_DESC';
	this.avatar_id = 0;
	this.avatar_name = '';
	this.__proto__ = CG_AVATAR_DESC;
}
CG_AVATAR_DESC.serialize = function(v) {
	this.avatar_id = parseInt(v['avatar_id']);
	if(this.avatar_id<0) return false;
	this.avatar_name = v['avatar_name'].toString();
	return true;
}
exports_struct.CG_AVATAR_DESC = CG_AVATAR_DESC;

function CG_AVATAR() {
	A_LIVE_OBJECT.call(this);
	this.__typename = 'CG_AVATAR';
	this.avatar_name = '';
	this.money = 0.0;
	this.__proto__ = CG_AVATAR;
}
CG_AVATAR.serialize = function(v) {
	if(!A_LIVE_OBJECT.serialize.call(this)) return false;
	this.avatar_name = v['avatar_name'].toString();
	this.money = parseFloat(v['money']);
	return true;
}
CG_AVATAR.prototype = A_LIVE_OBJECT;
exports_struct.CG_AVATAR = CG_AVATAR;

function CG_CARD() {
	A_LIVE_OBJECT.call(this);
	this.__typename = 'CG_CARD';
	this.content_uuid = '{00000000-0000-0000-0000-000000000000}';
	this.win_count = 0;
	this.lost_count = 0;
	this.__proto__ = CG_CARD;
}
CG_CARD.serialize = function(v) {
	if(!A_LIVE_OBJECT.serialize.call(this)) return false;
	this.content_uuid = v['content_uuid'].toString();
	this.win_count = parseInt(v['win_count']);
	if(this.win_count<0) return false;
	this.lost_count = parseInt(v['lost_count']);
	if(this.lost_count<0) return false;
	return true;
}
CG_CARD.prototype = A_LIVE_OBJECT;
exports_struct.CG_CARD = CG_CARD;

function CG_SHOPITEM() {
	A_CONTENT_OBJECT.call(this);
	this.__typename = 'CG_SHOPITEM';
	this.shopitem_id = '';
	this.price = 0;
	this.content_uuid = '{00000000-0000-0000-0000-000000000000}';
	this.__proto__ = CG_SHOPITEM;
}
CG_SHOPITEM.serialize = function(v) {
	if(!A_CONTENT_OBJECT.serialize.call(this)) return false;
	this.shopitem_id = v['shopitem_id'].toString();
	this.price = parseInt(v['price']);
	if(this.price<0) return false;
	this.content_uuid = v['content_uuid'].toString();
	return true;
}
CG_SHOPITEM.prototype = A_CONTENT_OBJECT;
exports_struct.CG_SHOPITEM = CG_SHOPITEM;

function CG_CARD_CONFIG() {
	A_CONTENT_OBJECT.call(this);
	this.__typename = 'CG_CARD_CONFIG';
	this.disp_name = '';
	this.point = 0;
	this.__proto__ = CG_CARD_CONFIG;
}
CG_CARD_CONFIG.serialize = function(v) {
	if(!A_CONTENT_OBJECT.serialize.call(this)) return false;
	this.disp_name = v['disp_name'].toString();
	this.point = parseInt(v['point']);
	if(this.point<0) return false;
	return true;
}
CG_CARD_CONFIG.prototype = A_CONTENT_OBJECT;
exports_struct.CG_CARD_CONFIG = CG_CARD_CONFIG;

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

