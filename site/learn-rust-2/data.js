// ═══ GRAYSCALE PALETTE ═══
// Position-based gray: light (#bbb) at start → dark (#333) at end
function gc(d){var t=Math.min(d/62,1);var v=Math.round(187-(187-51)*t);return[v,v,v]}
function rgb(c){return'rgb('+c.join(',')+')'}

// ═══ REGISTRIES ═══
var N={};   // nodes: {t, ctx, body, ch}
var ED={};  // edges: {from, to, t, w, card}
var W={};   // walks: {walkId: {chapters: [{title, nodes}]}}

// ═══ WALK HELPERS ═══
function walkNodes(wid){
  var w=W[wid];
  if(!w||!w.chapters)return[];
  var ns=[];
  for(var i=0;i<w.chapters.length;i++){
    var ch=w.chapters[i];
    for(var j=0;j<ch.nodes.length;j++) ns.push(ch.nodes[j]);
  }
  return ns;
}

function nodePos(id){return placed.indexOf(id)+1}

// ═══ LOAD COURSE DATA ═══
function loadCourse(data){
  // Nodes — compute ch from walk metadata
  var chMap={};
  if(data.walks&&data.walks.textbook){
    var chs=data.walks.textbook.chapters;
    for(var i=0;i<chs.length;i++){
      for(var j=0;j<chs[i].nodes.length;j++){
        chMap[chs[i].nodes[j]]=chs[i].title;
      }
    }
  }
  for(var id in data.nodes){
    var n=data.nodes[id];
    N[id]={t:n.t, ctx:n.ctx||'', body:n.body, ch:chMap[id]||''};
  }
  // Edges — auto-generate IDs
  for(var i=0;i<data.edges.length;i++){
    var e=data.edges[i];
    ED['e'+i]={from:e.from, to:e.to, t:e.t, w:e.w, card:e.card};
  }
  // Walks
  W=data.walks||{};
}
