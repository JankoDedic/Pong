#include <SDL2/SDL.h>

int numerals [10] [5] [3] = {
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 0, 1},
        {1, 0, 1},
        {1, 1, 1}
    },
    {
        {0, 0, 1},
        {0, 1, 1},
        {1, 0, 1},
        {0, 0, 1},
        {0, 0, 1}
    },
    {
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1}
    },
    {
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1}
    },
    {
        {1, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {0, 0, 1}
    },
    {
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1}
    },
    {
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}
    },
    {
        {1, 1, 1},
        {0, 0, 1},
        {0, 0, 1},
        {0, 1, 0},
        {1, 0, 0}
    },
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}
    },
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1}
    }
};

SDL_Rect left_score_nums [10] [15];
SDL_Rect right_score_nums [10] [15];

int main(int argc, char *argv[])
{
    SDL_Init (SDL_INIT_VIDEO);

    int window_width = 1920 / 2;
    int window_height = 1080 / 2;
    SDL_Window *game_window = SDL_CreateWindow (
        "Pong",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width, window_height,
        SDL_WINDOW_SHOWN
    );
    SDL_assert (game_window);

    SDL_Renderer *game_renderer = SDL_CreateRenderer (
        game_window, -1, SDL_RENDERER_ACCELERATED
    );
    SDL_assert (game_renderer);

    SDL_Rect top_bar = {
        0,
        window_height / 25,
        window_width,
        window_height / 25
    };

    SDL_Rect bottom_bar = {
        0,
        window_height - 2 * window_height / 25,
        window_width,
        window_height / 25
    };

    SDL_Color bar_color = {80, 80, 80, SDL_ALPHA_OPAQUE};

    SDL_Rect net [14];

    int net_fragment_size = (bottom_bar.y - (top_bar.y + top_bar.h)) / 26;
    int x = window_width / 2 - net_fragment_size / 2;
    int y = top_bar.y + top_bar.h;
    int step_size = 2 * net_fragment_size;

    for (int index = 0; index < 14; ++index) {
        net [index] = {
            x, y + index * step_size, net_fragment_size, net_fragment_size
        };
    }

    int paddle_width = window_width / 80;
    int paddle_height = window_height / 7;

    SDL_Rect left_paddle = {
        5 * paddle_width,
        window_height / 2 - paddle_height / 2,
        paddle_width,
        paddle_height
    };
    int left_paddle_direction = 0;

    SDL_Rect right_paddle = {
        window_width - 5 * paddle_width,
        window_height / 2 - paddle_height / 2,
        paddle_width,
        paddle_height
    };
    int right_paddle_direction = 0;

    int paddle_speed = 5;

    SDL_Color paddle_color = {200, 200, 200, SDL_ALPHA_OPAQUE};
    
    int ball_width = net_fragment_size;
    int ball_height = net_fragment_size;
    SDL_Rect ball = {
        window_width / 2 - ball_width / 2,
        window_height / 2 - ball_height / 2,
        ball_width,
        ball_height
    };
    int ball_x_direction = 1;
    int ball_y_direction = -1;
    SDL_Rect ball_projection = ball;
    SDL_Rect ball_spawn = ball;
    int ball_speed = 5;

    SDL_Rect window_left_border = {
        0, 0, 1, window_height
    };
    SDL_Rect window_right_border = {
        window_width, 0, 1, window_height
    };

    int left_score = 0;
    int right_score = 0;

    int score_area_width = 3 * net_fragment_size;
    int score_area_height = 5 * net_fragment_size;
    SDL_Rect left_score_area = {
        window_width / 2 - (3 * net_fragment_size / 2) - score_area_width,
        y + net_fragment_size,
        score_area_width,
        score_area_height
    };
    SDL_Rect right_score_area = {
        window_width / 2 + (3 * net_fragment_size / 2),
        y + net_fragment_size,
        score_area_width,
        score_area_height
    };

    int count = 0;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 5; ++j) {
            for (int k = 0; k < 3; ++k) {
                left_score_nums [i] [count] = {
                    left_score_area.x + k * net_fragment_size,
                    left_score_area.y + j * net_fragment_size,
                    numerals [i] [j] [k] * net_fragment_size,
                    numerals [i] [j] [k] * net_fragment_size
                };

                right_score_nums [i] [count] = {
                    right_score_area.x + k * net_fragment_size,
                    right_score_area.y + j * net_fragment_size,
                    numerals [i] [j] [k] * net_fragment_size,
                    numerals [i] [j] [k] * net_fragment_size
                };

                ++count;
            }
        }
        count = 0;
    }

    SDL_Event e;
    int mouse_x, mouse_y;
    Uint32 last_update_time = 0;
    Uint32 update_threshold = 16;
    bool is_running = true;
    while (is_running) {
        if (SDL_PollEvent (&e) && e.type == SDL_QUIT)
            is_running = false;

        left_paddle_direction = 0;
        right_paddle_direction = 0;
        if (SDL_BUTTON (SDL_GetMouseState (&mouse_x, &mouse_y))
        & SDL_BUTTON_LEFT) {
            if (mouse_x < window_width / 2) {
                if (mouse_y < window_height / 2)
                    left_paddle_direction = -1;
                else
                    left_paddle_direction = 1;
            }
            else {
                if (mouse_y < window_height / 2)
                    right_paddle_direction = -1;
                else
                    right_paddle_direction = 1;
            }
        }

        if (SDL_TICKS_PASSED (SDL_GetTicks (),
        last_update_time + update_threshold)) {
            left_paddle.y += left_paddle_direction * paddle_speed;
            right_paddle.y += right_paddle_direction * paddle_speed;

            bool scored = false;
            
            ball_projection.x += ball_x_direction * ball_speed;
            ball_projection.y += ball_y_direction * ball_speed;
            if (SDL_HasIntersection (&ball_projection, &top_bar)
            || SDL_HasIntersection (&ball_projection, &bottom_bar))
                ball_y_direction *= -1;
            else
            if (SDL_HasIntersection (&ball_projection, &left_paddle)
            || SDL_HasIntersection (&ball_projection, &right_paddle))
                ball_x_direction *= -1;
            else
            if (SDL_HasIntersection (&ball_projection, &window_left_border)) {
                if (++right_score > 9) {
                    left_score = 0;
                    right_score = 0;
                }
                ball_x_direction *= -1;
                ball_y_direction *= -1;
                ball = ball_spawn;
                scored = true;
            }
            else
            if (SDL_HasIntersection (&ball_projection, &window_right_border)) {
                if (++left_score > 9) {
                    left_score = 0;
                    right_score = 0;
                }
                ball_x_direction *= -1;
                ball_y_direction *= -1;
                ball = ball_spawn;
                scored = true;
            }

            ball.x += ball_x_direction * ball_speed;
            ball.y += ball_y_direction * ball_speed;

            if (scored)
                ball_projection = ball;

            last_update_time = SDL_GetTicks ();
        }

        SDL_SetRenderDrawColor (game_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear (game_renderer);

        SDL_SetRenderDrawColor (game_renderer,
            bar_color.r, bar_color.g, bar_color.b, bar_color.a);
        SDL_RenderFillRect (game_renderer, &top_bar);
        SDL_RenderFillRect (game_renderer, &bottom_bar);

        for (int index = 0; index < 14; ++index)
            SDL_RenderFillRect (game_renderer, &net [index]);

        SDL_SetRenderDrawColor (game_renderer,
            paddle_color.r, paddle_color.g, paddle_color.b, paddle_color.a);
        SDL_RenderFillRect (game_renderer, &left_paddle);
        SDL_RenderFillRect (game_renderer, &right_paddle);
        SDL_RenderFillRect (game_renderer, &ball);

        for (int i = 0; i < 15; ++i) {
            SDL_RenderFillRect (game_renderer,
                    &left_score_nums [left_score] [i]);
            SDL_RenderFillRect (game_renderer,
                    &right_score_nums [right_score] [i]);
        }

        SDL_RenderPresent (game_renderer);
    }

    SDL_Quit ();
    return 0;
}
