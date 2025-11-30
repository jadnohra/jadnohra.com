---
layout: post
title: "Game AI: From Tic-Tac-Toe to Navigation Meshes"
date: 2013-06-10
categories: [AI, GameDev]
tags: [ai, game-development, pathfinding, reinforcement-learning, minimax, navigation]
---

A collection of game AI experiments exploring different paradigms - from classic minimax to reinforcement learning to navigation mesh generation.

## Tic-Tac-Toe AI Testbed

What started as a 4-hour coding interview evolved into a comparison of three AI paradigms all playing perfect Tic-Tac-Toe:

### Three Approaches, One Game

1. **Rule-Based Perfect Play** - Hand-coded strategy exploiting Tic-Tac-Toe's solved nature
2. **Alpha-Beta Minimax** - Classic game tree search with pruning
3. **Reinforcement Learning** - Value iteration learning optimal play from scratch

The testbed allows AI vs AI matches to verify all three achieve perfect play, plus Human vs AI mode.

```
Current AIs:
  * Rule based perfect play (solved game)
  * Alpha-beta minimax (Steve Chapel's code)
  * Reinforcement-Learning value iteration
```

### Key Insight

The RL approach is overkill for Tic-Tac-Toe but demonstrates how value iteration converges to optimal policy - useful foundation for more complex games.

**Source**: [TicTacToe AI](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/AI/TicTacToe)

---

## Obstacle Avoidance

{% include embed/youtube.html id='RZHIchPK1TE' %}

Real-time obstacle avoidance demonstration - agents navigating dynamic environments while avoiding collisions.

---

## RecastMOD: Navigation Mesh System

Modified version of Mikko Mononen's Recast & Detour toolkit (v1.4) for automatic navigation mesh generation.

### Features

- **Voxel-based processing** - Convert arbitrary level geometry to walkable surfaces
- **Tiled mesh support** - Stream navigation data for large worlds
- **Detour pathfinding** - Efficient runtime path queries
- **Demo application** - Interactive visualization and testing

### Use Cases

- Game level walkability analysis
- NPC pathfinding in complex environments
- Procedural navigation for generated content

**Source**: [RecastMOD](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/AI/RecastMOD)
