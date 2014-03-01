var SOURCE_FILE = '../LibCommon/CardGameDDL.h';
var condition = '\tclass CARDGAME_S2C : public DDLProxy<CLIENT, BUFFER>';

var fs = require('fs');
var text = fs.readFileSync(SOURCE_FILE,'utf-8');
var lines = text.split('\n');

var cline = 0;

for(; cline<lines.length; cline++) {
    line = lines[cline].replace('\r', '');
    if(line==condition) break;
}
if(cline>=lines.length) {
    console.log('start line not found');
    return;
}

var sigslots = [];
var methods = [];

for(; cline<lines.length; cline++) {
    line = lines[cline].replace('\r', '');
    if(line=='\t};') break;
    if(line.length>=10 && line.substring(0, 6)=='\t\tbool') {
        left = line.indexOf('(');
        right = line.indexOf(')');

        name = line.substring(7, left);
        args = line.substring(left+1, right).split(',');
        if(left+1==right) args.shift();
        
        for(i=0; i<args.length; i++) {
            txt = args[i];
            s = txt.lastIndexOf(' ');
            args[i] = [];
            args[i][0] = txt.substring(0, s).trim();
            args[i][1] = txt.substring(s+1, txt.length).trim();
        }

        sigslot = "sigslot::signal"+args.length+'<';
        for(i=0; i<args.length; i++) {
            if(i>0) sigslot += ', ';
            sigslot += args[i][0];
        }
        sigslot += '> ' + '_On' + name + ';\n';

        method = 'void '+name+'(';
        for(i=0; i<args.length; i++) {
            if(i>0) method += ', ';
            method += args[i][0];
            method += ' ';
            method += args[i][1];
        }
        method += ')\n{\n';
        method += '\t_On' + name + '(';
        for(i=0; i<args.length; i++) {
            if(i>0) method += ', ';
            method += args[i][1];
        }
        method += ');\n}\n';

        sigslots.push(sigslot);
        methods.push(method);
    }
}

text = '';
text += sigslots.join('');
text += '\n';
text += methods.join('');

console.log(text);
