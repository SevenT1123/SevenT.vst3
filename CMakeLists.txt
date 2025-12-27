cmake_minimum_required(VERSION 3.15)

# Project name and version
project(SevenT VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE subdirectory
# You'll need to either:
# 1. Place JUCE in a "JUCE" subdirectory, or
# 2. Set JUCE_DIR to point to your JUCE installation
# Example: cmake -DJUCE_DIR=/path/to/JUCE ..
if(NOT DEFINED JUCE_DIR)
    set(JUCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/JUCE" CACHE PATH "Path to JUCE framework")
endif()

if(EXISTS "${JUCE_DIR}/CMakeLists.txt")
    add_subdirectory(${JUCE_DIR} JUCE)
else()
    message(FATAL_ERROR "JUCE not found at ${JUCE_DIR}. Please set JUCE_DIR to your JUCE installation path.")
endif()

# Define the plugin
juce_add_plugin(SevenT
    # Basic plugin settings
    COMPANY_NAME "SevenT"
    PLUGIN_MANUFACTURER_CODE Kh04
    PLUGIN_CODE Tr4n
    
    # Plugin formats to build
    FORMATS VST3 Standalone
    
    # Plugin characteristics
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    
    # Copy to plugin folder after build (optional)
    COPY_PLUGIN_AFTER_BUILD FALSE
    
    # VST3 settings
    VST3_CATEGORIES Instrument Synth
    
    # Plugin name
    PRODUCT_NAME "SevenT"
)

# Source files
target_sources(SevenT
    PRIVATE
        # Main plugin files
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h
        
        # Synth core
        Source/SynthVoice.cpp
        Source/SynthVoice.h
        Source/SynthSound.h
        
        # Data classes
        Source/Data/ADSRData.cpp
        Source/Data/ADSRData.h
        Source/Data/FilterData.cpp
        Source/Data/FilterData.h
        Source/Data/OSCData.cpp
        Source/Data/OSCData.h
        Source/Data/UnisonData.cpp
        Source/Data/UnisonData.h
        
        # UI Components
        Source/UI/ADSRComponent.cpp
        Source/UI/ADSRComponent.h
        Source/UI/FilterComponent.cpp
        Source/UI/FilterComponent.h
        Source/UI/OSCComponent.cpp
        Source/UI/OSCComponent.h
)

# Preprocessor definitions
target_compile_definitions(SevenT
    PUBLIC
        # JUCE app config
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        
        # Disable warnings
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
)

# Link JUCE modules
target_link_libraries(SevenT
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Include directories
target_include_directories(SevenT
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Platform-specific settings
if(APPLE)
    # macOS specific settings
    set_target_properties(SevenT PROPERTIES
        BUNDLE TRUE
        BUNDLE_EXTENSION "vst3"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.SevenT.SevenT"
        MACOSX_BUNDLE_BUNDLE_NAME "SevenT"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
    )
elseif(WIN32)
    # Windows specific settings
    target_compile_definitions(SevenT PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Build configuration messages
message(STATUS "")
message(STATUS "SevenT VST3 Synthesizer Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  JUCE Directory: ${JUCE_DIR}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "")
