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
private:
	char my_email[16] = "joe@mail.com";
public:
	Vector2 pos = (Vector2) {100, 100};
	Vector2 prev_pos = pos;
	float x_vel = 0;
	float y_vel = 0;
	float max_speed = 150;
	float acceleration = 5;
	float jump_height = 600;
	float gravity_accel = 20;
	float gravity_max = 800;
	// float max_speed = 5;
	// float acceleration = 0.1;
	int health = 1;
	int coyote = 0;
	int animation_time = 0;
	int animation_frame = 0;
	int animation_count = 0;
	// int idle_animation_frame = 0;
	// bool odd_frame = false;
	bool in_air = false;
	bool is_grounded = false;
	bool is_jump = true;
	bool can_jump = true;
	char facing = 'l';
	// Rectangle hitbox = (Rectangle) {pos.x, pos.y, 33, 99};
	// Rectangle hitbox = (Rectangle) {pos.x, pos.y, 50/2, 140/2};
	Rectangle hitbox = (Rectangle) {pos.x, pos.y, 40/2, 140/2};
	// Rectangle scanbox = (Rectangle) {hitbox.x - acceleration, hitbox.y - acceleration, hitbox.width + acceleration * 2, hitbox.height + acceleration * 2};
	Rectangle scanbox = (Rectangle) {hitbox.x - hitbox.width, hitbox.y - hitbox.height, hitbox.width * 3, hitbox.height * 3};
	Rectangle hurtbox = (Rectangle) {hitbox.x + 7, hitbox.y + 10, hitbox.width - 14, hitbox.height - 20};
	Texture2D texture;

	void gravity ()
	{
		// #define MAXSPEED 1200
		// #define MAXSPEED 800
		// #define MAXSPEED 1800
		// #define ACCELERATION 20
		// #define MAXSPEED 3
		// #define ACCELERATION 0.1

		if (y_vel < gravity_max) y_vel += gravity_accel;
		else y_vel = gravity_max;

		// #undef MAXSPEED
		// #undef ACCELERATION
	};

	void jump () 
	{
		// y_vel -= 1400;
		y_vel -= jump_height;
		can_jump = false;
		in_air = true;
	};

	void move_left_right (char dir)
	{
		// #define RUN_MAXSPEED 500
		// #define RUN_MAXSPEED 9000
		// #define RUN_ACCELERATION 100
		// int maxspeed = 500;
		// int acceleration = 1000;
 
		switch (dir) {
			case 'r':
				if (x_vel < 0) x_vel += acceleration * 2;
				else x_vel += acceleration;
				break;

			case 'l':
				if (x_vel > 0) x_vel -= acceleration * 2;
				else x_vel -= acceleration;
				break;

			default:
				if (x_vel > 0) {
					if (x_vel - acceleration > 0)
						x_vel -= acceleration;
						// x_vel -= acceleration / 3;
					else
						x_vel = 0;
				}

				if (x_vel < 0) {
					if (x_vel + acceleration < 0)
						x_vel += acceleration;
						// x_vel += acceleration / 3;
					else
						x_vel = 0;
				}
				break;
		}

		if (x_vel > max_speed) x_vel = max_speed;
		if (x_vel < -max_speed) x_vel = -max_speed;
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
					// pos.y = platform->hitbox.y - hitbox.height - 0.01;
					y_vel = 0;
					// y_vel = -y_vel/1.3;
					is_grounded = true;
				}

				if (pos.y < prev_pos.y) { // from below
					pos.y = prev_pos.y;
					// pos.y = platform->hitbox.y + platform->hitbox.height + 0.01;
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
					// pos.x = platform->hitbox.x - hitbox.width - 0.01;
					// pos.x = platform->hitbox.x - hitbox.width;
					pos.x = prev_pos.x;
					x_vel = 0;
					// x_vel = -x_vel;
				}
				else if (pos.x < prev_pos.x) {
					// pos.x = platform->hitbox.x + platform->hitbox.width + 0.01;
					// pos.x = platform->hitbox.x + platform->hitbox.width;
					pos.x = prev_pos.x;
					x_vel = 0;
					// x_vel = -x_vel;
				}
			}
		}
	};

	float distance_from_player (Platform *p) {

		float platform_y_top = p->hitbox.y;
		float platform_y_bottom = p->hitbox.y + p->hitbox.height;
		float platform_x_left = p->hitbox.x;
		float platform_x_right = p->hitbox.x + p->hitbox.width;
		float player_x = pos.x + hitbox.width / 2;
		float player_y = pos.y + hitbox.height / 2;
		// float player_y_left = prev_pos.y;
		// float player_y_right = prev_pos.y + hitbox.width;
		// float player_x_left = prev_pos.x;
		// float player_x_right = prev_pos.x + hitbox.width;

		float dx = std::max({0.0f, (platform_x_left - player_x), (player_x - platform_x_right)});
		float dy = std::max({0.0f, (platform_y_top - player_y), (player_y - platform_y_bottom)});
		// float dx = std::max({0.0f, (platform_x_left - player_x_right), (player_x_left - platform_x_right)});
		// float dy = std::max({0.0f, (platform_y_left - player_y_right), (player_y_left - platform_y_right)});

		// printf("distance from player = %f\n", sqrt(dx*dx + dy*dy));
		return std::sqrt(dx*dx + dy*dy);

	}

	// void collision_detection (Platform *platforms) {
	void collision_detection (std::vector<Platform> &platforms) {
		std::vector< std::pair<int, float> > possible_collisions_list;
		for (unsigned int i = 0; i < std::size(platforms); i++) {
			// platforms[i].color = WHITE;
			if (CheckCollisionRecs(scanbox, platforms[i].hitbox)) { possible_collisions_list.push_back( {i, distance_from_player( &platforms[i] ) } ); /* platforms[i].color = BLUE; */ }
		}
		std::sort(possible_collisions_list.begin(), possible_collisions_list.end(),
				[](
					const std::pair<int, float> &a,
					const std::pair<int, float> &b
				) 
				{ return a.second < b.second; }
		);
		for (auto i : possible_collisions_list) {
			cool_collision_detection(&platforms[i.first]);
		}
	}

	void update_pos ()
	{
		if (prev_pos.x != pos.x) prev_pos.x = pos.x;
		if (prev_pos.y != pos.y) prev_pos.y = pos.y;
		pos.x += x_vel * GetFrameTime();
		pos.y += y_vel * GetFrameTime();

		hitbox.x = pos.x;
		hitbox.y = pos.y;

		hurtbox.x = pos.x + 7;
		hurtbox.y = pos.y + 10;

		// scanbox.x = pos.x - acceleration;
		// scanbox.y = pos.y - acceleration;
		scanbox.x = pos.x - hitbox.width;
		scanbox.y = pos.y - hitbox.height;
	};

	void animate_from_to_frame (int from, int to, int speed)
	{
		// if (animation_time <= 0) { animation_time = 60;}
		// else animation_time--;

		animation_time -= speed;
		if (animation_time <= 0) animation_frame++;
		if (animation_frame < from || animation_frame > to) { 
			animation_frame = from; 
			return;
		}
		else if (animation_time > 0) return;

	};
};

int frame = 0;

int main ()
{
	InitWindow(1000, 800, "girls kissing gaem");
	InitAudioDevice();
	SetTargetFPS(240);

	Font mona = LoadFontEx("resources/MonaspaceRadonFrozen-Regular.ttf", 20, 0, 250);
	// Texture2D girls_kissing = LoadTexture("resources/sprites/bokita.png");
	Texture2D kita_idle = LoadTexture("resources/sprites/kita-all.png");
	Texture2D nijika = LoadTexture("resources/sprites/nijika.gif");
	Texture2D grass = LoadTexture("resources/sprites/grass.png");
	Texture2D tree = LoadTexture("resources/sprites/tree.png");
	Texture2D sunflower = LoadTexture("resources/sprites/sunflower.png");

	Sound sound[5] = {0};
	sound[0] = LoadSound("resources/sfx/stereotypical-japanese-pop.mp3");
	// sound[0] = LoadSound("resources/sfx/21_3.mp3");
	// sound[1] = LoadSound("resources/sfx/bamboo-hit-sound-effect.mp3");
	// sound[2] = LoadSound("resources/sfx/half-life-button.mp3");
	// sound[3] = LoadSound("resources/sfx/erro-win-7_3OFHeWK.mp3");
	// sound[4] = LoadSound("resources/sfx/fairy-dust-sfx.mp3");
	SetMasterVolume(0.02);

	Camera2D camera = {0};

	Player player;
	player.texture = kita_idle;

	std::vector<Platform> platforms;
	platforms.push_back((Platform){(Rectangle){800, 300, 300, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){300, 450, 300, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){500, 600, 300, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){500, 250, 30, 300}, nijika});
	platforms.push_back((Platform){(Rectangle){20, 700, 300, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){20, 600, 30, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){70, 500, 30, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){100, 550, 30, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){100, 300, 30, 30}, nijika});
	platforms.push_back((Platform){(Rectangle){320, 200, 30, 300}, nijika});
	platforms.push_back((Platform){(Rectangle){100, 730, 9900, 10}, nijika});
	platforms.push_back((Platform){(Rectangle){1200, 600, 200, 10}, nijika});
	platforms.push_back((Platform){(Rectangle){1400, 600, 200, 10}, nijika});
	platforms.push_back((Platform){(Rectangle){1620, 600, 200, 10}, nijika});
	platforms.push_back((Platform){(Rectangle){4000, 200, 5, 1000}, nijika});
	platforms.push_back((Platform){(Rectangle){2000, 600, 300, 20}, nijika});
	platforms.push_back((Platform){(Rectangle){2100, 600, 300, 20}, nijika});
	platforms.push_back((Platform){(Rectangle){2200, 600, 300, 20}, nijika});
	platforms.push_back((Platform){(Rectangle){2500, 600, 300, 20}, nijika});

	float camera_x_follow = player.pos.x + player.hitbox.width / 2;
	float camera_y_follow = player.pos.y + player.hitbox.height / 2;
	// float camera_steps = 0;

	while (!WindowShouldClose()) {

		frame++;

		if (IsKeyPressed(KEY_Q)) { 
			CloseWindow();
			CloseAudioDevice();
		}
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

		// player.move_left_right(dir);

		player.is_grounded = false;

		player.collision_detection(platforms);

		// {  // COLLISIONS
		// 	// std::vector<Platform> possible_collisions_list;
		// 	// std::vector< std::pair<Platform, float> > possible_collisions_list;
		// 	std::vector< std::pair<int, float> > possible_collisions_list;
		// 	// for (auto i : platforms) {
		// 	for (unsigned int i = 0; i < size(platforms); i++) {
		// 		// if (CheckCollisionRecs(player.scanbox, i.hitbox)) possible_collisions_list.push_back(i);
		// 		platforms[i].color = WHITE;
		// 		if (CheckCollisionRecs(player.scanbox, platforms[i].hitbox)) { possible_collisions_list.push_back( {i, player.distance_from_player( &platforms[i])} ); platforms[i].color = BLUE; }
		// 		// if (CheckCollisionRecs(player.scanbox, platforms[i].hitbox)) possible_collisions_list.push_back( {i, 3} );
		// 	}
		// 	std::sort(possible_collisions_list.begin(), possible_collisions_list.end(),
		// 			[](
		// 				const std::pair<int, float> &a,
		// 				const std::pair<int, float> &b
		// 			) 
		// 			{ return a.second < b.second; }
		// 	);
		// 	// if (!possible_collisions_list.empty()) { 
		// 	// 	std::cout << possible_collisions_list[0].first << "\n"; platforms[possible_collisions_list[0].first].color = RED;
		// 	// }
		// 	// printf("size of possible_collisions_list: %d\n", size(possible_collisions_list));
		// 	// std::cout << possible_collisions_list.front() << "\n";
		// 	// std::cout << "bocchi kita" << "\n";
		// 	// printf("nearest rect: %d\n", possible_collisions_list[0].first);
		// 	// for (auto i : possible_collisions_list) {
		// 	for (auto i : possible_collisions_list) {
		// 		player.cool_collision_detection(&platforms[i.first]);
		// 	}
		// }

		if (player.is_grounded) player.coyote = 8;
		else if (player.coyote && !player.is_grounded) player.coyote--;

		if (player.gravity_accel != 20) player.gravity_accel += 0.2;
		if (player.gravity_accel > 20 || player.gravity_accel < 0) player.gravity_accel = 20;
		if (!player.is_grounded) { player.gravity(); }
 
		if (IsKeyReleased(KEY_SPACE)) { player.can_jump = true; }
		if (IsKeyDown(KEY_SPACE) && player.is_grounded && player.can_jump || IsKeyDown(KEY_SPACE) && player.coyote && player.can_jump) { 
			player.jump(); player.is_grounded = false; player.coyote = 0; 
			if (player.facing == 'r') player.animation_frame = 2;
			// PlaySound(sound[std::rand() % (0 - 5)]);
			// else player.animation_frame = 3;
			// PlaySound(sound[2]);
			// PlaySound(sound[4]);
			// PlaySound(sound[0]);
			player.gravity_accel = 0;
		}
		// } else if (IsKeyDown(KEY_SPACE) && player.coyote && player.can_jump) { 
		// 	player.jump(); player.is_grounded = false; player.coyote = 0;
		// 	if (player.facing == 'r') player.animation_frame = 2;
		// 	// PlaySound(sound[std::rand() % (0 - 5)]);
		// 	PlaySound(sound[1]);
		// 	// else player.animation_frame = 3;
		// }

		player.move_left_right(dir);

		player.update_pos();

		// player.update_pos();

		// if (camera_x_follow < player.pos.x + player.hitbox.width / 2 - 60) camera_x_follow = player.pos.x + player.hitbox.width / 2 - 60;
		// if (camera_x_follow > player.pos.x + player.hitbox.width / 2 + 60) camera_x_follow = player.pos.x + player.hitbox.width / 2 + 60;
		if (camera_x_follow < player.pos.x + player.hitbox.width / 2 - 20) camera_x_follow = player.pos.x + player.hitbox.width / 2 - 20;
		if (camera_x_follow > player.pos.x + player.hitbox.width / 2 + 20) camera_x_follow = player.pos.x + player.hitbox.width / 2 + 20;

		if (camera_y_follow < player.prev_pos.y + player.hitbox.height / 2 + 60) camera_y_follow = player.prev_pos.y + player.hitbox.height / 2 + 60;
		if (camera_y_follow > player.prev_pos.y + player.hitbox.height / 2 - 30) camera_y_follow = player.prev_pos.y + player.hitbox.height / 2 - 30;
		// if (camera_y_follow > player.pos.y + player.hitbox.height / 2 - 40) camera_y_follow -= 1;

		// if (camera_y_follow < player.pos.y + player.hitbox.height / 2 - 60) camera_y_follow = player.pos.y + player.hitbox.height / 2 - 60;
		// if (camera_y_follow > player.pos.y + player.hitbox.height/ 2 + 60) camera_y_follow = player.pos.y + player.hitbox.height / 2 + 60;
		// camera.target = (Vector2) {camera_x_follow, 400};
		// camera.target = (Vector2) {camera_x_follow, player.prev_pos.y};
		// camera.target = (Vector2) {camera_x_follow, player.prev_pos.y};
		camera.target = (Vector2) {camera_x_follow, camera_y_follow};
		camera.offset = (Vector2){ GetRenderWidth()/2.0f, GetRenderHeight()/2.0f };
		camera.rotation = 0.0f;
		camera.zoom = 3.0f;

		BeginDrawing();

		BeginMode2D(camera);
		// ClearBackground(Color{0x11, 0x00, 0x11, 0xff});
		ClearBackground(Color{0xa5, 0xc2, 0xd6, 0xff});

		DrawTexturePro(tree, (Rectangle) {0, 0, 841, 1032}, (Rectangle) {100, 530, 841 / 6, 1032 / 6}, (Vector2) {0, 0}, -2, WHITE);
		DrawTexturePro(tree, (Rectangle) {0, 0, 841, 1032}, (Rectangle) {180, 535, 841 / 6, 1032 / 6}, (Vector2) {0, 0}, 3, WHITE);
		DrawTexturePro(tree, (Rectangle) {0, 0, 841, 1032}, (Rectangle) {300, 570, 841 / 6, 1032 / 6}, (Vector2) {0, 0}, -1, WHITE);
		DrawTexturePro(sunflower, (Rectangle) {0, 0, 802, 980}, (Rectangle) {280, 660, 802 / 20, 980 / 20}, (Vector2) {0, 0}, 3, WHITE);
		DrawTexturePro(sunflower, (Rectangle) {0, 0, 802, 980}, (Rectangle) {200, 670, 802 / 20, 980 / 20}, (Vector2) {0, 0}, -12, WHITE);
		DrawTexturePro(sunflower, (Rectangle) {0, 0, 802, 980}, (Rectangle) {160, 672, 802 / 20, 980 / 20}, (Vector2) {0, 0}, 1, WHITE);
		DrawTexturePro(sunflower, (Rectangle) {0, 0, 802, 980}, (Rectangle) {180, 660, 802 / 20, 980 / 20}, (Vector2) {0, 0}, 8, WHITE);

		// DrawRectangleRec(player.scanbox, Color{0x00, 0xcc, 0x00, 0x44});
		// DrawRectangleRec(player.hurtbox, Color{0xcc, 0x00, 0x00, 0x88});


		// DrawTexturePro(player.texture, (Rectangle) {150, 200, player.hitbox.width * 4, player.hitbox.height * 4}, player.hitbox, (Vector2) {0, 0}, 0, WHITE);
		// DrawTexturePro(player.texture, (Rectangle) {0, 0, 347, 347}, player.hitbox, (Vector2) {0, 0}, 0, WHITE);


		if (player.animation_time <= 0) { player.animation_time = 80;}
		else player.animation_time--;	
		if (player.x_vel != 0 && player.y_vel >= 0) {
			if (player.facing == 'r') player.animate_from_to_frame(4, 7, 1);
			if (player.facing == 'l') player.animate_from_to_frame(8, 11, 1);
		} else if (player.y_vel < 0) {
			if (player.facing == 'r') player.animate_from_to_frame(2, 2, 0);
			if (player.facing == 'l') player.animate_from_to_frame(3, 3, 0);
		} else { 
			player.animate_from_to_frame(0, 1, 0);
		}
		// DrawTexturePro(player.texture, (Rectangle) {(float)player.animation_frame * 50, 0, 50, 140}, player.hitbox, (Vector2) {0, 0}, 0, WHITE);
		DrawRectangleRec(player.hitbox, RED);
		DrawTexturePro(player.texture, (Rectangle) {(float)player.animation_frame * 50, 0, 50, 140}, (Rectangle) {player.pos.x - 2.5f, player.pos.y, player.hitbox.width + 5, player.hitbox.height}, (Vector2) {0, 0}, 0, WHITE);

		for (auto i : platforms) {
			// DrawRectangleRec(i.hitbox, i.color);
			// DrawTexturePro(i.texture, (Rectangle) {0, 0, i.hitbox.width, i.hitbox.height}, i.hitbox, Vector2 {0, 0}, 0, WHITE);
			DrawTexturePro(grass, (Rectangle) {0, 0, i.hitbox.width * 10, i.hitbox.height * 10}, i.hitbox, Vector2 {0, 0}, 0, WHITE);
			DrawTextEx(mona, TextFormat("%.1f %.1f\n", i.hitbox.x, i.hitbox.y), (Vector2){ i.hitbox.x, i.hitbox.y }, (float)mona.baseSize, 1, BLUE);
		}


		// DrawLineV((Vector2){player.pos.x, player.pos.y}, (Vector2){player.pos.x + player.x_vel, player.pos.y + player.y_vel}, RED);
		// DrawLineV((Vector2){player.pos.x + player.hitbox.width, player.pos.y + player.hitbox.height}, (Vector2){player.pos.x + player.hitbox.width + player.x_vel, player.pos.y + player.hitbox.height + player.y_vel}, RED);
		// DrawLineV((Vector2){player.pos.x + player.hitbox.width, player.pos.y}, (Vector2){player.pos.x + player.hitbox.width + player.x_vel, player.pos.y + player.y_vel}, RED);
		// DrawLineV((Vector2){player.pos.x, player.pos.y + player.hitbox.height}, (Vector2){player.pos.x + player.x_vel, player.pos.y + player.hitbox.height + player.y_vel}, RED);
 
		// DrawLineV((Vector2){player.pos.x + player.hitbox.width / 2, player.pos.y + player.hitbox.height / 2}, (Vector2){player.pos.x + player.hitbox.width / 2 +  player.x_vel, player.pos.y + player.hitbox.height / 2 + player.y_vel}, RED);

		EndMode2D();

		DrawFPS(5, 2);

		DrawTextEx(mona, "GIRLS ~ KISSING! pre-pre-alpha ver 0.0.0", (Vector2){ 1.0f, 20.0f }, (float)mona.baseSize, 2, MAGENTA);
		DrawTextEx(mona, "91rl5 ki55in9.", (Vector2){ 1.0f, 40.0f }, (float)mona.baseSize, 2, RED);
		DrawTextEx(mona, "girls kissing - girls kissing - girls kissing - girls kissing", (Vector2){ 1.0f, 60.0f }, (float)mona.baseSize, 2, ORANGE);
		DrawTextEx(mona, TextFormat("x_vel %f\ny_vel %f\n", player.x_vel, player.y_vel), (Vector2){ 1.0f, 80.0f }, (float)mona.baseSize, 2, YELLOW);
		DrawTextEx(mona, TextFormat("frame no. %d\n", frame), (Vector2){ 1.0f, 120.0f }, (float)mona.baseSize, 2, GREEN);
		DrawTextEx(mona, TextFormat("frame time: %f\n", GetFrameTime()), (Vector2){ 1.0f, 140.0f }, (float)mona.baseSize, 2, BLUE);
		DrawTextEx(mona, TextFormat("coyote: %d\n", player.coyote), (Vector2){ 1.0f, 160.0f }, (float)mona.baseSize, 2, PURPLE);


		EndDrawing();

		printf("\n\n\n\nx vel: %f y vel: %f facing: %c\n", player.x_vel, player.y_vel, player.facing);
		printf("prev x: %f prev y: %f\n", player.prev_pos.x, player.prev_pos.y);
		printf("x: %f y: %f\n", player.pos.x, player.pos.y);
		// printf("x hitbox: %f y hitbox: %f\n", player.hitbox.x, player.hitbox.y);
		// printf("is jump: %d in air: %d\n", player.is_jump, player.in_air);
		// printf("is grounded: %d\n", player.is_grounded);
	}
}
