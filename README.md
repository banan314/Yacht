# Yacht
We are students working on an OpenGL project.

#Adding a glut DLL reference
I would suggest focusing on freeglut library (freeglut.lib, glew32.lib).
Things you can try include: NuGet freeglut package, adding new include/library directories to VC project, pasting lib and dll files into the main directory, cleaning Debug directory from unnecessary files.

#Adding color
color-go-away-when-i-enable-lighting-in-opengl -> use material.

#Adding additional libraries
To add freeglut or AntTweakBar, go to *Manage NuGet extensions* and install the extension. Afterwards, you might have to restart Visual Studio.

[1] https://msdn.microsoft.com/en-us/library/ee855621.aspx
[2] https://www.youtube.com/watch?v=8p76pJsUP44
[3] http://stackoverflow.com/questions/8494942/why-does-my-color-go-away-when-i-enable-lighting-in-opengl
[4] Woo, Mason; Neider, Jackie; Davis, Tom OpenGL Programming Guide. The Official Guide to Learning OpenGL, Version 1.1. 2nd Edition
