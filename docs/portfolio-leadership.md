---
layout: post
title: Engineering Manager Portfolio
permalink: /portfolio-leadership/
toc: true
---

<style>
/* Compact layout - ensure headings visible for TOC */
.content p { margin-bottom: 0.5rem; }
.content h2 { margin-top: 1.5rem; margin-bottom: 0.5rem; display: block !important; visibility: visible !important; }
.content h3 { margin-top: 1rem; margin-bottom: 0.3rem; display: block !important; visibility: visible !important; }
.content h4 { margin-top: 0.8rem; margin-bottom: 0.2rem; display: block !important; visibility: visible !important; }
.content ul, .content ol { margin-bottom: 0.5rem; }
.content li { margin-bottom: 0.2rem; }
.content blockquote { margin: 0.5rem 0; }
.content hr { margin: 1rem 0; }

/* Story cards */
.story-card {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px;
  padding: 16px;
  margin: 1rem 0;
  border-left: 3px solid var(--story-color, #64748b);
}
.story-card h4 {
  color: var(--story-color, #f1f5f9);
  margin-top: 0 !important;
  margin-bottom: 8px;
}
.story-card p {
  margin-bottom: 0.3rem;
}
.story-card strong {
  color: #94a3b8;
}

/* LP tags */
.lp-tag {
  display: inline-block;
  background: rgba(255,255,255,0.05);
  border-radius: 4px;
  padding: 2px 8px;
  font-size: 11px;
  color: #94a3b8;
  margin-right: 4px;
  margin-bottom: 4px;
}
.lp-tag.highlight {
  background: rgba(249, 115, 22, 0.2);
  color: #f97316;
  border: 1px solid rgba(249, 115, 22, 0.3);
}

/* Popover for hover functionality */
.story-popover {
  position: fixed;
  max-width: 600px;
  max-height: 80vh;
  overflow-y: auto;
  padding: 16px 18px;
  background: #0f172a;
  border: 1px solid #f97316;
  border-radius: 8px;
  color: #e2e8f0;
  font-size: 13px;
  line-height: 1.6;
  z-index: 9999;
  pointer-events: none;
  opacity: 0;
  transition: opacity 0.12s ease;
  box-shadow: 0 8px 32px rgba(0,0,0,0.6);
}
.story-popover.visible {
  opacity: 1;
}
.story-popover-title {
  font-weight: 700;
  font-size: 15px;
  color: #f97316;
  margin-bottom: 12px;
  padding-bottom: 8px;
  border-bottom: 1px solid rgba(255,255,255,0.1);
}
.story-popover-star {
  margin-bottom: 12px;
}
.story-popover-star p {
  margin: 4px 0;
}
.story-popover-star strong {
  color: #f97316;
}
.story-popover-full {
  margin-top: 12px;
  padding-top: 12px;
  border-top: 1px solid rgba(255,255,255,0.1);
  color: #94a3b8;
  font-size: 12px;
}
.story-popover-full-title {
  font-weight: 600;
  color: #cbd5e1;
  margin-bottom: 6px;
}

/* Story links get hover behavior */
.content a[href^="#"] {
  cursor: help;
}

/* Tables */
.content table {
  width: 100%;
  border-collapse: collapse;
  font-size: 13px;
  margin: 1rem 0;
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 8px;
  overflow: hidden;
}
.content th {
  background: rgba(255,255,255,0.08);
  color: #f1f5f9;
  font-weight: 600;
  padding: 10px 12px;
  text-align: left;
  border-bottom: 1px solid rgba(255,255,255,0.1);
}
.content td {
  padding: 8px 12px;
  color: #e2e8f0;
  border-bottom: 1px solid rgba(255,255,255,0.05);
  vertical-align: top;
}
.content tr:hover td {
  background: rgba(255,255,255,0.02);
}

/* Links */
.content a {
  color: #f97316;
  text-decoration: underline;
  text-decoration-color: rgba(249, 115, 22, 0.4);
  text-underline-offset: 2px;
  transition: all 0.15s;
}
.content a:hover {
  color: #fb923c;
  text-decoration-color: #fb923c;
}

/* Arc visualization */
.arc-chain {
  font-family: 'SF Mono', 'Monaco', 'Inconsolata', 'Fira Code', monospace;
  font-size: 12px;
  color: #94a3b8;
  padding: 12px 16px;
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
  border-radius: 6px;
  margin: 0.5rem 0;
  overflow-x: auto;
  white-space: nowrap;
}
.arc-chain a {
  color: #f97316;
}
</style>

## Amazon LP Mapping

### Leadership Principles Table

<table>
<tr><th>#</th><th>Principle</th><th>Story IDs</th></tr>
<tr><td>5</td><td><b>Customer Obsession</b></td><td>
<a href="#aid-1-initial-challenge-star-customer">aid-1-initial-challenge</a><br>
<a href="#irr-3-building-founder-appreciation">irr-3-building-founder-appreciation</a><br>
<a href="#aid-29-systems-engineering-invitation">aid-29-systems-engineering-invitation</a><br>
<a href="#aid-21-identifying-the-gap">aid-21-identifying-the-gap</a><br>
<a href="#aid-30-virtual-testing-vision-hands-on">aid-30-virtual-testing-vision-hands-on</a><br>
<a href="#meta-1-consistent-focus-on-an-unsolved-problem">meta-1-consistent-focus</a>
</td></tr>
<tr><td>22</td><td><b>Ownership</b></td><td>
<a href="#aid-1-initial-challenge-star-customer">aid-1-initial-challenge</a><br>
<a href="#aid-28-reading-bridge-role-hands-on-star-customer-obsession">aid-28-reading-bridge-role</a><br>
<a href="#fer-9-radar-vendor-communication-star-ownership">fer-9-radar-vendor-communication</a><br>
<a href="#irr-2-team-diagnosis">irr-2-team-diagnosis</a><br>
<a href="#irr-3-building-founder-appreciation">irr-3-building-founder-appreciation</a><br>
<a href="#irr-6-xlang-compiler-initiative-hands-on">irr-6-xlang-compiler-initiative</a><br>
<a href="#irr-10-mpt-project-delivery-star-ownership">irr-10-mpt-project-delivery</a><br>
<a href="#irr-12-breaking-the-rebuild-cycle-hands-on-star-highlight">irr-12-breaking-the-rebuild-cycle</a><br>
<a href="#irr-16-coaching-peer-manager-star">irr-16-coaching-peer-manager</a><br>
<a href="#irr-17-half-sizing-survival-star-trust">irr-17-half-sizing-survival</a><br>
<a href="#aid-3-team-expansion">aid-3-team-expansion</a><br>
<a href="#aid-4-building-four-teams">aid-4-building-four-teams</a><br>
<a href="#fer-1-joining-fernride">fer-1-joining-fernride</a><br>
<a href="#irr-13-driving-co-design-hands-on">irr-13-driving-co-design</a>
</td></tr>
<tr><td>15</td><td><b>Invent and Simplify</b></td><td>
<a href="#irr-6-xlang-compiler-initiative-hands-on">irr-6-xlang-compiler-initiative</a><br>
<a href="#irr-8-principled-problem-solving-hands-on">irr-8-principled-problem-solving</a><br>
<a href="#irr-10-mpt-project-delivery-star-ownership">irr-10-mpt-project-delivery</a><br>
<a href="#irr-12-breaking-the-rebuild-cycle-hands-on-star-highlight">irr-12-breaking-the-rebuild-cycle</a><br>
<a href="#aid-2-architecture-discovery-hands-on">aid-2-architecture-discovery</a><br>
<a href="#aid-25-delivering-under-pressure-hands-on">aid-25-delivering-under-pressure</a><br>
<a href="#aid-27-mathematical-framework-hands-on">aid-27-mathematical-framework</a><br>
<a href="#fer-4-hazard-zone-design-hands-on">fer-4-hazard-zone-design</a><br>
<a href="#fer-12-safety-case-design-hands-on">fer-12-safety-case-design</a><br>
<a href="#irr-9-fpga-management-tool-hands-on">irr-9-fpga-management-tool</a>
</td></tr>
<tr><td>13</td><td><b>Are Right, A Lot</b></td><td>
<a href="#fer-2-safety-perception-research-hands-on-star-ownership">fer-2-safety-perception-research</a><br>
<a href="#fer-9-radar-vendor-communication-star-ownership">fer-9-radar-vendor-communication</a><br>
<a href="#irr-2-team-diagnosis">irr-2-team-diagnosis</a><br>
<a href="#irr-5-codebase-architecture-analysis-hands-on">irr-5-codebase-architecture-analysis</a><br>
<a href="#irr-6-xlang-compiler-initiative-hands-on">irr-6-xlang-compiler-initiative</a><br>
<a href="#irr-11-docker-advocacy-star">irr-11-docker-advocacy</a><br>
<a href="#irr-12-breaking-the-rebuild-cycle-hands-on-star-highlight">irr-12-breaking-the-rebuild-cycle</a><br>
<a href="#aid-2-architecture-discovery-hands-on">aid-2-architecture-discovery</a><br>
<a href="#aid-9-vtd-evaluation-hands-on">aid-9-vtd-evaluation</a>
</td></tr>
<tr><td>13</td><td><b>Learn and Be Curious</b></td><td>
<a href="#aid-28-reading-bridge-role-hands-on-star-customer-obsession">aid-28-reading-bridge-role</a><br>
<a href="#fer-2-safety-perception-research-hands-on-star-ownership">fer-2-safety-perception-research</a><br>
<a href="#irr-2-team-diagnosis">irr-2-team-diagnosis</a><br>
<a href="#irr-5-codebase-architecture-analysis-hands-on">irr-5-codebase-architecture-analysis</a><br>
<a href="#aid-2-architecture-discovery-hands-on">aid-2-architecture-discovery</a><br>
<a href="#aid-10-buy-vs-build">aid-10-buy-vs-build</a> <span class="lp-tag">negative</span><br>
<a href="#fer-1-joining-fernride">fer-1-joining-fernride</a><br>
<a href="#irr-1-joining-irreducible">irr-1-joining-irreducible</a><br>
<a href="#irr-23-staying-current">irr-23-staying-current</a>
</td></tr>
<tr><td>24</td><td><b>Hire and Develop the Best</b></td><td>
<a href="#aid-5-f-reversed-rejection-star-best-employer">aid-5-f-reversed-rejection</a><br>
<a href="#aid-8-t-us-experience-star-best-employer">aid-8-t-us-experience</a><br>
<a href="#fer-14-cto-integration-star-disagree">fer-14-cto-integration</a><br>
<a href="#irr-2-team-diagnosis">irr-2-team-diagnosis</a><br>
<a href="#irr-4-hiring-a-fpga-shell-design-hands-on">irr-4-hiring-fpga-shell</a><br>
<a href="#irr-7-compiler-engineer-hiring">irr-7-compiler-engineer-hiring</a><br>
<a href="#irr-15-dual-trials-star">irr-15-dual-trials</a><br>
<a href="#irr-16-coaching-peer-manager-star">irr-16-coaching-peer-manager</a><br>
<a href="#fer-15-junior-developer-coaching">fer-15-junior-developer-coaching</a><br>
<a href="#fer-16-promotion-advocacy">fer-16-promotion-advocacy</a><br>
<a href="#fer-17-comfort-zone-coaching">fer-17-comfort-zone-coaching</a><br>
<a href="#irr-14-hft-engineer-coaching">irr-14-hft-engineer-coaching</a>
</td></tr>
<tr><td>10</td><td><b>Insist on Highest Standards</b></td><td>
<a href="#fer-9-radar-vendor-communication-star-ownership">fer-9-radar-vendor-communication</a><br>
<a href="#irr-5-codebase-architecture-analysis-hands-on">irr-5-codebase-architecture-analysis</a><br>
<a href="#irr-8-principled-problem-solving-hands-on">irr-8-principled-problem-solving</a><br>
<a href="#irr-12-breaking-the-rebuild-cycle-hands-on-star-highlight">irr-12-breaking-the-rebuild-cycle</a><br>
<a href="#aid-2-architecture-discovery-hands-on">aid-2-architecture-discovery</a><br>
<a href="#aid-9-vtd-evaluation-hands-on">aid-9-vtd-evaluation</a><br>
<a href="#fer-13-challenging-requirements">fer-13-challenging-requirements</a>
</td></tr>
<tr><td>8</td><td><b>Think Big</b></td><td>
<a href="#irr-2-team-diagnosis">irr-2-team-diagnosis</a><br>
<a href="#irr-12-breaking-the-rebuild-cycle-hands-on-star-highlight">irr-12-breaking-the-rebuild-cycle</a><br>
<a href="#aid-4-building-four-teams">aid-4-building-four-teams</a><br>
<a href="#aid-14-vision-slides">aid-14-vision-slides</a><br>
<a href="#aid-30-virtual-testing-vision-hands-on">aid-30-virtual-testing-vision</a><br>
<a href="#irr-13-driving-co-design-hands-on">irr-13-driving-co-design</a><br>
<a href="#meta-1-consistent-focus-on-an-unsolved-problem">meta-1-consistent-focus</a>
</td></tr>
<tr><td>10</td><td><b>Bias for Action</b></td><td>
<a href="#fer-6-apple-demo-star-earn-trust">fer-6-apple-demo</a><br>
<a href="#irr-4-hiring-a-fpga-shell-design-hands-on">irr-4-hiring-fpga-shell</a><br>
<a href="#irr-15-dual-trials-star">irr-15-dual-trials</a><br>
<a href="#aid-24-responding-to-skepticism">aid-24-responding-to-skepticism</a><br>
<a href="#aid-25-delivering-under-pressure-hands-on">aid-25-delivering-under-pressure</a><br>
<a href="#fer-3-one-month-prototype-hands-on">fer-3-one-month-prototype</a><br>
<a href="#fer-7-rain-demo">fer-7-rain-demo</a><br>
<a href="#irr-9-fpga-management-tool-hands-on">irr-9-fpga-management-tool</a>
</td></tr>
<tr><td>3</td><td><b>Frugality</b></td><td>
<a href="#fer-8-hardware-constraints-star-disagree">fer-8-hardware-constraints</a><br>
<a href="#irr-7-compiler-engineer-hiring">irr-7-compiler-engineer-hiring</a><br>
<a href="#fer-3-one-month-prototype-hands-on">fer-3-one-month-prototype</a>
</td></tr>
<tr><td>16</td><td><b>Earn Trust</b></td><td>
<a href="#aid-16-integration-approach-star-earn-trust">aid-16-integration-approach</a><br>
<a href="#aid-17-supporting-team-members-star-best-employer">aid-17-supporting-team-members</a><br>
<a href="#fer-14-cto-integration-star-disagree">fer-14-cto-integration</a><br>
<a href="#irr-2-team-diagnosis">irr-2-team-diagnosis</a><br>
<a href="#irr-3-building-founder-appreciation">irr-3-building-founder-appreciation</a><br>
<a href="#irr-11-docker-advocacy-star">irr-11-docker-advocacy</a><br>
<a href="#irr-15-dual-trials-star">irr-15-dual-trials</a><br>
<a href="#irr-22-alumni-support-star-trust">irr-22-alumni-support</a><br>
<a href="#fer-16-promotion-advocacy">fer-16-promotion-advocacy</a><br>
<a href="#irr-18-pivot-navigation">irr-18-pivot-navigation</a>
</td></tr>
<tr><td>17</td><td><b>Dive Deep</b></td><td>
<a href="#aid-28-reading-bridge-role-hands-on-star-customer-obsession">aid-28-reading-bridge-role</a><br>
<a href="#fer-2-safety-perception-research-hands-on-star-ownership">fer-2-safety-perception-research</a><br>
<a href="#fer-9-radar-vendor-communication-star-ownership">fer-9-radar-vendor-communication</a><br>
<a href="#irr-2-team-diagnosis">irr-2-team-diagnosis</a><br>
<a href="#irr-5-codebase-architecture-analysis-hands-on">irr-5-codebase-architecture-analysis</a><br>
<a href="#irr-11-docker-advocacy-star">irr-11-docker-advocacy</a><br>
<a href="#irr-12-breaking-the-rebuild-cycle-hands-on-star-highlight">irr-12-breaking-the-rebuild-cycle</a><br>
<a href="#irr-16-coaching-peer-manager-star">irr-16-coaching-peer-manager</a><br>
<a href="#aid-2-architecture-discovery-hands-on">aid-2-architecture-discovery</a><br>
<a href="#fer-4-hazard-zone-design-hands-on">fer-4-hazard-zone-design</a>
</td></tr>
<tr><td>14</td><td><b>Have Backbone; Disagree and Commit</b></td><td>
<a href="#fer-8-hardware-constraints-star-disagree">fer-8-hardware-constraints</a><br>
<a href="#fer-11b-mechanical-sensor-proposal-star-bias-for-action">fer-11b-mechanical-sensor-proposal</a> <span class="lp-tag">negative</span><br>
<a href="#irr-7-compiler-engineer-hiring">irr-7-compiler-engineer-hiring</a><br>
<a href="#irr-8-principled-problem-solving-hands-on">irr-8-principled-problem-solving</a><br>
<a href="#irr-11-docker-advocacy-star">irr-11-docker-advocacy</a><br>
<a href="#irr-12-breaking-the-rebuild-cycle-hands-on-star-highlight">irr-12-breaking-the-rebuild-cycle</a><br>
<a href="#irr-15-dual-trials-star">irr-15-dual-trials</a><br>
<a href="#aid-9-vtd-evaluation-hands-on">aid-9-vtd-evaluation</a><br>
<a href="#aid-10-buy-vs-build">aid-10-buy-vs-build</a>
</td></tr>
<tr><td>19</td><td><b>Deliver Results</b></td><td>
<a href="#fer-6-apple-demo-star-earn-trust">fer-6-apple-demo</a><br>
<a href="#irr-4-hiring-a-fpga-shell-design-hands-on">irr-4-hiring-fpga-shell</a><br>
<a href="#irr-10-mpt-project-delivery-star-ownership">irr-10-mpt-project-delivery</a><br>
<a href="#irr-11-docker-advocacy-star">irr-11-docker-advocacy</a><br>
<a href="#irr-17-half-sizing-survival-star-trust">irr-17-half-sizing-survival</a><br>
<a href="#aid-4-building-four-teams">aid-4-building-four-teams</a><br>
<a href="#aid-25-delivering-under-pressure-hands-on">aid-25-delivering-under-pressure</a><br>
<a href="#fer-3-one-month-prototype-hands-on">fer-3-one-month-prototype</a><br>
<a href="#fer-12-safety-case-design-hands-on">fer-12-safety-case-design</a>
</td></tr>
<tr><td>11</td><td><b>Strive to be Earth's Best Employer</b></td><td>
<a href="#aid-16-integration-approach-star-earn-trust">aid-16-integration-approach</a><br>
<a href="#aid-17-supporting-team-members-star-best-employer">aid-17-supporting-team-members</a><br>
<a href="#fer-14-cto-integration-star-disagree">fer-14-cto-integration</a><br>
<a href="#irr-16-coaching-peer-manager-star">irr-16-coaching-peer-manager</a><br>
<a href="#irr-22-alumni-support-star-trust">irr-22-alumni-support</a><br>
<a href="#fer-16-promotion-advocacy">fer-16-promotion-advocacy</a><br>
<a href="#irr-18-pivot-navigation">irr-18-pivot-navigation</a><br>
<a href="#irr-19-maintaining-morale">irr-19-maintaining-morale</a>
</td></tr>
<tr><td>2</td><td><b>Success and Scale Bring Broad Responsibility</b></td><td>
<a href="#aid-29-systems-engineering-invitation">aid-29-systems-engineering-invitation</a><br>
<a href="#fer-12-safety-case-design-hands-on">fer-12-safety-case-design</a>
</td></tr>
</table>

---

## Highlighted Stories

### Arcs

#### Arc 1: Building the Systems Team (Irreducible)

<div class="arc-chain">
<a href="#irr-2-team-diagnosis">irr-2-team-diagnosis</a> <b>&rarr;</b> <a href="#irr-3-building-founder-appreciation">irr-3-building-founder-appreciation</a> <b>&rarr;</b> <a href="#irr-4-hiring-a-fpga-shell-design-hands-on">irr-4-hiring-a-fpga-shell-design-hands-on</a> <b>&rarr;</b> <a href="#irr-5-codebase-architecture-analysis-hands-on">irr-5-codebase-architecture-analysis-hands-on</a> <b>&rarr;</b> <a href="#irr-6-xlang-compiler-initiative-hands-on">irr-6-xlang-compiler-initiative-hands-on</a> <b>&rarr;</b> <a href="#irr-7-compiler-engineer-hiring">irr-7-compiler-engineer-hiring</a> <b>&rarr;</b> <a href="#irr-8-principled-problem-solving-hands-on">irr-8-principled-problem-solving-hands-on</a>
</div>

Team diagnosis &rarr; founder appreciation &rarr; hiring &rarr; codebase analysis &rarr; xlang compiler &rarr; compiler hiring &rarr; principled problem-solving

#### Arc 2: Hazard Zone Model HZM (FERNRIDE)

<div class="arc-chain">
<a href="#fer-2-safety-perception-research-hands-on-star-ownership">fer-2-safety-perception-research-hands-on</a> <b>&rarr;</b> <a href="#fer-4-hazard-zone-design-hands-on">fer-4-hazard-zone-design-hands-on</a> <b>&rarr;</b> <a href="#fer-5-function-testing-framework-hands-on">fer-5-function-testing-framework-hands-on</a> <b>&rarr;</b> <a href="#fer-6-apple-demo-star-earn-trust">fer-6-apple-demo</a> <b>&rarr;</b> <a href="#fer-7-rain-demo">fer-7-rain-demo</a> <b>&rarr;</b> <a href="#fer-12-safety-case-design-hands-on">fer-12-safety-case-design-hands-on</a>
</div>

Safety perception research &rarr; hazard zone design &rarr; function testing &rarr; apple demo &rarr; rain demo &rarr; safety case certification

#### Arc 3: Proving AV Safety Through Virtual Testing (Career-spanning)

<div class="arc-chain">
<a href="#meta-1-consistent-focus-on-an-unsolved-problem">meta-1-consistent-focus</a> <b>&rarr;</b> <a href="#aid-2-architecture-discovery-hands-on">aid-2-architecture-discovery</a> <b>&rarr;</b> <a href="#aid-26-scenario-fidelity-gap">aid-26-scenario-fidelity-gap</a> <b>&rarr;</b> <a href="#aid-27-mathematical-framework-hands-on">aid-27-mathematical-framework</a> <b>&rarr;</b> <a href="#aid-28-reading-bridge-role-hands-on-star-customer-obsession">aid-28-reading-bridge-role</a> <b>&rarr;</b> <a href="#aid-29-systems-engineering-invitation">aid-29-systems-engineering-invitation</a> <b>&rarr;</b> <a href="#fer-12-safety-case-design-hands-on">fer-12-safety-case-design</a>
</div>

Career focus &rarr; Built simulators (AID) &rarr; Simulation fidelity + noise (Argo) &rarr; Scenario generation patent (Argo) &rarr; Bridge to systems engineering (Argo) &rarr; Systems engineering invitation &rarr; HZM + TUV SUD certification (FERNRIDE)

**Customer context:** Ford/VW were the customers; AV validation was critical for their plans. Argo developers and some peers were to join back VW/Audi to prepare for that. In AV, validation and development are extremely intertwined.

---

### Top 10 Stories

| Rank | Story ID | Summary | Principles |
| ---- | -------- | ------- | ---------- |
| 1 | [irr-12-breaking-the-rebuild-cycle](#irr-12-breaking-the-rebuild-cycle-hands-on-star-highlight) + [irr-13-driving-co-design](#irr-13-driving-co-design-hands-on) | 3-time rebuild cycle diagnosis &rarr; designed three-mode architecture &rarr; pushed CTO to model &rarr; revealed product not competitive | Think Big, Have Backbone, Are Right, Dive Deep, Deliver Results |
| 2 | [aid-4-building-four-teams](#aid-4-building-four-teams) | Built 4 teams from 2-person team (20 engineers), owned vision/strategy/roadmap | Ownership, Think Big, Hire/Develop |
| 3 | [irr-17-half-sizing-survival](#irr-17-half-sizing-survival-star-trust) | Only non-cryptographer kept after half-sizing &rarr; remained in reduced leadership team | Deliver Results, Earn Trust, Ownership |
| 4 | [irr-10-mpt-project-delivery](#irr-10-mpt-project-delivery-star-ownership) | Introduced project weeklies + end-to-end ownership in complex ZK domain &rarr; interviewed experts, translated ZK into CS/algebra terms so non-experts could contribute &rarr; MPT delivered on time &rarr; continued approach, became standard process | Deliver Results, Ownership, Dive Deep, Invent/Simplify |
| 5 | [aid-15-acquisition-integration](#aid-15-acquisition-integration) + [aid-16-integration-approach](#aid-16-integration-approach-star-earn-trust) | Argo acquisition integration: "add value first, identity later" | Earn Trust, Best Employer, Ownership |
| 6 | [fer-12-safety-case-design](#fer-12-safety-case-design-hands-on) | HZM + DFD approach &rarr; TUV SUD certification | Deliver Results, Invent/Simplify, Broad Responsibility |
| 7 | [aid-10-buy-vs-build](#aid-10-buy-vs-build) | Buy-vs-build regret &rarr; gained confidence &rarr; challenged CEO/CTO | Have Backbone, Learn/Curious |
| 8 | [aid-9-vtd-evaluation](#aid-9-vtd-evaluation-hands-on) | VTD rejection despite Audi relationship pressure | Have Backbone, Dive Deep, Are Right |
| 9 | [aid-5-f-reversed-rejection](#aid-5-f-reversed-rejection-star-best-employer) | F. reversal hire &rarr; now leads at Apple | Hire/Develop, Are Right, Have Backbone |
| 10 | [fer-3-one-month-prototype](#fer-3-one-month-prototype-hands-on) | Prototype in 1 month with 2 devs | Frugality, Bias for Action, Deliver Results |

---

## Quick Reference for Common Questions

### Team Building
- [aid-4-building-four-teams](#aid-4-building-four-teams)
- [irr-2-team-diagnosis](#irr-2-team-diagnosis)
- [fer-18-team-growth](#fer-18-team-growth)

### Disagree and Commit
- [aid-10-buy-vs-build](#aid-10-buy-vs-build)
- [irr-13-driving-co-design-hands-on](#irr-13-driving-co-design-hands-on)
- [aid-9-vtd-evaluation-hands-on](#aid-9-vtd-evaluation-hands-on)
- [irr-7-compiler-engineer-hiring](#irr-7-compiler-engineer-hiring)

### Learning from Setbacks
- [aid-10-buy-vs-build](#aid-10-buy-vs-build) `negative`
- [fer-11-trailer-angle-hands-on](#fer-11-trailer-angle-hands-on) `negative`
- [fer-11b-mechanical-sensor-proposal](#fer-11b-mechanical-sensor-proposal-star-bias-for-action) `negative`
- [aid-4-building-four-teams](#aid-4-building-four-teams) `negative`

### Delivering Under Pressure
- [aid-24-responding-to-skepticism](#aid-24-responding-to-skepticism)
- [aid-25-delivering-under-pressure-hands-on](#aid-25-delivering-under-pressure-hands-on)
- [irr-10-mpt-project-delivery](#irr-10-mpt-project-delivery-star-ownership)

### Talent Development
- [aid-5-f-reversed-rejection](#aid-5-f-reversed-rejection-star-best-employer)
- [aid-11-l-communication-coaching](#aid-11-l-communication-coaching)
- [fer-15-junior-developer-coaching](#fer-15-junior-developer-coaching)
- [fer-17-comfort-zone-coaching](#fer-17-comfort-zone-coaching)
- [irr-14-hft-engineer-coaching](#irr-14-hft-engineer-coaching)

### Conflict Resolution
- [fer-14-cto-integration](#fer-14-cto-integration-star-disagree)
- [aid-19-u-colleague-code-quality](#aid-19-u-colleague-code-quality)
- [irr-16-coaching-peer-manager](#irr-16-coaching-peer-manager-star)

### Biggest Impact
- [irr-12-breaking-the-rebuild-cycle-hands-on](#irr-12-breaking-the-rebuild-cycle-hands-on-star-highlight)
- [irr-13-driving-co-design-hands-on](#irr-13-driving-co-design-hands-on)
- [fer-12-safety-case-design-hands-on](#fer-12-safety-case-design-hands-on)
- [aid-4-building-four-teams](#aid-4-building-four-teams)
- [meta-1-consistent-focus-on-an-unsolved-problem](#meta-1-consistent-focus-on-an-unsolved-problem)

### Career Motivation
- [meta-1-consistent-focus-on-an-unsolved-problem](#meta-1-consistent-focus-on-an-unsolved-problem)

### Strategic Vision
- [aid-30-virtual-testing-vision-hands-on](#aid-30-virtual-testing-vision-hands-on)
- [irr-12-breaking-the-rebuild-cycle-hands-on](#irr-12-breaking-the-rebuild-cycle-hands-on-star-highlight)
- [irr-13-driving-co-design-hands-on](#irr-13-driving-co-design-hands-on)
- [aid-4-building-four-teams](#aid-4-building-four-teams)

### Technical Problem Solving
- [aid-31-daisy-traffic-simulator-hands-on](#aid-31-daisy-traffic-simulator-hands-on)
- [aid-32-pacos-concurrency-simulator-hands-on](#aid-32-pacos-concurrency-simulator-hands-on)
- [fer-5-function-testing-framework-hands-on](#fer-5-function-testing-framework-hands-on)
- [irr-9-fpga-management-tool-hands-on](#irr-9-fpga-management-tool-hands-on)

### Technical Depth
- [irr-9-fpga-management-tool-hands-on](#irr-9-fpga-management-tool-hands-on)
- [irr-12-breaking-the-rebuild-cycle-hands-on](#irr-12-breaking-the-rebuild-cycle-hands-on-star-highlight)
- [irr-20-post-pivot-product-focus-hands-on](#irr-20-post-pivot-product-focus-hands-on)
- [fer-3-one-month-prototype-hands-on](#fer-3-one-month-prototype-hands-on)
- [fer-5-function-testing-framework-hands-on](#fer-5-function-testing-framework-hands-on)
- [fer-11-trailer-angle-hands-on](#fer-11-trailer-angle-hands-on)
- [aid-23-modeling-library-hands-on](#aid-23-modeling-library-hands-on)
- [aid-31-daisy-traffic-simulator-hands-on](#aid-31-daisy-traffic-simulator-hands-on)
- [aid-32-pacos-concurrency-simulator-hands-on](#aid-32-pacos-concurrency-simulator-hands-on)

---

## Stories

### Irreducible (Sep 2024 - Present)

### Context & Motivation

#### IRR-1-Joining-Irreducible
**Situation:** I was headhunted from outside my network—the crypto industry found me through my systems/FPGA background despite coming from autonomous vehicles.
**Task:** Decide whether to join a new domain.
**Action:** I evaluated the opportunity deliberately: I wanted to learn Rust as a modern systems language. Cryptography interested me as a math application I'd never worked on. The role offered low-level systems work: managing the team writing interfaces between crypto in Rust and FPGAs, owning performance optimization, ISA design, and FPGA design.
**Result:** I joined Irreducible, successfully transitioning from AV to cryptographic hardware.

### Building the Systems Team

#### IRR-2-Team-Diagnosis
**Situation:** The systems team had been disbanded twice previously. The organization recognized poor outcomes but hadn't identified root causes.
**Task:** Diagnose why and build a team that would succeed.
**Action:** I interviewed everyone involved—current engineers, leadership, and former team members. I found two issues: the team had been staffed with generalists who lacked domain expertise, and their scope was too narrow to influence design decisions. I restructured with two principles: every hire needed hands-on experience in cryptographic or hardware domains, and the team would own end-to-end integration.
**Result:** Previously unowned blockers gained accountability. The team functioned as connective tissue between crypto and hardware rather than an additional coordination layer.

#### IRR-3-Building-Founder-Appreciation
**Situation:** FPGA engineering, ZK engineering, and ZK research operated in silos. The interdependencies between systems work and cryptography weren't visible across teams.
**Task:** Bridge the silos and build shared understanding.
**Action:** I worked closely with both founders (hands-on with HW and ZK) to demonstrate how systems engineering connects the domains and enables cryptographic performance.
**Result:** Systems work gained visibility; cross-team collaboration improved significantly.

#### IRR-4-Hiring-A-FPGA-Shell-Design-Hands-On
**Situation:** We needed someone senior for low-level development. The existing FPGA system had a critical packet loss problem (open-loop design).
**Task:** Hire the right person and fix the architecture.
**Action:** I hired A.—very senior, perfect for the role. We worked with the senior FPGA hire to design a new FPGA shell. We designed the system upfront for robustness against edge cases.
**Result:** New shell solved the packet loss problem. Patched holes in existing code.

#### IRR-5-Codebase-Architecture-Analysis-Hands-On
**Situation:** The existing code had evolved organically to serve specific crypto designs. It functioned as memory passing engine, compute compiler, and kernel management—but these concerns weren't cleanly separated.
**Task:** Understand the codebase and determine the right path forward.
**Action:** I studied the architecture deeply. I concluded that the current compilation approach wouldn't scale—we needed a more general compiler to support future requirements.
**Result:** Clear technical direction: build a proper compiler capability. This insight led to the xlang compiler initiative.

#### IRR-6-XLang-Compiler-Initiative-Hands-On
**Situation:** A crypto team member at the interface intersection had built private bespoke tooling. The approach aligned with my intuition that compilation should be owned by systems.
**Task:** Formalize this into a real compiler capability.
**Action:** I proposed a slimmed-down backend-only compiler ("xlang") allowing developers to use the backend API directly. I moved this developer to my team to focus on compiler work and provide closer mentorship. I moderated design discussions between the 3 devs and contributed to parts on a need basis.
**Result:** xlang became a real product; developer grew under coaching.

#### IRR-7-Compiler-Engineer-Hiring
**Situation:** We needed a compiler engineer. The founders posed Rust expertise as an immutable JD requirement, but the intersection of Rust + compiler skills was rare.
**Task:** Improve the candidate funnel without lowering the bar.
**Action:** I worked with the recruiter to argue that seniority with appetite to learn was sufficient. We removed the Rust constraint.
**Result:** Funnel improved dramatically; hired a strong compiler engineer.

#### IRR-8-Principled-Problem-Solving-Hands-On
**Situation:** The FPGA cores were specific—VLIW-like with quirks, not Turing complete, like crypto DSPs with multi-register bank constraints. The optimization approach was iterative rather than systematic.
**Task:** Establish a principled approach.
**Action:** I worked with the team on a greedy algorithm generalizing an existing approach. I coached the team on how a systematic method would reduce iteration cycles and improve cross-team alignment.
**Result:** Algorithm handles over 80% of code optimally. Team adopted the principled approach.

### Technical Initiatives

#### IRR-9-FPGA-Management-Tool-Hands-On
**Situation:** We had a crude shell script for FPGA management that didn't handle edge cases (wrong versions, timing issues, resetting, querying multiple FPGAs).
**Task:** Build a proper tool.
**Action:** I wrote an FPGA management tool in Rust.
**Result:** Became the tool used by infrastructure for CI/CD; would have been used in production.

#### IRR-10-MPT-Project-Delivery-star-ownership
**Situation:** Merkle Patricia Trie was a company goal for end of 2024. Cross-team dependencies made delivery challenging—previously only team weeklies existed, so cross-functional risks stayed invisible. ZK cryptography is technically demanding, making cross-functional coordination harder.
**Task:** Deliver on time and establish a repeatable process.
**Action:** I introduced project weeklies—a unified forum I moderated with technical questions to surface risks teams might not raise unprompted. I interviewed experts across teams and maintained documents mapping the end-to-end view to disseminate system knowledge. I dove into technical details across the stack to spot critical-path dependencies early. I created a visual diagram of projects and blockers for shared visibility.
**Result:** MPT delivered on time. I continued this approach beyond MPT—maintaining end-to-end documents and translating ZK concepts into familiar computer science and algebra terms so non-experts could think along and contribute. This became the standard process for all cross-functional projects.

#### IRR-11-Docker-Advocacy-star
**Situation:** A technical disagreement on Docker containers was stalling the MPT project. Infrastructure believed they affected performance.
**Task:** Unblock the project.
**Action:** I set up a discussion between A. (virtualization expertise from Amazon) and the infra team. I facilitated the conversation, helping both sides clarify their positions and resolve the opinion differences.
**Result:** Docker became a standard tool in infrastructure. Project unblocked.

#### IRR-12-Breaking-the-Rebuild-Cycle-Hands-On-star-highlight
**Situation:** As the systems team learned the product, I noticed a recurring pattern: crypto design &rarr; HW &rarr; implement &rarr; discover performance issues &rarr; the cycle repeated. The company had rebuilt the product three times this way.
**Task:** Break this cycle before repeating it with expensive HBM hardware.
**Action:** I proposed three implementations of the systems layer—simulated, verilated, and HW—each with different use case, all implementing the same interface. I implemented the emulated mode and led architecture decisions for verilated and HW modes. The founders challenged this as unnecessary. I held my ground.
**Result:** Sim vs verilated found implementation bugs; verilated vs HW found HW bugs. Simulation became powerful enough to estimate future performance quickly.

#### IRR-13-Driving-Co-Design-Hands-On
**Situation:** The company aspired to "co-design" between crypto and systems, but the workflow was sequential—crypto designed, systems implemented, issues surfaced late.
**Task:** Make co-design real.
**Action:** I pushed for modeling future designs with back-of-envelope calculations turned into simulation before committing to implementation.
**Result:** Revealed product was potentially not competitive—something we would have discovered months later using the old process. Enabled fixing crypto techniques upfront.

### People & Leadership

#### IRR-14-HFT-Engineer-Coaching
**Situation:** I hired a third engineer from HFT background—excellent at performance optimization, parallel computing, kernel writing. But he had difficulty communicating on non-technical matters; his style was extremely sparse.
**Task:** Help him improve.
**Action:** I worked with him on the topic. He'd tried books multiple times. We concluded together that books couldn't solve this.
**Result:** He agreed to work with a specialized counselor. Honest guidance toward a real solution.

#### IRR-15-Dual-Trials-star
**Situation:** As compiler work expanded, we needed to hire. I asked for two trials in the same week due to time constraints. The founders raised concerns about conflicts.
**Task:** Run both trials successfully.
**Action:** I reframed this as a feature: senior candidates should be able to handle transparency about parallel evaluations. I prepared carefully—dedicated Slack channels, intro meetings.
**Result:** Trial went seamlessly. When the company half-sized during the trial, I stopped it mid-process with full transparency.

#### IRR-16-Coaching-Peer-Manager-star
**Situation:** The infrastructure engineering manager had role difficulties with his team.
**Task:** Help him without taking over.
**Action:** I diagnosed the issue as a communication mismatch—his style didn't match his team's expectations for directness. I helped him adjust his approach. I also collaborated on CI/CD workflows.
**Result:** He improved team interactions; we strengthened infrastructure automation.

### Organizational Challenges & Pivot

#### IRR-17-Half-Sizing-Survival-star-trust
**Situation:** The company half-sized, removing HW, systems, and infrastructure teams to pivot to pure SW crypto.
**Task:** Continue contributing.
**Action:** I was the only non-cryptographer not let go—recognized for helping the engineering organization function smoothly.
**Result:** Remained in the leadership team, now reduced to two founders + me. Role became mostly IC while contributing to leadership decisions.

#### IRR-18-Pivot-Navigation
**Situation:** Team morale was fragile during the pivot.
**Task:** Help founders navigate and keep team grounded.
**Action:** I moderated group meetings, injecting soberness, reality checks, and optimism. I reviewed founders' pitches to the team. My principle: treat people as adults with transparency—state realities, balance with future scenarios, help team see their strengths.
**Result:** Team stayed cohesive through difficult transition.

#### IRR-19-Maintaining-Morale
**Situation:** Difficult post-pivot period with uncertainty.
**Task:** Keep everyone motivated.
**Action:** I celebrated small wins, had 1-1s addressing core frustrations, listened carefully, and shared perspectives from researching pivots that became successes—real examples, not empty optimism.
**Result:** Team stayed engaged despite uncertainty.

#### IRR-20-Post-Pivot-Product-Focus-Hands-On
**Situation:** After the pivot, everyone became a "circuit writer"—implementing cryptographic primitives as ZK-provable algorithms.
**Task:** Contribute technically while supporting critical resources.
**Action:** I wrote ZK circuits myself. I also built prototypes for a ZK circuit compiler—exploring how to make circuit development more systematic. I worked closely with the compiler engineer but deliberately didn't interrupt him—he was critical. Instead, I prototyped ideas on the side and discussed with CTO. I moderated brainstorming, focusing on unconsidered views about how to use our skills for new technologies.
**Result:** Kept momentum; circuit compiler prototypes informed future direction; explored adjacent opportunities.

#### IRR-21-AI-First-Mandate
**Situation:** The founders created a mandate to become AI-first for coding. Questions arose about quality of AI-generated ZK circuits.
**Task:** Balance adoption with quality.
**Action:** I defended the engineer by balancing points of view—the process around AI tools wasn't fully clarified, and aggressive adoption carried risks. I shared that I had the same experience: wrote circuits I didn't fully understand and needed peer review.
**Result:** More balanced discussion about AI adoption risks.

#### IRR-22-Alumni-Support-star-trust
**Situation:** Irreducible closed after the pivot.
**Task:** Support team members through the transition.
**Action:** I created a WhatsApp alumni channel for my systems team. I stay in touch with their job searches and offer recommendations.
**Result:** Several have landed new roles with my references. The relationship doesn't end when the role ends.

### Post-Irreducible

#### IRR-23-Staying-Current
**Situation:** Irreducible closed after the pivot.
**Task:** Stay technically relevant and prepare for next role.
**Action:** I've been researching LLMs and agentic workflows to stay technically current. I'm also researching structured ways for engineering managers to handle transitions—developing frameworks to help other EMs.
**Result:** Maintaining technical edge while contributing to the EM community.

#### IRR-24-Consulting
**Situation:** After Irreducible, I had expertise to share.
**Task:** Continue contributing while between roles.
**Action:** I started consulting—coaching tech leads. I helped one engineer obtain a lead position at his workplace that he was qualified for but struggled to get buy-in for.
**Result:** He got the role. Validated my coaching approach.

---

### FERNRIDE (Jan 2023 - Sep 2024)

### Context & Foundation

#### FER-1-Joining-FERNRIDE
**Situation:** I wanted to move from simulation to actual robotics, connecting to my Havok and Guerrilla background. FERNRIDE had remote trucks and was starting an autonomy team.
**Task:** Define my role in a two-person team.
**Action:** I joined with one peer—two developers total. I recognized a safety layer was critical due to applicable standards and took ownership of that effort while my peer focused on performance autonomy.
**Result:** Established clear ownership of safety-critical systems from day one.

#### FER-2-Safety-Perception-Research-Hands-On-star-ownership
**Situation:** We needed a viable approach for safety perception.
**Task:** Determine feasibility.
**Action:** I did two weeks of deep research. Concluded LiDAR-based ground plane segmentation with no false negatives was viable given current hardware. Early discussions with safety confirmed we needed both radar and LiDAR for weather.
**Result:** Clear technical direction with safety buy-in.

#### FER-3-One-Month-Prototype-Hands-On
**Situation:** We had two developers and needed to prove viability fast.
**Task:** Build a working prototype.
**Action:** I found a paper with code we could modify. We had a LiDAR driver. I attracted an engineer looking for more technical depth who was keen to join. I soldered wires for missing connectors since we weren't an automotive company yet.
**Result:** Working prototype in one month—end-to-end, surprising speed.

### Technical Development

#### FER-4-Hazard-Zone-Design-Hands-On
**Situation:** We needed a comprehensive safety specification.
**Task:** Design a spec that captures all risk factors.
**Action:** I designed the Hazard Zone (HZM)—expanding the perceived area with all potential noise sources: LiDAR calibration, weather effects, uncertainty of future driver action.
**Result:** HZM became central to safety design and certification effort.

#### FER-5-Function-Testing-Framework-Hands-On
**Situation:** We needed to validate the safety function systematically, but operations were being established.
**Task:** Build testing capability despite constraints.
**Action:** We succeeded in getting operations in Estonia (very snowy—added complexity). I built a function testing code framework that uses replay data from real drives. I designed the approach for labeling pass/fail—determining ground truth for our open-loop function (single decision: stop or not). This framework enabled systematic validation against recorded scenarios.
**Result:** Systematic testing capability established. Framework became the foundation for ongoing validation work.

#### FER-6-Apple-Demo-star-earn-trust
**Situation:** We needed to demonstrate capability.
**Task:** Show the system works with high precision.
**Action:** I threw an apple in front of the truck during a demo.
**Result:** Truck stopped with very high precision. We called it the "Apple Demo." I ate the apple afterward.

#### FER-7-Rain-Demo
**Situation:** Heavy rain started during a demo.
**Task:** Decide whether to continue.
**Action:** We decided to do another run in the heavy rain anyway.
**Result:** Function worked perfectly—no false positives, no false negatives. Powerful demonstration.

#### FER-8-Hardware-Constraints-star-disagree
**Situation:** Nvidia hardware wouldn't ship due to budget restrictions. We faced a choice: keep developing on unavailable hardware or optimize for underpowered but certifiable hardware.
**Task:** Align on direction and execute.
**Action:** I had a different view than the tech director. When the decision was made, I disagreed but committed fully.
**Result:** Spent months rewriting and optimizing for limited hardware. Made it work.

### External Collaboration

#### FER-9-Radar-Vendor-Communication-star-ownership
**Situation:** Radar projects differ from LiDAR—they require close manufacturer collaboration. Communication with Continental was handled by procurement. Communication wasn't progressing as expected.
**Task:** Fix the relationship.
**Action:** I asked to join the meetings. I realized we weren't being technical enough. I asked to speak directly with their technical people—opened a new channel. We organized a two-day measurement campaign in Italy, carefully choosing scenarios.
**Result:** Continental evaluated our data and agreed quality was insufficient. Honest technical partnership established.

#### FER-10-Radar-De-risking
**Situation:** The radar data quality didn't meet our requirements. We needed alternatives.
**Task:** Find a path forward.
**Action:** I led internal brainstorming: bypass radar? Get more from LiDAR? Operational constraints? Director chose to continue with radar. In parallel, I contracted a radar expert company in Eastern Europe to double-check Continental.
**Result:** They reached similar conclusions; interested in supplying custom-tuned radars. Multiple options opened.

#### FER-11-Trailer-Angle-Hands-On
**Situation:** I discovered late (my blind spot) that the trailer needed to be in the Hazard Zone—it has its own trajectory when not aligned.
**Task:** Fix it despite not being in my expertise.
**Action:** I was listening to an image processing course on my commute. I realized I could apply it. I built a prototype with geometric projection and fitting.
**Result:** Made it to product. Had calibration shortcomings. A later hire with deep expertise improved it.

#### FER-11b-Mechanical-Sensor-Proposal-star-bias-for-action
**Situation:** With the head of safety in Tallinn, we explored measuring trailer angle with a mechanical sensor while examining the trailer structure.
**Task:** Evaluate whether mechanical sensing could simplify the SW approach.
**Action:** I advocated for the mechanical approach—it would shift complexity out of SW, letting my understaffed team focus on higher priorities. CTO disagreed. I committed to the SW path.
**Result:** CTO was right. I did systems thinking—considered impact on procurement, operations, cabling—but biased the analysis toward my team's benefit without being transparent about it. Even if shifting complexity is valid (understaffed team, need to focus), I should have explicitly argued the trade-off. Without that transparency, my proposal looked like scope bias rather than strategic prioritization.

### Safety & Requirements

#### FER-12-Safety-Case-Design-Hands-On
**Situation:** We needed a safety case for certification.
**Task:** Design an approach that's both rigorous and practical.
**Action:** I worked closely with safety and systems engineering. Together we developed a simple but effective approach using DFD diagrams (my suggestion)—keeping design close to code with provable statements about output relations.
**Result:** Technique was modified after I left but remained part of TUV SUD certification.

#### FER-13-Challenging-Requirements
**Situation:** Some systems engineering requirements needed adjustment for implementation.
**Task:** Get them corrected.
**Action:** I challenged several requirements in tight discussions.
**Result:** Many changes made; requirements became more appropriate.

### People & Leadership

#### FER-14-CTO-Integration-star-disagree
**Situation:** The CTO wanted to stay technical but couldn't operate as a team member—handled everything himself. Others avoided giving feedback out of fear.
**Task:** Help him or end the experiment.
**Action:** I tried integrating him into my team. After two weeks, I gave direct honest feedback: "This isn't working as long as you can't become a team member instead of handling everything yourself."
**Result:** We ended without hard feelings. He needed that mirror.

#### FER-15-Junior-Developer-Coaching
**Situation:** A junior programmer struggled with C++ despite passing the interview.
**Task:** Help her succeed.
**Action:** We had 1-1s to understand struggles. I gave her resources and time. When that wasn't enough, I paired her with an experienced C++ developer for mentoring.
**Result:** She was promoted to engineer after 10 months.

#### FER-16-Promotion-Advocacy
**Situation:** My best contributor deserved a promotion.
**Task:** Get him promoted.
**Action:** I worked his case and got him promoted. This drew critique from peer managers who felt I'd taken from their pool. I had close discussions to understand perspectives.
**Result:** We aligned on approaches. I realized I was overfitting to my previous company's culture.

#### FER-17-Comfort-Zone-Coaching
**Situation:** A senior C++ expert wanted robotics but kept defaulting to C++ tasks—his comfort zone.
**Task:** Help him grow.
**Action:** After many discussions, we agreed he'd work on radar. It was difficult—not purely SW engineering.
**Result:** Moderate success: he appreciated both the new domain's difficulty and his C++ value. I got him promoted; it helped his self-confidence.

#### FER-18-Team-Growth
**Situation:** I had learned from hiring too senior-heavy at AID.
**Task:** Build a balanced team.
**Action:** At FERNRIDE, I deliberately hired at multiple seniority levels including junior engineers.
**Result:** Better team balance; work distributed appropriately.

### Planning & Roadmap

#### FER-19-Version-Planning
**Situation:** We needed to align feature development with deployment use cases.
**Task:** Create a planning framework.
**Action:** I defined the planning horizon for versions of the safety function, mapping features to deployment use cases—which capabilities for which operational scenarios and when. Balanced certification timelines with business targets.
**Result:** Clear roadmap tied to real deployment needs.

---

### AID/Argo (Apr 2018 - Dec 2022)

### Context & Discovery

#### AID-1-Initial-Challenge-star-customer
**Situation:** I joined AID to work on crowd simulation. Only a small hard-coded traffic simulation existed from Audi.
**Task:** Understand customer needs and define direction.
**Action:** I interviewed internal clients (systems engineers, developers) and discovered a gap: crowd simulation wasn't a priority—basic simulation capabilities were non-existent.
**Result:** Pivoted my focus to building foundational simulation capability instead of crowd simulation.

#### AID-2-Architecture-Discovery-Hands-On
**Situation:** We were evaluating a contracted startup's tool for simulation. I needed to understand what AV simulation actually requires.
**Task:** Define the right architecture for AV simulation.
**Action:** While prototyping, I discovered how AV simulation differs from gaming and traditional traffic simulation. I identified the structure: traffic sim with agent behaviors, visualization sim for perception developers, runtime simulation.
**Result:** The startup product turned out to be more tech demo than production-ready. This research defined the architecture that guided all subsequent work.

### Team Building

#### AID-3-Team-Expansion
**Situation:** Sim team was 2 people including me.
**Task:** Build a team.
**Action:** I discussed with management and got approval to expand. After a couple of hires, I became de facto tech lead. I was actively involved in interviewing.
**Result:** Growth without budget constraints; strong candidate pipeline.

#### AID-4-Building-Four-Teams
**Situation:** We needed comprehensive simulation capability.
**Task:** Build the organization.
**Action:** I grew from 2-person team to 4 teams: traffic sim, visualization sim, runtime, scenario evaluation.
**Result:** 20 engineers across 4 teams. Full simulation stack.

### Key Hiring Stories

#### AID-5-F-Reversed-Rejection-star-best-employer
**Situation:** Runtime became a gap as recording database grew. A candidate with fitting domain background had been rejected for failing the C++ test.
**Task:** Get the right person.
**Action:** I identified that C++ wasn't the critical skill—domain knowledge was. I reversed the decision and hired F.
**Result:** He became team lead, contributed key designs, now leads similar technology at Apple.

#### AID-6-N-OSS-Sourcing
**Situation:** For visualization sim, we needed a full-system expert. Carla was a promising open-source project.
**Task:** Get top talent.
**Action:** We sourced N., Carla's lead developer, and convinced him to join.
**Result:** Strong technical leadership for viz sim.

#### AID-7-U-Internal-Transfer
**Situation:** U. was looking for a better team fit.
**Task:** Attract good talent.
**Action:** He was drawn by our team culture.
**Result:** U. joined internally and thrived.

#### AID-8-T-US-Experience-star-best-employer
**Situation:** EU team lacked AV experience compared to US.
**Task:** Close the experience gap.
**Action:** I pushed hard to hire T. from the US.
**Result:** Brought AV experience the team needed.

### Technical Decisions

#### AID-9-VTD-Evaluation-Hands-On
**Situation:** Management recommended evaluating VTD due to Audi relationship.
**Task:** Make the right technical call.
**Action:** I visited their HQ and got the agent behavior developer on phone. I discovered their techniques weren't suited for our models.
**Result:** Rejected VTD despite relationship pressure. Led to building our own traffic sim with planner team.

#### AID-10-Buy-vs-Build
**Situation:** Early on, I lacked confidence to push back on buy-vs-build decisions.
**Task:** Change company direction.
**Action:** After receiving a BMW offer and AID counter-offer, I found confidence. I had direct talks with CEO/CTO about stopping the startup partnership.
**Result:** Built our own technology. Exception: VectorZero for landscape/map rendering.

### People Development

#### AID-11-L-Communication-Coaching
**Situation:** L. had difficulty expressing himself.
**Task:** Help him communicate better.
**Action:** Multiple 1-1s working on lowering nervousness and improving communication.
**Result:** Improved confidence and expression.

#### AID-12-Scenario-Evaluation-Team
**Situation:** Scenario evaluation was too complex for test engineers alongside other work.
**Task:** Staff appropriately.
**Action:** We formed a dedicated scenario evaluation team.
**Result:** Proper focus on complex challenge.

### Planning & Product

#### AID-13-Product-Owner-Training
**Situation:** Product owner training was offered with an external consultant.
**Task:** Learn relevant methodologies.
**Action:** Although I wasn't a product owner, I asked to join because planning methodologies were relevant to managing the simulation roadmap.
**Result:** Applied PO practices to roadmap management.

#### AID-14-Vision-Slides
**Situation:** We needed long-term direction.
**Task:** Articulate the vision.
**Action:** I created long-term vision slides for simulation, articulating where we were heading and why.
**Result:** Clear shared vision for the organization.

#### AID-30-Virtual-Testing-Vision-Hands-On
**Situation:** AV simulation requirements differ fundamentally from gaming and traditional traffic simulation.
**Task:** Define what we actually need to build.
**Action:** I researched how AV simulation differs: gaming focuses on visual fidelity and frame rate; traditional traffic simulation on aggregate flow. AV needs behavioral realism, sensor simulation, deterministic replay, and coverage analysis. I made this vision concrete by building the full stack.
**Result:** Complete virtual testing framework: traffic sim, viz sim, runtime, scenario evaluation.

### Prototyping Tools

#### AID-31-daisy-Traffic-Simulator-Hands-On
**Situation:** We needed to prototype and validate traffic simulation ideas before committing to full implementation.
**Task:** Create a lightweight tool for rapid experimentation.
**Action:** I built [daisy](https://github.com/jadnohra/daisy), a minimal open-source traffic simulator in Python. Agent-based simulation focused on exploring traffic flow modeling concepts.
**Result:** Used to prototype ideas that were later incorporated into the main product. Open-sourced for the community.

#### AID-32-PaCoS-Concurrency-Simulator-Hands-On
**Situation:** AV software stacks involve complex distributed systems with concurrency challenges. Debugging non-deterministic behavior was difficult.
**Task:** Create a tool to reason about concurrency deterministically.
**Action:** I developed [PaCoS](https://github.com/jadnohra/PaCoS), an open-source deterministic concurrency simulator. Discrete event simulation with deterministic scheduling—ensuring reproducible behavior for debugging distributed systems.
**Result:** Used for architecture decisions on distributed systems. Enabled reasoning about race conditions and timing issues in a controlled environment.

### Argo Acquisition

#### AID-15-Acquisition-Integration
**Situation:** When Argo acquired us, many were let go. Sim team largely survived but was removed from simulation work.
**Task:** Navigate the transition.
**Action:** Small core team with unclear mission. Different focus from US (large-scale cloud testing vs our behavior quality).
**Result:** Preserved team; found new direction.

#### AID-16-Integration-Approach-star-earn-trust
**Situation:** Other teams had more friction during integration.
**Task:** Integrate smoothly.
**Action:** We did direct integration—assimilated with existing sim team and started working on their backlog. "Add value first, think about identity later."
**Result:** Our approach avoided conflicts; team stayed intact.

#### AID-17-Supporting-Team-Members-star-best-employer
**Situation:** Team members were scattered across the organization after acquisition.
**Task:** Support them regardless of reporting lines.
**Action:** I supported team members wherever they ended up. I advocated for L.'s transfer despite initial skepticism from my manager.
**Result:** Team members landed well; L. switched successfully.

### Technical Leadership at Argo

#### AID-18-F-Communication-Coaching
**Situation:** F. had trouble getting ideas through at Argo. His style didn't land well in the new environment.
**Task:** Help him succeed in the new environment.
**Action:** We debugged together. I coached him on building consensus through design document discussions (major alignment vehicle at Argo).
**Result:** Improved his influence and impact.

#### AID-19-U-Colleague-Code-Quality
**Situation:** U. raised concerns about a colleague's code quality. There was a skills gap between them.
**Task:** Resolve the tension productively.
**Action:** I coached them to transform their dynamic into pair programming and mentoring.
**Result:** Pushed both out of comfort zones; situation improved.

#### AID-20-Le-Standup-Balance
**Situation:** Le. brought energy to standups that sometimes extended them.
**Task:** Find the right balance.
**Action:** We discussed the purpose of dailies and how to balance engagement with focus.
**Result:** Found balance between focus and team morale.

### Model Fidelity Work

#### AID-21-Identifying-the-Gap
**Situation:** Large-scale simulation testing had model fidelity issues. I found evidence in recording replay showing failures that should have been caught by existing scenarios.
**Task:** Diagnose and fix.
**Action:** Identified the gap: missing probability aspect in model fidelity.
**Result:** Clear problem definition for the team to address.

#### AID-22-Design-Docs
**Situation:** We needed buy-in for fidelity work.
**Task:** Build credibility.
**Action:** I wrote design docs on modeling fidelity.
**Result:** Gained appreciation from systems engineers and AV dev team leads.

#### AID-23-Modeling-Library-Hands-On
**Situation:** We needed realistic simulation.
**Task:** Build systematic noise injection.
**Action:** Team specialized in a modeling library with meticulous analysis of every component's inputs and outputs. Created functions to inject realistic noise.
**Result:** Systematic approach to simulation fidelity.

#### AID-24-Responding-to-Skepticism
**Situation:** Management prioritized delivery speed and challenged whether simulation could keep pace.
**Task:** Deliver value and demonstrate simulation wasn't a bottleneck.
**Action:** I took feedback as signal to deliver faster, not stop. Focused on proving value quickly.
**Result:** With noise injection, significant percentage of previously-passing tests started correctly failing. Validated the approach.

#### AID-25-Delivering-Under-Pressure-Hands-On
**Situation:** Management increased visibility on the project given its importance.
**Task:** Demonstrate solid planning and execution.
**Action:** I created a granular Excel tracker (Jira couldn't handle the granularity) with tasks, min/max estimates, status, dependencies, assignees. Focused on thin vertical slice first.
**Result:** Gave management confidence; delivered on time.

### Scenario Generation

#### AID-26-Scenario-Fidelity-Gap
**Situation:** Existing system used hand-created scenarios with parameter fuzzing, but wasn't responsive to AV behavior. Analysis showed significant percentage of millions of scenarios weren't providing value.
**Task:** Fix scenario quality.
**Action:** Identified the fundamental problem.
**Result:** Clear direction for scenario generation work.

#### AID-27-Mathematical-Framework-Hands-On
**Situation:** We needed a principled approach to scenario generation.
**Task:** Build a better system.
**Action:** I used my math background to develop a framework viewing scenarios as variable dimensions that expand as actors are added.
**Result:** Patent for scenario generation; extended existing Argo generators.

### Bridge to Systems Engineering

#### AID-28-Reading-Bridge-Role-Hands-On-star-customer-obsession
**Situation:** Testing safety-critical systems is a hard problem across industries.
**Task:** Learn from other domains.
**Action:** I read extensively about statistics and testing—not just AV, but nuclear reactors, aviation, medical devices. These industries have decades of experience proving safety when you can't test every scenario.
**Result:** Became bridge between developers and systems engineering. This research informed my fidelity and coverage approach.

#### AID-29-Systems-Engineering-Invitation
**Situation:** I was working on real-world testing for trajectory control.
**Task:** Contribute to safety validation.
**Action:** I was asked to travel to US to meet test engineering and systems engineering leads. I was invited to join systems engineering to work on the testing plan proving vehicle safety.
**Result:** Recognition of my contribution to this problem.

---

## Career Arc: Proving AV Safety Through Virtual Testing

#### META-1-Consistent-Focus-on-an-Unsolved-Problem

Looking across my career, there's a consistent thread: proving autonomous vehicles are safe through virtual testing—at increasing sophistication.

| Phase | Company | Focus | What I Solved |
|-------|---------|-------|---------------|
| 1 | AID | Built simulators from scratch | AV developers need to test without real roads—created traffic sim, viz sim, runtime, scenario eval |
| 2 | Argo | Simulation fidelity + noise modeling | Tests must reflect reality—built modeling library with realistic noise injection |
| 3 | Argo | Scenario generation (patent) | Coverage must be meaningful—developed mathematical framework for scenario dimensions |
| 4 | Argo | Bridge to systems engineering | Testing plan must prove safety—invited to join sys eng team for safety validation |
| 5 | FERNRIDE | HZM + DFD for TUV SUD certification | Regulators need provable safety case—designed approach that achieved certification |

This problem—how do you prove an autonomous vehicle is safe enough for public roads—remains an open challenge in the industry. I've approached it from multiple angles: building infrastructure, improving realism, generating meaningful scenarios, connecting to formal safety cases. I also studied how other industries solve similar problems—nuclear reactors, aviation, medical devices—domains with decades of experience proving safety when exhaustive testing is impossible.

---

## Appendix: Job Transitions

| Transition | External Factor |
|------------|-----------------|
| AID &rarr; Argo | AID acquired by Argo |
| Argo &rarr; FERNRIDE | Argo shut down (Ford/VW dispute) |
| FERNRIDE &rarr; Irreducible | headhunted by Irreducible |
| Irreducible &rarr; Present | Irreducible closed after pivot |

---

<!-- Hover functionality script (prepared for stage 2) -->
<script src="{{ '/assets/js/portfolio-leadership-hover.js' | relative_url }}"></script>
