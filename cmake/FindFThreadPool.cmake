set(PackageName FThreadPool)

set(${PackageName}_FOUND TRUE)
message(STATUS "已找到Find${PackageName}.cmake文件！")

if (NOT DEFINED ${PackageName}_ROOT)
    set(${PackageName}_ROOT ${PROJECT_SOURCE_DIR}/${PackageName})
else ()
    # 强制转换为绝对路径
    file(REAL_PATH ${${PackageName}_ROOT} ${PackageName}_ROOT)
#    message(STATUS "${PackageName}_ROOT == ${${PackageName}_ROOT}")
endif ()

# 根据构建类型配置库路径
set(${PackageName}_INCLUDE_DIR ${${PackageName}_ROOT}/include)
set(${PackageName}_LIBRARY_DIR ${${PackageName}_ROOT}/lib)
set(${PackageName}_RUNTIME_DIR ${${PackageName}_ROOT}/bin)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(${PackageName}_LIBRARY_DIR ${${PackageName}_LIBRARY_DIR}/debug)
    set(${PackageName}_RUNTIME_DIR ${${PackageName}_RUNTIME_DIR}/debug)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(${PackageName}_LIBRARY_DIR ${${PackageName}_LIBRARY_DIR}/release)
    set(${PackageName}_RUNTIME_DIR ${${PackageName}_RUNTIME_DIR}/release)
else ()
    message(FATAL_ERROR "没有对应的版本适配当前的构建类型...")
endif ()

# 添加库的依赖项
set(${PackageName}_DEPENDENT_LIBRARIES)

find_package(unofficial-godot-cpp CONFIG REQUIRED)
list(APPEND ${PackageName}_DEPENDENT_LIBRARIES unofficial::godot::cpp)

# 为用户配置库的包含目录、库目录以及依赖项（如果有）
# 动态库
add_library(${PackageName}::${PackageName} SHARED IMPORTED)
set_target_properties(
        ${PackageName}::${PackageName} PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${${PackageName}_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${${PackageName}_DEPENDENT_LIBRARIES}"
        IMPORTED_LOCATION "${${PackageName}_RUNTIME_DIR}/${PackageName}.dll"
        IMPORTED_IMPLIB "${${PackageName}_LIBRARY_DIR}/${PackageName}.lib"
)

# 静态库
add_library(${PackageName}::${PackageName}-static STATIC IMPORTED)
set_target_properties(
        ${PackageName}::${PackageName}-static PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${${PackageName}_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${${PackageName}_DEPENDENT_LIBRARIES}"
        IMPORTED_LOCATION "${${PackageName}_LIBRARY_DIR}/${PackageName}-static.lib"
)

# 告知用户库的使用方式
message(STATUS
        "请添加：\n
        find_package(${PackageName} REQUIRED)

        动态库：
        target_link_libraries(\${PROJECT_NAME} PRIVATE ${PackageName}::${PackageName})

        静态库：
        target_link_libraries(\${PROJECT_NAME} PRIVATE ${PackageName}::${PackageName}-static)
        \n以使用库"
)
