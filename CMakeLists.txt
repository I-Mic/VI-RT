cmake_minimum_required(VERSION 3.9)

project(Renderer)
add_subdirectory(Scene/tinyobjloader)

add_executable(renderer Camera/perspective.cpp Primitive/BRDF/Phong.cpp Primitive/Geometry/mesh.cpp Scene/scene.cpp Image/ImagePPM.cpp main.cpp utils/AABB.cpp "Primitive/primitive.cpp")
target_include_directories(renderer PRIVATE "3DSortingStruct/" "Camera/" "Image/" "Light/" "Primitive/" "Primitive/BRDF" "Primitive/Geometry" "Rays/" "Renderer/" "Scene/" "utils/" )
target_link_libraries(renderer tinyobjloader)