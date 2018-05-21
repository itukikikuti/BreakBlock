#include <string>
#include "XLibrary11.hpp"
using namespace std;
using namespace XLibrary11;

int MAIN()
{
	const int width = 7;
	const int height = 3;
	const float ballSpeed = 5.0f;
	const float playerSpeed = 5.0f;

	Camera camera;
	camera.color = Float4(0.0f, 0.0f, 0.0f, 1.0f);

	Sprite block(L"block.png");
	block.scale.x = 2.0f;

	Float3 blockPositions[width][height];
	int blockLift[width][height];

	Sprite ball(L"ball.png");
	Float3 ballVelocity;

	Sprite player(L"ball.png");
	player.position = Float3(0.0f, -200.0f, 0.0f);
	player.scale.x = 5.0f;

	int score = 0;

	Sound hitSound(L"hit.wav");
	Sound pointSound(L"point.wav");

	App::SetWindowSize(320, 480);

	bool initFlag = true;

	while (App::Refresh())
	{
		camera.Update();

		if (initFlag)
		{
			initFlag = false;

			for (int x = 0; x < width; x++)
			{
				for (int y = 0; y < height; y++)
				{
					Float3 offset(-width / 2 * 35.0f, 160.0f, 0.0f);
					blockPositions[x][y] = Float3(x * 35.0f, y * 20.0f, 0.0f) + offset;
					blockLift[x][y] = 3;
				}
			}

			ball.position = Float3(0.0f, 0.0f, 0.0f);
			ballVelocity = Float3(0.0f, ballSpeed, 0.0f);
		}

		initFlag = true;

		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				if (blockLift[x][y] <= 0)
					continue;

				initFlag = false;

				block.position = blockPositions[x][y];

				Float3 now = ball.position;
				Float3 next = ball.position + ballVelocity;

				if (next.x > block.position.x - 24.0f &&
					next.x < block.position.x + 24.0f &&
					now.y > block.position.y - 16.0f &&
					now.y < block.position.y + 16.0f)
				{
					ballVelocity.x = -ballVelocity.x;
					blockLift[x][y]--;
					pointSound.Play();
				}

				if (now.x > block.position.x - 24.0f &&
					now.x < block.position.x + 24.0f &&
					next.y > block.position.y - 16.0f &&
					next.y < block.position.y + 16.0f)
				{
					ballVelocity.y = -ballVelocity.y;
					blockLift[x][y]--;
					pointSound.Play();
				}

                if (blockLift[x][y] == 3)
                {
                    block.color = Float4(0.0f, 1.0f, 0.0f, 1.0f);
                }
                else if (blockLift[x][y] == 2)
                {
                    block.color = Float4(1.0f, 1.0f, 0.0f, 1.0f);
                }
                else if (blockLift[x][y] == 1)
                {
                    block.color = Float4(1.0f, 0.0f, 0.0f, 1.0f);
                }

				block.Draw();
			}
		}

		ball.position += ballVelocity;

		if (ball.position.y > App::GetWindowSize().y / 2.0f)
		{
			ballVelocity.y = -fabsf(ballVelocity.y);
			hitSound.Play();
		}
		if (ball.position.y < -App::GetWindowSize().y / 2.0f)
		{
			initFlag = true;
		}
		if (ball.position.x > App::GetWindowSize().x / 2.0f)
		{
			ballVelocity.x = -fabsf(ballVelocity.x);
			hitSound.Play();
		}
		if (ball.position.x < -App::GetWindowSize().x / 2.0f)
		{
			ballVelocity.x = fabsf(ballVelocity.x);
			hitSound.Play();
		}

		ball.Draw();

		if (App::GetKey(VK_RIGHT))
		{
			player.position.x += playerSpeed;
		}
		if (App::GetKey(VK_LEFT))
		{
			player.position.x -= playerSpeed;
		}

		if (player.position.x > App::GetWindowSize().x / 2.0f)
		{
			player.position.x = App::GetWindowSize().x / 2.0f;
		}
		if (player.position.x < -App::GetWindowSize().x / 2.0f)
		{
			player.position.x = -App::GetWindowSize().x / 2.0f;
		}

		if (player.position.x - 48.0f < ball.position.x &&
			player.position.x + 48.0f > ball.position.x &&
			player.position.y - 16.0f < ball.position.y &&
			player.position.y + 16.0f > ball.position.y)
		{
			ballVelocity.x = (ball.position.x - player.position.x) * 0.2f;
			ballVelocity.y = fabsf(ballSpeed);
			hitSound.Play();
		}

		player.Draw();
	}

	return 0;
}
