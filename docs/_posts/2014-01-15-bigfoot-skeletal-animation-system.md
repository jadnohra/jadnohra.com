---
layout: post
title: "Bigfoot: A Skeletal Animation and Locomotion System"
date: 2014-01-15
categories: [Animation, GameDev]
tags: [animation, skeletal, locomotion, motion-capture, physics, c++]
---

Bigfoot is a skeletal animation system I developed for exploring motion capture processing, skeletal physics, and procedural locomotion. The project combines real-time visualization with advanced animation techniques.

## Features

- **BVH Motion Capture Loading** - Parse and visualize standard motion capture data
- **Skeletal Physics Model** - Physics-based bone simulation with constraints
- **Locomotion Planning** - Footstep-based, skate-free walking animation
- **Motion Retargeting** - Transfer animations between different skeletal rigs

## Demo Videos

### Animation Testbed

{% include embed/youtube.html id='RZHIchPK1TE' %}

The Bigfoot viewer showing skeletal animation with motion trails, physics visualization, and real-time playback controls.

### Skeleton Semantics and Footplant

{% include embed/youtube.html id='XMj0xhCzx4U' %}

Work-in-progress on skeleton semantics and footplant detection - key for preventing foot skating in procedural animation.

## Locomotion Algorithm

The locomotion planner generates footstep sequences that respect physical constraints:

```cpp
bool execute(const LocomoWorld& inWorld, const LocomoBody& inBody,
             const LocomoState& inState, const WalkParams& inParams,
             LocomoState& outState, float& outDuration)
{
    const FootState& support = (inState.support & ELFlag) ? inState.l : inState.r;

    glm::vec2 world_fwd = glm::rotate(inWorld.fwd, support.dir);
    glm::vec2 world_side = (inState.support & ELFlag) ? world_right : -world_right;

    // Calculate stride geometry
    float dist_fwd = sqrtf(inParams.stride * inParams.stride -
                           inParams.side * inParams.side);

    glm::vec2 pos_end = pos_mid_flight + (world_side * dist_right) +
                        (world_fwd * dist_fwd);

    // Update foot state
    next_support.pos = pos_end;
    next_support.dir = support.dir + inParams.turn;

    outDuration = glm::distance(other.pos, pos_end) / inParams.speed;
    return true;
}
```

## Bigeye UI Framework

The Bigfoot viewer uses **Bigeye**, a custom retained-mode UI framework I built for rapid prototyping of visualization tools. Features include:

- Widget-based architecture with scene transforms
- OpenGL render tree builder
- Real-time update/render pipeline
- MagickWand integration for texture processing

The orange button panels you see in the videos (Branches, Semantics, Physics, Velocities, Trails, C.O.M, Joints) are all rendered through Bigeye.

## Technical Stack

- **Language**: C++ with OpenGL
- **UI Framework**: Bigeye (custom)
- **Math**: GLM for vector/matrix operations
- **Input**: BVH motion capture format
- **Visualization**: Real-time 3D rendering with debug overlays

## Source Code

- [Bigfoot Animation System](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Bigfoot)
- [Bigeye UI Framework](https://github.com/jadnohra/jad-pre-2015-dabblings/tree/master/Labs/Bigeye)
