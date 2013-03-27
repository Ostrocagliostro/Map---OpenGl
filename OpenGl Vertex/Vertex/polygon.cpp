#include <iostream>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include <string> // fotogramPerSecond
#include <sstream>

int Init(void);
double fotogramsPerSecond(unsigned int*, int, double);


void Draw(std::vector<GLfloat>, GLuint, unsigned int, unsigned int);

int main(){

   unsigned int frameCount = 0;
   double oldTime = 0;

   if(Init() != 0){
      return -1;
   }

   GLuint VertexArrayID;
   glGenVertexArrays(1, &VertexArrayID);
   glBindVertexArray(VertexArrayID);

   // An array of 3 vectors which represents 3 vertices
   static const  GLfloat g_vertex_buffer_polygon[] = {
      -1.0, -1.0, 0.0,
      1.0, -1.0, 0.0,
      0.0,  1.0, 0.0,
      0.0, 1.0, 0.0,
      1.0, -1.0, 0.0,
      0.5,  0.5, 0.0,
   };

   static const GLfloat g_vertex_buffer_points[] = {
      0.9, 0.9, 0.0,
      -0.9, 0.9, 0.0
   };

   std::vector<GLfloat> vect_g_vertex_buffer_polygon;

   unsigned int number_of_polygon = sizeof(g_vertex_buffer_polygon)/sizeof(GLfloat);
   unsigned int number_of_points = sizeof(g_vertex_buffer_points)/sizeof(GLfloat);

   for(unsigned int j=0; j<number_of_polygon; ++j){
      vect_g_vertex_buffer_polygon.push_back(g_vertex_buffer_polygon[j]);
   }

   for(unsigned int j=0; j<number_of_points; ++j){
      vect_g_vertex_buffer_polygon.push_back(g_vertex_buffer_points[j]);
   }

   // This will identify our vertex buffer
   GLuint vertexbuffer;

   // Generate 1 buffer, put the resulting identifier in vertexbuffer
   glGenBuffers(1, &vertexbuffer);

   // The following commands will talk about our 'vertexbuffer' buffer
   glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

   do{
      // Draw the triangles!

      double time = glfwGetTime();

      vect_g_vertex_buffer_polygon[number_of_polygon+0] = std::sin(time);
      vect_g_vertex_buffer_polygon[number_of_polygon+3] = -std::sin(time);
      vect_g_vertex_buffer_polygon[number_of_polygon-2] = 0.5 + 0.25* std::sin(time) * std::cos(time);
      vect_g_vertex_buffer_polygon[number_of_polygon-3] = 0.5 + 0.25* std::sin(time) * std::cos(time);

      Draw(vect_g_vertex_buffer_polygon, vertexbuffer, number_of_polygon, number_of_points);

      glDisableVertexAttribArray(0);

       // Swap buffers
       glfwSwapBuffers();

       ++frameCount;
       oldTime = fotogramsPerSecond(&frameCount, 1.0, oldTime);

   } // Check if the ESC key was pressed or the window was closed
   while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED ) );

   return(0);
}

void Draw(std::vector<GLfloat> g_vertex_buffer_polygon, GLuint vertexbuffer, unsigned int number_of_polygon, unsigned int number_of_points){

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  unsigned int g_vertex_buffer_polygon_size = g_vertex_buffer_polygon.size();

   // Give our vertices to OpenGL.
   glBufferData(GL_ARRAY_BUFFER, (g_vertex_buffer_polygon_size * sizeof(GLfloat)), &g_vertex_buffer_polygon[0], GL_STATIC_DRAW);

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

   glDrawArrays (GL_TRIANGLES, 0, number_of_polygon/3); // Starting from vertex 0; 6 vertices total -> 2 triangles
   glDrawArrays (GL_POINTS, number_of_polygon/3, 2); // Starting from vertex 6; 1 vertices total -> 1 point
}


int Init(void){  // Initialise GLFW

   if( !glfwInit() )
   {
       std::cerr<<"Failed to initialize GLFW"<<std::endl;
       return -1;
   }

   glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing
   glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 4); // Major number of the desired minimum OpenGL version.
   glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3); // Minor number of the desired minimum OpenGL version.
   glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //I don't want the old OpenGL

   const unsigned int window_width=1024, window_height=768;

   // Open a window "1024x768" and create its OpenGL context
   if( !glfwOpenWindow(window_width, window_height, 0,0,0,0, 32,0, GLFW_WINDOW ) )
   {
       std::cerr<<"Failed to open GLFW window"<<std::endl;
       glfwTerminate();
       return -1;
   }

   // Initialize GLEW
   glewExperimental=true; // Needed in core profile
   if (glewInit() != GLEW_OK) {
       std::cerr<<"Failed to initialize GLEW"<<std::endl;
       return -1;
   }

   glfwSetWindowTitle( "Rob Halford?" );

   // Ensure we can capture the escape key being pressed below
   glfwEnable( GLFW_STICKY_KEYS );

   return 0;
}

double fotogramsPerSecond(unsigned int *frameCount, int step, double oldTime){

   double newTime = glfwGetTime();

   if(*frameCount == 0){
      glfwSetTime(0.0);
   }

   if(std::abs(oldTime - newTime) > step){
      float fps = *(frameCount)/(newTime - oldTime);

      std::stringstream oss;

      oss << "Rob Halford? @" << fps << " fps";

      std::string newTitle (oss.str());

      glfwSetWindowTitle( newTitle.c_str() );

      oldTime = newTime;
      *frameCount=0;
   }

   return oldTime;
}


