#!/usr/bin/env node
// Generates a static HTML export of the full textbook walk.
// Usage: node export.js > index-static.html

var fs=require('fs'),path=require('path');
var dir=__dirname;

// Set up globals the data files expect
var N={}, ED={}, W={}, placed=[];
function EA(eid,txt){
  var e=ED[eid];
  if(!e)return txt;
  return'<span class="ea-static" title="'+e.w.replace(/"/g,'&quot;')+'">'+txt+'</span>';
}
function nodePos(id){return placed.indexOf(id)+1}

// Load data files
var files=['data.js','nodes/ch01.js','nodes/ch02.js','nodes/ch03.js','nodes/ch04.js',
  'nodes/ch05.js','nodes/ch06.js','nodes/ch07.js','nodes/ch08.js','walks.js'];
files.forEach(function(f){
  var src=fs.readFileSync(path.join(dir,f),'utf8');
  // Strip the helper functions we already defined
  if(f==='data.js') src=src.replace(/^function EA\b[\s\S]*?^}/m,'').replace(/^function (gc|rgb|walkNodes|nodePos)\b[\s\S]*?^}/gm,'');
  eval(src);
});

// Walk the textbook path
function walkNodes(wid){
  var edges=W[wid];
  if(!edges||!edges.length)return[];
  var ns=[ED[edges[0]].from];
  for(var i=0;i<edges.length;i++)ns.push(ED[edges[i]].to);
  return ns;
}

var nodes=walkNodes('textbook');
nodes.forEach(function(id){placed.push(id)});

// Render
var body='';
nodes.forEach(function(id,i){
  var n=N[id];
  if(!n)return;
  body+='<div class="node" id="nd-'+id+'">\n';
  body+='<div class="nch">'+n.ch+' \u00b7 '+(i+1)+'</div>\n';
  body+='<h2>'+n.t+'</h2>\n';
  if(i>0&&n.ctx) body+='<div class="nctx">'+n.ctx+'</div>\n';
  body+='<div class="pr">'+n.b()+'</div>\n';
  body+='</div>\n\n';
});

var html=`<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Learn Rust \u2014 jadnohra.com</title>
<meta name="description" content="A Rust course for experienced C++ programmers. Derives the borrow checker from SPACE \u00d7 TIME \u00d7 COORDINATES.">
<link rel="stylesheet" href="/css/editor.css">
<style>
.node{margin-bottom:16px;padding-bottom:14px;border-bottom:1px solid #ddd}
.nch{font-size:10px;font-weight:500;color:#555;margin-bottom:3px}
h2{font-size:14px;font-weight:500;margin-bottom:4px}
.nctx{font-size:12px;line-height:1.5;margin-bottom:10px;padding-bottom:8px;border-bottom:1px solid #eee}
.pr p{margin-bottom:8px}
.pr strong{font-weight:500}
.pr code{font-size:12px;border:1px solid #ddd;padding:0 3px}
.cb{border:1px solid #ddd;padding:8px 10px;margin:6px 0 10px;overflow-x:auto;white-space:pre}
.cb code{font-size:12px;line-height:1.5;color:#111;border:none;padding:0}
.cb .k{color:#7c4dff;font-weight:500}.cb .c{color:#999;font-style:italic}.cb .s{color:#2e7d32}.cb .n{color:#e65100}.cb .m{color:#1565c0}
.er{border-left:2px solid #c62828;padding:6px 10px;margin:-2px 0 10px;font-size:11px;line-height:1.5;color:#c62828;background:#fafafa}
.co{border-left:2px solid #ddd;padding:8px 12px;margin:10px 0;font-size:12px;line-height:1.5}
.ea-static{text-decoration:underline;text-decoration-style:dotted;text-underline-offset:2px}
.pr table{width:100%;border-collapse:collapse;margin:8px 0 10px;font-size:11px;line-height:1.5}
.pr th{text-align:left;font-weight:500;padding:4px 8px;border-bottom:1px solid #111}
.pr td{padding:4px 8px;border-bottom:1px solid #eee;vertical-align:top}
.nxt{display:none}
</style>
</head>
<body data-page="learn-rust">
<div class="content">
<p style="font-size:11px;color:#555;margin-bottom:16px"><a href="/learn-rust/">Interactive version</a></p>
${body}
</div>
</body>
</html>`;

process.stdout.write(html);
