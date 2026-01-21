#include <iostream>
#include <fstream>
#include <SDL.h>
#include <glew.h>
#include <string>

using namespace std;
//#define GLEW_STATIC

float x = 0;
float y = 0;
float vx = 0.007f;
float vy = 0.02f;
float sx = 0.3f;
float sy = 0.3f;

string LoadShader(string filename);

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
	}
	else
	{
		cout << "SDL initialization succeeded!\n";
	}

	///////////SETTING UP SDL/////////////
	//Create a simple window
	int width = 400;
	int height = 300;
	unsigned int center = SDL_WINDOWPOS_CENTERED;
	SDL_Window* Window = SDL_CreateWindow("My window", center, center, width, height, SDL_WINDOW_OPENGL);
	//SDL_WINDOW_OPENGL is a u32 flag !

	//Create an OpenGL compatible context to let glew draw on it
	SDL_GLContext Context = SDL_GL_CreateContext(Window);

	/////////SETTING UP OPENGL WITH GLEW///
	//Initialize glew
	glewExperimental = GL_TRUE;
	if (glewInit() == GLEW_OK) {
		cout << "Glew initialized successfully\n";
	}

	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);


	float vertices[] = {
		 0.0f, -0.2f, 0.0f,     1.0f, 0.5f, 0.3f,
		 0.2f, -0.2f, 0.0f,		1.0f, 0.5f, 0.3f,
		 0.3f,  0.0f, 0.0f,		1.0f, 0.5f, 0.3f,
		 0.3f,  0.2f, 0.0f,		1.0f, 0.5f, 0.3f,
		 0.2f,  0.4f, 0.0f,		1.0f, 0.5f, 0.3f,
		 0.1f,  0.3f, 0.0f,		1.0f, 0.5f, 0.3f,
		-0.1f,  0.3f, 0.0f,		1.0f, 0.5f, 0.3f,
		-0.2f,  0.4f, 0.0f,		1.0f, 0.5f, 0.3f,
		-0.3f,  0.2f, 0.0f,		1.0f, 0.5f, 0.3f,
		-0.3f,  0.0f, 0.0f,		1.0f, 0.5f, 0.3f,
		-0.2f, -0.2f, 0.0f,		1.0f, 0.5f, 0.3f,

		-0.7f,  0.8f, 0.0f,
		 0.7f,  0.8f, 0.0f,
		 0.0f, -0.6f, 0.0f
	};

	unsigned int vbo;
	glGenBuffers(1, &vbo);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	string vs = LoadShader("vertex.vert");
	const char* vertexShaderSource = vs.c_str();
	string fs = LoadShader("fragment.frag");
	const char* fragmentShaderSource = fs.c_str();

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	string vs2 = LoadShader("BasicVertex.vert");
	const char* vertex2ShaderSource = vs2.c_str();
	string fs2 = LoadShader("BasicFragment.frag");
	const char* fragment2ShaderSource = fs2.c_str();

	unsigned int vao2;
	glGenVertexArrays(1, &vao2);
	glBindVertexArray(vao2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(11 * 6 * sizeof(float)));
	glEnableVertexAttribArray(0);

	unsigned int vertex2Shader;
	vertex2Shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex2Shader, 1, &vertex2ShaderSource, NULL);
	glCompileShader(vertex2Shader);

	unsigned int fragment2Shader;
	fragment2Shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment2Shader, 1, &fragment2ShaderSource, NULL);
	glCompileShader(fragment2Shader);

	unsigned int shader2Program;
	shader2Program = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glAttachShader(shader2Program, vertex2Shader);
	glAttachShader(shader2Program, fragment2Shader);
	glLinkProgram(shader2Program);
	
	glUseProgram(shaderProgram);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	bool isRunning = true;
	while (isRunning)
	{
		//Inputs
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;
			default:
				break;
			}
		}

		x += vx;
		y += vy;

		if (x > 1 - sx || x < -1 + sx) vx *= -1;
		if (y > 1 - sy || y < -1 + sy) vy *= -1;

		int vertexOffsetLocation = glGetUniformLocation(shaderProgram, "offset");
		glUseProgram(shaderProgram);
		glUniform3f(vertexOffsetLocation, x, y, 0.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear Screen
		
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 11);

		glUseProgram(shader2Program);
		glBindVertexArray(vao2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(Window);
	}

	SDL_DestroyWindow(Window);
	SDL_GL_DeleteContext(Context);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	cout << "Glew closed successfully\n";
	return 0;
}

string LoadShader(string filename)
{
	ifstream myFile;
	myFile.open(filename);
	if (myFile.fail())
	{
		cerr << "Error - Failed to open " << filename << endl;
	}

	string fileText = "";
	string line = "";

	while (getline(myFile, line))
	{
		fileText += line + '\n';
	}

	myFile.close();
	return fileText;
}