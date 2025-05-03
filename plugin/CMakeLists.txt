cmake_minimum_required(VERSION 3.30)

project(CaravanPlugin VERSION 0.1.0)

juce_add_plugin(Caravan
    COMPANY_NAME "KradKradLabs"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE KKLS
    PLUGIN_CODE CRVN
    FORMATS VST3
    COPY_PLUGIN_AFTER_BUILD FALSE
    PRODUCT_NAME "Caravan"
)

# Create source groups to match our directory structure
# These paths are relative to the plugin directory
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/caravan/source")
set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/caravan/resources")

# Core module
set(CORE_FILES
    "${SOURCE_DIR}/core/AudioParameters.cpp"
    "${SOURCE_DIR}/core/AudioParameters.h"
    "${SOURCE_DIR}/core/CaravanProcessor.cpp"
    "${SOURCE_DIR}/core/CaravanProcessor.h"
)

# Effects module
set(EFFECTS_FILES
    "${SOURCE_DIR}/effects/DustDriveSaturator.cpp"
    "${SOURCE_DIR}/effects/DustDriveSaturator.h"
    "${SOURCE_DIR}/effects/StereoWidthProcessor.cpp"
    "${SOURCE_DIR}/effects/StereoWidthProcessor.h"
    "${SOURCE_DIR}/effects/VocalEQ.cpp"
    "${SOURCE_DIR}/effects/VocalEQ.h"
)

# Presets module
set(PRESETS_FILES
    "${SOURCE_DIR}/presets/PresetManager.cpp"
    "${SOURCE_DIR}/presets/PresetManager.h"
)

# UI module
set(UI_COMPONENTS_FILES
    "${SOURCE_DIR}/ui/components/CaravanLookAndFeel.cpp"
    "${SOURCE_DIR}/ui/components/CaravanLookAndFeel.h"
    "${SOURCE_DIR}/ui/components/CaravanFontManager.h"
)

set(UI_EDITOR_FILES
    "${SOURCE_DIR}/ui/editor/CaravanEditor.cpp"
    "${SOURCE_DIR}/ui/editor/CaravanEditor.h"
)

# OpenGL module
set(UI_OPENGL_FILES
    "${SOURCE_DIR}/ui/opengl/CaravanOpenGLContext.h"
)

# Add all source files to the target
target_sources(Caravan
    PRIVATE
        ${CORE_FILES}
        ${EFFECTS_FILES}
        ${PRESETS_FILES}
        ${UI_COMPONENTS_FILES}
        ${UI_EDITOR_FILES}
        ${UI_OPENGL_FILES}
)

# Set up source groups for better organization in IDEs
source_group("Core" FILES ${CORE_FILES})
source_group("Effects" FILES ${EFFECTS_FILES})
source_group("Presets" FILES ${PRESETS_FILES})
source_group("UI/Components" FILES ${UI_COMPONENTS_FILES})
source_group("UI/Editor" FILES ${UI_EDITOR_FILES})
source_group("UI/OpenGL" FILES ${UI_OPENGL_FILES})

# Include directories
target_include_directories(Caravan
    PRIVATE
        ${SOURCE_DIR}
        ${RESOURCES_DIR}
)

# Bundle resources with the plugin
set(IMAGE_FILES 
    "${RESOURCES_DIR}/images/knob.png"
    "${RESOURCES_DIR}/images/background.png"
    "${RESOURCES_DIR}/images/compass.png"
    "${RESOURCES_DIR}/images/dunes.png"
    "${RESOURCES_DIR}/images/mirage.png"
    "${RESOURCES_DIR}/images/nomad.png"
    "${RESOURCES_DIR}/images/oasis.png"
    "${RESOURCES_DIR}/images/pristine.png"
    "${RESOURCES_DIR}/images/sahara.png"
    "${RESOURCES_DIR}/images/sandstorm.png"
    "${RESOURCES_DIR}/images/sunset.png"
)

# Add font resources
set(FONT_FILES
    "${RESOURCES_DIR}/fonts/changa.ttf"
)

# Bundle the image resources
juce_add_binary_data(CaravanResources
    SOURCES 
        ${IMAGE_FILES}
        ${FONT_FILES}
)

# Link with resources
target_link_libraries(Caravan PRIVATE CaravanResources)

# Enable module source grouping for JUCE modules
set(JUCE_ENABLE_MODULE_SOURCE_GROUPING ON)

# Link JUCE modules
target_link_libraries(Caravan 
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_opengl
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Various compile definitions
target_compile_definitions(Caravan 
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

# MSVC-specific definitions
if(MSVC)
    target_compile_definitions(Caravan 
        PRIVATE 
            _CRT_SECURE_NO_WARNINGS
    )
    
    # Add debug information when in Debug configuration
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
endif()

# Installer Configuration
include(InstallRequiredSystemLibraries)

# Set path to LICENSE file
set(LICENSE_FILE "${CMAKE_SOURCE_DIR}/LICENSE.txt")

# Set package metadata
set(CPACK_PACKAGE_NAME "Caravan")
set(CPACK_PACKAGE_VENDOR "KradKradLabs")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Caravan - Vocal processing plugin")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_RESOURCE_FILE_LICENSE "${LICENSE_FILE}")
set(CPACK_PACKAGE_ICON "${PACKAGE_ICON}")

# Platform-specific settings
if(WIN32)
    # Windows/NSIS specific settings
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_PACKAGE_NAME "Caravan")
    set(CPACK_NSIS_DISPLAY_NAME "Caravan VST3")
    set(CPACK_NSIS_CONTACT "void@kradkrad.com")
    set(CPACK_NSIS_MODIFY_PATH OFF)
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_MUI_FINISHPAGE_RUN "Caravan.vst3")
    
    # Windows paths for VST3 plugins
    set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64\\\\Common Files\\\\VST3")
    set(VST3_INSTALL_DIR "")
    
# macOS-specific settings
elseif(APPLE)
    # macOS bundle settings
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "Caravan")
    set(CPACK_DMG_FORMAT "UDZO")
    set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_SOURCE_DIR}/plugin/caravan/resources/images/background.png")
    set(CPACK_SYSTEM_NAME "macOS")
    
    # The macOS install path
    set(VST3_INSTALL_DIR "Library/Audio/Plug-Ins/VST3")
    
    # Additional macOS-specific properties
    set(CPACK_BUNDLE_NAME "Caravan")
    set(CPACK_BUNDLE_PLIST "${CMAKE_SOURCE_DIR}/plugin/Info.plist")

# Linux-specific settings
elseif(UNIX AND NOT APPLE)
    # Linux settings
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "KradKradLabs <void@kradkrad.com>")
    set(CPACK_DEBIAN_PACKAGE_SECTION "sound")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libasound2, libjack0")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Caravan - Vocal processing plugin for music production")
    
    # The Linux install path
    set(VST3_INSTALL_DIR "usr/local/lib/vst3")
endif()

# Ensure proper file installation for all platforms
install(
    DIRECTORY "${CMAKE_BINARY_DIR}/plugin/Caravan_artefacts/$<CONFIG>/VST3/Caravan.vst3/"
    DESTINATION "Caravan.vst3"
    COMPONENT VST3
)

# Define components
set(CPACK_COMPONENTS_ALL VST3)
set(CPACK_COMPONENT_VST3_DISPLAY_NAME "Caravan VST3 Plugin")
set(CPACK_COMPONENT_VST3_DESCRIPTION "VST3 version of the Caravan plugin")
set(CPACK_COMPONENT_VST3_REQUIRED TRUE)

# Include CPack at the end
include(CPack)