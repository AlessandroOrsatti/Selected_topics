# Install script for directory: /Users/alessandroorsatti/Documents/JUCE

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

set(CMAKE_BINARY_DIR "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools")

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/modules/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/extras/Build/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-7.0.5" TYPE FILE FILES
    "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/JUCEConfigVersion.cmake"
    "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/JUCEConfig.cmake"
    "/Users/alessandroorsatti/Documents/JUCE/extras/Build/CMake/JUCECheckAtomic.cmake"
    "/Users/alessandroorsatti/Documents/JUCE/extras/Build/CMake/JUCEHelperTargets.cmake"
    "/Users/alessandroorsatti/Documents/JUCE/extras/Build/CMake/JUCEModuleSupport.cmake"
    "/Users/alessandroorsatti/Documents/JUCE/extras/Build/CMake/JUCEUtils.cmake"
    "/Users/alessandroorsatti/Documents/JUCE/extras/Build/CMake/JuceLV2Defines.h.in"
    "/Users/alessandroorsatti/Documents/JUCE/extras/Build/CMake/LaunchScreen.storyboard"
    "/Users/alessandroorsatti/Documents/JUCE/extras/Build/CMake/PIPAudioProcessor.cpp.in"
    "/Users/alessandroorsatti/Documents/JUCE/extras/Build/CMake/PIPAudioProcessorWithARA.cpp.in"
    "/Users/alessandroorsatti/Documents/JUCE/extras/Build/CMake/PIPComponent.cpp.in"
    "/Users/alessandroorsatti/Documents/JUCE/extras/Build/CMake/PIPConsole.cpp.in"
    "/Users/alessandroorsatti/Documents/JUCE/extras/Build/CMake/RecentFilesMenuTemplate.nib"
    "/Users/alessandroorsatti/Documents/JUCE/extras/Build/CMake/UnityPluginGUIScript.cs.in"
    "/Users/alessandroorsatti/Documents/JUCE/extras/Build/CMake/checkBundleSigning.cmake"
    "/Users/alessandroorsatti/Documents/JUCE/extras/Build/CMake/copyDir.cmake"
    "/Users/alessandroorsatti/Documents/JUCE/extras/Build/CMake/juce_runtime_arch_detection.cpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5" TYPE EXECUTABLE FILES "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/Debug/juce_lv2_helper")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5/juce_lv2_helper" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5/juce_lv2_helper")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5/juce_lv2_helper")
      endif()
    endif()
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5" TYPE EXECUTABLE FILES "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/Release/juce_lv2_helper")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5/juce_lv2_helper" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5/juce_lv2_helper")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5/juce_lv2_helper")
      endif()
    endif()
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5" TYPE EXECUTABLE FILES "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/MinSizeRel/juce_lv2_helper")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5/juce_lv2_helper" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5/juce_lv2_helper")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5/juce_lv2_helper")
      endif()
    endif()
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5" TYPE EXECUTABLE FILES "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/RelWithDebInfo/juce_lv2_helper")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5/juce_lv2_helper" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5/juce_lv2_helper")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/JUCE-7.0.5/juce_lv2_helper")
      endif()
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-7.0.5/LV2_HELPER.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-7.0.5/LV2_HELPER.cmake"
         "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/CMakeFiles/Export/0a8757d299ae06c55806401f6461d0d1/LV2_HELPER.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-7.0.5/LV2_HELPER-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-7.0.5/LV2_HELPER.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-7.0.5" TYPE FILE FILES "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/CMakeFiles/Export/0a8757d299ae06c55806401f6461d0d1/LV2_HELPER.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-7.0.5" TYPE FILE FILES "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/CMakeFiles/Export/0a8757d299ae06c55806401f6461d0d1/LV2_HELPER-debug.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-7.0.5" TYPE FILE FILES "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/CMakeFiles/Export/0a8757d299ae06c55806401f6461d0d1/LV2_HELPER-minsizerel.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-7.0.5" TYPE FILE FILES "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/CMakeFiles/Export/0a8757d299ae06c55806401f6461d0d1/LV2_HELPER-relwithdebinfo.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-7.0.5" TYPE FILE FILES "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/CMakeFiles/Export/0a8757d299ae06c55806401f6461d0d1/LV2_HELPER-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/alessandroorsatti/Documents/GitHub/Selected_topics/markov_Photographer/build/JUCE/tools/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
