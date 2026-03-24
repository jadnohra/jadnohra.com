// ═══ GRAYSCALE PALETTE ═══
// Position-based gray: light (#bbb) at start → dark (#333) at end
function gc(d){var t=Math.min(d/66,1);var v=Math.round(187-(187-51)*t);return[v,v,v]}
function rgb(c){return'rgb('+c.join(',')+')'}

// ═══ REGISTRIES ═══
var N={};   // nodes: {ch, t, ctx, b()}
var ED={};  // edges: {from, to, t, w, card}
var W={};   // walks: {walkId: [edgeId, ...]}

// ═══ PROSE ANCHOR HELPER ═══
function EA(eid,txt){
  var e=ED[eid];
  if(!e)return txt;
  return'<span class="ea" data-e="'+eid+'" data-t="'+e.to+'">'+txt+'<span class="tt"><span class="em-q">'+e.w+'</span></span></span>';
}

// ═══ WALK HELPERS ═══
function walkNodes(wid){
  var edges=W[wid];
  if(!edges||!edges.length)return[];
  var ns=[ED[edges[0]].from];
  for(var i=0;i<edges.length;i++)ns.push(ED[edges[i]].to);
  return ns;
}

function nodePos(id){return placed.indexOf(id)+1}
