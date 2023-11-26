# Defense 3D

Open `DefenseProject.sln` in Visual Studio 2022 with the Desktop development
with C++ workload, MSVC v143 and a Windows 10/11 SDK installed.
Set Defense as the startup project and select Debug or Release, Win32.

Build and run with F5. The build copies the bundled EGL/GLES DLLs to the
output directory automatically. The debugger runs from that directory so
the game can find `../Resources`.

The bundled EGL runtime initializes OpenGL while its DLL is loading. To avoid
a startup deadlock with NVIDIA drivers, the application initializes the OpenGL
driver first and delay-loads the EGL/GLES DLLs. Keep both parts of this workaround.
