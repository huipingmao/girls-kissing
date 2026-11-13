#include <iostream>
#include <raylib.h>
#include <vector>

class Player {
	public:
	// float x;
	// float y;
	Vector2 pos;
	Vector2 prev_pos;
	float x_vel;
	float y_vel;
	int health;
	bool in_air;
	bool is_jump;
	bool can_jump;
	char facing;
	Rectangle hitbox;
	Rectangle hurtbox;
	Texture2D texture;

	void gravity ()
	{
		#define MAXSPEED 1800
		// #define MAXSPEED 1200
		#define ACCELERATION 20

		if (y_vel < MAXSPEED) y_vel += ACCELERATION;
		else y_vel = MAXSPEED;

		#undef MAXSPEED
		#undef ACCELERATION
	};

	void jump () 
	{
	// 	#define MAXSPEED 5000
	// 	#define ACCELERATION 100

		// y_vel -= 3500;
		y_vel -= 1400;
		// is_jump = false;
		can_jump = false;
		in_air = true;
	// 	y_vel += ACCELERATION;
	// 	if (y_vel < MAXSPEED) jump();

	// 	#undef MAXSPEED
	// 	#undef ACCELERATION
	};

	void move_left_right (char dir)
	{
		#define MAXSPEED 500
		#define ACCELERATION 123
		// int maxspeed = 500;
		// int acceleration = 1000;

		switch (dir) {
			case 'r':
				// if (facing == 'l') break;
				// if (x_vel < 0) x_vel += ACCELERATION * 2;
				if (x_vel < 0) x_vel += ACCELERATION;
				else x_vel += ACCELERATION;
				break;

			case 'l':
				// if (facing == 'r') break;
				// if (x_vel > 0) x_vel -= ACCELERATION * 2;
				if (x_vel > 0) x_vel -= ACCELERATION;
				else x_vel -= ACCELERATION;
				break;

			default:
				if (x_vel > 0)
					if (x_vel - ACCELERATION > 0)
						x_vel -= ACCELERATION;
					else
						x_vel = 0;

				if (x_vel < 0)
					if (x_vel + ACCELERATION < 0)
						x_vel += ACCELERATION;
					else
						x_vel = 0;
				break;
		}

		if (x_vel > MAXSPEED) x_vel = MAXSPEED;
		if (x_vel < -MAXSPEED) x_vel = -MAXSPEED;


		#undef MAXSPEED
		#undef ACCELERATION
	};

	void update_pos ()
	{
		if (prev_pos.x != pos.x) prev_pos.x = pos.x;
		if (prev_pos.y != pos.y) prev_pos.y = pos.y;
		pos.x += x_vel * GetFrameTime();
		pos.y += y_vel * GetFrameTime();
		hitbox.x = pos.x;
		hitbox.y = pos.y;
	}
};

typedef class {
	public:
	Rectangle hitbox;
	Texture2D texture;
} Platform;

int frame = 0;

int main ()
{
	InitWindow(1000, 800, "girls kissing gaem");
	SetTargetFPS(240);

	Font mona = LoadFontEx("resources/MonaspaceRadonFrozen-Regular.ttf", 20, 0, 250);
	Texture2D girls_kissing = LoadTexture("resources/sprites/bokita.png");
	Texture2D nijika = LoadTexture("resources/sprites/nijika.gif");

	Camera2D camera = {0};

	Player player = (Player) {
		.pos = Vector2 {700, 100},
		.prev_pos = Vector2 {player.pos.x, player.pos.y},
		.x_vel = 0,
		.y_vel = 0,
		.health = 3,
		.in_air = false,
		.is_jump = true,
		.can_jump = true,
		.facing = 'l',
		.hitbox = (Rectangle) {player.pos.x, player.pos.y, 50, 100},
		.hurtbox = (Rectangle) {player.pos.x, player.pos.y, 0, 0},
		// .texture = girls_kissing,
		.texture = nijika,
	};

	std::vector<Platform> platforms;
	platforms.push_back((Platform){(Rectangle){800, 300, 300, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){300, 450, 300, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){500, 600, 300, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){500, 450, 30, 300}, nijika});
	platforms.push_back((Platform){(Rectangle){20, 700, 300, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){20, 600, 30, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){100, 300, 30, 30}, nijika});

	while (!WindowShouldClose()) {

		frame++;


		if (IsKeyPressed(KEY_Q)) CloseWindow();
		if (IsKeyPressed(KEY_R)) { player.pos.x = 700; player.pos.y = 100;
							 player.prev_pos.x = 700; player.prev_pos.y = 100; }

		char dir = ' ';
		if (IsKeyPressed(KEY_A)) player.facing = 'l';
		if (IsKeyPressed(KEY_D)) player.facing = 'r';

		if (IsKeyDown(KEY_A) && IsKeyReleased(KEY_D)) player.facing = 'l';
		if (IsKeyDown(KEY_D) && IsKeyReleased(KEY_A)) player.facing = 'r';

		if (IsKeyDown(KEY_A) && player.facing == 'l') dir = 'l';
		if (IsKeyDown(KEY_D) && player.facing == 'r') dir = 'r';


		player.in_air = true;

		player.move_left_right(dir);
		if (player.in_air) player.gravity();

		for (auto i : platforms) {
			if (CheckCollisionRecs(player.hitbox, i.hitbox)) {

				Rectangle tmp = GetCollisionRec(player.hitbox, i.hitbox);
				// printf("%f %f\n", tmp.height, tmp.width);

				bool is_in_range_of_x = true;

				if (
					i.hitbox.x > player.prev_pos.x
					&&
					i.hitbox.x > player.prev_pos.x + player.hitbox.width
					||
					i.hitbox.x + i.hitbox.width < player.prev_pos.x
					&&
					i.hitbox.x  + i.hitbox.width < player.prev_pos.x + player.hitbox.width
				) 
					{ is_in_range_of_x = false; puts("isnt in range"); };

				if (is_in_range_of_x) {
					if (player.pos.y > player.prev_pos.y) { // from the top
						player.pos.y = player.prev_pos.y;
						player.y_vel = 0;
						player.in_air = false;
					}

					if (player.pos.y < player.prev_pos.y) { // from below
						player.pos.y = player.prev_pos.y;
						player.y_vel = 0;
					}
				}

				if (player.pos.y + player.hitbox.height > i.hitbox.y) {
					printf("player y pos: %f, i y pos: %f\n", player.pos.y + player.hitbox.height, i.hitbox.y);
					if (player.pos.x != player.prev_pos.x) { // both dirs
						player.pos.x = player.prev_pos.x;
						player.x_vel = 0;
						// player.x_vel = -player.x_vel;
					}
				}
			}
			/* ... if (CheckCollisionRecs(player.hitbox, i.hitbox)) */
		}
		/* ... for () */

		if (IsKeyReleased(KEY_SPACE)) { player.can_jump = true; }
		if (IsKeyDown(KEY_SPACE) && !player.in_air && player.can_jump) { player.jump(); }

		player.update_pos();

		camera.target = (Vector2) {player.pos.x + player.hitbox.width / 2, player.pos.y - 50};
		camera.offset = (Vector2){ GetRenderWidth()/2.0f, GetRenderHeight()/2.0f };
		camera.rotation = 0.0f;
		camera.zoom = 1.0f;


		BeginDrawing();

		BeginMode2D(camera);

		// DrawTexture(nijika, 100, 100, WHITE);
		// DrawTexturePro(player.texture, (Rectangle) {150, 200, player.hitbox.width * 4, player.hitbox.height * 4}, player.hitbox, (Vector2) {0, 0}, 0, WHITE);
		DrawTexturePro(player.texture, (Rectangle) {0, 0, 347, 347}, player.hitbox, (Vector2) {0, 0}, 0, WHITE);

		for (auto i : platforms) {
			// DrawRectangleRec(i.hitbox, GREEN);
			DrawTexturePro(i.texture, (Rectangle) {0, 0, 347, 347}, i.hitbox, Vector2 {0, 0}, 0, WHITE);
		}

		ClearBackground(Color{ 0x11, 0x00, 0x11, 0xff});
		EndMode2D();

		DrawFPS(5, 2);

		DrawTextEx(mona, "91rl5 ki55in9.", (Vector2){ 1.0f, 20.0f }, (float)mona.baseSize, 2, RED);
		DrawTextEx(mona, "girls kissing - girls kissing - girls kissing - girls kissing", (Vector2){ 1.0f, 40.0f }, (float)mona.baseSize, 2, ORANGE);
		DrawTextEx(mona, TextFormat("x_vel %f\ny_vel %f\n", player.x_vel, player.y_vel), (Vector2){ 1.0f, 60.0f }, (float)mona.baseSize, 2, YELLOW);
		DrawTextEx(mona, TextFormat("frame no. %d\n", frame), (Vector2){ 1.0f, 100.0f }, (float)mona.baseSize, 2, GREEN);
		DrawTextEx(mona, TextFormat("frame time: %f\n", GetFrameTime()), (Vector2){ 1.0f, 120.0f }, (float)mona.baseSize, 2, BLUE);

		// GetWorldToScreen2D(player.pos, camera);

		EndDrawing();
		printf("x vel: %f y vel: %f facing: %c\n", player.x_vel, player.y_vel, player.facing);
		// printf("x: %f y: %f\n", player.x, player.y);
		// printf("x hitbox: %f y hitbox: %f\n", player.hitbox.x, player.hitbox.y);
		// printf("is jump: %d in air: %d\n", player.is_jump, player.in_air);
	}
}
