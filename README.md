# Introduction

Welcome this project was used to introduce myself in the graphics programming world. I decided to start with DirectX 11 but I have done one a [Vulkan project](https://github.com/Woestijnbok/Vulkan-Tutorial) as well.
Since this was my introduction, I mainly fondled with same basic phong and transparent shaders. I also tested the use of normal maps and the different sampler filters. 
I tried to understand the graphics pipeline and DirectX 11 api as much as possible.

<div align="center">
  <img src="https://github.com/Woestijnbok/DirectX-11/blob/main/Screenshots/Vehicle.jpg" width="400" height="auto">
  <img src="https://github.com/Woestijnbok/DirectX-11/blob/main/Screenshots/Fire.jpg" width="400" height="auto">
</div>

# Controls

- **Camera**: The camera can be moved forward via **WS**, up with **QE** and to the right using **AD** (Unreal Engine Controls).
- **General**: To disable the transparent fire press **3** and to stop the vehicle from rotating use **R**.
- **Other**: You can switch between sampler filters with **1** and toggle the normal map via **2**.

# Sampler Filter

Like I told in the introduction, I tested different sampler filters. 
Below you can see the different sampler filters starting from left to right: Point, Linear and lastly Anisotropic.

<div align="center">
  <img src="https://github.com/Woestijnbok/DirectX-11/blob/main/Screenshots/PointSampler.jpg" width="250" height="auto">
  <img src="https://github.com/Woestijnbok/DirectX-11/blob/main/Screenshots/LinearSampler.jpg" width="250" height="auto">
  <img src="https://github.com/Woestijnbok/DirectX-11/blob/main/Screenshots/AnisotropicSampler.jpg" width="250" height="auto">
</div>

# Normals

I also played with normal maps the first image showcases the vehicle with a normal map and one directional light the other is without the normal map.

<div align="center">
  <img src="https://github.com/Woestijnbok/DirectX-11/blob/main/Screenshots/WithNormal.jpg" width="400" height="auto">
  <img src="https://github.com/Woestijnbok/DirectX-11/blob/main/Screenshots/WithoutNormal.jpg" width="400" height="auto">
</div>

# Final Thoughts

This project was a very interesting and challenging experience but totally worth it. 
I feel like graphics programming has a steep beginner curve but once you are over the bump it's a very interesting and fun topic.
I hope this project can be used as a resource or example for other's people introduction into graphics programming and DirectX 11.
