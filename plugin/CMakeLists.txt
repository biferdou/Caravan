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
    "${RESOURCES_DIR}/images/background.png"
    "${RESOURCES_DIR}/images/compass.png"
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