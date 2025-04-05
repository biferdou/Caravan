#pragma once
#include <juce_opengl/juce_opengl.h>

namespace Caravan
{
    // Simple OpenGL context for the Caravan plugin
    class CaravanOpenGLContext : public juce::OpenGLContext
    {
    public:
        CaravanOpenGLContext() = default;
        ~CaravanOpenGLContext() = default;

        // No implementation needed for now
        void updateShaderUniforms(float dustDrive, float width, float air)
        {
            // Will be implemented later if needed
            juce::ignoreUnused(dustDrive, width, air);
        }
    };
}