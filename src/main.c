#include "wasm4.h"

// 6x6 ball = 36 bits => 5 bytes
// 001100
// 011110
// 111111
// 111111
// 011110
// 001100
//
// For some reason it has to be reversed
// if I want to keep the color
const uint8_t ball[] = { 
	0b11001110,
	0b00010000,
	0b00000000,
	0b10000111,
	0b00111111,
};

const int32_t player_width = 6;
const int32_t player_height = 40;
const int32_t player1_x = 1;
const int32_t player2_x = 153;
char player1_score[3] = "0";
char player2_score[3] = "0";
int32_t player1_y = 60;
int32_t player2_y = 60;

int32_t ball_speed_x;
int32_t ball_speed_y;
int32_t ball_x;
int32_t ball_y;
int32_t collision_timeout_x = 0;
int32_t collision_timeout_y = 0;

void restart_ball() {
	ball_speed_x = 2;
	ball_speed_y = 2;
	ball_x = 77;
	ball_y = 77;
}

void start() {
	restart_ball();
}

void update() {
	const uint8_t PLAYER1 = *GAMEPAD1;
	const uint8_t PLAYER2 = *GAMEPAD2;

	if (collision_timeout_x != 0) --collision_timeout_x;
	if (collision_timeout_y != 0) --collision_timeout_y;

	ball_x += ball_speed_x;
	ball_y += ball_speed_y;

	if (PLAYER1 & BUTTON_UP)
		player1_y -= 1;

	if (PLAYER1 & BUTTON_DOWN)
		player1_y += 1;

	if (PLAYER2 & BUTTON_UP)
		player2_y -= 1;

	if (PLAYER2 & BUTTON_DOWN)
		player2_y += 1;

	if ((ball_y <= 0 || ball_y - 160 >= 0) ) {
		ball_speed_y = - ball_speed_y;
	}

	if (
			(ball_x <= 7 && ball_y + 6 >= player1_y && ball_y <= player1_y + player_height) ||
			(ball_x >= 153 && (ball_y + 6 >= player2_y && ball_y <= player2_y + player_height)) 
		 )
	{
		ball_speed_x = -ball_speed_x;
	}

	if (ball_x < 0) {
		if (player1_score[1] == '\0') {
			if (player1_score[0] != '9') {
				++player1_score[0];
			} else {
				player1_score[0] = '1';
				player1_score[1] = '0';
			}
		} else {
			if (player1_score[1] != '9') {
				++player1_score[1];
			} else {
				if (player1_score[0] == '9') {
					player1_score[0] = '0';
					player1_score[1] = '\0';
				} else {
					++player1_score[0];
					player1_score[1] = '0';
				}
			}
		}
		restart_ball();
	}

	if (ball_x >= 160) {
		if (player2_score[1] == '\0') {
			if (player2_score[0] != '9') {
				++player2_score[0];
			} else {
				player2_score[0] = '1';
				player2_score[1] = '0';
			}
		} else {
			if (player2_score[1] != '9') {
				++player2_score[1];
			} else {
				if (player2_score[0] == '9') {
					player2_score[0] = '0';
					player2_score[1] = '\0';
				} else {
					++player2_score[0];
					player2_score[1] = '0';
				}
			}
		}
		restart_ball();
	}

	*DRAW_COLORS = 0x02;
	vline(79, 0, 160);
	vline(80, 0, 160);

	*DRAW_COLORS = 0x04;
	text(player1_score, 40, 30);
	text(player2_score, 110, 30);

	*DRAW_COLORS = 0x03;
	rect(player1_x, player1_y, player_width, player_height);
	rect(player2_x, player2_y, player_width, player_height);

	*DRAW_COLORS = 0x03;
	blit(ball, ball_x, ball_y, 6, 6, BLIT_1BPP);
}
