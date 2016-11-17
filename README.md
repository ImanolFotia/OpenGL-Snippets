# Epsilon-Snippets

Modules of the Epsilon Engine (C).

These modules are distributed under the GPL License v3, a copy of the license can be found on the root directory.
The game engine (Epsilon Engine (C)) in which these modules are implemented is Proprietary Software.

Compile the modules using Code::Blocks solution.

Content:

-Audio loader using OpenAL.

-Quake 3 BSP renderer.

-GUI library (In progress).

-Sprite renderer using texture atlases.


Dependencies:

GLFW3

GLEW

SOIL

Compiling:

On Linux add the following string to the linker:

-lGLEW -lGL -lglfw3 -lX11 -lXxf86vm -lpthread -lXrandr -lXi -lXcursor -lXinerama

On Windows be sure that you link to the needed dependencies located on your system.

Survtech Games(C)
Epsilon Engine(C)

