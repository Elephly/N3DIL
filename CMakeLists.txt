cmake_minimum_required(VERSION 2.6)

project(N3DIL)

set(PUBLIC_INCLUDE_DIR "${PROJECT_BINARY_DIR}/include")
set(CONFIG_HEADER_IN "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.h.in")
set(CONFIG_HEADER_OUT "${PUBLIC_INCLUDE_DIR}/${PROJECT_NAME}Config.h")
set(LOCAL_SUBMODULE_DIR "${PROJECT_SOURCE_DIR}/submodules")
set(LOCAL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(LOCAL_RESOURCE_DIR "${PROJECT_SOURCE_DIR}/res")
set(LOCAL_SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(LOCAL_INCLUDES
  "${CONFIG_HEADER_IN}"
  "${LOCAL_INCLUDE_DIR}/MyAnimation.h"
  "${LOCAL_INCLUDE_DIR}/MyAnimationTrack.h"
  "${LOCAL_INCLUDE_DIR}/MyApplication.h"
  "${LOCAL_INCLUDE_DIR}/MyCamera.h"
  "${LOCAL_INCLUDE_DIR}/MyCube.h"
  "${LOCAL_INCLUDE_DIR}/MyDefines.h"
  "${LOCAL_INCLUDE_DIR}/MyFileIO.h"
  "${LOCAL_INCLUDE_DIR}/MyGeometry.h"
  "${LOCAL_INCLUDE_DIR}/MyGraphics.h"
  "${LOCAL_INCLUDE_DIR}/MyGraphicsObject3D.h"
  "${LOCAL_INCLUDE_DIR}/MyImage.h"
  "${LOCAL_INCLUDE_DIR}/MyImageUtil.h"
  "${LOCAL_INCLUDE_DIR}/MyIncludes.h"
  "${LOCAL_INCLUDE_DIR}/MyIndexedVertexArray.h"
  "${LOCAL_INCLUDE_DIR}/MyInputManager.h"
  "${LOCAL_INCLUDE_DIR}/MyKeyframe.h"
  "${LOCAL_INCLUDE_DIR}/MyLightSource.h"
  "${LOCAL_INCLUDE_DIR}/MyManikin.h"
  "${LOCAL_INCLUDE_DIR}/MyMaterial.h"
  "${LOCAL_INCLUDE_DIR}/MyMath.h"
  "${LOCAL_INCLUDE_DIR}/MyMatrix.h"
  "${LOCAL_INCLUDE_DIR}/MyMeshFactory.h"
  "${LOCAL_INCLUDE_DIR}/MyObject3D.h"
  "${LOCAL_INCLUDE_DIR}/MyObjectManager.h"
  "${LOCAL_INCLUDE_DIR}/MyQuad.h"
  "${LOCAL_INCLUDE_DIR}/MyShaderManager.h"
  "${LOCAL_INCLUDE_DIR}/MyShaderProgram.h"
  "${LOCAL_INCLUDE_DIR}/MySphere.h"
  "${LOCAL_INCLUDE_DIR}/MyStringUtil.h"
  "${LOCAL_INCLUDE_DIR}/MyTexture2D.h"
  "${LOCAL_INCLUDE_DIR}/MyTriangle.h"
  "${LOCAL_INCLUDE_DIR}/MyVector.h"
  "${LOCAL_INCLUDE_DIR}/MyVertex.h"
  "${LOCAL_INCLUDE_DIR}/SOIL.h"
)
set(LOCAL_RESOURCES
  "${LOCAL_RESOURCE_DIR}/images/lenovo.PNG"
  "${LOCAL_RESOURCE_DIR}/images/sample2.png"
  "${LOCAL_RESOURCE_DIR}/images/sample.png"
  "${LOCAL_RESOURCE_DIR}/images/TuxTorso.png"
  "${LOCAL_RESOURCE_DIR}/shaders/ColorFrag.glsl"
  "${LOCAL_RESOURCE_DIR}/shaders/ColorVert.glsl"
  "${LOCAL_RESOURCE_DIR}/shaders/GouraudFrag.glsl"
  "${LOCAL_RESOURCE_DIR}/shaders/GouraudVert.glsl"
  "${LOCAL_RESOURCE_DIR}/shaders/PhongFrag.glsl"
  "${LOCAL_RESOURCE_DIR}/shaders/PhongVert.glsl"
  "${LOCAL_RESOURCE_DIR}/shaders/WhiteVert.glsl"
)
set(LOCAL_SRCS
  "${LOCAL_SRC_DIR}/main.cpp"
  "${LOCAL_SRC_DIR}/MyAnimation.cpp"
  "${LOCAL_SRC_DIR}/MyAnimationTrack.cpp"
  "${LOCAL_SRC_DIR}/MyApplication.cpp"
  "${LOCAL_SRC_DIR}/MyCamera.cpp"
  "${LOCAL_SRC_DIR}/MyCube.cpp"
  "${LOCAL_SRC_DIR}/MyFileIO.cpp"
  "${LOCAL_SRC_DIR}/MyGeometry.cpp"
  "${LOCAL_SRC_DIR}/MyGraphics.cpp"
  "${LOCAL_SRC_DIR}/MyGraphicsObject3D.cpp"
  "${LOCAL_SRC_DIR}/MyImage.cpp"
  "${LOCAL_SRC_DIR}/MyImageUtil.cpp"
  "${LOCAL_SRC_DIR}/MyIndexedVertexArray.cpp"
  "${LOCAL_SRC_DIR}/MyInputManager.cpp"
  "${LOCAL_SRC_DIR}/MyKeyframe.cpp"
  "${LOCAL_SRC_DIR}/MyLightSource.cpp"
  "${LOCAL_SRC_DIR}/MyManikin.cpp"
  "${LOCAL_SRC_DIR}/MyMaterial.cpp"
  "${LOCAL_SRC_DIR}/MyMath.cpp"
  "${LOCAL_SRC_DIR}/MyMatrix.cpp"
  "${LOCAL_SRC_DIR}/MyMeshFactory.cpp"
  "${LOCAL_SRC_DIR}/MyObject3D.cpp"
  "${LOCAL_SRC_DIR}/MyQuad.cpp"
  "${LOCAL_SRC_DIR}/MyShaderManager.cpp"
  "${LOCAL_SRC_DIR}/MyShaderProgram.cpp"
  "${LOCAL_SRC_DIR}/MySphere.cpp"
  "${LOCAL_SRC_DIR}/MyStringUtil.cpp"
  "${LOCAL_SRC_DIR}/MyTexture2D.cpp"
  "${LOCAL_SRC_DIR}/MyTriangle.cpp"
  "${LOCAL_SRC_DIR}/MyVector.cpp"
  "${LOCAL_SRC_DIR}/MyVertex.cpp"
)

# The version number.
set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 1)
set(${PROJECT_NAME}_VERSION_PATCH 0)
execute_process(COMMAND git rev-list --count HEAD WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}" OUTPUT_VARIABLE ${PROJECT_NAME}_VERSION_REVISION)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Configure a header file to pass some of the CMake settings to the source code.
configure_file(
  "${CONFIG_HEADER_IN}"
  "${CONFIG_HEADER_OUT}"
)

include_directories("${LOCAL_INCLUDE_DIR}")
include_directories("${PUBLIC_INCLUDE_DIR}")
#### include_directories("${LOCAL_SUBMODULE_DIR}/MemMon/include")
#### include_directories("${LOCAL_SUBMODULE_DIR}/SFML/include")

#### add_subdirectory(submodules/MemMon)
#### add_subdirectory(submodules/SFML)

add_executable("${PROJECT_NAME}" "${LOCAL_INCLUDES}" "${LOCAL_RESOURCES}" "${LOCAL_SRCS}")
#### target_link_libraries("${PROJECT_NAME}"
####   MemMon
####   sfml-audio
####   sfml-graphics
####   sfml-main
####   sfml-network
####   sfml-system
####   sfml-window
#### )

# Add the install targets
install (TARGETS "${PROJECT_NAME}" DESTINATION bin)
install (FILES "${CONFIG_HEADER_OUT}" DESTINATION include)