/*
  ==============================================================================

    BunnyComponent.h
    Created: 1 Jul 2021 7:58:09am
    Author:  Alex

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <fstream>

#include "Logger.h"

//==============================================================================
/*
*/
class BunnyComponent  : public juce::Component, public juce::OpenGLRenderer
{
public:
    BunnyComponent();
    BunnyComponent(juce::Matrix3D<float> ViewMatrix, juce::Matrix3D<float> ProjectionMatrix) : m_ViewMatrix(ViewMatrix), m_ProjectionMatrix(ProjectionMatrix) {};
    ~BunnyComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void newOpenGLContextCreated() override;
    void renderOpenGL() override;
    void openGLContextClosing() override;

private:
    juce::OpenGLContext openGLContext;

    struct Vertex
    {
        float position[3];
        float colour[4];
    };

    std::vector<Vertex> m_VertexBuffer;
    std::vector<unsigned int> m_IndexBuffer;

    juce::Matrix3D<float> m_ProjectionMatrix, m_ViewMatrix;

    GLuint m_vbo;
    GLuint m_ibo;

    std::string m_VertexShader;
    std::string m_FragmentShader;

    std::unique_ptr<juce::OpenGLShaderProgram> m_ShaderProgram;

    std::vector<Vertex> GetBunnyVertices() const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BunnyComponent)
};
