#include <iostream>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>


int main(){
   // Initialise GLFW
   if( !glfwInit() )
   {
       std::cerr<<"Failed to initialize GLFW\n"<<std::endl;
       return -1;
   }

   glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing
   glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3); // I want OpenGL 3.3
   glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
   glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //I don't want the old OpenGL

   int window_width=1024, window_height=768;

   std::cout<<"Window Size:";
   std::cin>>window_width>>window_height;

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

   do{
       // Draw nothing.. for now

       // Swap buffers
       glfwSwapBuffers();

   } // Check if the ESC key was pressed or the window was closed
   while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED ) );

   return 0;
}
