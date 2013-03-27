#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <deque>

#include <GL/glew.h>          // Include GLEW
#include <GL/glfw.h>          // Include GLFW
#include <glm/glm.hpp>        // Include GLM
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.cpp>  // Include "LoadShaders"
#include <common/controls.cpp>



#include <cstdlib> //randNumber
#include <ctime>

#define WINDOW_TITLE_DEFAULT "Rob Halford?"
#define NUMBER_OF_TRIANGLES 500


int Init(void);
double fotogramsPerSecond(unsigned int &, double, double);
void createVBO(GLuint &, GLuint &, GLuint &, GLuint &);
GLuint createShader(void);
void cleanVBO(GLuint &, GLuint &, GLuint &);
float randomNumber(float, float);
float triangleArea(float, float, float, float, float, float);

float randomColor(void);

struct VBO
{
   GLfloat x;
   GLfloat y;

   GLfloat r;

   public:

   void evaluateRadius(void);
};

void VBO::evaluateRadius()
{
   r = std::sqrt(std::pow(x+1,2)+std::pow(y+1, 2));
}

bool mySortFunction (VBO i,VBO j) // <
{
   return (i.r < j.r);
}

int main()
{

   unsigned int frameCount = 0;
   double oldTime = 0;
   srand((unsigned)time(0));

   //Initialize GLFW, GLEW, and open a window
   Init();

   GLuint vertexbuffer, colorbuffer, pointbuffer, VertexArrayID, programID;

   createVBO(vertexbuffer, colorbuffer, pointbuffer ,VertexArrayID);

   programID = createShader();

   do
   {
      // Clear the screen
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

      // 1st attribute buffer: points
      glVertexAttribPointer(
         0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
         2,                  // size
         GL_FLOAT,           // type
         GL_FALSE,           // normalized?
         0,                  // stride
         (void*)0            // array buffer offset
      );

      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);

      // 2nd attribute buffer : colors
      glVertexAttribPointer(
         1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
         3,                                // size
         GL_FLOAT,                         // type
         GL_FALSE,                         // normalized?
         0,                                // stride
         (void*)0                          // array buffer offset
      );

      // Draw!!
      glDrawArrays(GL_TRIANGLES, 0, NUMBER_OF_TRIANGLES*3);
      glDrawArrays(GL_POINTS, NUMBER_OF_TRIANGLES*3, 4);

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);

      ++frameCount;
      oldTime = fotogramsPerSecond(frameCount, 1.0, oldTime);

      glfwSwapBuffers();   // Swap buffers
   }
   while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));

   // Close OpenGL window and terminate GLFW
   glfwTerminate();

   // Cleanup VBO
   cleanVBO(vertexbuffer, VertexArrayID, programID);

   return 0;
}

int Init(void)
{
   // Initialize GLFW
   if(!glfwInit())
   {
       std::cerr<<"Failed to initialize GLFW"<<std::endl;
       return -1;
   }

   glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);          // 4x antialiasing
   glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 4);  // Major number of the desired minimum OpenGL version.
   glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);  // Minor number of the desired minimum OpenGL version.
   glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);        // Let the system choose which context should implement.

   const unsigned int window_width=1024, window_height=768;

   // Open a window "1024x768" and create its OpenGL context
   if(!glfwOpenWindow(window_width, window_height, 0,0,0,0, 32,0, GLFW_WINDOW))
   {
       std::cerr<<"Failed to open GLFW window"<<std::endl;
       glfwTerminate();
       return -1;
   }

   // Initialize GLEW
   glewExperimental = true; // Needed for core profile
   if (glewInit() != GLEW_OK) {
       std::cerr<<"Failed to initialize GLEW"<<std::endl;
       return -1;
   }

   glfwSetWindowTitle(WINDOW_TITLE_DEFAULT); // Set a default Title

   // Black background -> Red-Green-Blue-Alpha
   glClearColor(0.0, 0.0, 0.0, 0.0);

   // Ensure we can capture the escape key being pressed below
   glfwEnable(GLFW_STICKY_KEYS);

   return 0;
}

GLuint createShader(void)
{
   // Create and compile our GLSL program from the shaders
   GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

   // Use our shader
   glUseProgram(programID);

   return programID;
}

void createVBO(GLuint &vertexbuffer, GLuint &colorbuffer, GLuint &pointbuffer, GLuint &VertexArrayID)
{
   glGenVertexArrays(1, &VertexArrayID);
   glBindVertexArray(VertexArrayID);

   VBO point1, point2, point3;

   std::deque<VBO> vec_VBO;

   for(unsigned i=0; i<NUMBER_OF_TRIANGLES; ++i)
   {
      do
      {
         point1.x = randomNumber(-1.0, 1.0);
         point1.y = randomNumber(-1.0, 1.0);
         point2.x = randomNumber(-1.0, 1.0);
         point2.y = randomNumber(-1.0, 1.0);
         point3.x = randomNumber(-1.0, 1.0);
         point3.y = randomNumber(-1.0, 1.0);
      } while(!(triangleArea(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y) > 0.5));

      std::cout<<"Area triangle #"<<i<<": "<<triangleArea(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y)<<std::endl;

      point1.evaluateRadius();
      point2.evaluateRadius();
      point3.evaluateRadius();

      vec_VBO.push_back(point1);
      vec_VBO.push_back(point2);
      vec_VBO.push_back(point3);
   }

   //std::sort(vec_VBO.begin(), vec_VBO.end(), mySortFunction);   USLESS NOW BECAUSE OF GL_TRIANGLES

   std::vector<GLfloat> g_vertex_buffer_data;
   std::vector<GLfloat> g_color_buffer_data;

   static const GLfloat g_points_buffer_data[] = {
         -0.98, 0.0,
         0.0, 0.98,
         0.98, 0.0,
         0.0, -0.98
      };

   unsigned vec_VBO_size = vec_VBO.size();

   for(unsigned i=0; i<vec_VBO_size; ++i)
   {
      g_vertex_buffer_data.push_back(vec_VBO[0].x);
      g_vertex_buffer_data.push_back(vec_VBO[0].y);

      vec_VBO.pop_front();

      std::cout<<i<<") "<<g_vertex_buffer_data[i]<<" - "<<g_vertex_buffer_data[i+1]<< std::endl;
   }

   for(unsigned i=0; i<(vec_VBO_size/3); ++i)
   {
      float tmp1 = randomColor(), tmp2 = randomColor(), tmp3 = randomColor();
      for(unsigned j=0; j<3; ++j)
      {
         g_color_buffer_data.push_back(tmp1);
         g_color_buffer_data.push_back(tmp2);
         g_color_buffer_data.push_back(tmp3);
      }

      std::cout<<"Color: ";
      for(unsigned j=0; j<3; ++j)
      {
         std::cout<<g_color_buffer_data[i]<<"  ";
      }
      std::cout<<std::endl;

   }


   for(unsigned i=0; i<8; ++i)
   {
      g_vertex_buffer_data.push_back(g_points_buffer_data[i]);

      g_color_buffer_data.push_back(1.);
      g_color_buffer_data.push_back(0.);
      g_color_buffer_data.push_back(0.);

      std::cout<<i<<") "<<g_vertex_buffer_data[vec_VBO_size*2+i]<<std::endl;
   }

   // Generate 1 buffer, put the resulting identifier in vertexbuffer
   glGenBuffers(1, &vertexbuffer);

   // The following commands will talk about our 'vertexbuffer' buffer
   glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
   glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size()*sizeof(GLfloat), &g_vertex_buffer_data[0], GL_STATIC_DRAW);

   // Generate 1 buffer, put the resulting identifier in colorbuffer
   glGenBuffers(1, &colorbuffer);

   // The following commands will talk about our 'colorbuffer' buffer
   glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
   glBufferData(GL_ARRAY_BUFFER, g_color_buffer_data.size()*sizeof(GLfloat), &g_color_buffer_data[0], GL_STATIC_DRAW);
}

double fotogramsPerSecond(unsigned int &frameCount, double step, double oldTime){

   double newTime = glfwGetTime();

   if(frameCount == 0){
      glfwSetTime(0.0);
   }

   if(std::abs(oldTime - newTime) > step)
   {
      unsigned int fps = frameCount/(newTime - oldTime);

      std::stringstream oss;

      oss << WINDOW_TITLE_DEFAULT <<" @"<< fps << " fps";

      std::string newTitle (oss.str());

      glfwSetWindowTitle(newTitle.c_str());

      oldTime = newTime;
      frameCount=0;
   }

   return oldTime;
}

void cleanVBO(GLuint &vertexbuffer, GLuint &VertexArrayID, GLuint &programID)
{
   glDeleteBuffers(1, &vertexbuffer);
   glDeleteVertexArrays(1, &VertexArrayID);
   glDeleteProgram(programID);
}

float randomNumber(float randMin, float randMax)
{

   float random_integer = randMin + (float)rand()/((float)RAND_MAX/(randMax-randMin));

   //trunc 2 decimal

   int tmp = random_integer * 10;

   random_integer = tmp / 10.0;

   return random_integer;
}

float randomColor()
{
   float random = randomNumber(0.0, 4.0);

   if(random < 1.0) return 0.2;
   if(random < 2.0) return 0.4;
   if(random < 3.0) return 0.6;
   if(random < 4.0) return 0.8;

   return 0.0;
}

float triangleArea(float x1, float y1, float x2, float y2, float x3, float y3)
{
   glm::vec3 area;

   glm::vec3 v1 ((x1-x2), (y1-y2), 0.0);
   glm::vec3 v2 ((x3-x2), (y3-y2), 0.0);

   area = glm::cross(v2, v1);

   area[2] = std::abs(area[2])/2;

   return area[2];
}
