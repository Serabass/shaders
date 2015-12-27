#include "include/GL/glew.h"
#include "include/GL/glut.h"

#include <iostream>
#include <fstream>

//! Переменные с индентификаторами ID
//! ID шейдерной программы
GLuint Program;
//! ID атрибута
GLint  Attrib_vertex;

//! ID Vertex Buffer Object
GLuint VBO;

GLint timePointer;

DWORD startTime;

//! Вершина
struct vertex
{
  GLfloat x;
  GLfloat y;
};

//! Функция печати лога шейдера
void shaderLog(unsigned int shader) 
{ 
  int   infologLen   = 0;
  int   charsWritten = 0;
  char *infoLog;

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);

  if(infologLen > 1)
  { 
    infoLog = new char[infologLen];
    if(infoLog == NULL)
    {
      std::cout<<"ERROR: Could not allocate InfoLog buffer\n";
       exit(1);
    }
    glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
    std::cout<< "InfoLog: " << infoLog << "\n\n\n";
    delete[] infoLog;
  }
}

//! Инициализация OpenGL, здесь пока по минимальному=)
void initGL()
{
  glClearColor(0, 0, 0, 0);
}

//! Проверка ошибок OpenGL, если есть то выводит в консоль тип ошибки
void checkOpenGLerror()
{
  GLenum errCode;
  if((errCode=glGetError()) != GL_NO_ERROR) {
	  const GLubyte * errStr = gluErrorString(errCode);
    std::cout << "OpenGl error! - " << errStr;
  }
}

char * readFile(const char *filename) {
	std::ifstream is(filename, std::ios::in | std::ios::binary | std::ios::ate);

	if (!is.is_open()) {
		std::cerr << "Unable to open file " << filename << std::endl;
		return false;
	}
	long size = is.tellg();
	char *buffer = new char[size + 1];
	is.seekg(0, std::ios::beg);
	is.read(buffer, size);
	is.close();
	buffer[size] = 0;

	return buffer;
}

//! Инициализация шейдеров
void initShader()
{

  startTime = GetTickCount();

  //! Исходный код шейдеров
  const char* vsSource = readFile("C:\\Users\\Serabass\\Downloads\\Simple_Shaders\\Simple_shaders\\Simple_Shaders\\First_shaders\\shaders\\vert_shader.glsl");
  const char* fsSource = readFile("C:\\Users\\Serabass\\Downloads\\Simple_Shaders\\Simple_shaders\\Simple_Shaders\\First_shaders\\shaders\\frag_shader.glsl");
  //! Переменные для хранения идентификаторов шейдеров
  GLuint vShader, fShader;
  
  //! Создаем вершинный шейдер
  vShader = glCreateShader(GL_VERTEX_SHADER);
  //! Передаем исходный код
  glShaderSource(vShader, 1, &vsSource, NULL);
  //! Компилируем шейдер
  glCompileShader(vShader);

  std::cout << "vertex shader \n";
  shaderLog(vShader);

  //! Создаем фрагментный шейдер
  fShader = glCreateShader(GL_FRAGMENT_SHADER);
  //! Передаем исходный код
  glShaderSource(fShader, 1, &fsSource, NULL);
  //! Компилируем шейдер
  glCompileShader(fShader);

  std::cout << "fragment shader \n";
  shaderLog(fShader);

  //! Создаем программу и прикрепляем шейдеры к ней
  Program = glCreateProgram();
  glAttachShader(Program, vShader);
  glAttachShader(Program, fShader);

  //! Линкуем шейдерную программу
  glLinkProgram(Program);

  //! Проверяем статус сборки
  int link_ok;
  glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
  if(!link_ok)
  {
    std::cout << "error attach shaders \n";
    return;
  }
  ///! Вытягиваем ID атрибута из собранной программы 
  const char* attr_name = "coord";
  Attrib_vertex = glGetAttribLocation(Program, attr_name);
  if (Attrib_vertex == -1)
  {
	  std::cout << "could not bind attrib " << attr_name << std::endl;
	  return;
  }

  const char* attr_time_name = "time";
  timePointer = glGetUniformLocation(Program, attr_time_name);
  if (timePointer == -1)
  {
	  std::cout << "could not bind uniform " << attr_time_name << std::endl;
	  return;
  }

  checkOpenGLerror();
}

//! Инициализация VBO
void initVBO()
{
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  //! Вершины нашего треугольника
  vertex triangle[6] = {
	  {  1.0f,  1.0f },
	  { -1.0f, -1.0f },
	  { -1.0f,  1.0f },
	  { -1.0f, -1.0f },
	  {  1.0f,  1.0f },
	  {  1.0f, -1.0f }
  };
  //! Передаем вершины в буфер
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

  checkOpenGLerror();
}

//! Освобождение шейдеров
void freeShader()
{
  //! Передавая ноль, мы отключаем шейдрную программу
  glUseProgram(0); 
  //! Удаляем шейдерную программу
  glDeleteProgram(Program);
}

//! Освобождение шейдеров
void freeVBO()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &VBO);
}

void resizeWindow(int width, int height)
{
  glViewport(0, 0, width, height);
}

//! Отрисовка
void render()
{
  glClear(GL_COLOR_BUFFER_BIT);
  //! Устанавливаем шейдерную программу текущей
  glUseProgram(Program); 

  GLfloat glTime =  ((float)GetTickCount() - startTime) / 1000 ;

  glUniform1f(timePointer, glTime);

  //! Включаем массив атрибутов
  glEnableVertexAttribArray(Attrib_vertex);
    //! Подключаем VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
      //! Указывая pointer 0 при подключенном буфере, мы указываем что данные в VBO
      glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
    //! Отключаем VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //! Передаем данные на видеокарту(рисуем)
    glDrawArrays(GL_TRIANGLES, 0, sizeof (vertex));

	glutPostRedisplay();

  //! Отключаем массив атрибутов
  glDisableVertexAttribArray(Attrib_vertex);

  //! Отключаем шейдерную программу
  glUseProgram(0);

  checkOpenGLerror();

  glutSwapBuffers();
}

int main( int argc, char **argv )
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Simple shaders");

  //! Обязательно перед инициализации шейдеров
  GLenum glew_status = glewInit();
  if(GLEW_OK != glew_status) 
  {
     //! GLEW не проинициализировалась
    std::cout << "Error: " << glewGetErrorString(glew_status) << "\n";
    return 1;
  }

  //! Проверяем доступность OpenGL 2.0
  if(!GLEW_VERSION_2_0) 
   {
     //! OpenGl 2.0 оказалась не доступна
    std::cout << "No support for OpenGL 2.0 found\n";
    return 1;
  }

  //! Инициализация
  initGL();
  initVBO();
  initShader();
  
  glutReshapeFunc(resizeWindow);
  glutDisplayFunc(render);
  glutMainLoop();
  
  //! Освобождение ресурсов
  freeShader();
  freeVBO();
}
