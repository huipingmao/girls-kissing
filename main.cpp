#include <iostream>
#include <cmath>
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdio>

class Platform {
public:
	Vector2 vel;
	Rectangle hitbox;
	Texture2D texture;
	Color color = WHITE;

	void update_platform_pos ()
	{
		hitbox.x += vel.x * GetFrameTime();
		hitbox.y += vel.y * GetFrameTime();
	};
};

void read_level_file (char *file_path, std::vector<Platform> *platforms)
{
	// std::ifstream f(file_path);
	FILE *f = fopen(file_path ,"r");
	// char ch;
	Rectangle tmp = {0};

	char sign = fgetc(f);
	while (sign != EOF) {
		if (sign == '{') {
			fscanf(f, "%f, %f, %f, %f}\n", &tmp.x, &tmp.y, &tmp.width, &tmp.height);
			platforms->push_back((Platform){(Vector2){0, 0}, tmp, NULL});
			puts ("%%\t\tINFO\t\t%% added platform");
		}
		// if (sign == '\n') sign = fgetc(f);
		sign = fgetc(f);
	}
	fclose(f);

}

class Player {
private:
	char my_email[16] = "joe@mail.com";
public:
	Vector2 pos = (Vector2) {0, 0};
	Vector2 prev_pos = pos;
	Vector2 vel;
	float x_vel = 0;
	float y_vel = 0;
	// float max_speed = 155;
	float max_speed = 200;
	float acceleration = 5;
	// float jump_height = 600;
	float jump_height = 525;
	float gravity_accel = 20;
	float gravity_max = 700;
	int health = 1;
	int coyote = 0;
	int animation_time = 0;
	int animation_frame = 0;
	int animation_count = 0;
	bool animation_forward = true;
	// int idle_animation_frame = 0;
	// bool odd_frame = false;
	bool in_air = false;
	bool is_grounded = false;
	bool is_truly_grounded = false;
	int truly_grounded_counter = 0;
	int index_of_platform_that_set_true_grounded_state = 0; // lol
	// bool is_jump = true;
	bool can_jump = true;
	char facing = 'l';
	// Rectangle hitbox = (Rectangle) {pos.x, pos.y, 33, 99};
	// Rectangle hitbox = (Rectangle) {pos.x, pos.y, 50/2, 140/2};
	// Rectangle hitbox = (Rectangle) {pos.x, pos.y, 40/2, 140/2};
	Rectangle hitbox = (Rectangle) {100, 100, 40/2, 140/2};
	// Rectangle scanbox = (Rectangle) {hitbox.x - acceleration, hitbox.y - acceleration, hitbox.width + acceleration * 2, hitbox.height + acceleration * 2};
	Rectangle scanbox = (Rectangle) {hitbox.x - hitbox.width, hitbox.y - hitbox.height, hitbox.width * 3, hitbox.height * 3};
	Rectangle hurtbox = (Rectangle) {hitbox.x + 7, hitbox.y + 10, hitbox.width - 14, hitbox.height - 20};
	Texture2D texture;

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


	void gravity ()
	{
		if (y_vel < gravity_max) y_vel += gravity_accel;
		else y_vel = gravity_max;

	};

	void jump () 
	{
		y_vel -= jump_height;
		can_jump = false;
		in_air = true;
	};

	void move_left_right (char dir)
	{
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

	void standing_on_moving_platform_update (Platform *platform) 
	{
			// if (platform->vel.x != 0 && within_x) { pos.x += platform->vel.x * GetFrameTime(); /* prev_pos.x = pos.x; */ } // moving platforms update
			if (platform->vel.x != 0 && is_truly_grounded) { pos.x += platform->vel.x * GetFrameTime(); /* prev_pos.x = pos.x; */ } // moving platforms update
	};

	bool collision_resolution_v2 (Platform *platform, int index) {
		if (CheckCollisionRecs(hitbox, platform->hitbox)) {

			bool within_x; if (
				platform->hitbox.x > prev_pos.x
				&&
				platform->hitbox.x > prev_pos.x + hitbox.width
				||
				platform->hitbox.x + platform->hitbox.width < prev_pos.x
				&&
				platform->hitbox.x  + platform->hitbox.width < prev_pos.x + hitbox.width
			) within_x = false; else within_x = true;


			if (
				platform->vel.y < 0 // platform is rising
				&& 
				pos.y + hitbox.height / 2 <= platform->hitbox.y + platform->hitbox.height / 2 // player is in upper half of rec
				&& within_x)
				{ 
					y_vel = 0; pos.y = platform->hitbox.y - hitbox.height + platform->vel.y * GetFrameTime(); is_grounded = true; index_of_platform_that_set_true_grounded_state = index; 
			} // moving platforms update
			else if (
				platform->vel.y > 0 // platform is falling
				&& 
				pos.y + hitbox.height / 2 > platform->hitbox.y + platform->hitbox.height / 2 // player is in lower half of rec
				&& within_x)
				{ 
					y_vel = 0; pos.y = platform->hitbox.y + platform->hitbox.height + platform->vel.y * GetFrameTime(); index_of_platform_that_set_true_grounded_state = index; 
			} // moving platforms update
			else if (prev_pos.y != pos.y && within_x) {
				pos.y = prev_pos.y;
				y_vel = 0;
				// is_grounded = true;
				// if (prev_pos.y == pos.y) {
				if (prev_pos.y <= pos.y) {
					index_of_platform_that_set_true_grounded_state = index;
					is_grounded = true;
				}
			}

			if (platform->vel.x < 0 && x_vel >= 0 && !within_x) { pos.x = platform->hitbox.x - hitbox.width - 1; /*return true;*/ } // moving platforms update
			else if (platform->vel.x > 0 && x_vel <= 0 && !within_x) { pos.x = platform->hitbox.x + platform->hitbox.width + 1; /* return true; */ } // moving platforms update
			else if (prev_pos.x != pos.x && !within_x) { 
				pos.x = prev_pos.x;
				x_vel = 0;
			} 

			hitbox.y = pos.y;
			hitbox.x = pos.x;

			if (within_x && CheckCollisionRecs(hitbox, platform->hitbox)) { // holy shit does it actually WORK????!! nvm whatever
				// force player out of collision
				if (pos.x + hitbox.width / 2 <= platform->hitbox.x + platform->hitbox.width / 2){ pos.x = platform->hitbox.x - hitbox.width - 1; /*return true;*/ } // if in left half of rec
				else if (pos.x + hitbox.width / 2 > platform->hitbox.x + platform->hitbox.width / 2){ pos.x = platform->hitbox.x + platform->hitbox.width + 1; /* return true; */ } // if in right half of rec
			}

			// if (CheckCollisionRecs(hitbox, platform->hitbox)) goto repeat; // i dont fucking know anymore dont judge this solution please leave me alone ok?
			// if (CheckCollisionRecs(hitbox, platform->hitbox)) collision_resolution_v2(platform); // idk i think this is better

			return true;
		}
		return false;
	}

	float distance_from_player (Platform *p) {

		float platform_y_top = p->hitbox.y;
		float platform_y_bottom = p->hitbox.y + p->hitbox.height;
		float platform_x_left = p->hitbox.x;
		float platform_x_right = p->hitbox.x + p->hitbox.width;
		float player_x = pos.x + hitbox.width / 2;
		float player_y = pos.y + hitbox.height / 2;

		float dx = std::max({0.0f, (platform_x_left - player_x), (player_x - platform_x_right)});
		float dy = std::max({0.0f, (platform_y_top - player_y), (player_y - platform_y_bottom)});

		return std::sqrt(dx*dx + dy*dy);

	}

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
		bool collision_frame = false;

		for (auto i : possible_collisions_list) {
			if (collision_resolution_v2(&platforms[i.first], i.first)) collision_frame = true;
		}
	}

	void animate_from_to_frame (int from, int to, int speed)
	{
		// if (animation_time <= 0) { animation_time = 60;}
		// else animation_time--;
		animation_forward = true;
		animation_time -= speed;
		if (animation_time <= 0) animation_frame++;
		if (animation_frame < from || animation_frame > to) { 
			animation_frame = from; 
			return;
		}
		else if (animation_time > 0) return;

	};

	void animate_ping_pong (int from, int to, int speed)
	{
		animation_time -= speed;
		// if (animation_time <= 0) animation_frame++;

		if (animation_frame <= from) { animation_frame = from; animation_forward = true; }
		if (animation_frame >= to) { animation_frame = to; animation_forward = false; }

		if (animation_time > 0) return;
		if (animation_time <= 0 && animation_forward) animation_frame++;
		else if (animation_time <= 0) animation_frame--;
		// if (animation_frame < to) { animation_forward = true; animation_frame++;  return; }
		// if (animation_frame > to) { animation_forward = true; animation_frame++;  return; }
		// if (animation_frame > from) { animation_forward = false; animation_frame--; animation_frame = from; return; }
	};
};

int frame = 0;
int global_animation_time = 0;
int global_animation = 0;

int main ()
{
	InitWindow(1000, 800, "girls kissing gaem");
	InitAudioDevice();
	SetTargetFPS(240);
	SetExitKey(KEY_ESCAPE);


	Font mona = LoadFontEx("resources/MonaspaceRadonFrozen-Regular.ttf", 20, 0, 250);
	// Texture2D girls_kissing = LoadTexture("resources/sprites/bokita.png");
	Texture2D kita_idle = LoadTexture("resources/sprites/kita-new.png");
	Texture2D nijika = LoadTexture("resources/sprites/nijika.gif");
	Texture2D grass = LoadTexture("resources/sprites/grass.png");
	Texture2D tree = LoadTexture("resources/sprites/tree.png");
	Texture2D sunflower = LoadTexture("resources/sprites/sunflower.png");
	// Texture2D background = LoadTexture("resources/sprites/background.png");
	Texture2D background = LoadTexture("resources/sprites/sky2.png");

	// Sound sound[5] = {0};
	// sound[0] = LoadSound("resources/sfx/stereotypical-japanese-pop.mp3");
	// sound[0] = LoadSound("resources/sfx/21_3.mp3");
	// sound[1] = LoadSound("resources/sfx/bamboo-hit-sound-effect.mp3");
	// sound[2] = LoadSound("resources/sfx/half-life-button.mp3");
	// sound[3] = LoadSound("resources/sfx/erro-win-7_3OFHeWK.mp3");
	// sound[4] = LoadSound("resources/sfx/fairy-dust-sfx.mp3");
	Sound sound = LoadSound("resources/sfx/half-life-crowbar.mp3");
	SetMasterVolume(0.02);

	Camera2D camera = {0};

	Player player;
	player.texture = kita_idle;

	std::vector<Platform> platforms;

	read_level_file("./level-1.lesbian", &platforms);

	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){800, 300, 300, 30}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){300, 450, 300, 30}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){500, 600, 300, 30}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){500, 250, 30, 300}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){20, 700, 300, 30}, nijika});

	// // platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){20, 600, 30, 30}, nijika});
	// // platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){70, 610, 30, 30}, nijika});
	// // platforms.push_back({(Vector2) {0, 0}, (Platform)(Rectangle){80, 601, 30, 30}, nijika});
	// // platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){70, 500, 30, 30}, nijika});
	// // platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){100, 550, 30, 30}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){100, 300, 30, 30}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){320, 200, 30, 300}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){100, 730, 9900, 10}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){1200, 600, 200, 10}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){1400, 600, 200, 10}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){1620, 600, 200, 10}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){4000, 200, 5, 1000}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){2000, 600, 300, 20}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){2100, 600, 300, 20}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){2200, 600, 300, 20}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 0}, (Rectangle){2500, 600, 300, 20}, nijika});

	// platforms.push_back((Platform){(Vector2) {0, -5}, (Rectangle){100, 550, 30, 30}, nijika});
	// platforms.push_back((Platform){(Vector2) {0, 5}, (Rectangle){70, 500, 30, 30}, nijika});
	// platforms.push_back((Platform){(Vector2) {-10, 0}, (Rectangle){240, 600, 30, 30}, nijika});

	float camera_x_follow = player.pos.x + player.hitbox.width / 2;
	float camera_y_follow = player.pos.y + player.hitbox.height / 2;
	// float camera_steps = 0;

	while (!WindowShouldClose()) {

		frame++;
		if (global_animation_time <= 0) { global_animation++; global_animation_time = 240; }
		global_animation_time--;

		if (IsKeyPressed(KEY_R)) { player.pos.x = 0; player.pos.y = 0;
							 player.prev_pos.x = 0; player.prev_pos.y = 0; }

		// if (player.pos.y > 1200) { player.pos.x = 700; player.pos.y = 100; // tmp
		// 					 player.prev_pos.x = 700; player.prev_pos.y = 100; }

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

		player.standing_on_moving_platform_update(&platforms[player.index_of_platform_that_set_true_grounded_state]);

		if (player.is_grounded) { player.is_truly_grounded = true; player.truly_grounded_counter = 1; }
		else if (!player.is_grounded && player.truly_grounded_counter) { player.truly_grounded_counter--; }
		else { player.is_truly_grounded = false; }
		// printf("player TRULY grounded %d\n", player.is_truly_grounded);
		// printf("platform that set..... %d\n", player.index_of_platform_that_set_true_grounded_state);
	
		// player.truly_grounded_counter = 1;
		// player.update_pos();

		// if (player.is_grounded) player.coyote = 8;
		// else if (player.coyote && !player.is_grounded) player.coyote--;
		// if (player.is_truly_grounded) player.coyote = 8;
		// 
		// if (player.is_grounded) player.coyote = 100;
		// if (!player.is_truly_grounded) player.coyote = 100;
		// else if (player.coyote && !player.is_truly_grounded) player.coyote--;
		// if (player.coyote) player.coyote--;

		// if (player.gravity_accel != 20) player.gravity_accel += 0.2;
		// if (player.gravity_accel > 20 || player.gravity_accel < 0) player.gravity_accel = 20;
		// if (!player.is_grounded && !player.coyote) { player.gravity(); }
		// if (!player.coyote) player.gravity();
		// if (player.coyote) player.is_grounded = true;
 
		// if (IsKeyReleased(KEY_SPACE)) { player.can_jump = true; }
		// if (IsKeyDown(KEY_SPACE) && player.is_grounded && player.can_jump || IsKeyDown(KEY_SPACE) && player.coyote && player.can_jump) { 
		// 	player.jump(); player.is_grounded = false; player.coyote = 0; 
		// 	if (player.facing == 'r') player.animation_frame = 2;
		// 	// PlaySound(sound[std::rand() % (0 - 5)]);
		// 	// else player.animation_frame = 3;
		// 	// PlaySound(sound[2]);
		// 	// PlaySound(sound[4]);
		// 	// PlaySound(sound[0]);
		// 	// PlaySound(sound);
		// 	player.gravity_accel = 0;
		// }

		// if (player.coyote) player.coyote--;
		if (player.gravity_accel != 20) player.gravity_accel += 0.2;
		if (player.gravity_accel > 20 || player.gravity_accel < 0) player.gravity_accel = 20;
 
		// if (player.is_grounded && player.can_jump) player.coyote = 100;
		if (player.is_grounded && player.can_jump) player.coyote = 8;
		else player.coyote--;
		if (!player.is_grounded) player.gravity();
		if (player.coyote < 0) player.coyote = 0;

		if (IsKeyReleased(KEY_SPACE)) { player.can_jump = true; }
		if (IsKeyDown(KEY_SPACE) && player.is_truly_grounded && player.can_jump || IsKeyDown(KEY_SPACE) && player.coyote && player.can_jump) { 
			player.y_vel = 0; 
			player.jump();
			player.is_grounded = false;
			player.coyote = 0; 
			// if (player.facing == 'r') player.animation_frame = 2;
			// PlaySound(sound[std::rand() % (0 - 5)]);
			// else player.animation_frame = 3;
			// PlaySound(sound[2]);
			// PlaySound(sound[4]);
			// PlaySound(sound[0]);
			// PlaySound(sound);
			player.gravity_accel = 0;
		}

		player.move_left_right(dir);

		player.update_pos();

		if (camera_x_follow < player.pos.x + player.hitbox.width / 2 - 20) camera_x_follow = player.pos.x + player.hitbox.width / 2 - 20;
		if (camera_x_follow > player.pos.x + player.hitbox.width / 2 + 20) camera_x_follow = player.pos.x + player.hitbox.width / 2 + 20;


		if (camera_y_follow < player.pos.y + player.hitbox.height / 2 - 80) camera_y_follow = player.pos.y + player.hitbox.height / 2 - 80; // camera dohn
		else if (camera_y_follow < player.pos.y + player.hitbox.height / 2 - 40) camera_y_follow +=1 ; // camera dohn
		else if (player.is_truly_grounded && camera_y_follow < player.pos.y + player.hitbox.height / 2 - 30) camera_y_follow += 1; // camera down

		else if (camera_y_follow > player.pos.y + player.hitbox.height / 2 + 100) camera_y_follow = player.prev_pos.y + player.hitbox.height / 2 + 100 - 1; // camera up
		else if (player.is_truly_grounded && camera_y_follow > player.pos.y + player.hitbox.height / 2 + 20) camera_y_follow -= 1; // camera up
		else if (camera_y_follow > player.pos.y + player.hitbox.height / 2 + 80) camera_y_follow -= 1; // camera up



		// camera.target = (Vector2) {camera_x_follow, player.pos.y + 30};
		camera.target = (Vector2) {camera_x_follow, camera_y_follow};
		camera.offset = (Vector2){ GetRenderWidth()/2.0f, GetRenderHeight()/2.0f };
		camera.rotation = 0.0f;
		camera.zoom = 3.0f;
		// camera.zoom = 1.0f;

		if (IsKeyDown(KEY_C)) camera.zoom = 2.0f;

		BeginDrawing();

		// DrawTexturePro(background, (Rectangle){0, 0, 1920, 1200}, (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()}, (Vector2){0, 0}, 0, WHITE);
		// DrawTexturePro(background, (Rectangle){0, 0, 7762, 5177}, (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()}, (Vector2){0, 0}, 0, WHITE);
		// DrawTexturePro(background, (Rectangle){0, 0, 1470, 980}, (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()}, (Vector2){0, 0}, 0, WHITE);
		DrawTexturePro(background, (Rectangle){0, 0, 1470, 980}, (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()}, (Vector2){0, 0}, 0, WHITE);
		BeginMode2D(camera);
		// ClearBackground(Color{0x11, 0x00, 0x11, 0xff});
		// ClearBackground(Color{0xa5, 0xc2, 0xd6, 0xff});
		// ClearBackground(Color{0x00, 0x00, 0x00, 0xff});

		DrawTexturePro(tree, (Rectangle) {0, 0, 841, 1032}, (Rectangle) {140, -280, 841 / 6, 1032 / 6}, (Vector2) {0, 0}, -2, WHITE);
		DrawTexturePro(tree, (Rectangle) {0, 0, 841, 1032}, (Rectangle) {98, -134, 841 / 8, 1032 / 8}, (Vector2) {0, 0}, -6, WHITE);
		// DrawTexturePro(tree, (Rectangle) {0, 0, 841, 1032}, (Rectangle) {180, 535, 841 / 6, 1032 / 6}, (Vector2) {0, 0}, 3, WHITE);
		// DrawTexturePro(tree, (Rectangle) {0, 0, 841, 1032}, (Rectangle) {300, 570, 841 / 6, 1032 / 6}, (Vector2) {0, 0}, -1, WHITE);
		// DrawTexturePro(sunflower, (Rectangle) {0, 0, 802, 980}, (Rectangle) {280, 660, 802 / 20, 980 / 20}, (Vector2) {0, 0}, 3, WHITE);
		// DrawTexturePro(sunflower, (Rectangle) {0, 0, 802, 980}, (Rectangle) {200, 670, 802 / 20, 980 / 20}, (Vector2) {0, 0}, -12, WHITE);
		// DrawTexturePro(sunflower, (Rectangle) {0, 0, 802, 980}, (Rectangle) {160, 672, 802 / 20, 980 / 20}, (Vector2) {0, 0}, 1, WHITE);
		// DrawTexturePro(sunflower, (Rectangle) {0, 0, 802, 980}, (Rectangle) {180, 660, 802 / 20, 980 / 20}, (Vector2) {0, 0}, 8, WHITE);

		// DrawRectangleRec(player.scanbox, Color{0x00, 0xcc, 0x00, 0x44});
		// DrawRectangleRec(player.hurtbox, Color{0xcc, 0x00, 0x00, 0x88});

		if (player.animation_time <= 0) { player.animation_time = 120;}
		else player.animation_time--;	
		// if (player.x_vel != 0 && player.y_vel >= 0) {
		if (player.x_vel != 0 && player.is_truly_grounded) {
			// if (player.facing == 'r') player.animate_from_to_frame(4, 7, 2);
			// if (player.facing == 'l') player.animate_from_to_frame(8, 11, 2);
			if (player.facing == 'r') player.animate_ping_pong(4, 6, 2);
			if (player.facing == 'l') player.animate_ping_pong(7, 9, 2);
		// } else if (player.y_vel < 0) {
		} else if (!player.is_truly_grounded) {
			if (player.facing == 'r') player.animate_from_to_frame(2, 2, 0);
			if (player.facing == 'l') player.animate_from_to_frame(3, 3, 0);
		} else { 
			player.animate_from_to_frame(0, 1, 0);
		}
		// DrawTexturePro(player.texture, (Rectangle) {(float)player.animation_frame * 50, 0, 50, 140}, player.hitbox, (Vector2) {0, 0}, 0, WHITE);
		// DrawRectangleRec(player.hitbox, RED);
		// DrawTexturePro(player.texture, (Rectangle) {(float)player.animation_frame * 50, 0, 50, 140}, (Rectangle) {player.pos.x - 2.5f, player.pos.y, player.hitbox.width + 5, player.hitbox.height}, (Vector2) {0, 0}, 0, WHITE);



		Rectangle cull = (Rectangle) {
			player.pos.x + player.hitbox.width / 2 - GetRenderWidth() / 3, 
			player.pos.y + player.hitbox.height / 2 - GetRenderHeight() / 3, 
			GetRenderWidth() - GetRenderWidth() / 3,
			GetRenderHeight() - GetRenderHeight() / 3
		};
		// DrawRectangleRec(cull, (Color) {255,0,0,100});
		{
			int index = 0;
			for (auto i : platforms) {
				// DrawRectangleRec(i.hitbox, i.color);
				platforms[index].update_platform_pos();
				// DrawRectangleRec(i.hitbox, Color{0x87, 0x67, 0x60, 0xff});
				// DrawRectangleRec(i.hitbox, Color{0xff, 0xff, 0xff, 0xaa});
				// DrawRectangleRec(i.hitbox, Color{0xaa, 0x67, 0xc2, 0xff});
				// DrawTexturePro(i.texture, (Rectangle) {0, 0, i.hitbox.width, i.hitbox.height}, i.hitbox, Vector2 {0, 0}, 0, WHITE);

				if (CheckCollisionRecs(cull, i.hitbox)) DrawTexturePro(grass, (Rectangle) {0 + global_animation * 1000, 0 + -global_animation * 20, i.hitbox.width * 10, i.hitbox.height * 10}, i.hitbox, Vector2 {0, 0}, 0, WHITE);

				// DrawTextEx(mona, TextFormat("%d %.1f %.1f\n", asdlkf, i.hitbox.x, i.hitbox.y), (Vector2){ i.hitbox.x, i.hitbox.y }, (float)mona.baseSize, 1, BLUE);
				// DrawTextEx(mona, TextFormat("%.1f %.1f\n", i.vel.x, i.vel.y), (Vector2){ i.hitbox.x, i.hitbox.y + 20 }, (float)mona.baseSize, 1, BLUE);
				index++;
			}
		}


		// DrawLineV((Vector2){player.pos.x, player.pos.y}, (Vector2){player.pos.x + player.x_vel, player.pos.y + player.y_vel}, RED);
		// DrawLineV((Vector2){player.pos.x + player.hitbox.width, player.pos.y + player.hitbox.height}, (Vector2){player.pos.x + player.hitbox.width + player.x_vel, player.pos.y + player.hitbox.height + player.y_vel}, RED);
		// DrawLineV((Vector2){player.pos.x + player.hitbox.width, player.pos.y}, (Vector2){player.pos.x + player.hitbox.width + player.x_vel, player.pos.y + player.y_vel}, RED);
		// DrawLineV((Vector2){player.pos.x, player.pos.y + player.hitbox.height}, (Vector2){player.pos.x + player.x_vel, player.pos.y + player.hitbox.height + player.y_vel}, RED);
 
		// DrawLineV((Vector2){player.pos.x + player.hitbox.width / 2, player.pos.y + player.hitbox.height / 2}, (Vector2){player.pos.x + player.hitbox.width / 2 +  player.x_vel, player.pos.y + player.hitbox.height / 2 + player.y_vel}, RED);

		DrawTexturePro(player.texture, (Rectangle) {(float)player.animation_frame * 50, 0, 50, 140}, (Rectangle) {player.pos.x - 2.5f, player.pos.y, player.hitbox.width + 5, player.hitbox.height}, (Vector2) {0, 0}, 0, WHITE);

		EndMode2D();

		DrawFPS(5, 2);

		DrawTextEx(mona, "GIRLS ~ KISSING! pre-pre-alpha ver 0.0.0", (Vector2){ 1.0f, 20.0f }, (float)mona.baseSize, 2, MAGENTA);
		DrawTextEx(mona, "91rl5 ki55in9.", (Vector2){ 1.0f, 40.0f }, (float)mona.baseSize, 2, RED);
		DrawTextEx(mona, "girls kissing - girls kissing - girls kissing - girls kissing", (Vector2){ 1.0f, 60.0f }, (float)mona.baseSize, 2, ORANGE);
		DrawTextEx(mona, TextFormat("x_vel %f\ny_vel %f\n", player.x_vel, player.y_vel), (Vector2){ 1.0f, 80.0f }, (float)mona.baseSize, 2, YELLOW);
		DrawTextEx(mona, TextFormat("pos x %f pos y %f\n", player.pos.x, player.pos.y), (Vector2){ 1.0f, 180.0f }, (float)mona.baseSize, 2, RED);
		DrawTextEx(mona, TextFormat("frame no. %d\n", frame), (Vector2){ 1.0f, 120.0f }, (float)mona.baseSize, 2, GREEN);
		DrawTextEx(mona, TextFormat("frame time: %f\n", GetFrameTime()), (Vector2){ 1.0f, 140.0f }, (float)mona.baseSize, 2, BLUE);
		DrawTextEx(mona, TextFormat("coyote: %d\n", player.coyote), (Vector2){ 1.0f, 160.0f }, (float)mona.baseSize, 2, PURPLE);


		EndDrawing();

		// printf("\n\n\n\nx vel: %f y vel: %f facing: %c\n", player.x_vel, player.y_vel, player.facing);
		// printf("prev x: %f prev y: %f\n", player.prev_pos.x, player.prev_pos.y);
		// printf("x: %f y: %f\n", player.pos.x, player.pos.y);
		// printf("x hitbox: %f y hitbox: %f\n", player.hitbox.x, player.hitbox.y);
		// printf("is jump: %d in air: %d\n", player.is_jump, player.in_air);
		// printf("is grounded: %d\n", player.is_grounded);

		// printf ("check: %d\n",
		// 	is_in_range(
		// 		platforms[20].hitbox.x,
		// 		platforms[20].hitbox.x + platforms[6].hitbox.width,
		// 		player.prev_pos.x,
		// 		player.prev_pos.x + player.hitbox.width
		// 	));
	}
	CloseWindow();
	CloseAudioDevice();
}
