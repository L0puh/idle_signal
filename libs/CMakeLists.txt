
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

add_subdirectory(assimp)
add_subdirectory(fmt)
add_subdirectory(freetype)
add_subdirectory(glfw)
add_subdirectory(openal-soft)
add_subdirectory(bullet3)
add_subdirectory(glad)
add_subdirectory(spdlog)
add_subdirectory(imgui)
add_subdirectory(libsndfile)
