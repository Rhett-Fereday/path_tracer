# path_tracer

This is a simple path tracer using modern C++ and a basic data-oriented approach.

It's WIP, but current supported features are:

- Simple Monte-Carlo path tracing (no next event estimation or russian roulette)
- Multiple renderable shapes - sphere, plane, box
- Simple diffuse/emissive materials
- Post processes (Modified Reinhard tone-mapping, gamma correction)

Example image(s):
The test scene is a modified version of the Cornell Box.

![image](https://github.com/Rhett-Fereday/path_tracer/blob/post_processes/example_images/raw_output.png)
Raw image (no post processing, other than clamping to rgb space)


![image](https://github.com/Rhett-Fereday/path_tracer/blob/post_processes/example_images/gamma_corrected.png)
Gamma corrected image (only gamma correction and clamping)

![image](https://github.com/Rhett-Fereday/path_tracer/blob/post_processes/example_images/tone_mapped.png)
Tone-mapped image (Modified Reinhard tone-mapping followed by gamma correction and clamping)
