
var fs = require('fs');
var path = require('path');
var util = require('util');

if(!String.prototype.trim) {
	String.prototype.trim = function () {
		return this.replace(/^\s+|\s+$/g, '');
	};
}

if(!String.prototype.ltrim) {
	String.prototype.ltrim = function () {
		return this.replace(/^\s+/,'');
	};
}

if(!String.prototype.rtrim) {
	String.prototype.rtrim = function () {
		return this.replace(/\s+$/,'');
	};
}

if(!String.prototype.fulltrim) {
	String.prototype.fulltrim = function ()
	{
		return this.replace(/(?:(?:^|\n)\s+|\s+(?:$|\n))/g,'').replace(/\s+/g,' ');
	};
}

if(!String.prototype.format) {
	String.prototype.format = function() {
		var args = arguments;
		return this.replace(/{(\d+)}/g, function(match, number) { 
			return typeof args[number] != 'undefined'
			? args[number]
			: match
			;
		});
	};
}

function mkdirp_sync (p, mode, made) {
	if(mode === undefined) {
		mode = 0777 & (~process.umask());
	}
	if(!made) made = null;

	if(typeof mode === 'string') mode = parseInt(mode, 8);
	p = path.resolve(p);

	try {
		fs.mkdirSync(p, mode);
		made = made || p;
	}
	catch (err0) {
		switch (err0.code) {
		case 'ENOENT' :
			made = mkdirp_sync(path.dirname(p), mode, made);
			mkdirp_sync(p, mode, made);
			break;

		// In the case of any other error, just see if there's a dir
		// there already.  If so, then hooray!  If not, then something
		// is borked.
		default:
			var stat;
			try {
				stat = fs.statSync(p);
			}
			catch (err1) {
				throw err0;
			}
			if(!stat.isDirectory()) throw err0;
			break;
		}
	}

	return made;
};

function autoMakeDir(p) {
	if(p.indexOf('/')>=0) {
		p = p.substring(0, p.lastIndexOf('/'));
	}
	if(p=='./') return true;

	mkdirp_sync(p);
	return true;
}

function ProcessPropertyFile (proj, filename) {
	var path = proj.path + filename;
	path = path.replace(/\\/g, "/");
	var lines = fs.readFileSync(path, 'utf-8').split('\n');

	for(var i=0; i<lines.length; i++) {
		var pos;
		pos = lines[i].indexOf('<AdditionalIncludeDirectories');
		if(pos>0) {
			pos = lines[i].indexOf('>');
			var value = lines[i].substring(pos+1);
			value = value.substring(0, value.indexOf('<'));
			value = value.replace(/\\/g, "/");
			value = value.replace(/\;\%\(AdditionalIncludeDirectories\)/g, "/");
			value = value.split(';');
			for(i=0; i<value.length; i++) {
				if(value[i].indexOf("$(SolutionDir)")>=0) {
					value[i] = value[i].replace(/\$\(SolutionDir\)/g, "");
				} else {
					value[i] = proj.path + value[i];
				}
			}
			return value;
		}
	}
	return [];
}

function ProjectFile () {
	this.path = '';
	this.name = '';
	this.deps = [];
	this.type = '';
	this.inc_files = [];
	this.ddl_files = [];
	this.rpc_files = [];
	this.src_files = [];

	this.inc_dir = [];
}

ProjectFile.prototype.load = function (filename) {
	this.path = filename.substring(0, filename.lastIndexOf('\\')+1);
	this.name = filename.substring(filename.lastIndexOf('\\')+1);
	this.name = this.name.substring(0, this.name.lastIndexOf('.'));

	filename = filename.replace(/\\/g, "/");

	var lines = fs.readFileSync(path.normalize(filename), 'utf-8').split('\n');

	for(var i=0; i<lines.length; i++) {
		var pos;

		pos = lines[i].indexOf('<Import Project="');
		if(pos>0) {
			pos = lines[i].indexOf('"');
			var value = lines[i].substring(pos+1);
			value = value.substring(0, value.indexOf('"'));
			if(value.indexOf('$')>=0) {
				continue;
			}
			this.inc_dir = this.inc_dir.concat(ProcessPropertyFile(this, value));
			continue;
		}

		pos = lines[i].indexOf('<ConfigurationType>');
		if(pos>0) {
			pos = lines[i].indexOf('>');
			var name = lines[i].substring(pos+1);
			name = name.substring(0, name.indexOf('<'));
			this.type = name;
			continue;
		}

		pos = lines[i].indexOf('<ProjectReference Include="');
		if(pos>0) {
			pos = lines[i].indexOf('>');
			var name = lines[i].substring(pos+1);
			name = name.substring(0, name.indexOf('"'));
			this.deps.push(name);
			continue;
		}

		pos = lines[i].indexOf('<ClCompile Include="');
		var files;
		if(pos>=0) {
			files = this.src_files;
		} else {
			var pos = lines[i].indexOf('<ClInclude Include="');
			if(pos>=0) {
				files = this.inc_files;
			} else {
				var pos = lines[i].indexOf('<CustomBuild Include="');
				if(pos>=0) {
					var pos = lines[i].indexOf('.ddl"');
					if(pos>=0) {
						files = this.ddl_files;
					} else {
						var pos = lines[i].indexOf('.rpc"');
						if(pos>=0) {
							files = this.rpc_files;
						} else {
							continue;
						}
					}
				} else {
					continue;
				}
			}
		}
		pos = lines[i].indexOf('"');
		if(pos<0) throw "wwwwww";
		var name = lines[i].substring(pos+1);
		pos = name.indexOf('"');
		if(pos<0) throw "wwwwww";
		name = name.substring(0, pos);

		files.push(name);
	}
}

function SolutionFile () {
	this.projs = [];
}

SolutionFile.prototype.load = function (filename) {
	var lines = fs.readFileSync(filename, 'utf-8').split('\n');

	for(var i=0; i<lines.length; i++) {
		var pos = lines[i].indexOf('Project("');
		if(pos<0) continue;
		pos = lines[i].indexOf(',');
		if(pos<0) throw "wwwwww";
		var name = lines[i].substring(pos+3);
		pos = name.indexOf('"');
		if(pos<0) throw "wwwwww";
		name = name.substring(0, pos);
		pos = name.indexOf('.vcxproj');
		if(pos<0) continue;
		this.projs.push(name);
	}

	for(var i=0; i<this.projs.length; i++) {
		var proj = new ProjectFile();
		proj.load(this.projs[i]);
		this.projs[i] = proj;
	}
}

SolutionFile.prototype.getProject = function (name) {
	for(var i=0; i<this.projs.length; i++) {
		if(this.projs[i].name==name) {
			return this.projs[i];
		}
	}
	return undefined;
}

function AppBuilder (solution) {
	this.files_data = {};
	this.solution = solution;
	this.cc_exe = 'clang {1} {0} -c -o {2}';
	this.ln_exe = 'clang {0} -o {1}';
	this.sl_exe = 'ar crv {0} {1}';
	this.dl_exe = '';
}

AppBuilder.prototype.getFileTime = function (filename) {
	if(filename in this.files_data) {
		return this.files_data[filename];
	} else {
		var ts;
		try {
			ts = Date.parse(fs.statSync(filename)) / 1000;
		} catch(e) {
			ts = Date.now() / 1000;
		}
		this.files_data[filename] = ts;
		return ts;
	}
}

AppBuilder.prototype.setPlatform = function (platform) {
	if(platform=='Win32' || platform=='Win64') {
		this.obj_ext = '.obj';
		this.exe_ext = '.exe';
		this.dll_ext = '.dll';
		this.lib_ext = '.lib';
	} else {
		this.obj_ext = '.o';
		this.exe_ext = '';
		this.dll_ext = '.so';
		this.lib_ext = '.a';
	}
}

AppBuilder.prototype.setConfiguration = function (config) {
	if(config=='Debug') {
		this.output_dir = "../../Binaries/Debug/";
		this.object_dir = "../Intermediate/Debug/";
		this.cc_flag = '-g';
		this.ln_flag = '';
		this.sl_flag = '';
		this.dl_flag = '';
		return;
	}
	if(config=='Release') {
		this.output_dir = "../../Binaries/Release/";
		this.object_dir = "../Intermediate/Release/";
		this.cc_flag = '-O2';
		this.ln_flag = '';
		this.sl_flag = '';
		this.dl_flag = '';
		return;
	}
}

AppBuilder.prototype.buildRPC = function (proj) {
	var rpc_exe = path.normalize(this.output_dir + 'RpcGen' + this.exe_ext);
	for(var i=0; i<proj.rpc_files.length; i++) {
		var rpc_path = path.normalize(proj.path + proj.rpc_files[i]);
		var rpc_gen = rpc_path.substring(0, rpc_path.lastIndexOf('.'));
		if(!this.needUpdate([rpc_exe, rpc_path], [rpc_gen+'.c.cpp', rpc_gen+'.c.h', rpc_gen+'.s.cpp', rpc_gen+'.s.h'])) {
			continue;
		}

		var cmdline = '{0} {1}'.format(rpc_exe, rpc_path);
		console.log(cmdline);
	}
}

AppBuilder.prototype.buildDDL = function (proj) {
	var ddl_exe = path.normalize(this.output_dir + 'DDLGen' + this.exe_ext);
	for(var i=0; i<proj.ddl_files.length; i++) {
		var ddl_path = path.normalize(proj.path + proj.ddl_files[i]);
		var ddl_gen = ddl_path.substring(0, ddl_path.lastIndexOf('.'));
		if(!this.needUpdate([ddl_exe, ddl_path], [ddl_gen+'.cpp', ddl_gen+'.h'])) {
			continue;
		}

		var cmdline = '{0} {1}'.format(ddl_exe, ddl_path);
		console.log(cmdline);
	}
}

AppBuilder.prototype.buildINC = function (proj) {
	/*
	for(var i=0; i<proj.inc_files.length; i++) {
		if(proj.inc_files[i].indexOf('AutoGen.h')>0) {
			var cwd = process.cwd();
			var proj_path = cwd + '\\' + proj.path;
			proj_path = path.normalize(proj_path.replace(/\\/g, "/"));
			console.log('cd ' + proj_path);
			console.log('node CodeGen.js');
			console.log('cd ' + cwd);
		}
	}
	*/
}

AppBuilder.prototype.loadCPPDepend = function (filename) {
	var ret;
	try {
		ret = fs.readFileSync(obj_files, 'utf-8').split('\n');
		ret = ret.join('').replace(/\//g, '').trim().split(' ');
	} catch(e) {
		return undefined;
	}
	return ret;
}

AppBuilder.prototype.needUpdate = function (src_files, gen_files) {
	if(typeof(src_files)=='String') {
		src_files = [src_files];
	}

	var src_ts = 0;
	for(var i=0; i<src_files.length; i++) {
		if(!fs.existsSync(src_files[i])) {
			return true;
		}
		var ts = Date.parse(fs.statSync(src_files[i]).mtime) / 1000;
		if(ts>src_ts) src_ts = ts;
	}

	if(typeof(gen_files)=='String') {
		gen_files = [gen_files];
	}

	for(var i=0; i<gen_files.length; i++) {
		if(!fs.existsSync(gen_files[i])) {
			return true;
		}
		var ts = Date.parse(fs.statSync(gen_files[i]).mtime) / 1000;
		if(ts <= src_ts) {
			return true;
		}
	}

	return false;
}

AppBuilder.prototype.buildBIN = function (proj) {
	var inc_cmd = '';
	for(var i=0; i<proj.inc_dir.length; i++) {
		if(i>0) inc_cmd += ' ';
		var inc_path = process.cwd() + '\\' + proj.inc_dir[i];
		inc_path = inc_path.replace(/\\/g, "/");
		inc_path = path.normalize(inc_path);
		inc_cmd += '-I' + inc_path;
	}

	var objs = [];
	for(var i=0; i<proj.src_files.length; i++) {
		var src_path = proj.path + proj.src_files[i];
		src_path = src_path.replace(/\\/g, "/");
		var dst_path = proj.src_files[i];
		dst_path = dst_path.replace(/\\/g, "/");
		dst_path = dst_path.replace(/\//g, "_");
		dst_path = dst_path.replace(/\./g, "_");
		dst_path = this.object_dir + proj.name + '/' + dst_path;
		src_path = path.normalize(src_path);
		dst_path = path.normalize(dst_path);

		objs.push(dst_path);

		var src_files = this.loadCPPDepend(dst_path+'.d');
		if(src_files && !this.needUpdate(src_files, this.cc_flag, dst_path+this.obj_ext)) {
			continue;
		}

		autoMakeDir(dst_path);

		console.log('echo compile '+proj.src_files[i]);
		cmdline = this.cc_exe.format(src_path, inc_cmd + ' -M ' + this.cc_flag, dst_path+'.d');
		console.log(cmdline);
		var cmdline = this.cc_exe.format(src_path, inc_cmd + ' ' + this.cc_flag, dst_path+this.obj_ext);
		console.log(cmdline);
	}

	var objs_str = objs.join(this.obj_ext + ' ') + this.obj_ext;
	var cmdline;
	if(proj.type=='StaticLibrary') {
		var lib_path = this.object_dir + proj.name;
		lib_path = lib_path.replace(/\\/g, "/");
		autoMakeDir(lib_path);

		if(!this.needUpdate(objs, lib_path+this.lib_ext)) {
			return;
		}
		cmdline = this.sl_exe.format(lib_path+this.lib_ext, objs_str);
	} else {
		var exe_path = this.output_dir + proj.name;
		exe_path = exe_path.replace(/\\/g, "/");
		autoMakeDir(exe_path);

		if(!this.needUpdate(objs, exe_path+this.exe_ext)) {
			return;
		}
		cmdline = this.ln_exe.format(objs_str, exe_path+this.exe_ext);
	}

	console.log(cmdline);
}

AppBuilder.prototype.build = function (project_name) {
	var proj = this.solution.getProject(project_name);
	if(!proj) {
		console.log('project '+project_name+' not found!');
		process.exit();
	}

	this.buildRPC(proj);
	this.buildDDL(proj);
	this.buildINC(proj);
	this.buildBIN(proj);
}

AppBuilder.prototype.clean = function (project_name) {
}

if(process.argv.length<3) {
	console.log('invalid parameter');
	process.exit();
}

var solution = new SolutionFile();
solution.load(process.argv[2]);

var builder = new AppBuilder(solution);
builder.setConfiguration('Debug');
builder.setPlatform('Win32');

builder.build('LibCommon');

/*
builder.build('DDLGen');
builder.build('RpcGen');
builder.build('LibBase');
builder.build('LibClient');
builder.build('LibCommon');
builder.build('LibCardCommon');
*/
