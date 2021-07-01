/*
  ==============================================================================

    BunnyComponent.cpp
    Created: 1 Jul 2021 7:58:09am
    Author:  Alex

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BunnyComponent.h"

//==============================================================================
BunnyComponent::BunnyComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    // Indicates that no part of this Component is transparent.
    setOpaque(true);

    // Set this instance as the renderer for the context.
    openGLContext.setRenderer(this);

    // Tell the context to repaint on a loop.
    openGLContext.setContinuousRepainting(true);

    // Finally - we attach the context to this Component.
    openGLContext.attachTo(*this);
}

BunnyComponent::~BunnyComponent()
{
}

void BunnyComponent::paint (juce::Graphics& g)
{
}

void BunnyComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

std::vector<BunnyComponent::Vertex> BunnyComponent::GetBunnyVertices() const {
    std::vector<BunnyComponent::Vertex> vertices;

    std::ofstream find("EIFJISEFJLSEFSLDKFJSKDF.txt");
    find << "IFEjLSDFSJEIFJ" << std::endl;
    find.close();

    std::ifstream bunny_file("bun_zipper.txt");
    float test;

    Logger logger;

    while (!bunny_file.fail()) {
        float x, y, z;
        bunny_file >> x >> y >> z;

        Vertex v{
            { x, y, z },
            { 1.f, 0.f, 0.f, 1.f }
        };

        logger.Log(std::to_string(x));

        vertices.emplace_back(v);
    }

    bunny_file.close();

    return vertices;
}

void BunnyComponent::newOpenGLContextCreated()
{
    // Generate 1 buffer, using our vbo variable to store its ID.
    openGLContext.extensions.glGenBuffers(1, &m_vbo);

    // Generate 1 more buffer, this time using our IBO variable.
    openGLContext.extensions.glGenBuffers(1, &m_ibo);

    // m_VertexBuffer = GetBunnyVertices();

    // Vertex 0
    m_VertexBuffer = {
        {
        { -0.5f, 0.5f },        // (-0.5, 0.5)
        { 1.f, 0.f, 0.f, 1.f }  // Red
    },
        // Vertex 1
        {
            { 0.5f, 0.5f },         // (0.5, 0.5)
            { 1.f, 0.5f, 0.f, 1.f } // Orange
        },
        // Vertex 2
    {
        { 0.5f, -0.5f },        // (0.5, -0.5)
        { 1.f, 1.f, 0.f, 1.f }  // Yellow
    },
        // Vertex 3
    {
        { -0.5f, -0.5f },       // (-0.5, -0.5)
        { 1.f, 0.f, 1.f, 1.f }  // Purple
    }
    };

    // We need 6 indices, 1 for each corner of the two triangles.
    m_IndexBuffer = {
        0, 1, 2,
        0, 2, 3
    };

    
    {
        using namespace juce;

        // Bind the VBO.
        openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        // Send the vertices data.
        openGLContext.extensions.glBufferData(
            GL_ARRAY_BUFFER,                        // The type of data we're sending.           
            sizeof(Vertex) * m_VertexBuffer.size(),   // The size (in bytes) of the data.
            m_VertexBuffer.data(),                    // A pointer to the actual data.
            GL_STATIC_DRAW                          // How we want the buffer to be drawn.
        );

        // Bind the IBO.
        openGLContext.extensions.glBindBuffer(juce::MissingOpenGLDefinitions::GL_ELEMENT_ARRAY_BUFFER, m_ibo);

        // Send the indices data.
        openGLContext.extensions.glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(unsigned int) * m_IndexBuffer.size(),
            m_IndexBuffer.data(),
            GL_STATIC_DRAW
        );
    }

    m_VertexShader =
        R"(
            #version 330 core
            
            in vec4 position;      
            in vec4 sourceColour;
            
            out vec4 fragColour;
            
            void main()
            {
                gl_Position = position;
                
                fragColour = sourceColour;
            }
        )";

    m_FragmentShader =
        R"(
            #version 330 core
            
            in vec4 fragColour;
            
            void main()
            {
                gl_FragColor = fragColour;
            }
        )";
    // Create an instance of OpenGLShaderProgram
    m_ShaderProgram.reset(new juce::OpenGLShaderProgram(openGLContext));

    // Compile and link the shader.
    // Each of these methods will return false if something goes wrong so we'll
    // wrap them in an if statement
    if (m_ShaderProgram->addVertexShader(m_VertexShader)
        && m_ShaderProgram->addFragmentShader(m_FragmentShader)
        && m_ShaderProgram->link())
    {
        // No compilation errors - set the shader program to be active
        m_ShaderProgram->use();
    }
    else
    {
        // Oops - something went wrong with our shaders!
        // Check the output window of your IDE to see what the error might be.
        jassertfalse;
    }
}

void BunnyComponent::renderOpenGL()
{
    juce::OpenGLHelpers::clear(juce::Colours::black);

    m_ShaderProgram->use();

    {
        using namespace juce;
        openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    }

    openGLContext.extensions.glVertexAttribPointer(
        0,              // The attribute's index (AKA location).
        2,              // How many values this attribute contains.
        GL_FLOAT,       // The attribute's type (float).
        GL_FALSE,       // Tells OpenGL NOT to normalise the values.
        sizeof(Vertex), // How many bytes to move to find the attribute with
                        // the same index in the next vertex.
        nullptr         // How many bytes to move from the start of this vertex
                        // to find this attribute (the default is 0 so we just
                        // pass nullptr here).
    );
    openGLContext.extensions.glEnableVertexAttribArray(0);

    // Enable to colour attribute.
    openGLContext.extensions.glVertexAttribPointer(
        1,                              // This attribute has an index of 1
        4,                              // This time we have four values for the
                                        // attribute (r, g, b, a)
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (GLvoid*)(sizeof(float) * 2)    // This attribute comes after the
                                        // position attribute in the Vertex
                                        // struct, so we need to skip over the
                                        // size of the position array to find
                                        // the start of this attribute.
    );
    openGLContext.extensions.glEnableVertexAttribArray(1);

    glDrawElements(
        GL_TRIANGLES,       // Tell OpenGL to render triangles.
        m_IndexBuffer.size(), // How many indices we have.
        GL_UNSIGNED_INT,    // What type our indices are.
        nullptr             // We already gave OpenGL our indices so we don't
                            // need to pass that again here, so pass nullptr.
    );

    openGLContext.extensions.glDisableVertexAttribArray(0);
    openGLContext.extensions.glDisableVertexAttribArray(1);
}

void BunnyComponent::openGLContextClosing()
{
}
