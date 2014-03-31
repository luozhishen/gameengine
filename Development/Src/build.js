
var fs = require('fs');
var path = require('path');
var util = require('util');
var os = require('os');

if(!String.prototype.trim) {
	String.prototype.trim = function () {
		return this.replace(/^\s+|\s+$/g, '');
	};
}

if(!String.prototype.ltrim) {
	String.prototype.ltrim = function () {
		return this.replace(/^\s+/, '');
	};
}

if(!String.prototype.rtrim) {
	String.prototype.rtrim = function () {
		return this.replace(/\s+$/, '');
	};
}

if(!String.prototype.fulltrim) {
	String.prototype.fulltrim = function ()
	{
		return this.replace(/(?:(?:^|\n)\s+|\s+(?:$|\n))/g,'').replace(/\s+/g, ' ');
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
			name = lines[i];
			name = name.substring(name.lastIndexOf('\\')+1);
			name = name.substring(0, name.indexOf('.vcxproj'));
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
		name = name.substring(0, pos).replace(/\\/g, "/");
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
		if(pos<0) throw "invalid solution file";
		var name = lines[i].substring(pos+3);
		pos = name.indexOf('"');
		if(pos<0) throw "invalid solution file";
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
	this.setPlatform(os.platform());
	this.setConfiguration('debug');
	this.jobs = [];
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
	if(platform=='win32') {
		this.obj_ext = '.obj';
		this.exe_ext = '.exe';
		this.dll_ext = '.dll';
		this.lib_ext = '.lib';
		this.cd_exe = 'echo cl {1} {0} /c /Fo{2}';
		this.cc_exe = 'cl {1} {0} /c /Fo{2}';
		this.ld_exe = 'cl {2} {0} /Fe{1}';
		this.sl_exe = 'echo {0} {1}';
		this.dl_exe = '';
		this.platform = platform;
		return;
	} else {
		this.obj_ext = '.o';
		this.exe_ext = '';
		this.dll_ext = '.so';
		this.lib_ext = '.a';
		this.cd_exe = 'clang -M {1} {0} -c -o {2}';
		this.cc_exe = 'g++ -D_LINUX {1} {0} -c -o {2}';
		this.ld_exe = 'g++ {0} {2} -o {1}';
		this.sl_exe = 'ar rcs {0}lib{1} {2}';
		this.dl_exe = '';
		this.platform = platform;
		return;
	}
}

AppBuilder.prototype.setConfiguration = function (config) {
	if(config=='debug') {
		this.output_dir = path.resolve("./", "../..") + "/Binaries/Debug/";
		this.object_dir = path.resolve("./", "..") + "/Intermediate/Debug/";
		this.cc_flag = '-g -D__x86_64__';
		this.ln_flag = '';
		this.sl_flag = '';
		this.dl_flag = '';
		this.config = config;
		return;
	}
	if(config=='release') {
		this.output_dir = path.resolve("./", "../..") + "/Binaries/Release/";
		this.object_dir = path.resolve("./", "..") + "/Intermediate/Release/";
		this.cc_flag = '-O2 -D__x86_64__';
		this.ln_flag = '';
		this.sl_flag = '';
		this.dl_flag = '';
		this.config = config;
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
		var ddl_path = proj.path + proj.ddl_files[i];
		ddl_path = ddl_path.replace(/\\/g, "/");
		ddl_path = path.normalize(ddl_path);
		var ddl_gen = ddl_path.substring(0, ddl_path.lastIndexOf('.'));
		if(!this.needUpdate([ddl_exe, ddl_path], [ddl_gen+'.cpp', ddl_gen+'.h'])) {
			continue;
		}

		var cmdline = '{0} {1}'.format(ddl_exe, ddl_path);
		console.log(cmdline);
	}
}

AppBuilder.prototype.loadCPPDepend = function (filename) {
	var ret = [];
	try {
		arr = fs.readFileSync(filename, 'utf-8').split('\n');
		arr = arr.join('').replace(/\\/g, '').trim().split(' ');
		for(var i=1; i<arr.length; i++) {
			if(arr[i].trim()=='') continue;
			if(arr[i].trim()=='-') continue;
			ret.push(arr[i]);
		}
	} catch(e) {
		return undefined;
	}
	return ret;
}

AppBuilder.prototype.needUpdate = function (src_files, gen_files) {
	if(typeof(src_files)=='string') {
		src_files = [src_files];
	}

	var src_ts = 0;
	for(var i=0; i<src_files.length; i++) {
		if(!fs.existsSync(src_files[i])) {
			return true;
		}
		var ts = Date.parse(fs.statSync(src_files[i]).mtime) / 1000;
		if(ts>src_ts) {
			src_ts = ts;
		}
	}

	if(typeof(gen_files)=='string') {
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
	if(this.jobs.indexOf(proj.name)>=0) return;
	this.jobs.push(proj.name);
/*
	if(proj.name!='DDLGen' && proj.name!='RPCGen') {
		var dep_proj;
		dep_proj = this.solution.getProject('DDLGen');
		if(dep_proj) {
			this.buildBIN(dep_proj);
		}
		dep_proj = this.solution.getProject('RPCGen');
		if(dep_proj) {
			this.buildBIN(dep_proj);
		}
	}
*/

	var dep_objs = [];

	for(var i=0; i<proj.deps.length; i++) {
		var dep_proj = this.solution.getProject(proj.deps[i]);
		if(!dep_proj) {
			console.log('project '+project_name+' not found!');
			process.exit();
		}
		this.buildBIN(dep_proj);
		dep_objs = dep_objs.concat(dep_proj.objs);
	}

	var inc_cmd = '';
	for(var i=0; i<proj.inc_dir.length; i++) {
		if(i>0) inc_cmd += ' ';
		var inc_path = process.cwd() + '\\' + proj.inc_dir[i];
		inc_path = inc_path.replace(/\\/g, "/");
		inc_path = path.normalize(inc_path);
		inc_cmd += '-I' + inc_path;
	}

	proj.objs = [];
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

		proj.objs.push(dst_path+this.obj_ext);

		var src_files = this.loadCPPDepend(dst_path+'.d');
		if(src_files && !this.needUpdate(src_files, dst_path+this.obj_ext)) {
			continue;
		}

		autoMakeDir(dst_path);

		console.log('echo compile', proj.src_files[i], '[', this.platform, this.config, ']');
		var cpp_flag = ' ';
		var cc_exe = this.cc_exe;
		if(src_path.substring(src_path.length-2)!='.c') {
			if(this.cc_exe.substring(0,3)!='cl ') {
				cpp_flag = ' -std=c++0x -fpermissive '
			}
		} else {
			cc_exe = cc_exe.replace(/clang\+\+/, "clang");
			cc_exe = cc_exe.replace(/g\+\+/, "gcc");
		}
		console.log(cc_exe.format(src_path, inc_cmd + cpp_flag + this.cc_flag, dst_path+this.obj_ext));
		// console.log(this.cd_exe.format(src_path, inc_cmd + cpp_flag + '-M ' + this.cc_flag, dst_path+'.d'));
	}

	proj.objs_str = proj.objs.join(' ');
	if(proj.type=='StaticLibrary') {
		var lib_path = this.object_dir + proj.name;
		lib_path = lib_path.replace(/\\/g, "/");
		autoMakeDir(lib_path);

		if(!this.needUpdate(proj.objs, lib_path+this.lib_ext)) {
			return;
		}
		console.log('echo link library', proj.name, '[', this.platform, this.config, ']');
		console.log(this.sl_exe.format(this.object_dir, proj.name+this.lib_ext, proj.objs_str));
	} else {
		var exe_path = this.output_dir + proj.name;
		exe_path = exe_path.replace(/\\/g, "/");
		autoMakeDir(exe_path);

		if(!this.needUpdate(proj.objs, exe_path+this.exe_ext)) {
			return;
		}

		var dep_lib = "-L" + this.object_dir;
		for(var i=0; i<proj.deps.length; i++) {
			dep_lib += " -l" + proj.deps[i];
		}
		dep_lib = " -ldl -pthread -lmysqlclient -luuid -lrt ";

		console.log('echo link execute', proj.name, '[', this.platform, this.config, ']');
		console.log(this.ld_exe.format(proj.objs_str + ' ' + dep_objs.join(' ') + ' ', exe_path+this.exe_ext, dep_lib));
//		console.log(dep_objs);

	}

	console.log('');
}

AppBuilder.prototype.build = function (project_name) {
	var proj = this.solution.getProject(project_name);
	if(!proj) {
		console.log('project '+project_name+' not found!');
		process.exit();
	}

	this.buildRPC(proj);
	this.buildDDL(proj);
	this.buildBIN(proj);
}

AppBuilder.prototype.clean = function (project_name) {
	var proj = this.solution.getProject(project_name);
	if(!proj) {
		console.log('project '+project_name+' not found!');
		process.exit();
	}

	for(var i=0; i<proj.deps.length; i++) {
		this.clean(proj.deps[i]);
	}

	for(var i=0; i<proj.rpc_files.length; i++) {
		var rpc_path = proj.path + proj.rpc_files[i];
		rpc_path = rpc_path.replace(/\\/g, "/");
		rpc_path = path.normalize(rpc_path);
		var rpc_gen = rpc_path.substring(0, rpc_path.lastIndexOf('.'));

		if(fs.existsSync(rpc_gen+'.c.cpp')) {
			fs.unlinkSync(rpc_gen+'.c.cpp');
		}
		if(fs.existsSync(rpc_gen+'.s.cpp')) {
			fs.unlinkSync(rpc_gen+'.s.cpp');
		}
		if(fs.existsSync(rpc_gen+'.c.h')) {
			fs.unlinkSync(rpc_gen+'.c.h');
		}
		if(fs.existsSync(rpc_gen+'.s.h')) {
			fs.unlinkSync(rpc_gen+'.s.h');
		}
	}
/*
	for(var i=0; i<proj.ddl_files.length; i++) {
		var ddl_path = proj.path + proj.ddl_files[i];
		ddl_path = ddl_path.replace(/\\/g, "/");
		ddl_path = path.normalize(ddl_path);
		var ddl_gen = ddl_path.substring(0, ddl_path.lastIndexOf('.'));
		if(fs.existsSync(ddl_gen+'.h')) {
			fs.unlinkSync(ddl_gen+'.h');
		}
		if(fs.existsSync(ddl_gen+'.cpp')) {
			fs.unlinkSync(ddl_gen+'.cpp');
		}
	}
*/
	for(var i=0; i<proj.src_files.length; i++) {
		var dst_path = proj.src_files[i];
		dst_path = dst_path.replace(/\\/g, "/");
		dst_path = dst_path.replace(/\//g, "_");
		dst_path = dst_path.replace(/\./g, "_");
		dst_path = this.object_dir + proj.name + '/' + dst_path;
		dst_path = path.normalize(dst_path);
		if(fs.existsSync(dst_path+this.obj_ext)) {
			fs.unlinkSync(dst_path+this.obj_ext);
		}
	}

	if(proj.type=='StaticLibrary') {
		var lib_path = this.object_dir + proj.name;
		lib_path = lib_path.replace(/\\/g, "/");
		if(fs.existsSync(lib_path+this.lib_ext)) {
			fs.unlinkSync(lib_path+this.lib_ext);
		}
	} else {
		var exe_path = this.output_dir + proj.name;
		exe_path = exe_path.replace(/\\/g, "/");
		if(fs.existsSync(exe_path+this.exe_ext)) {
			fs.unlinkSync(exe_path+this.exe_ext);
		}
	}
}

if(process.argv.length<4) {
	console.log('invalid parameter');
	process.exit();
}

var solution = new SolutionFile();
solution.load(process.argv[2]);

var builder = new AppBuilder(solution);

var do_task;
var tasks = [];

switch(process.argv[3]) {
case 'build':
	do_task = function(project_name) {
		builder.build(project_name);
	};
	break;
case 'clean':
	do_task = function(project_name) {
		builder.clean(project_name);
	};
	break;
case 'rebuild':
	do_task = function(project_name) {
		builder.clean(project_name);
		builder.build(project_name);
	};
	break;
default:
	console.log('invalid parameter');
	process.exit();
}

for(var i=4; i<process.argv.length; i++) {
	var pos = process.argv[i].indexOf('=');
	if(pos<0) {
		tasks.push(process.argv[i]);
	} else {
		switch(process.argv[i].substring(0, pos)) {
		case 'config':
			builder.setConfiguration(process.argv[i].substring(pos+1));
			break;
		case 'platform':
			builder.setPlatform(process.argv[i].substring(pos+1));
			break;
		}
	}
}

for(var i=0; i<tasks.length; i++) {
	do_task(tasks[i]);
}
