
# COMPILE
include_directories("include")

file(GLOB SOURCE_FILES "src/*.cpp")
add_executable(raytracer "${SOURCE_FILES}")