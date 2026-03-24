// ═══ STATE ═══
var placed=[];      // node ids in document order
var curN='';        // current node id
var mouseX=0, mouseY=0;

// Navigation history: array of node ids, cursor points to current position
var navHist=[];
var navCur=-1;

// Auto-hover state
var autoHovered=null;
var scrollLock=false;
var scrollRAF=null;

// ═══ REFS ═══
var $main=document.getElementById('main');
var $doc=document.getElementById('doc');
var $svg=document.getElementById('hoverline');
var $dir=document.getElementById('dir-indicator');
var $wl=document.getElementById('wl');

// ═══ HELPERS ═══

// ═══ MOUSE TRACKING ═══
document.addEventListener('mousemove',function(e){
  mouseX=e.clientX; mouseY=e.clientY;
  clearAutoHover();
});

// ═══ NAV HISTORY ═══
function navPush(id){
  if(navCur<navHist.length-1) navHist=navHist.slice(0,navCur+1);
  navHist.push(id);
  navCur=navHist.length-1;
}

// ═══ NAVIGATE TO NODE ═══
function navigateTo(id){
  var el=document.getElementById('nd-'+id);
  if(!el)return;
  if(el.classList.contains('collapsed')) el.classList.remove('collapsed');

  scrollLock=true;
  clearAutoHover();
  closeFan();

  var titleEl=el.querySelector('.nt');
  var tRect=titleEl.getBoundingClientRect();
  var mRect=$main.getBoundingClientRect();
  var titleInScroll=tRect.top - mRect.top + $main.scrollTop;
  var targetY=mouseY - mRect.top;
  var desired=titleInScroll - targetY;
  var max=$main.scrollHeight - $main.clientHeight;
  desired=Math.max(0, Math.min(desired, max));

  $main.scrollTo({top:desired, behavior:'smooth'});

  el.classList.add('target-glow');
  setTimeout(function(){el.classList.remove('target-glow')},1500);
  setTimeout(function(){scrollLock=false},600);
}

// ═══ CONNECTION LINE (shared by anchor hover + fan menu) ═══
function drawConnLine(el1,el2){
  if(!el1||!el2){$svg.innerHTML='';return}
  var r1=el1.getBoundingClientRect(),r2=el2.getBoundingClientRect();
  if(r1.width===0||r2.width===0){$svg.innerHTML='';return}
  var sx=r1.left,sy=r1.top+r1.height/2,tx=r2.left,ty=r2.top+r2.height/2;
  var dy=Math.abs(ty-sy),cpx=Math.min(sx,tx)-50,dn=ty>sy;
  var d='M'+sx+' '+sy+' C'+cpx+' '+(dn?sy+dy*.35:sy-dy*.35)+' '+cpx+' '+(dn?ty-dy*.35:ty+dy*.35)+' '+tx+' '+ty;
  $svg.innerHTML='<path d="'+d+'" fill="none" stroke="#111" stroke-width="1" stroke-linecap="round"/>';
}

// ═══ ANCHOR HOVER/CLICK ═══
function hoverAnchor(el){
  var tid=el.getAttribute('data-t');

  // Target not placed yet — no element to draw line to
  if(placed.indexOf(tid)===-1){
    $svg.innerHTML='';
    return;
  }

  // Target is placed — glow, direction indicator, line
  var tn=document.getElementById('nd-'+tid);
  if(!tn)return;
  tn.classList.add('target-glow');

  var r=tn.getBoundingClientRect();
  var mr=$main.getBoundingClientRect();
  var title=N[tid]?N[tid].t:'';
  if(r.bottom<mr.top+36){$dir.className='top';$dir.textContent='^ '+title;$dir.style.display='block'}
  else if(r.top>mr.bottom){$dir.className='bot';$dir.textContent='v '+title;$dir.style.display='block'}
  else{$dir.style.display='none'}

  drawConnLine(el,tn.querySelector('.nt'));
}

function leaveAnchor(el){
  document.querySelectorAll('.target-glow').forEach(function(n){n.classList.remove('target-glow')});
  $dir.style.display='none';
  $svg.innerHTML='';
}

function clickAnchor(el){
  var tid=el.getAttribute('data-t');
  if(placed.indexOf(tid)===-1){
    addNode(tid);
  } else {
    navPush(tid);
    navigateTo(tid);
  }
  leaveAnchor(el);
}

// ═══ FAN MENU ═══
var $em=document.getElementById('em');
var $fanOv=document.getElementById('fan-overlay');
var $fanSvg=document.getElementById('fan-svg');

var fanNode=null,fanConns=[],fanSettled=false,fanIdx=-1,fanFlipped=false;
var fanOX=0,fanOY=0,fanCX=0,fanCY=0,fanCW=0,fanCH=0;
var fanWT=0,fanWB=0,fanWL=0,fanWR=0;
var fanSettleTimer=null,fanScrollLockUntil=0;

var FAN_BEND=42,FAN_ANGLE=5,FAN_GAP=60,FAN_OL=15,FAN_MS=70;
var FAN_DECEL=0.3,FAN_MPEAK=0.3,FAN_FLOOR=0.05;

// Speed tracking
var mSpeeds=[],mPeak=0,mLX=0,mLY=0,mLT=0;
function resetSpeed(){mSpeeds=[];mPeak=0;mLT=0}
function trackSpeed(mx,my){
  var now=performance.now();
  if(mLT>0){var dt=now-mLT;if(dt>2){
    var sp=Math.hypot(mx-mLX,my-mLY)/dt;
    mSpeeds.push(sp);if(mSpeeds.length>3)mSpeeds.shift();
    if(sp>mPeak)mPeak=sp;
    var avg=0;for(var i=0;i<mSpeeds.length;i++)avg+=mSpeeds[i];avg/=mSpeeds.length;
    mLX=mx;mLY=my;mLT=now;
    if(avg<FAN_FLOOR)return true;
    if(mPeak>FAN_MPEAK&&avg<mPeak*FAN_DECEL)return true;
  }}else{mLX=mx;mLY=my;mLT=now}
  return false;
}

// Proximity
function ptSegDist(px,py,ax,ay,bx,by){
  var dx=bx-ax,dy=by-ay,l2=dx*dx+dy*dy;
  if(l2===0)return Math.hypot(px-ax,py-ay);
  var t=Math.max(0,Math.min(1,((px-ax)*dx+(py-ay)*dy)/l2));
  return Math.hypot(px-(ax+t*dx),py-(ay+t*dy));
}
function ptPolyDist(px,py,ax,ay,bx,by,cx,cy){
  return Math.min(ptSegDist(px,py,ax,ay,bx,by),ptSegDist(px,py,bx,by,cx,cy));
}

function edgeConns(nid,cardsOnly){
  var inb=[],out=[],ni=placed.indexOf(nid);
  for(var eid in ED){var e=ED[eid];
    if(!cardsOnly&&e.to===nid&&placed.indexOf(e.from)!==-1) inb.push({peer:e.from,q:e.t});
    if(e.from===nid&&(cardsOnly?e.card:placed.indexOf(e.to)!==-1)) out.push({peer:e.to,q:e.t});
  }
  // Sort by distance: peers above (back) first, peers below/unplaced (forward) last
  function dist(c){var pi=placed.indexOf(c.peer);return pi===-1?1e6:pi-ni}
  inb.sort(function(a,b){return dist(a)-dist(b)});
  out.sort(function(a,b){return dist(a)-dist(b)});
  return{inb:inb,out:out};
}

var fanCardsOnly=false;
function buildFan(nid,cardsOnly){
  closeFan();
  fanNode=nid;fanSettled=false;fanCardsOnly=!!cardsOnly;resetSpeed();
  var cn=edgeConns(nid,cardsOnly);
  fanConns=[];
  cn.inb.forEach(function(c){fanConns.push(c)});
  cn.out.forEach(function(c){fanConns.push(c)});
  if(!fanConns.length){fanNode=null;return false}

  var h='';
  if(cn.inb.length){
    h+='<div class="em-head"><- from</div><div class="em-list">';
    cn.inb.forEach(function(c,i){h+='<div class="em-item" data-idx="'+i+'"><div class="em-q">'+c.q+'</div></div>'});
    h+='</div>';
  }
  if(cn.inb.length&&cn.out.length)h+='<div class="em-sep"></div>';
  if(cn.out.length){
    h+='<div class="em-head">-> to</div><div class="em-list">';
    cn.out.forEach(function(c,i){var idx=cn.inb.length+i;
      h+='<div class="em-item" data-idx="'+idx+'"><div class="em-q">'+c.q+'</div></div>'});
    h+='</div>';
  }
  $em.innerHTML=h;$em.classList.add('show');
  return true;
}

function positionFan(mx,my){
  fanOX=mx-FAN_OL;fanOY=my;
  $em.style.visibility='hidden';
  fanCW=$em.offsetWidth;fanCH=$em.offsetHeight;
  fanFlipped=false;
  fanCX=fanOX+FAN_GAP;fanCY=fanOY-fanCH/2;
  if(fanCX+fanCW>window.innerWidth-8){fanCX=fanOX-FAN_GAP-fanCW;fanFlipped=true}
  var mr=$main.getBoundingClientRect();
  fanCY=Math.max(mr.top+4,Math.min(fanCY,window.innerHeight-fanCH-8));
  $em.style.left=fanCX+'px';$em.style.top=fanCY+'px';$em.style.visibility='visible';

  var items=$em.querySelectorAll('.em-item');
  fanConns.forEach(function(c,i){var el=items[i];if(!el)return;
    var q=el.querySelector('.em-q');var r=(q||el).getBoundingClientRect();
    c.tx=fanFlipped?r.right+2:r.left-2;c.ty=r.top+r.height/2});

  var n=fanConns.length,step=FAN_ANGLE*Math.PI/180,dir=fanFlipped?-1:1;
  fanConns.forEach(function(c,i){var a=(i-(n-1)/2)*step*dir;
    c.bx=fanOX+dir*FAN_BEND*Math.cos(a);c.by=fanOY+FAN_BEND*Math.sin(a)});

  var pad=30,byMin=fanOY,byMax=fanOY;
  fanConns.forEach(function(c){if(c.by<byMin)byMin=c.by;if(c.by>byMax)byMax=c.by});
  fanWT=Math.min(byMin-pad,fanCY-pad);fanWB=Math.max(byMax+pad,fanCY+fanCH+pad);
  if(fanFlipped){fanWL=fanCX-20;fanWR=fanOX+30}else{fanWL=fanOX-10;fanWR=fanCX+fanCW+20}
  drawFanLines(-1);
}

function settleFan(){if(!fanNode)return;fanSettled=true;$fanOv.classList.add('show')}

function drawFanLines(sel){
  var h='',n=fanConns.length;if(!n)return;
  if(fanSettled){
    var wpad=20,wy1=fanOY-6,wy2=fanOY+6,farX=fanFlipped?fanCX-6:fanCX+fanCW+6;
    h+='<polygon points="'+fanOX+','+wy1+' '+farX+','+(fanCY-wpad)+' '+farX+','+(fanCY+fanCH+wpad)+' '+fanOX+','+wy2+'" fill="rgba(0,0,0,.02)" stroke="rgba(0,0,0,.06)" stroke-width="1"/>';
  }
  fanConns.forEach(function(c,i){var on=i===sel;
    h+='<path d="M'+fanOX+' '+fanOY+' Q'+c.bx+' '+c.by+' '+c.tx+' '+c.ty+'" fill="none" stroke="'+(on?'#111':'rgba(0,0,0,.12)')+'" stroke-width="'+(on?1.5:1)+'" stroke-linecap="round"/>'});
  h+='<circle cx="'+fanOX+'" cy="'+fanOY+'" r="2" fill="#111"/>';
  $fanSvg.innerHTML=h;
}

function drawFanConn(nid,peer){
  drawConnLine(document.querySelector('#nd-'+nid+' .nt'),document.querySelector('#nd-'+peer+' .nt'));
}

function closeFan(){
  fanNode=null;fanConns=[];fanIdx=-1;fanSettled=false;
  clearTimeout(fanSettleTimer);
  $fanOv.classList.remove('show');
  $em.classList.remove('show');$em.innerHTML='';
  $fanSvg.innerHTML='';$svg.innerHTML='';
  document.querySelectorAll('.target-glow').forEach(function(n){n.classList.remove('target-glow')});
}

// Settled phase — proximity selection on overlay
$fanOv.addEventListener('mousemove',function(e){
  var mx=e.clientX,my=e.clientY;
  if(mx<fanWL||mx>fanWR||my<fanWT||my>fanWB){closeFan();return}
  var best=-1,bestD=Infinity;
  fanConns.forEach(function(c,i){var d=ptPolyDist(mx,my,fanOX,fanOY,c.bx,c.by,c.tx,c.ty);if(d<bestD){bestD=d;best=i}});
  if(bestD>30)best=-1;
  if(best!==fanIdx){
    fanIdx=best;
    $em.querySelectorAll('.em-item').forEach(function(el,i){el.classList.toggle('active',i===fanIdx)});
    drawFanLines(fanIdx);
    if(fanIdx>=0){
      drawFanConn(fanNode,fanConns[fanIdx].peer);
      document.querySelectorAll('.target-glow').forEach(function(n){n.classList.remove('target-glow')});
      var pe=document.getElementById('nd-'+fanConns[fanIdx].peer);
      if(pe)pe.classList.add('target-glow');
    } else {
      $svg.innerHTML='';
      document.querySelectorAll('.target-glow').forEach(function(n){n.classList.remove('target-glow')});
    }
  }
});

// Settled phase — click (line selection OR click on card text)
$fanOv.addEventListener('click',function(e){
  var idx=fanIdx;
  // Fallback: if no line selected, check if click is over a card item
  if(idx<0){
    var mx=e.clientX,my=e.clientY;
    var items=$em.querySelectorAll('.em-item');
    for(var i=0;i<items.length;i++){
      var r=items[i].getBoundingClientRect();
      if(mx>=r.left&&mx<=r.right&&my>=r.top&&my<=r.bottom){idx=i;break}
    }
  }
  if(idx>=0&&fanConns[idx]){
    var peer=fanConns[idx].peer;
    closeFan();
    if(placed.indexOf(peer)===-1){
      addNode(peer);
    } else {
      navPush(peer);navigateTo(peer);
    }
    fanScrollLockUntil=Date.now()+600;
    // After scroll settles, check if cursor landed on a title or next
    setTimeout(function(){
      var el=document.elementFromPoint(mouseX,mouseY);
      if(!el)return;
      var nt=el.closest('.nt[data-nid]')||el.closest('.nxt[data-nid]');
      if(!nt)return;
      var nid=nt.getAttribute('data-nid');
      var co=nt.classList.contains('nxt');
      if(buildFan(nid,co)){positionFan(mouseX,mouseY);fanSettleTimer=setTimeout(settleFan,FAN_MS)}
    },650);
  } else closeFan();
});

// ═══ AUTO-HOVER ON SCROLL ═══
function clearAutoHover(){
  if(!autoHovered)return;
  autoHovered.classList.remove('autohover');
  if(autoHovered.classList.contains('ea')) leaveAnchor(autoHovered);
  else if(autoHovered.classList.contains('nt')) closeFan();
  autoHovered=null;
}

function findNearestInteractive(){
  var best=null, bestDist=Infinity;
  var candidates=document.querySelectorAll('.ea.on, .nt.linked, .nxt');
  for(var i=0;i<candidates.length;i++){
    var r=candidates[i].getBoundingClientRect();
    if(r.width===0||r.height===0)continue;
    if(mouseY>=r.top-2 && mouseY<=r.bottom+2){
      var cx=r.left+r.width/2;
      var dx=Math.abs(mouseX-cx);
      if(dx<bestDist){bestDist=dx;best=candidates[i]}
    }
  }
  return best;
}

$main.addEventListener('scroll',function(){
  if(scrollLock||scrollRAF)return;
  scrollRAF=requestAnimationFrame(function(){
    scrollRAF=null;
    if(scrollLock)return;
    var target=findNearestInteractive();
    if(target===autoHovered)return;
    clearAutoHover();
    if(!target)return;
    autoHovered=target;
    target.classList.add('autohover');
    if(target.classList.contains('ea')) hoverAnchor(target);
    else if(target.classList.contains('nxt')){var nid2=target.getAttribute('data-nid');if(nid2&&buildFan(nid2,true)){positionFan(mouseX,mouseY);settleFan()}}
    else if(target.classList.contains('nt')){var nid2=target.getAttribute('data-nid');if(nid2&&buildFan(nid2)){positionFan(mouseX,mouseY);settleFan()}}
  });
},{passive:true});

// Click on empty space triggers auto-hovered element
$main.addEventListener('click',function(e){
  if(!autoHovered||scrollLock)return;
  if(e.target.closest('.ea.on')||e.target.closest('.nt[data-nid]')||e.target.closest('.nch')||e.target.closest('.ch')||e.target.closest('.tb')||e.target.closest('a')||e.target.closest('.em'))return;
  if(autoHovered.classList.contains('ea')) clickAnchor(autoHovered);
  else if(autoHovered.classList.contains('nt')) autoHovered.click();
});

// ═══ DELEGATED TITLE CLICK ═══
document.addEventListener('click',function(e){
  // Click inside edge menu → handled by the menu itself
  if(e.target.closest('.em'))return;
  // Skip collapse for "next" elements
  if(e.target.closest('.nxt'))return;
  // Click on title → toggle collapse
  var nt=e.target.closest('.nt[data-nid]');
  if(!nt)return;
  var nid=nt.getAttribute('data-nid');
  closeFan();
  togN(nid);
});

// ═══ CHAPTER LABEL CLICK → COLLAPSE ═══
$doc.addEventListener('click',function(e){
  var nch=e.target.closest('.nch');
  if(nch){
    var node=nch.closest('.node');
    if(node) togN(node.id.replace('nd-',''));
    return;
  }
  // Choice card clicks
  var ch=e.target.closest('.ch[data-to]');
  if(ch){var tid=ch.getAttribute('data-to');if(placed.indexOf(tid)===-1)addNode(tid);else{navPush(tid);navigateTo(tid)}return}
  // Anchor clicks
  var ea=e.target.closest('.ea.on');
  if(ea){clickAnchor(ea);return}
});

// Direction indicator click
$dir.addEventListener('click',function(){
  if(!autoHovered)return;
  var tid=autoHovered.getAttribute('data-t');
  if(tid&&placed.indexOf(tid)!==-1){
    navPush(tid);
    navigateTo(tid);
  }
  $dir.style.display='none';
});

// Anchor hover via delegation
$doc.addEventListener('mouseover',function(e){
  var ea=e.target.closest('.ea.on');
  if(ea&&!ea.classList.contains('autohover'))hoverAnchor(ea);
});
$doc.addEventListener('mouseout',function(e){
  var ea=e.target.closest('.ea.on');
  if(ea&&!ea.classList.contains('autohover'))leaveAnchor(ea);
});

// Title/next hover → fan menu tracking phase
$doc.addEventListener('mouseover',function(e){
  if(Date.now()<fanScrollLockUntil)return;
  if(fanSettled)return;
  var nt=e.target.closest('.nt[data-nid]')||e.target.closest('.nxt[data-nid]');
  if(!nt||nt.classList.contains('autohover'))return;
  var nid=nt.getAttribute('data-nid');
  if(fanNode===nid)return;
  var cardsOnly=nt.classList.contains('nxt');
  if(buildFan(nid,cardsOnly)){
    positionFan(e.clientX,e.clientY);
    clearTimeout(fanSettleTimer);
    fanSettleTimer=setTimeout(settleFan,FAN_MS);
  }
});

// Tracking mousemove — reposition fan while mouse moves over title/next
document.addEventListener('mousemove',function(e){
  if(Date.now()<fanScrollLockUntil)return;
  if(fanSettled||!fanNode)return;
  var nt=e.target.closest('.nt[data-nid]')||e.target.closest('.nxt[data-nid]');
  if(!nt||nt.getAttribute('data-nid')!==fanNode){closeFan();return}
  positionFan(e.clientX,e.clientY);
  clearTimeout(fanSettleTimer);
  if(trackSpeed(e.clientX,e.clientY)) settleFan();
  else fanSettleTimer=setTimeout(settleFan,FAN_MS);
});

// ═══ SIDEBAR ═══
function renderWalkList(){
  var ns=walkNodes('textbook');
  if(!ns||!ns.length){$wl.innerHTML='';return}
  var chs={},order=[];
  for(var i=0;i<ns.length;i++){var n=N[ns[i]];if(!n)continue;if(!chs[n.ch]){chs[n.ch]=[];order.push(n.ch)}chs[n.ch].push({id:ns[i],n:n,idx:i})}
  var h='';
  order.forEach(function(ch){var its=chs[ch],col=gc(its[0].idx+1);
    h+='<div class="wls"><div class="wlc"><div class="wlcd" style="background:'+rgb(col)+'"></div>'+ch+'</div>';
    its.forEach(function(it){var v=placed.indexOf(it.id)!==-1,c=it.id===curN;
      h+='<div class="wln '+(c?'cur':v?'vis':'unv')+'" data-sid="'+it.id+'"><div class="wlnd"></div>'+it.n.t+'<span class="wlnn">'+(it.idx+1)+'</span></div>'});
    h+='</div>'});
  $wl.innerHTML=h;
}
$wl.addEventListener('click',function(e){
  var el=e.target.closest('.wln');
  if(!el)return;
  var id=el.getAttribute('data-sid');
  if(placed.indexOf(id)!==-1){navPush(id);navigateTo(id)}
  else addNode(id);
});

// ═══ TOPBAR BUTTONS ═══
document.getElementById('btn-col').addEventListener('click',function(){document.querySelectorAll('.node').forEach(function(n){n.classList.add('collapsed')})});
document.getElementById('btn-exp').addEventListener('click',function(){document.querySelectorAll('.node').forEach(function(n){n.classList.remove('collapsed')})});

// ═══ ADD NODE ═══
function addNode(id,silent){
  if(placed.indexOf(id)!==-1){if(!silent){navPush(id);navigateTo(id)}return}
  var n=N[id]; if(!n)return;
  curN=id; placed.push(id);

  var pos=nodePos(id);
  var col=gc(pos);

  var h='<div class="node'+(silent?'':' ni')+'" id="nd-'+id+'">';
  h+='<div class="nch"><div class="nchd" style="background:'+rgb(col)+'"></div> '+n.ch+' · '+pos+'</div>';
  h+='<div class="nt" data-nid="'+id+'">'+n.t+'</div>';
  if(placed.length>1&&n.ctx)h+='<div class="nctx">'+n.ctx+'</div>';
  var body=n.b();
  // Inject "next" inline at end of last paragraph if node has outgoing card edges
  var hasCards=false;
  for(var eid in ED){if(ED[eid].from===id&&ED[eid].card){hasCards=true;break}}
  if(hasCards){
    var nxtSpan=' <span class="nxt" data-nid="'+id+'">next.</span>';
    var li=body.lastIndexOf('</p>');
    if(li!==-1) body=body.slice(0,li)+nxtSpan+'</p>'+body.slice(li+4);
    else body+=nxtSpan;
  }
  h+='<div class="pr">'+body+'</div>';

  h+='</div>';
  $doc.insertAdjacentHTML('beforeend',h);

  if(!silent){
    navPush(id);
    scrollLock=true;clearAutoHover();
    setTimeout(function(){
      var el=document.getElementById('nd-'+id);
      var mRect=$main.getBoundingClientRect();
      var elRect=el.getBoundingClientRect();
      var desired=$main.scrollTop+(elRect.top-mRect.top)-20;
      $main.scrollTo({top:Math.max(0,desired),behavior:'smooth'});
      setTimeout(function(){scrollLock=false},600);
    },50);
  }
  updateAnchors();
  updateStats();
}

// ═══ UPDATE ANCHORS + LINKED TITLES ═══
function updateAnchors(){
  // All anchors are active. Mark placed vs unplaced targets.
  document.querySelectorAll('.ea').forEach(function(a){
    a.classList.add('on');
    var tid=a.getAttribute('data-t');
    a.classList.toggle('placed',placed.indexOf(tid)!==-1);
  });

  // Mark titles that have connections (for linked class + auto-hover candidates)
  placed.forEach(function(pid){
    var tn=document.getElementById('nd-'+pid);
    if(!tn)return;
    var titleEl=tn.querySelector('.nt');
    var hasConn=false;
    for(var eid in ED){
      var e=ED[eid];
      if((e.to===pid&&placed.indexOf(e.from)!==-1)||(e.from===pid&&placed.indexOf(e.to)!==-1)){hasConn=true;break}
    }
    titleEl.classList.toggle('linked',hasConn);
  });
}

function togN(id){document.getElementById('nd-'+id).classList.toggle('collapsed')}
function updateStats(){
  document.getElementById('sbv').textContent=placed.length;
  document.getElementById('sbn').textContent=nodePos(curN);
  renderWalkList();
}

// ═══ INIT ═══
// Load all textbook walk nodes, expanded
var twNodes=walkNodes('textbook');
for(var i=0;i<twNodes.length;i++) addNode(twNodes[i],true);
// Also place EA anchor targets not in the walk
document.querySelectorAll('.ea[data-t]').forEach(function(a){
  var tid=a.getAttribute('data-t');
  if(placed.indexOf(tid)===-1) addNode(tid,true);
});
renderWalkList();
