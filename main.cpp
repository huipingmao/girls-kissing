#include <iostream>
#include <cmath>
#include <raylib.h>
#include <vector>
#include <algorithm>

class Platform {
	public:
	Rectangle hitbox;
	Texture2D texture;
	Color color = WHITE;

	void move_in_cycles (float move_x, float move_y, float speed, int wait)
	{
		puts("wip");
	};
};

	// Player player = (Player) {
	// 	// .pos = Vector2 {700, 100},
	// 	.pos = Vector2 {100, 100},
	// 	.prev_pos = Vector2 {player.pos.x, player.pos.y},
	// 	.x_vel = 0,
	// 	.y_vel = 0,
	// 	.health = 3,
	// 	.coyote = 0,
	// 	.in_air = false,
	// 	.is_grounded = false,
	// 	.is_jump = true,
	// 	.can_jump = true,
	// 	.facing = 'l',
	// 	.hitbox = (Rectangle) {player.pos.x, player.pos.y, 50, 100},
	// 	.hurtbox = (Rectangle) {player.pos.x, player.pos.y, 0, 0},
	// 	// .texture = girls_kissing,
	// 	.texture = nijika,
	// };

class Player {
	public:
	Vector2 pos = (Vector2) {100, 100};
	Vector2 prev_pos = pos;
	float x_vel = 0;
	float y_vel = 0;
	float max_speed = 500;
	float acceleration = 100;
	int health = 1;
	int coyote = 0;
	bool in_air = false;
	bool is_grounded = false;
	bool is_jump = true;
	bool can_jump = true;
	char facing = 'l';
	Rectangle hitbox = (Rectangle) {pos.x, pos.y, 50, 100};
	Rectangle scanbox = (Rectangle) {hitbox.x - acceleration, hitbox.y - acceleration, hitbox.width + acceleration * 2, hitbox.height + acceleration * 2};
	Rectangle hurtbox = (Rectangle) {hitbox.x + 10, hitbox.y + 10, hitbox.width - 10, hitbox.height - 10};
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
		y_vel -= 1400;
		can_jump = false;
		in_air = true;
	};

	void move_left_right (char dir)
	{
		#define RUN_MAXSPEED 500
		// #define RUN_MAXSPEED 9000
		#define RUN_ACCELERATION 100
		// int maxspeed = 500;
		// int acceleration = 1000;

		switch (dir) {
			case 'r':
				if (x_vel < 0) x_vel += RUN_ACCELERATION;
				else x_vel += RUN_ACCELERATION;
				break;

			case 'l':
				if (x_vel > 0) x_vel -= RUN_ACCELERATION;
				else x_vel -= RUN_ACCELERATION;
				break;

			default:
				if (x_vel > 0) {
					if (x_vel - RUN_ACCELERATION > 0)
						x_vel -= RUN_ACCELERATION;
					else
						x_vel = 0;
				}

				if (x_vel < 0) {
					if (x_vel + RUN_ACCELERATION < 0)
						x_vel += RUN_ACCELERATION;
					else
						x_vel = 0;
				}
				break;
		}

		if (x_vel > RUN_MAXSPEED) x_vel = RUN_MAXSPEED;
		if (x_vel < -RUN_MAXSPEED) x_vel = -RUN_MAXSPEED;
	};

	void check_collisions (Platform *platform)
	{
		// if (CheckCollisionRecs(hitbox, platform->hitbox)) {

		// 	if (pos.y > prev_pos.y) { // from the top
		// 		pos.y = prev_pos.y;
		// 		y_vel = 0;
		// 		is_grounded = true;
		// 	}
		// }
	};

	void cool_collision_detection (Platform *platform)
	{
		if (CheckCollisionRecs(hitbox, platform->hitbox)) {

			// Rectangle tmp = GetCollisionRec(hitbox, platform->hitbox);

			// Rectangle tmp = { pos.x  / 2 - RUN_ACCELERATION, pos.y / 2 - RUN_ACCELERATION , hitbox.width + RUN_ACCELERATION * 2, hitbox.height + RUN_ACCELERATION * 2};
			// std::vector<std::pair<>>;

			bool is_in_range_of_x = true;

			if (
				platform->hitbox.x > prev_pos.x
				&&
				platform->hitbox.x > prev_pos.x + hitbox.width
				||
				platform->hitbox.x + platform->hitbox.width < prev_pos.x
				&&
				platform->hitbox.x  + platform->hitbox.width < prev_pos.x + hitbox.width
			) // 2 trivial cases where both points of a platform cannot be in x range of rectangle 
				{ is_in_range_of_x = false; /* puts("isnt in range"); */ };

			if (is_in_range_of_x) {
				if (pos.y > prev_pos.y) { // from the top
					pos.y = prev_pos.y;
					y_vel = 0;
					is_grounded = true;
				}

				if (pos.y < prev_pos.y) { // from below
					pos.y = prev_pos.y;
					y_vel = 0;
					return;
				}
			}

			if (pos.y + hitbox.height > platform->hitbox.y && pos.y < platform->hitbox.y + platform->hitbox.height) { // check for x collisions when isnt atop platforms y axis and isnt below platforms y axis
				// if (pos.x != prev_pos.x) { // both dirs
				// 	pos.x = prev_pos.x;
				// 	x_vel = 0;
				// }
				if (pos.x > prev_pos.x) {
					pos.x = platform->hitbox.x - hitbox.width - 0.01;
					x_vel = 0;
				}
				else if (pos.x < prev_pos.x) {
					pos.x = platform->hitbox.x + platform->hitbox.width + 0.01;
					x_vel = 0;
				}
			}
		}
	};

	float distance_from_player (Platform *p) {
		// float player_y_left = pos.y;
		// float player_y_right = pos.y + hitbox.width;
		// float player_x_left = pos.x;
		// float player_x_right = pos.x + hitbox.width;
		float player_y_left = prev_pos.y;
		float player_y_right = prev_pos.y + hitbox.width;
		float player_x_left = prev_pos.x;
		float player_x_right = prev_pos.x + hitbox.width;
		float platform_y_left = p->hitbox.y;
		float platform_y_right = p->hitbox.y + p->hitbox.width;
		float platform_x_left = p->hitbox.x;
		float platform_x_right = p->hitbox.x + p->hitbox.width;

		float dx = std::max({0.0f, (player_x_left - platform_x_right), (platform_x_left - player_x_right)});
		float dy = std::max({0.0f, (player_y_left - platform_y_right), (platform_y_left - player_x_right)});
		// float dx = std::max(0.0f, std::max(player_x_left - platform_x_right, platform_x_left - player_x_right));
		// float dy = std::max(0.0f, std::max(player_y_left - platform_y_right, platform_y_left - player_x_right));
		// printf("distance from player = %f\n", sqrt(dx*dx + dy*dy));
		return sqrt(dx*dx + dy*dy);


		// float dist_x = 0;
		// float dist_y = 0;

		// dist_x = (pos.x + hitbox.width / 2) - p->hitbox.x;
		// if (dist_x < 0) dist_x = -dist_x;
		// dist_y = (pos.y + hitbox.height / 2) - p->hitbox.y;
		// if (dist_y < 0) dist_y = -dist_y;

		// return dist_x + dist_y;
		// if (dist_x >= dist_y) {
		// 	return dist_x;
		// } else {
		// 	return dist_y;
		// }
	}

	void update_pos ()
	{
		if (prev_pos.x != pos.x) prev_pos.x = pos.x;
		if (prev_pos.y != pos.y) prev_pos.y = pos.y;
		pos.x += x_vel * GetFrameTime();
		pos.y += y_vel * GetFrameTime();

		hitbox.x = pos.x;
		hitbox.y = pos.y;

		hurtbox.x = pos.x + 10;
		hurtbox.y = pos.y + 10;

		scanbox.x = pos.x - acceleration;
		scanbox.y = pos.y - acceleration;
	};
};

int frame = 0;

int main ()
{
	InitWindow(1000, 800, "girls kissing gaem");
	SetTargetFPS(240);

	Font mona = LoadFontEx("resources/MonaspaceRadonFrozen-Regular.ttf", 20, 0, 250);
	Texture2D girls_kissing = LoadTexture("resources/sprites/bokita.png");
	Texture2D nijika = LoadTexture("resources/sprites/nijika.gif");

	Camera2D camera = {0};

	// Player player = (Player) {
	// 	// .pos = Vector2 {700, 100},
	// 	.pos = Vector2 {100, 100},
	// 	.prev_pos = Vector2 {player.pos.x, player.pos.y},
	// 	.x_vel = 0,
	// 	.y_vel = 0,
	// 	.health = 3,
	// 	.coyote = 0,
	// 	.in_air = false,
	// 	.is_grounded = false,
	// 	.is_jump = true,
	// 	.can_jump = true,
	// 	.facing = 'l',
	// 	.hitbox = (Rectangle) {player.pos.x, player.pos.y, 50, 100},
	// 	.hurtbox = (Rectangle) {player.pos.x, player.pos.y, 0, 0},
	// 	// .texture = girls_kissing,
	// 	.texture = nijika,
	// };

	Player player;
	player.texture = nijika;

	std::vector<Platform> platforms;
	platforms.push_back((Platform){(Rectangle){800, 300, 300, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){300, 450, 300, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){500, 600, 300, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){500, 250, 30, 300}, nijika});
	platforms.push_back((Platform){(Rectangle){20, 700, 300, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){20, 600, 30, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){100, 300, 30, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){320, 200, 30, 300}, nijika});
	platforms.push_back((Platform){(Rectangle){100, 730, 9900, 10}, nijika});
	platforms.push_back((Platform){(Rectangle){1200, 600, 200, 10}, nijika});
	platforms.push_back((Platform){(Rectangle){1400, 600, 200, 10}, nijika});
	platforms.push_back((Platform){(Rectangle){1620, 600, 200, 10}, nijika});
	platforms.push_back((Platform){(Rectangle){4000, 200, 5, 1000}, nijika});

	float camera_x_follow = player.pos.x + player.hitbox.width / 2;
	// float camera_steps = 0;

	while (!WindowShouldClose()) {

		// frame++;

		if (IsKeyPressed(KEY_Q)) CloseWindow();
		if (IsKeyPressed(KEY_R)) { player.pos.x = 700; player.pos.y = 100;
							 player.prev_pos.x = 700; player.prev_pos.y = 100; }

		if (player.pos.y > 1200) { player.pos.x = 700; player.pos.y = 100; // tmp
							 player.prev_pos.x = 700; player.prev_pos.y = 100; }

		char dir = ' ';
		if (IsKeyPressed(KEY_A)) player.facing = 'l';
		if (IsKeyPressed(KEY_D)) player.facing = 'r';

		if (IsKeyDown(KEY_A) && IsKeyReleased(KEY_D)) player.facing = 'l';
		if (IsKeyDown(KEY_D) && IsKeyReleased(KEY_A)) player.facing = 'r';

		if (IsKeyDown(KEY_A) && player.facing == 'l') dir = 'l';
		if (IsKeyDown(KEY_D) && player.facing == 'r') dir = 'r';

		player.move_left_right(dir);

		player.is_grounded = false;

		// for (auto i : platforms) {
			// if (CheckCollisionRecs(player.hitbox, i.hitbox)) {

			// 	// Rectangle tmp = GetCollisionRec(player.hitbox, i.hitbox);
			// 	bool is_in_range_of_x = true;

			// 	if (
			// 		i.hitbox.x > player.prev_pos.x
			// 		&&
			// 		i.hitbox.x > player.prev_pos.x + player.hitbox.width
			// 		||
			// 		i.hitbox.x + i.hitbox.width < player.prev_pos.x
			// 		&&
			// 		i.hitbox.x  + i.hitbox.width < player.prev_pos.x + player.hitbox.width
			// 	) // 2 trivial cases where both points of a platform cannot be in x range of player rectangle 
			// 		{ is_in_range_of_x = false; /* puts("isnt in range"); */ };

			// 	if (is_in_range_of_x) {
			// 		if (player.pos.y > player.prev_pos.y) { // from the top
			// 			player.pos.y = player.prev_pos.y;
			// 			player.y_vel = 0;
			// 			player.is_grounded = true;
			// 		}

			// 		if (player.pos.y < player.prev_pos.y) { // from below
			// 			player.pos.y = player.prev_pos.y;
			// 			player.y_vel = 0;
			// 			continue;
			// 		}
			// 	}

			// 	if (player.pos.y + player.hitbox.height > i.hitbox.y && player.pos.y < i.hitbox.y + i.hitbox.height) { // check for x collisions when player isnt atop platforms y axis and isnt below platforms y axis
			// 		// if (player.pos.x != player.prev_pos.x) { // both dirs
			// 		// 	player.pos.x = player.prev_pos.x;
			// 		// 	player.x_vel = 0;
			// 		// }
			// 		if (player.pos.x > player.prev_pos.x) {
			// 			player.pos.x = i.hitbox.x - player.hitbox.width - 0.01;
			// 			player.x_vel = 0;
			// 		}
			// 		else if (player.pos.x < player.prev_pos.x) {
			// 			player.pos.x = i.hitbox.x + i.hitbox.width + 0.01;
			// 			player.x_vel = 0;
			// 		}
			// 	}
			// }
			// /* ... if (CheckCollisionRecs(player.hitbox, i.hitbox)) */
		// }
		/* ... for () */

		{  // COLLISIONS
			// std::vector<Platform> possible_collisions_list;
			// std::vector< std::pair<Platform, float> > possible_collisions_list;
			std::vector< std::pair<int, float> > possible_collisions_list;
			// for (auto i : platforms) {
			for (unsigned int i = 0; i < size(platforms); i++) {
				// if (CheckCollisionRecs(player.scanbox, i.hitbox)) possible_collisions_list.push_back(i);
				platforms[i].color = WHITE;
				if (CheckCollisionRecs(player.scanbox, platforms[i].hitbox)) { possible_collisions_list.push_back( {i, player.distance_from_player( &platforms[i])} ); platforms[i].color = BLUE; }
				// if (CheckCollisionRecs(player.scanbox, platforms[i].hitbox)) possible_collisions_list.push_back( {i, 3} );
			}
			std::sort(possible_collisions_list.begin(), possible_collisions_list.end(),
					[](
						const std::pair<int, float> &a,
						const std::pair<int, float> &b
					) 
					{ return a.second < b.second; }
			);
			if (!possible_collisions_list.empty()) { std::cout << possible_collisions_list[0].first << "\n"; platforms[possible_collisions_list[0].first].color = RED; }
			// printf("size of possible_collisions_list: %d\n", size(possible_collisions_list));
			// std::cout << possible_collisions_list.front() << "\n";
			// std::cout << "bocchi kita" << "\n";
			// printf("nearest rect: %d\n", possible_collisions_list[0].first);
			// for (auto i : possible_collisions_list) {
			for (auto i : possible_collisions_list) {
				player.cool_collision_detection(&platforms[i.first]);
			}
		}

		if (player.is_grounded) player.coyote = 8;
		else if (player.coyote && !player.is_grounded) player.coyote--;

		if (!player.is_grounded) player.gravity();
 
		if (IsKeyReleased(KEY_SPACE)) { player.can_jump = true; }
		if (IsKeyDown(KEY_SPACE) && player.is_grounded && player.can_jump) { player.jump(); player.is_grounded = false; player.coyote = 0; }
		else if (IsKeyDown(KEY_SPACE) && player.coyote && player.can_jump) { player.jump(); player.is_grounded = false; player.coyote = 0; }

		player.update_pos();

		if (camera_x_follow < player.pos.x + player.hitbox.width / 2 - 60) camera_x_follow = player.pos.x + player.hitbox.width / 2 - 60;
		if (camera_x_follow > player.pos.x + player.hitbox.width / 2 + 60) camera_x_follow = player.pos.x + player.hitbox.width / 2 + 60;
		camera.target = (Vector2) {camera_x_follow, 400};
		camera.offset = (Vector2){ GetRenderWidth()/2.0f, GetRenderHeight()/2.0f };
		camera.rotation = 0.0f;
		camera.zoom = 0.8f;

		BeginDrawing();

		BeginMode2D(camera);
		ClearBackground(Color{0x11, 0x00, 0x11, 0xff});
		DrawRectangleRec(player.scanbox, Color{0x00, 0xcc, 0x00, 0x44});


		// DrawTexturePro(player.texture, (Rectangle) {150, 200, player.hitbox.width * 4, player.hitbox.height * 4}, player.hitbox, (Vector2) {0, 0}, 0, WHITE);
		DrawTexturePro(player.texture, (Rectangle) {0, 0, 347, 347}, player.hitbox, (Vector2) {0, 0}, 0, WHITE);

		for (auto i : platforms) {
			DrawRectangleRec(i.hitbox, i.color);
			// DrawTexturePro(i.texture, (Rectangle) {0, 0, 347, 347}, i.hitbox, Vector2 {0, 0}, 0, WHITE);
		}

		DrawLineV((Vector2){player.pos.x, player.pos.y}, (Vector2){player.pos.x + player.x_vel, player.pos.y + player.y_vel}, RED);
		DrawLineV((Vector2){player.pos.x + player.hitbox.width, player.pos.y + player.hitbox.height}, (Vector2){player.pos.x + player.hitbox.width + player.x_vel, player.pos.y + player.hitbox.height + player.y_vel}, RED);
		DrawLineV((Vector2){player.pos.x + player.hitbox.width, player.pos.y}, (Vector2){player.pos.x + player.hitbox.width + player.x_vel, player.pos.y + player.y_vel}, RED);
		DrawLineV((Vector2){player.pos.x, player.pos.y + player.hitbox.height}, (Vector2){player.pos.x + player.x_vel, player.pos.y + player.hitbox.height + player.y_vel}, RED);

		EndMode2D();

		DrawFPS(5, 2);

		DrawTextEx(mona, "91rl5 ki55in9.", (Vector2){ 1.0f, 20.0f }, (float)mona.baseSize, 2, RED);
		DrawTextEx(mona, "girls kissing - girls kissing - girls kissing - girls kissing", (Vector2){ 1.0f, 40.0f }, (float)mona.baseSize, 2, ORANGE);
		DrawTextEx(mona, TextFormat("x_vel %f\ny_vel %f\n", player.x_vel, player.y_vel), (Vector2){ 1.0f, 60.0f }, (float)mona.baseSize, 2, YELLOW);
		DrawTextEx(mona, TextFormat("frame no. %d\n", frame), (Vector2){ 1.0f, 100.0f }, (float)mona.baseSize, 2, GREEN);
		DrawTextEx(mona, TextFormat("frame time: %f\n", GetFrameTime()), (Vector2){ 1.0f, 120.0f }, (float)mona.baseSize, 2, BLUE);
		DrawTextEx(mona, TextFormat("coyote: %d\n", player.coyote), (Vector2){ 1.0f, 140.0f }, (float)mona.baseSize, 2, PURPLE);


		EndDrawing();

		// printf("x vel: %f y vel: %f facing: %c\n", player.x_vel, player.y_vel, player.facing);
		// printf("x: %f y: %f\n", player.x, player.y);
		// printf("x hitbox: %f y hitbox: %f\n", player.hitbox.x, player.hitbox.y);
		// printf("is jump: %d in air: %d\n", player.is_jump, player.in_air);
		// printf("is grounded: %d\n", player.is_grounded);
	}
}
