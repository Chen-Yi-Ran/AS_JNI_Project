# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1) #最低支持的CMake版本
 #[[project("as_jni_project")]]
# 第一步：导入头文件
include_directories("inc")

add_library(native-lib  # 库的名字
           SHARED      # 动态库.so
           native-lib.cpp  # 源文件地址)
)

# 第二步：设置库文件路径，设置CMAKE_CXX_FLAGS环境变量（库的路径）
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")




# TODO 第三步 链接具体的库 到 libnative-lib.so
# 链接具体的库
target_link_libraries(
        native-lib
        fmod  # 具体的一定要链接到 native-lib里面去
        fmodL # 具体的一定要链接到 native-lib里面去
        log # 如果没有写，在native-lip.cpp使用log时，会报错：找不到 liblog.so 库
)