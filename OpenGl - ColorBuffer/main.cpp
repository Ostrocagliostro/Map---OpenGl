// Include standard headers
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>
#include <common/controls.cpp>


#include <string>
#include <sstream>

double fotogramsPerSecond(unsigned int*, int, double);

int main()
{
        // Initialise GLFW
        if( !glfwInit() )
        {
                fprintf( stderr, "Failed to initialize GLFW\n" );
                return -1;
        }

        glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
        glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwOpenWindowHint(GLFW_REFRESH_RATE, 0);


        // Open a window and create its OpenGL context
        if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
        {
                fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
                glfwTerminate();
                return -1;
        }

        // Initialize GLEW
        glewExperimental = true; // Needed for core profile
        if (glewInit() != GLEW_OK) {
                fprintf(stderr, "Failed to initialize GLEW\n");
                return -1;
        }

        glfwSetWindowTitle( "Tutorial 02" );

        unsigned int frameCount = 0;
        double oldTime = 0;

        // Ensure we can capture the escape key being pressed below
        glfwEnable( GLFW_STICKY_KEYS );

        // Dark blue background
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        // Create and compile our GLSL program from the shaders
        GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

        // Get a handle for our "MVP" uniform
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");


        static const GLfloat g_vertex_buffer_data[] = {
           -1.0, -1.0, 0.0,
           1.0, -1.0, 0.0,
           0.0,  1.0, 0.0,
           0.5,  0.5, 0.0,
        };

        static const GLfloat g_color_buffer_data[] = {
           0.583f,  0.771f,  0.014f,
           0.583f,  0.771f,  0.014f,
           0.822f,  0.569f,  0.201f,
           0.822f,  0.569f,  0.201f,
        };

        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        GLuint colorbuffer;
        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


        do{

                // Clear the screen
           glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Use our shader
                glUseProgram(programID);

                // Compute the MVP matrix from keyboard and mouse input
                computeMatricesFromInputs();
                glm::mat4 ProjectionMatrix = getProjectionMatrix();
                glm::mat4 ViewMatrix = getViewMatrix();
                glm::mat4 ModelMatrix = glm::mat4(1.0);
                glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

                // Send our transformation to the currently bound shader,
                // in the "MVP" uniform
                glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


                // 1rst attribute buffer : vertices
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
                glVertexAttribPointer(
                        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                        3,                  // size
                        GL_FLOAT,           // type
                        GL_FALSE,           // normalized?
                        0,                  // stride
                        (void*)0            // array buffer offset
                );

                // 2nd attribute buffer : colors
                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
                glVertexAttribPointer(
                        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                        3,                                // size
                        GL_FLOAT,                         // type
                        GL_FALSE,                         // normalized?
                        0,                                // stride
                        (void*)0                          // array buffer offset
                );


                // Draw the triangle !
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // 3 indices starting at 0 -> 1 triangle

                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);

                // Swap buffers
                glfwSwapBuffers();

                ++frameCount;
                oldTime = fotogramsPerSecond(&frameCount, 1.0, oldTime);

        } // Check if the ESC key was pressed or the window was closed
        while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
                   glfwGetWindowParam( GLFW_OPENED ) );

        // Close OpenGL window and terminate GLFW
        glfwTerminate();

        // Cleanup VBO
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteVertexArrays(1, &VertexArrayID);

        return 0;
}

double fotogramsPerSecond(unsigned int *frameCount, int step, double oldTime){

   double newTime = glfwGetTime();

   if(*frameCount == 0){
      glfwSetTime(0.0);
   }

   if(std::abs(oldTime - newTime) > step){
      int fps = *(frameCount)/(newTime - oldTime);

      std::stringstream oss;

      oss << "Rob Halford? @" << fps << " fps";

      std::string newTitle (oss.str());

      glfwSetWindowTitle( newTitle.c_str() );

      oldTime = newTime;
      *frameCount=0;
   }

   return oldTime;
}
