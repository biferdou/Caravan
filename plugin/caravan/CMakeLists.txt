cmake_minimum_required(VERSION 3.30)

project(Caravan VERSION 0.1.0)

juce_add_plugin (${PROJECT_NAME}
    COMPANY_NAME "KradKradLabs"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE KKLS
    PLUGIN_CODE CRVN
    FORMATS VST3
    PRODUCT_NAME "Caravan"
)

# Define source files
target_sources(${PROJECT_NAME}
    PRIVATE
        source/core/AudioParameters.cpp
        source/core/CaravanProcessor.cpp
        source/effects/DustDriveSaturator.cpp
        source/effects/StereoWidthProcessor.cpp
        source/effects/VocalEQ.cpp
        source/presets/PresetManager.cpp
        source/ui/components/CaravanLookAndFeel.cpp
        source/ui/editor/CaravanEditor.cpp
)

# Include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
        source
)

set(JUCE_ENABLE_MODULE_SOURCE_GROUPING ON)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

target_compile_definitions(${PROJECT_NAME} 
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

if(MSVC)
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        _CRT_SECURE_NO_WARNINGS
)
endif()