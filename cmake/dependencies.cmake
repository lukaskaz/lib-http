cmake_minimum_required(VERSION 3.10)

include(ExternalProject)

set(source_dir "${CMAKE_BINARY_DIR}/libnlohmann-src")
set(build_dir "${CMAKE_BINARY_DIR}/libnlohmann-build")

EXTERNALPROJECT_ADD(
  libnlohmann
  GIT_REPOSITORY    https://github.com/nlohmann/json.git
  GIT_TAG           v3.11.3
  PATCH_COMMAND     ${patching_cmd}
  PREFIX            libnlohmann-workspace
  SOURCE_DIR        ${source_dir}
  BINARY_DIR        ${build_dir}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  UPDATE_COMMAND    ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)

include_directories(${source_dir}/include)
link_directories(${build_dir}/build)

set(source_dir "${CMAKE_BINARY_DIR}/libcpr-src")
set(build_dir "${CMAKE_BINARY_DIR}/libcpr-build")

EXTERNALPROJECT_ADD(
  libcpr
  GIT_REPOSITORY    https://github.com/libcpr/cpr.git
  GIT_TAG           1.10.5
  PATCH_COMMAND     ${patching_cmd}
  PREFIX            libcpr-workspace
  SOURCE_DIR        ${source_dir}
  BINARY_DIR        ${build_dir}
  CONFIGURE_COMMAND mkdir /${build_dir}/build &> /dev/null
  BUILD_COMMAND     cd ${build_dir}/build && cmake -D CPR_USE_SYSTEM_CURL=ON
                    ${source_dir} && make
  UPDATE_COMMAND    ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)

include_directories(${source_dir}/include)
include_directories(${build_dir}/build/cpr_generated_includes)
link_directories(${build_dir}/build/lib)
