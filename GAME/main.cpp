#include <stdio.h>
#include "SDL.h"
#include <thread>
#include <boost/asio.hpp>
#include "game.h"
using namespace boost::asio;
using  boost::asio::ip::tcp;

#include "game_client.h"

#define WINDOW_WIDTH  800  //窗体的宽度
#define WINDOW_HEIGHT 600  //窗体的高度

enum WALK_DIR
{
	DOWN_DIR = 0,
	LEFT_DIR = 1,
	RIGHT_DIR = 2,
	UP_DIR = 3
};


boost::asio::io_service g_io_service;



int main(int argc, char** argv)
{
	boost::asio::io_service io_service;
	tcp::resolver resolver(g_io_service);
	tcp::resolver::query query("127.0.0.1", "6666");
	tcp::resolver::iterator iterator = resolver.resolve(query);

	game game_(io_service, iterator);

	game_.connect_server();

	SDL_Init(SDL_INIT_VIDEO);  //初始化SDL

	SDL_Window* window = SDL_CreateWindow("PalOL",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
	);

	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface* Image = SDL_LoadBMP("lxy2.bmp");

	SDL_SetColorKey(Image, SDL_TRUE, SDL_MapRGB(Image->format, 255, 0, 255)); //去掉背景色

	SDL_Texture* Texture = SDL_CreateTextureFromSurface(rend, Image);

	int i, j;
	int imgWidth = Image->w / 4;      //每一个图像宽
	int imgHeight = Image->h / 4;     //图片高度/ 4 即图像高度

	SDL_Rect position;
	position.x = 0;
	position.y = 0;
	position.w = imgWidth;
	position.h = imgHeight;

	SDL_Rect clips[4][4];

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			clips[i][j].x = j * imgWidth;
			clips[i][j].y = i * imgHeight;
			clips[i][j].w = imgWidth;
			clips[i][j].h = imgHeight;
		}
	}
	i = 0;
	j = 0;

	SDL_Event game_event;
	bool gameOver = false;

	while (gameOver == false)
	{
		while (SDL_PollEvent(&game_event))
		{
			switch (game_event.type)
			{
			case SDL_QUIT:
				gameOver = true;
				break;
			case SDL_KEYDOWN:
			{
				if (game_event.key.keysym.sym == SDLK_ESCAPE)
				{
					gameOver = true;
					break;
				}
				else if (game_event.key.keysym.sym == SDLK_DOWN)
				{
					i = DOWN_DIR;
					//i = i + 1;
					//if ( i >=4 )
					//{
					//	i = 0;
					//}
					position.y += (imgHeight / 4);
				}
				else if (game_event.key.keysym.sym == SDLK_LEFT)
				{
					i = LEFT_DIR;
					position.x -= (imgWidth / 4);
				}
				else if (game_event.key.keysym.sym == SDLK_RIGHT)
				{
					i = RIGHT_DIR;
					position.x += (imgWidth / 4);
				}
				else if (game_event.key.keysym.sym == SDLK_UP)
				{
					i = UP_DIR;
					position.y -= (imgHeight / 4);
				}
				j = (j + 1) % 4;
			}
			break;
			default:
				break;
			}
		}

		//渲染的清理
		SDL_RenderClear(rend);
		//拷贝纹理到渲染
		SDL_RenderCopy(rend, Texture, &clips[i][j], &position);

		SDL_RenderPresent(rend);
		SDL_Delay(100);
		if (gameOver)
		{
			game_.disconnect();
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	return 0;
}