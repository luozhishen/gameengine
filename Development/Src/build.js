"use strict";

var fs = require('fs');
var path = require('path');

var mkdirp = require("mkdirp")

if (!String.prototype.format) {
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
    if (mode === undefined) {
        mode = 0777 & (~process.umask());
    }
    if (!made) made = null;

    if (typeof mode === 'string') mode = parseInt(mode, 8);
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
                if (!stat.isDirectory()) throw err0;
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

	console.log('create path ' + strPath);
	mkdirp_sync(strPath);

	return true;
}

function ProcessPropertyFile (proj, filename) {
	var path = proj.path + filename;
	console.log(path);
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

	var lines = fs.readFileSync(filename, 'utf-8').split('\n');

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
	this.solution = solution;
	this.cc_exe = 'gcc';
	this.ln_exe = 'ln';
}

AppBuilder.prototype.setDebug = function (project_name) {
	this.output_dir = "./output/"
	this.cc_flag = "";
}

AppBuilder.prototype.setRelease = function (project_name) {
	this.output_dir = "....."
	this.cc_flag = "";
}

AppBuilder.prototype.build = function (project_name) {
	var proj = this.solution.getProject(project_name);
	if(!proj) {
		console.log('project '+project_name+' not found!');
		process.exit();
	}

	for(var i=0; i<proj.rpc_files.length; i++) {
		console.log('process '+proj.rpc_files[i]);
	}
	for(var i=0; i<proj.ddl_files.length; i++) {
		console.log('process '+proj.ddl_files[i]);
	}
	for(var i=0; i<proj.inc_files.length; i++) {
		if(proj.inc_files[i].indexOf('AutoGen.h')>0) {
			console.log('process '+proj.inc_files[i]);
		}
	}

	var inc_cmd = '';
	for(var i=0; i<proj.inc_dir.length; i++) {
		if(i>0) inc_cmd += ' ';
		inc_cmd += '-I'+proj.inc_dir[i];
	}

	for(var i=0; i<proj.src_files.length; i++) {
		console.log('process '+proj.src_files[i]);

		var src_path = proj.path + proj.src_files[i];
		var dst_path = proj.path + proj.src_files[i];
		src_path = src_path.replace(/\\/g, "/");
		dst_path = dst_path.replace(/\\/g, "/");
		dst_path = dst_path.replace(/\//g, "_");
		dst_path = dst_path.replace(/\./g, "_");
		dst_path = this.output_dir + proj.name + '/' + dst_path;

		autoMakeDir(dst_path);

		var cmdline = this.cc_exe + " {1} {0} -o {2} ".format(src_path, inc_cmd, dst_path);
		console.log(cmdline);
	}
}

if(process.argv.length<3) {
	console.log('invalid parameter');
	process.exit();
}

var solution = new SolutionFile();
solution.load(process.argv[2]);

var builder = new AppBuilder(solution);
builder.setDebug();
builder.build('DDLGen');
builder.build('RpcGen');
