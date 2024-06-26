# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw EXCLUDE_FROM_ALL)

# glad
add_subdirectory(glad)

# spdlog
add_subdirectory(spdlog)

# glm
add_subdirectory(glm)

# stb_image
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb)

# noise
add_library(FastNoiseLite INTERFACE)
target_include_directories(FastNoiseLite INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/FastNoiseLite/Cpp)

# spline
add_library(spline INTERFACE)
target_include_directories(spline INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/spline/src)

# ImGui
set(IMGUI_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

list(APPEND IMGUI_PRIVATE_HEADERS
		"${IMGUI_SRC_DIR}/imgui_internal.h"
		"${IMGUI_SRC_DIR}/imconfig.h"
		"${IMGUI_SRC_DIR}/stb_rect_pack.h"
		"${IMGUI_SRC_DIR}/stb_textedit.h"
		"${IMGUI_SRC_DIR}/stb_truetype.h"
		"${IMGUI_SRC_DIR}/misc/cpp/imgui_stdlib.h"
		)

list(APPEND IMGUI_SOURCES
		"${IMGUI_SRC_DIR}/imgui.cpp"
		"${IMGUI_SRC_DIR}/imgui_draw.cpp"
		"${IMGUI_SRC_DIR}/imgui_demo.cpp"
		"${IMGUI_SRC_DIR}/imgui_widgets.cpp"
		"${IMGUI_SRC_DIR}/imgui_tables.cpp"
		"${IMGUI_SRC_DIR}/misc/cpp/imgui_stdlib.cpp"
		)

list(APPEND IMGUI_PUBLIC_INCLUDE_DIRS
		${IMGUI_SRC_DIR})

list(APPEND IMGUI_PUBLIC_LIBRARIES)

set(NAME imgui)
add_library(${NAME} STATIC ${IMGUI_SOURCES})
target_link_libraries(${NAME} PUBLIC ${IMGUI_PUBLIC_LIBRARIES})
target_include_directories(${NAME}
		PUBLIC ${IMGUI_PUBLIC_INCLUDE_DIRS}
		PRIVATE ${IMGUI_PRIVATE_INCLUDE_DIRS})