/*
Timothy Seid
Final Project
Attempt at creating a multiplayer version of the T-Rex jumping game in chrome
example: apps.thecodepost.org/trex/trex.html
NOTE: The win sound effect is a little loud so don't have your volume up that high
Controls: 
P1 jump- 'S'
P2 Jump- 'K'
*/
#ifdef _WINDOWS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include "SDL_opengles2.h"
#include "vector"
#include <math.h>
#include <SDL_mixer.h>
using namespace std;

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
}	

class Player1 {
public:
	Player1();
	Player1(unsigned int textureID1, float u1, float v1, float width1, float height1, float
		size1) : textureID(textureID1), u(u1), v(v1), width(width1), height(height1), size(size1){
	};
	void Draw(ShaderProgram *p, int index, int spriteCountX, int spriteCountY);
	float getX1();
	float getX2();
	float getX3();
	float getX4();
	float getX5();
	float getX6();
	float getY1();
	float getY2();
	float getY3();
	float getY4();
	float getY5();
	float getY6();
	float size;
	unsigned int textureID;
	float u;
	float v;
	float width;
	float height;
	float velocity_y = 1.5f;
	float acceleration_y = 1.0f;
	float y = 0;
	bool is_jumping = false;
	bool jump_max = false;
	bool isAlive = true;
	Matrix matrix;
	float initPos = 0.0f;
	float initPosY = 0.0f;
};

class Enemy {
public:
	Enemy();
	Enemy(unsigned int textureID1, float u1, float v1, float width1, float height1, float
		size1) : textureID(textureID1), u(u1), v(v1), width(width1), height(height1), size(size1){};
	void Draw(ShaderProgram *p, int index, int spriteCountX, int spriteCountY);
	float getX1();
	float getX2();
	float getX3();
	float getX4();
	float getX5();
	float getX6();
	float getY1();
	float getY2();
	float getY3();
	float getY4();
	float getY5();
	float getY6();
	float size;
	unsigned int textureID;
	float u;
	float v;
	float width;
	float height;
	bool isActive = false;
	Matrix matrix;
	float movement;
	float initPos = 0.0f;  //3.5f
	float initPosY = 0.0f;
};
//og code for the player(not being used)
int index = 19;
int spriteCountX = 30;
int spriteCountY = 30;
float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
float spriteWidth = 1.0 / (float)spriteCountX;
float spriteHeight = 1.0 / (float)spriteCountY;

float Player1::getX1(){
	return -0.5f;
}
float Player1::getX2(){
	return 0.5f;
}
float Player1::getX3(){
	return -0.5f;
}
float Player1::getX4(){
	return 0.5f;
}
float Player1::getX5(){
	return -0.5f;
}
float Player1::getX6(){
	return 0.5f;
}
float Player1::getY1(){
	return -0.5f + y + initPosY;
}
float Player1::getY2(){
	return 0.5f + y + initPosY;
}
float Player1::getY3(){
	return 0.5f + y + initPosY;
}
float Player1::getY4(){
	return 0.5f + y + initPosY;
}
float Player1::getY5(){
	return -0.5f + y + initPosY;
}
float Player1::getY6(){
	return -0.5f + y + initPosY;
}


void Player1::Draw(ShaderProgram *program, int index, int spriteCountX, int spriteCountY) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
	float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
	float spriteWidth = 1.0 / (float)spriteCountX;
	float spriteHeight = 1.0 / (float)spriteCountY;
	GLfloat texCoords[] = {
		u, v + spriteHeight,
		u + spriteWidth, v,
		u, v,
		u + spriteWidth, v,
		u, v + spriteHeight,
		u + spriteWidth, v + spriteHeight
	};
	float vertices[] = { -0.5f * size, (-0.5f + y + initPosY) * size, 0.5f * size, (0.5f + y + initPosY) * size,
		-0.5f * size, (0.5f + y + initPosY) * size, 0.5f * size, (0.5f + y + initPosY) * size,
		-0.5f * size, (-0.5f + y + initPosY) * size, 0.5f * size, (-0.5f + y + initPosY) * size };
	// draw our arrays
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

//enemy coordinates (not currently being used)
int indexE = 169;
int spriteCountXE = 30;
int spriteCountYE = 30;
float uE = (float)(((int)indexE) % spriteCountXE) / (float)spriteCountXE;
float vE = (float)(((int)indexE) / spriteCountXE) / (float)spriteCountYE;
float spriteWidthE = 1.0 / (float)spriteCountXE;
float spriteHeightE = 1.0 / (float)spriteCountYE;

//might need inits for p1/p2 stuff
float Enemy::getX1()
{
	return -0.5f + movement + initPos;
}
float Enemy::getX2()
{
	return  0.5f + movement + initPos;
}
float Enemy::getX3()
{
	return -0.5f + movement + initPos;
}
float Enemy::getX4()
{
	return 0.5f + movement + initPos;
}
float Enemy::getX5()
{
	return  -0.5f + movement + initPos;
}
float Enemy::getX6()
{
	return 0.5f + movement + initPos;
}
float Enemy::getY1()
{
	return -0.5f + initPosY;
}
float Enemy::getY2()
{
	return 0.5f + initPosY;
}
float Enemy::getY3()
{
	return 0.5f + initPosY;
}
float Enemy::getY4()
{
	return 0.5f + initPosY;
}
float Enemy::getY5()
{
	return -0.5f + initPosY;
}
float Enemy::getY6()
{
	return -0.5f + initPosY;
}
void Enemy::Draw(ShaderProgram *program, int index, int spriteCountX, int spriteCountY) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
	float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
	float spriteWidth = 1.0 / (float)spriteCountX;
	float spriteHeight = 1.0 / (float)spriteCountY;
	GLfloat texCoords[] = {
		u, v + spriteHeight,
		u + spriteWidth, v,
		u, v,
		u + spriteWidth, v,
		u, v + spriteHeight,
		u + spriteWidth, v + spriteHeight
	};
	float vertices[] = { (-0.5f + movement + initPos) * size, (-0.5f + initPosY)  * size, (0.5f + movement + initPos) * size,
		(0.5f + initPosY)  * size, (-0.5f + movement + initPos) * size, (0.5f + initPosY) * size,
		(0.5f + movement + initPos) * size, (0.5f + initPosY) * size, (-0.5f + movement + initPos) * size,
		(-0.5f + initPosY) * size, (0.5f + movement + initPos) * size, (-0.5 + initPosY) * size };
	// draw our arrays
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing) {
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void renderMenu(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	SDL_Event event;
	bool done = false;
#ifdef _WINDOWS
	glewInit();
#endif

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix modelMatrix2;
	Matrix modelMatrix3;
	Matrix modelMatrix4;
	Matrix viewMatrix;
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	const Uint8 *key = SDL_GetKeyboardState(NULL);
	GLuint font = LoadTexture("font2.png");
	projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	glUseProgram(program.programID);

	string start = "Final Project";
	string lvl1 = "LEVEL 1: PRESS [T]";
	string lvl2 = "LEVEL 2: PRESS [Y]";
	string lvl3 = "LEVEL 3: PRESS [U]";
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE){
				done = true;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT);
		program.setModelMatrix(modelMatrix);
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);
		glBindTexture(GL_TEXTURE_2D, font);

		DrawText(&program, font, start, 0.2f, 0.1f);
		modelMatrix.identity();
		modelMatrix.Translate(-2.0f, 1.0f, 0); 

		program.setModelMatrix(modelMatrix2);
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);
		modelMatrix2.identity();
		DrawText(&program, font, lvl1, 0.2f, 0.1f);
		modelMatrix2.Translate(-2.0f, 0.5f, 0);

		program.setModelMatrix(modelMatrix3);
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);
		modelMatrix3.identity();
		DrawText(&program, font, lvl2, 0.2f, 0.1f);
		modelMatrix3.Translate(-2.0f, 0, 0);

		program.setModelMatrix(modelMatrix4);
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);
		modelMatrix4.identity();
		DrawText(&program, font, lvl3, 0.2f, 0.1f);
		modelMatrix4.Translate(-2.0f, -0.5f, 0);
		SDL_GL_SwapWindow(displayWindow);

		//if (key[SDL_SCANCODE_SPACE]){
		//	state = GAME_LEVEL_1;
		//}
	}

	SDL_Quit();
}

string start = "Final Project";
string lvl1 = "LEVEL 1: PRESS [T]";
string lvl2 = "LEVEL 2: PRESS [Y]";
string lvl3 = "LEVEL 3: PRESS [U]";
string p1W = "P1 WIN";
string p2W = "P2 WIN";
string pTie = "TIE";
string message = "PRESS ESC";
//player 1
const int runAnimation[] = {20, 21, 26, 21};
const int numFrames = 4;
float animationElapsed = 0.0f;
float framesPerSecond = 10.0f;
int currentIndex = 0;

//player2
const int runAnimation1[] = {80, 81, 86, 81};
const int numFrames1 = 4;
float animationElapsed1 = 0.0f;
float framesPerSecond1 = 10.0f;
int currentIndex1 = 0;

//enemy1
const int runAnimation2[] = {239, 238};
const int numFrames2 = 2;
float animationElapsed2 = 0.0f;
float framesPerSecond2 = 5.0f;
int currentIndex2 = 0;
//enemy2
const int runAnimation3[] = { 235, 236 };
const int numFrames3 = 2;
float animationElapsed3 = 0.0f;
float framesPerSecond3 = 5.0f;
int currentIndex3 = 0;
//enemy3
const int runAnimation4[] = { 446, 447 };
const int numFrames4 = 2;
float animationElapsed4 = 0.0f;
float framesPerSecond4 = 5.0f;
int currentIndex4 = 0;
//p1 jump
const int runAnimation5[] = { 22, 29 };
const int numFrames5 = 2;
float animationElapsed5 = 0.0f;
float framesPerSecond5 = 10.0f;
int currentIndex5 = 0;
//p2 jump
const int runAnimation6[] = { 82, 89 };
const int numFrames6 = 2;
float animationElapsed6 = 0.0f;
float framesPerSecond6 = 10.0f;
int currentIndex6 = 0;


enum GameState { MAIN_MENU, LEVEL_1, LEVEL_2, LEVEL_3, P1_WIN, P2_WIN, P_TIE };
int state;

void game()
{
	SDL_Event event;
	bool done = false;
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix modelMatrix2;
	Matrix modelMatrix3;
	Matrix modelMatrix4;
	Matrix viewMatrix;
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	GLuint spriteSheetTexture = LoadTexture("spritesheet_rgba.png");
	GLuint font = LoadTexture("font2.png");
	projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	glUseProgram(program.programID);
	float lastFrameTicks = 0.0f;
	float elap = 0.0f;
	Player1 mySprite = Player1(spriteSheetTexture, u, v,
		spriteWidth, spriteHeight, 0.5f);
	mySprite.initPosY = 1.0f;
	//P2
	Player1 mySprite2 = Player1(spriteSheetTexture, u, v,
		spriteWidth, spriteHeight, 0.5f);

	mySprite2.initPosY = -3.0f;
	Enemy enemy = Enemy(spriteSheetTexture, uE, vE,
		spriteWidthE, spriteHeightE, 0.5f);
	enemy.initPos = 4.5f;
	enemy.initPosY = 1.0f;

	Enemy enemy2 = Enemy(spriteSheetTexture, uE, vE,
		spriteWidthE, spriteHeightE, 0.5f);
	enemy2.initPos = 4.5f;
	enemy2.initPosY = -3.0f;

	float enemyMovement = 0.0f;
	float enemyMovement2 = 0.0f;
	float test2 = 1.0f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//music
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_Chunk *jump;
	jump = Mix_LoadWAV("jump.wav");
	Mix_Chunk *land;
	land = Mix_LoadWAV("land.wav");
	Mix_Chunk *rit;
	rit = Mix_LoadWAV("yay.wav");
	Mix_Music *music;
	music = Mix_LoadMUS("OMan8bit.mp3");
	Mix_PlayMusic(music, 0);

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT);
		program.setModelMatrix(modelMatrix);
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);
		glBindTexture(GL_TEXTURE_2D, spriteSheetTexture);
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		//run animation
		if (state == MAIN_MENU)
		{
			DrawText(&program, font, start, 0.2f, 0.1f);
			modelMatrix.identity();
			modelMatrix.Translate(-2.0f, 1.0f, 0);

			program.setModelMatrix(modelMatrix2);
			program.setProjectionMatrix(projectionMatrix);
			program.setViewMatrix(viewMatrix);
			modelMatrix2.identity();
			DrawText(&program, font, lvl1, 0.2f, 0.1f);
			modelMatrix2.Translate(-2.0f, 0.5f, 0);

			program.setModelMatrix(modelMatrix3);
			program.setProjectionMatrix(projectionMatrix);
			program.setViewMatrix(viewMatrix);
			modelMatrix3.identity();
			DrawText(&program, font, lvl2, 0.2f, 0.1f);
			modelMatrix3.Translate(-2.0f, 0, 0);

			program.setModelMatrix(modelMatrix4);
			program.setProjectionMatrix(projectionMatrix);
			program.setViewMatrix(viewMatrix);
			modelMatrix4.identity();
			DrawText(&program, font, lvl3, 0.2f, 0.1f);
			modelMatrix4.Translate(-2.0f, -0.5f, 0);

			if (keys[SDL_SCANCODE_T]){
				program.setModelMatrix(modelMatrix);
				modelMatrix.identity();
				state = LEVEL_1;
			}
			if (keys[SDL_SCANCODE_Y]){
				program.setModelMatrix(modelMatrix);
				modelMatrix.identity();
				state = LEVEL_2;
			}
			if (keys[SDL_SCANCODE_U]){
				program.setModelMatrix(modelMatrix);
				modelMatrix.identity();
				state = LEVEL_3;
			}
			SDL_GL_SwapWindow(displayWindow);
		}
		if (state == LEVEL_1){
			animationElapsed += elapsed;
			if (animationElapsed > 1.0 / framesPerSecond) {
				currentIndex++;
				animationElapsed = 0.0;
				if (currentIndex > numFrames - 1) {
					currentIndex = 0;
				}
			}

			//enemy 
			animationElapsed2 += elapsed;
			if (animationElapsed2 > 1.0 / framesPerSecond2) {
				currentIndex2++;
				animationElapsed2 = 0.0;
				if (currentIndex2 > numFrames2 - 1) {
					currentIndex2 = 0;
				}
			}

			animationElapsed5 += elapsed;
			if (animationElapsed5 > 1.0 / framesPerSecond5) {
				currentIndex5++;
				animationElapsed5 = 0.0;
				if (currentIndex5 > numFrames5 - 1) {
					currentIndex5 = 1;
				}
			}

			animationElapsed6 += elapsed;
			if (animationElapsed6 > 1.0 / framesPerSecond6) {
				currentIndex6++;
				animationElapsed6 = 0.0;
				if (currentIndex6 > numFrames6 - 1) {
					currentIndex6 = 1;
				}
			}

			if (mySprite.is_jumping == false){
				if (keys[SDL_SCANCODE_S]){
					mySprite.is_jumping = true;
					Mix_PlayChannel(-1, jump, 0);
					Mix_PlayChannel(-1, land, 0);
				}
			}

			//work on acceleration and stuff
			if (mySprite.is_jumping == true){
				if (mySprite.y <= 2.5f && mySprite.jump_max == false){
					mySprite.y += 3.0f * elapsed;
					if (mySprite.y > 2.5f){
						mySprite.y = 2.5f;
					}
					if (mySprite.y == 2.5f)	{
						mySprite.jump_max = true;
					}
				}
				if (mySprite.y <= 0.0f)	{
					Mix_PlayChannel(-1, land, 0);
					mySprite.y = 0.0f;
					mySprite.jump_max = false;
					mySprite.is_jumping = false;
				}
				if (mySprite.jump_max == true)	{
					mySprite.y -= 3.0f * elapsed;
				}
			}

			if (mySprite2.is_jumping == false){
				if (keys[SDL_SCANCODE_K]){
					mySprite2.is_jumping = true;
					Mix_PlayChannel(-1, jump, 0);
					Mix_PlayChannel(-1, land, 0);
				}
			}

			//work on acceleration and stuff
			if (mySprite2.is_jumping == true){
				if (mySprite2.y < 2.5f && mySprite2.jump_max == false){
					mySprite2.y += 3.0f * elapsed;
					if (mySprite2.y > 2.5f)	{
						mySprite2.y = 2.5f;
					}
					if (mySprite2.y == 2.5f)	{
						mySprite2.jump_max = true;
					}
				}
				if (mySprite2.y <= 0.0f)	{
					Mix_PlayChannel(-1, land, 0);
					mySprite2.y = 0.0f;
					mySprite2.jump_max = false;
					mySprite2.is_jumping = false;
				}
				if (mySprite2.jump_max == true)	{
					mySprite2.y -= 3.0f * elapsed;
				}
			}
			if (mySprite.isAlive == true){
				if (mySprite.is_jumping == true){
					mySprite.Draw(&program, runAnimation5[currentIndex5], spriteCountX, spriteCountY);
				}
				if (mySprite.is_jumping == false){
					mySprite.Draw(&program, runAnimation[currentIndex], spriteCountX, spriteCountY);
					currentIndex5 = 0;
				}
			}
			if (mySprite2.isAlive == true){
				if (mySprite2.is_jumping == true){
					mySprite2.Draw(&program, runAnimation6[currentIndex6], spriteCountX, spriteCountY);
				}
				if (mySprite2.is_jumping == false){
					mySprite2.Draw(&program, runAnimation1[currentIndex], spriteCountX, spriteCountY);
					currentIndex6 = 0;
				}
			}
			int test = ticks;
			int min = 0;
			int max = 200;
			int drawMin = 2;
			int drawMax = 5;
			//attempt at "randomizing" obstacles to jump over
			//also increases the speed of the obstacles spawning
			if ((test % (rand() % (drawMax - drawMin + 1) + drawMin)) == 0) //randomize the mod #
			{
				if ((min + (rand() % (int)(max - min + 1))) == 0){
					enemy.isActive = true;
					enemy2.isActive = true;
				}
			}

			if ((test % 5) == 0){
				test2 += 0.0001f;
			}

			if (enemy.isActive == true){
				//enemy.Draw(&program, indexE, spriteCountXE, spriteCountYE);
				enemy.Draw(&program, runAnimation2[currentIndex2], spriteCountXE, spriteCountYE);
				enemyMovement -= 2.50f * elapsed * test2;
				enemy.movement = enemyMovement;
			}
			if (enemy2.isActive == true){
				enemy2.Draw(&program, runAnimation2[currentIndex2], spriteCountXE, spriteCountYE);
				enemyMovement2 -= 2.50f * elapsed * test2;
				enemy2.movement = enemyMovement2;
			}

			//note:add conditional for mySprite.isAlive later

			if ((mySprite.getY5() <= enemy.getY3() && mySprite.getY6() <= enemy.getY4())
				&& (((mySprite.getX5() <= enemy.getX6()) && (mySprite.getX5() >= enemy.getX5()))
				|| ((mySprite.getX6() <= enemy.getX6()) && mySprite.getX6() >= enemy.getX5())))
			{
				mySprite.isAlive = false;
				enemy.isActive = false;
				enemyMovement = 0.0f;
				enemy.movement = 0.0f;
			}
			//point where the obstacle disappears
			if (enemyMovement <= -7.0f){
				enemy.isActive = false;
				enemyMovement = 0.0f;
				enemy.movement = 0.0f;
			}


			if ((mySprite2.getY5() <= enemy2.getY3() && mySprite2.getY6() <= enemy2.getY4())
				&& (((mySprite2.getX5() <= enemy2.getX6()) && (mySprite2.getX5() >= enemy2.getX5()))
				|| ((mySprite2.getX6() <= enemy2.getX6()) && mySprite2.getX6() >= enemy2.getX5())))
			{
				mySprite2.isAlive = false;
				enemy2.isActive = false;
				enemyMovement2 = 0.0f;
				enemy2.movement = 0.0f;
			}
			if (enemyMovement2 <= -7.0f){
				enemy2.isActive = false;
				enemyMovement2 = 0.0f;
				enemy2.movement = 0.0f;
			}
			if (keys[SDL_SCANCODE_ESCAPE]){
				done = true;
			}
			if (mySprite.isAlive == false && mySprite2.isAlive == true){
				state = P2_WIN;
			}
			if (mySprite2.isAlive == false && mySprite.isAlive == true){
				state = P1_WIN;
			}
			if (mySprite.isAlive == false && mySprite2.isAlive == false){
				state = P_TIE;
			}
			SDL_GL_SwapWindow(displayWindow);
		}
		if (state == LEVEL_2){//run animation
			animationElapsed += elapsed;
			if (animationElapsed > 1.0 / framesPerSecond) {
				currentIndex++;
				animationElapsed = 0.0;
				if (currentIndex > numFrames - 1) {
					currentIndex = 0;
				}
			}

			//enemy 
			animationElapsed3 += elapsed;
			if (animationElapsed3 > 1.0 / framesPerSecond3) {
				currentIndex3++;
				animationElapsed3 = 0.0;
				if (currentIndex3 > numFrames3 - 1) {
					currentIndex3 = 0;
				}
			}

			//players jump animations
			animationElapsed5 += elapsed;
			if (animationElapsed5 > 1.0 / framesPerSecond5) {
				currentIndex5++;
				animationElapsed5 = 0.0;
				if (currentIndex5 > numFrames5 - 1) {
					currentIndex5 = 1;
				}
			}

			animationElapsed6 += elapsed;
			if (animationElapsed6 > 1.0 / framesPerSecond6) {
				currentIndex6++;
				animationElapsed6 = 0.0;
				if (currentIndex6 > numFrames6 - 1) {
					currentIndex6 = 1;
				}
			}

			if (mySprite.is_jumping == false){
				if (keys[SDL_SCANCODE_S]){
					mySprite.is_jumping = true;
					Mix_PlayChannel(-1, jump, 0);
					Mix_PlayChannel(-1, land, 0);
				}
			}

			//work on acceleration and stuff
			if (mySprite.is_jumping == true){
				if (mySprite.y <= 2.5f && mySprite.jump_max == false){
					mySprite.y += 1.5f * elapsed;
					if (mySprite.y > 2.5f){
						mySprite.y = 2.5f;
					}
					if (mySprite.y == 2.5f)	{
						mySprite.jump_max = true;
					}
				}
				if (mySprite.y <= 0.0f)	{
					Mix_PlayChannel(-1, land, 0);
					mySprite.y = 0.0f;
					mySprite.jump_max = false;
					mySprite.is_jumping = false;
				}
				if (mySprite.jump_max == true)	{
					mySprite.y -= 1.5f * elapsed;
				}
			}

			if (mySprite2.is_jumping == false){
				if (keys[SDL_SCANCODE_K]){
					mySprite2.is_jumping = true;
					Mix_PlayChannel(-1, jump, 0);
					Mix_PlayChannel(-1, land, 0);
				}
			}

			//work on acceleration and stuff
			if (mySprite2.is_jumping == true){
				if (mySprite2.y < 2.5f && mySprite2.jump_max == false){
					mySprite2.y += 1.5f * elapsed;
					if (mySprite2.y > 2.5f)	{
						mySprite2.y = 2.5f;
					}
					if (mySprite2.y == 2.5f)	{
						mySprite2.jump_max = true;
					}
				}
				if (mySprite2.y <= 0.0f)	{
					Mix_PlayChannel(-1, land, 0);
					mySprite2.y = 0.0f;
					mySprite2.jump_max = false;
					mySprite2.is_jumping = false;
				}
				if (mySprite2.jump_max == true)	{
					mySprite2.y -= 1.5f * elapsed;
				}
			}

			if (mySprite.isAlive == true){
				if (mySprite.is_jumping == true){
					mySprite.Draw(&program, runAnimation5[currentIndex5], spriteCountX, spriteCountY);
				}
				if (mySprite.is_jumping == false){
					mySprite.Draw(&program, runAnimation[currentIndex], spriteCountX, spriteCountY);
					currentIndex5 = 0;
				}
			}
			if (mySprite2.isAlive == true){
				if (mySprite2.is_jumping == true){
					mySprite2.Draw(&program, runAnimation6[currentIndex6], spriteCountX, spriteCountY);
				}
				if (mySprite2.is_jumping == false){
					mySprite2.Draw(&program, runAnimation1[currentIndex], spriteCountX, spriteCountY);
					currentIndex6 = 0;
				}
			}
			int test = ticks;
			int min = 0;
			int max = 200;
			int drawMin = 2;
			int drawMax = 5;
			//attempt at "randomizing" obstacles to jump over
			//also increases the speed of the obstacles spawning
			if ((test % (rand() % (drawMax - drawMin + 1) + drawMin)) == 0) //randomize the mod #
			{
				if ((min + (rand() % (int)(max - min + 1))) == 0){
					enemy.isActive = true;
					enemy2.isActive = true;
				}
			}

			if ((test % 5) == 0){
				test2 += 0.0001f;
			}

			if (enemy.isActive == true){
				//enemy.Draw(&program, indexE, spriteCountXE, spriteCountYE);
				enemy.Draw(&program, runAnimation3[currentIndex3], spriteCountXE, spriteCountYE);
				enemyMovement -= elapsed * test2;
				enemy.movement = enemyMovement;
			}
			if (enemy2.isActive == true){
				enemy2.Draw(&program, runAnimation3[currentIndex3], spriteCountXE, spriteCountYE);
				enemyMovement2 -= elapsed * test2;
				enemy2.movement = enemyMovement2;
			}

			//note:add conditional for mySprite.isAlive later

			if ((mySprite.getY5() <= enemy.getY3() && mySprite.getY6() <= enemy.getY4())
				&& (((mySprite.getX5() <= enemy.getX6()) && (mySprite.getX5() >= enemy.getX5()))
				|| ((mySprite.getX6() <= enemy.getX6()) && mySprite.getX6() >= enemy.getX5())))
			{
				mySprite.isAlive = false;
				enemy.isActive = false;
				enemyMovement = 0.0f;
				enemy.movement = 0.0f;
			}
			//point where the obstacle disappears
			if (enemyMovement <= -7.0f){
				enemy.isActive = false;
				enemyMovement = 0.0f;
				enemy.movement = 0.0f;
			}


			if ((mySprite2.getY5() <= enemy2.getY3() && mySprite2.getY6() <= enemy2.getY4())
				&& (((mySprite2.getX5() <= enemy2.getX6()) && (mySprite2.getX5() >= enemy2.getX5()))
				|| ((mySprite2.getX6() <= enemy2.getX6()) && mySprite2.getX6() >= enemy2.getX5())))
			{
				mySprite2.isAlive = false;
				enemy2.isActive = false;
				enemyMovement2 = 0.0f;
				enemy2.movement = 0.0f;
			}
			if (enemyMovement2 <= -7.0f){
				enemy2.isActive = false;
				enemyMovement2 = 0.0f;
				enemy2.movement = 0.0f;
			}
			if (keys[SDL_SCANCODE_ESCAPE]){
				done = true;
			}
			if (mySprite.isAlive == false && mySprite2.isAlive == true){
				state = P2_WIN;
			}
			if (mySprite2.isAlive == false && mySprite.isAlive == true){
				state = P1_WIN;
			}
			if (mySprite.isAlive == false && mySprite2.isAlive == false){
				state = P_TIE;
			}
			SDL_GL_SwapWindow(displayWindow);
		}
		if (state == LEVEL_3){//run animation
			animationElapsed += elapsed;
			if (animationElapsed > 1.0 / framesPerSecond) {
				currentIndex++;
				animationElapsed = 0.0;
				if (currentIndex > numFrames - 1) {
					currentIndex = 0;
				}
			}

			//enemy 
			animationElapsed4 += elapsed;
			if (animationElapsed4 > 1.0 / framesPerSecond3) {
				currentIndex4++;
				animationElapsed4 = 0.0;
				if (currentIndex4 > numFrames4 - 1) {
					currentIndex4 = 0;
				}
			}

			//players jump animations
			animationElapsed5 += elapsed;
			if (animationElapsed5 > 1.0 / framesPerSecond5) {
				currentIndex5++;
				animationElapsed5 = 0.0;
				if (currentIndex5 > numFrames5 - 1) {
					currentIndex5 = 1;
				}
			}

			animationElapsed6 += elapsed;
			if (animationElapsed6 > 1.0 / framesPerSecond6) {
				currentIndex6++;
				animationElapsed6 = 0.0;
				if (currentIndex6 > numFrames6 - 1) {
					currentIndex6 = 1;
				}
			}

			if (mySprite.is_jumping == false){
				if (keys[SDL_SCANCODE_S]){
					mySprite.is_jumping = true;
					Mix_PlayChannel(-1, jump, 0);
					Mix_PlayChannel(-1, land, 0);
				}
			}

			//work on acceleration and stuff
			if (mySprite.is_jumping == true){
				if (mySprite.y <= 2.5f && mySprite.jump_max == false){
					mySprite.y += 5.0f * elapsed;
					if (mySprite.y > 2.5f){
						mySprite.y = 2.5f;
					}
					if (mySprite.y == 2.5f)	{
						mySprite.jump_max = true;
					}
				}
				if (mySprite.y <= 0.0f)	{
					Mix_PlayChannel(-1, land, 0);
					mySprite.y = 0.0f;
					mySprite.jump_max = false;
					mySprite.is_jumping = false;
				}
				if (mySprite.jump_max == true)	{
					mySprite.y -= 5.0f * elapsed;
				}
			}

			if (mySprite2.is_jumping == false){
				if (keys[SDL_SCANCODE_K]){
					mySprite2.is_jumping = true;
					Mix_PlayChannel(-1, jump, 0);
					Mix_PlayChannel(-1, land, 0);
				}
			}

			//work on acceleration and stuff
			if (mySprite2.is_jumping == true){
				if (mySprite2.y < 2.5f && mySprite2.jump_max == false){
					mySprite2.y += 5.0f * elapsed;
					if (mySprite2.y > 2.5f)	{
						mySprite2.y = 2.5f;
					}
					if (mySprite2.y == 2.5f)	{
						mySprite2.jump_max = true;
					}
				}
				if (mySprite2.y <= 0.0f)	{
					Mix_PlayChannel(-1, land, 0);
					mySprite2.y = 0.0f;
					mySprite2.jump_max = false;
					mySprite2.is_jumping = false;
				}
				if (mySprite2.jump_max == true)	{
					mySprite2.y -= 5.0f * elapsed;
				}
			}

			if (mySprite.isAlive == true){
				if (mySprite.is_jumping == true){
					mySprite.Draw(&program, runAnimation5[currentIndex5], spriteCountX, spriteCountY);
				}
				if (mySprite.is_jumping == false){
					mySprite.Draw(&program, runAnimation[currentIndex], spriteCountX, spriteCountY);
					currentIndex5 = 0;
				}
			}
			if (mySprite2.isAlive == true){
				if (mySprite2.is_jumping == true){
					mySprite2.Draw(&program, runAnimation6[currentIndex6], spriteCountX, spriteCountY);
				}
				if (mySprite2.is_jumping == false){
					mySprite2.Draw(&program, runAnimation1[currentIndex], spriteCountX, spriteCountY);
					currentIndex6 = 0;
				}
			}
			int test = ticks;
			int min = 0;
			int max = 200;
			int drawMin = 2;
			int drawMax = 5;
			//attempt at "randomizing" obstacles to jump over
			//also increases the speed of the obstacles spawning
			if ((test % (rand() % (drawMax - drawMin + 1) + drawMin)) == 0) //randomize the mod #
			{
				if ((min + (rand() % (int)(max - min + 1))) == 0){
					enemy.isActive = true;
					enemy2.isActive = true;
				}
			}

			if ((test % 10) == 0){
				test2 += 0.0001f;
			}

			if (enemy.isActive == true){
				//enemy.Draw(&program, indexE, spriteCountXE, spriteCountYE);
				enemy.Draw(&program, runAnimation4[currentIndex4], spriteCountXE, spriteCountYE);
				enemyMovement -= 4.0f * elapsed * test2;
				enemy.movement = enemyMovement;
			}
			if (enemy2.isActive == true){
				enemy2.Draw(&program, runAnimation4[currentIndex4], spriteCountXE, spriteCountYE);
				enemyMovement2 -= 4.0f * elapsed * test2;
				enemy2.movement = enemyMovement2;
			}

			//note:add conditional for mySprite.isAlive later

			if ((mySprite.getY5() <= enemy.getY3() && mySprite.getY6() <= enemy.getY4())
				&& (((mySprite.getX5() <= enemy.getX6()) && (mySprite.getX5() >= enemy.getX5()))
				|| ((mySprite.getX6() <= enemy.getX6()) && mySprite.getX6() >= enemy.getX5())))
			{
				mySprite.isAlive = false;
				enemy.isActive = false;
				enemyMovement = 0.0f;
				enemy.movement = 0.0f;
			}
			//point where the obstacle disappears
			if (enemyMovement <= -7.0f){
				enemy.isActive = false;
				enemyMovement = 0.0f;
				enemy.movement = 0.0f;
			}


			if ((mySprite2.getY5() <= enemy2.getY3() && mySprite2.getY6() <= enemy2.getY4())
				&& (((mySprite2.getX5() <= enemy2.getX6()) && (mySprite2.getX5() >= enemy2.getX5()))
				|| ((mySprite2.getX6() <= enemy2.getX6()) && mySprite2.getX6() >= enemy2.getX5())))
			{
				mySprite2.isAlive = false;
				enemy2.isActive = false;
				enemyMovement2 = 0.0f;
				enemy2.movement = 0.0f;
			}
			if (enemyMovement2 <= -7.0f){
				enemy2.isActive = false;
				enemyMovement2 = 0.0f;
				enemy2.movement = 0.0f;
			}
			if (keys[SDL_SCANCODE_ESCAPE]){
				done = true;
			}
			if (mySprite.isAlive == false && mySprite2.isAlive == true){
				state = P2_WIN;
			}
			if (mySprite2.isAlive == false && mySprite.isAlive == true){
				state = P1_WIN;
			}
			if (mySprite.isAlive == false && mySprite2.isAlive == false){
				state = P_TIE;
			}
			SDL_GL_SwapWindow(displayWindow);
		}
		if (state == P1_WIN)
		{
			Mix_PlayChannel(-1, rit, 0);
			DrawText(&program, font, p1W, 0.2f, 0.1f);
			modelMatrix.identity();
			modelMatrix.Translate(-2.0f, 1.0f, 0);

			program.setModelMatrix(modelMatrix2);
			program.setProjectionMatrix(projectionMatrix);
			program.setViewMatrix(viewMatrix);
			modelMatrix2.identity();
			DrawText(&program, font, message, 0.2f, 0.1f);
			modelMatrix2.Translate(-2.0f, 0.5f, 0);
			SDL_GL_SwapWindow(displayWindow);

			if (keys[SDL_SCANCODE_ESCAPE]){
				done = true;
			}
		}
		if (state == P2_WIN)
		{
			Mix_PlayChannel(-1, rit, 0);
			DrawText(&program, font, p2W, 0.2f, 0.1f);
			modelMatrix.identity();
			modelMatrix.Translate(-2.0f, 1.0f, 0);

			program.setModelMatrix(modelMatrix2);
			program.setProjectionMatrix(projectionMatrix);
			program.setViewMatrix(viewMatrix);
			modelMatrix2.identity();
			DrawText(&program, font, message, 0.2f, 0.1f);
			modelMatrix2.Translate(-2.0f, 0.5f, 0);
			SDL_GL_SwapWindow(displayWindow);

			if (keys[SDL_SCANCODE_ESCAPE]){
				done = true;
			}
		}
		if (state == P_TIE)
		{
			Mix_PlayChannel(-1, rit, -1);
			DrawText(&program, font, pTie, 0.2f, 0.1f);
			modelMatrix.identity();
			modelMatrix.Translate(-2.0f, 1.0f, 0);

			program.setModelMatrix(modelMatrix2);
			program.setProjectionMatrix(projectionMatrix);
			program.setViewMatrix(viewMatrix);
			modelMatrix2.identity();
			DrawText(&program, font, message, 0.2f, 0.1f);
			modelMatrix2.Translate(-2.0f, 0.5f, 0);
			SDL_GL_SwapWindow(displayWindow);

			if (keys[SDL_SCANCODE_ESCAPE]){
				done = true;
			}
		}
	}
	SDL_Quit();
}


void renderGame()
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Final Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	SDL_Event event;
	bool done = false;
	
#ifdef _WINDOWS
	glewInit();
#endif
	
	state = MAIN_MENU;
	game();
}


int main(int argc, char *argv[])
{
	/*
#ifdef _WIN32
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
#endif
	*/
	renderGame(); 
	return 0;
}
