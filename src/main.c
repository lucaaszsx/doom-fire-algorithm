#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
// #define DEBUG

// Constants
const char *APP_TITLE       = "DOOM Fire Effect";
const char *DEBUG_FONT_PATH = "./assets/fonts/YOUR_FONT.ext";
const double TARGET_FPS        = 24.0;

enum {
    SCREEN_WIDTH        = 800,
    SCREEN_HEIGHT       = 400,
    MAP_ROWS            = 80,
    MAP_COLUMNS         = 100,
    MAX_INTENSITY_LEVEL = 36,
    FIRE_DECAY          = 3,
    FIRE_SPREAD         = 2
};

const double TARGET_FRAME_TIME = 1.0 / TARGET_FPS;

const int TOTAL_MAP_SIZE = MAP_ROWS * MAP_COLUMNS;
const int CELL_WIDTH     = SCREEN_WIDTH / MAP_COLUMNS;
const int CELL_HEIGHT    = SCREEN_HEIGHT / MAP_ROWS;

const SDL_Color DEBUG_RECT_COLOR   = { 0, 0, 0, 0 };
const SDL_Color DEBUG_STROKE_COLOR = { 0, 0, 255, 255 };
const SDL_Color DEBUG_TEXT_COLOR   = { 255, 255, 255, 255 };

const SDL_Color FIRE_INTENSITY[MAX_INTENSITY_LEVEL + 1] = {
    { 7, 7, 7, 255 }, { 31, 7, 7, 255 }, { 47, 15, 7, 255 }, { 71, 15, 7, 255 }, { 87, 23, 7, 255 },
    { 103, 31, 7, 255 }, { 119, 31, 7, 255 }, { 143, 39, 7, 255 }, { 159, 47, 7, 255 },
    { 175, 63, 7, 255 }, { 191, 71, 7, 255 }, { 199, 71, 7, 255 }, { 223, 79, 7, 255 },
    { 223, 87, 7, 255 }, { 223, 87, 7, 255 }, { 215, 95, 7, 255 }, { 215, 95, 7, 255 },
    { 215, 103, 15, 255 }, { 207, 111, 15, 255 }, { 207, 119, 15, 255 }, { 207, 127, 15, 255 },
    { 207, 135, 23, 255 }, { 199, 135, 23, 255 }, { 199, 143, 23, 255 }, { 199, 151, 31, 255 },
    { 191, 159, 31, 255 }, { 191, 159, 31, 255 }, { 191, 167, 39, 255 }, { 191, 167, 39, 255 },
    { 191, 175, 47, 255 }, { 183, 175, 47, 255 }, { 183, 183, 47, 255 }, { 183, 183, 55, 255 },
    { 207, 207, 111, 255 }, { 223, 223, 159, 255 }, { 239, 239, 199, 255 }, { 255, 255, 255, 255 }
};

// Structs
typedef struct {
    SDL_Color color;
    float width;
} RectStroke;

typedef struct {
    SDL_Color color;
    float x;
    float y;
    float w;
    float h;
    RectStroke stroke;
} CustomFRect;

#ifdef DEBUG
typedef struct {
    SDL_Texture *textures[MAX_INTENSITY_LEVEL + 1];
    int widths[MAX_INTENSITY_LEVEL + 1];
    int heights[MAX_INTENSITY_LEVEL + 1];
} TextCache;
#endif

/**
 * Initializes SDL3, SDL3_ttf and creates window, renderer and font
 * Exits program if initialization fails
 */
void init_sdl(SDL_Window **window, SDL_Renderer **renderer
              #ifdef DEBUG
              , TTF_Font **font
              #endif
);

/**
 * Allocates and initializes fire map with base intensity values
 * Bottom row is set to MAX_INTENSITY_LEVEL, others to 0
 */
void create_base_map(int **fire_map);

/**
 * Calculates fire propagation for each frame
 * Applies vertical and horizontal decay with randomness
 */
void calculate_propagation(int *fire_map);

/**
 * Renders a custom rectangle with stroke support
 */
void render_rect(SDL_Renderer *renderer, CustomFRect custom_rect);

#ifdef DEBUG
/**
 * Initializes text cache by pre-rendering all intensity values
 */
void init_text_cache(TextCache *cache, SDL_Renderer *renderer, TTF_Font *font);

/**
 * Destroys all cached textures
 */
void destroy_text_cache(TextCache *cache);

/**
 * Renders fire with debug information using cached textures
 */
void render_fire(SDL_Renderer *renderer, int *fire_map, TextCache *cache);
#else
/**
 * Renders fire without debug information
 */
void render_fire(SDL_Renderer *renderer, int *fire_map);
#endif

/**
 * Cleans up all resources before closing application
 */
void close_app(SDL_Window *window, SDL_Renderer *renderer, int *fire_map
               #ifdef DEBUG
               , TTF_Font *font,
               TextCache *cache
               #endif
);

int main(int argc, char **argv) {
    srand(time(NULL));

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    bool running = true;
    
    #ifdef DEBUG
    TTF_Font *font;
    TextCache text_cache;
    #endif

    init_sdl(&window, &renderer
        #ifdef DEBUG
        , &font
        #endif
    );

    #ifdef DEBUG
    init_text_cache(&text_cache, renderer, font);
    #endif

    int *fire_map;
    create_base_map(&fire_map);
    
    Uint64 freq = SDL_GetPerformanceFrequency();

    while (running) {
        Uint64 frame_start = SDL_GetPerformanceCounter();

        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Logic
        calculate_propagation(fire_map);

        // Draw elements
        #ifdef DEBUG
        render_fire(renderer, fire_map, &text_cache);
        #else
        render_fire(renderer, fire_map);
        #endif

        // Render
        SDL_RenderPresent(renderer);

        // FPS
        Uint64 frame_end = SDL_GetPerformanceCounter();
        double frame_time = (double)(frame_end - frame_start) / (double)freq;

        if (frame_time < TARGET_FRAME_TIME) {
            double delay_seconds = TARGET_FRAME_TIME - frame_time;
            SDL_Delay((Uint32)(delay_seconds * 1000.0));
        }
    }

    close_app(window, renderer, fire_map
              #ifdef DEBUG
              , font, &text_cache
              #endif
    );

    return 0;
}

void init_sdl(SDL_Window **window, SDL_Renderer **renderer
              #ifdef DEBUG
              , TTF_Font **font
              #endif
) {
    if (!SDL_CreateWindowAndRenderer(APP_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT,
                                    0, window, renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        exit(1);
    }

    #ifdef DEBUG
    if (!TTF_Init()) {
        SDL_Log("Couldn't initialize the SDL3 TTF: %s", SDL_GetError());
        exit(1);
    }

    *font = TTF_OpenFont(DEBUG_FONT_PATH, 18.0f);

    if (!*font) {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        exit(1);
    }
    #endif
}

void create_base_map(int **fire_map) {
    int *new_fire_map = malloc(TOTAL_MAP_SIZE * sizeof(int));

    if (!new_fire_map) {
        SDL_Log("Failed to allocate fire map");
        exit(1);
    }

    for (int i = 0; i < TOTAL_MAP_SIZE; i++)
        new_fire_map[i] = i >= TOTAL_MAP_SIZE - MAP_COLUMNS
            ? MAX_INTENSITY_LEVEL
            : 0;
    
    *fire_map = new_fire_map;
}

void calculate_propagation(int *fire_map) {
    for (int j = 0; j < MAP_ROWS - 1; j++) {
        for (int k = 0; k < MAP_COLUMNS; k++) {
            int current_index = k + j * MAP_COLUMNS;
            int below_index   = current_index + MAP_COLUMNS;
            int below_intensity = fire_map[below_index];

            if (below_intensity == 0) {
                fire_map[current_index] = 0;
                continue;
            }
            
            int decay = rand() % (FIRE_DECAY + 1);
            int spread = rand() % (FIRE_SPREAD + 1);
            
            int new_intensity = MAX(below_intensity - decay, 0);
            
            int new_col = MAX(k - spread, 0);
            int target_index = new_col + j * MAP_COLUMNS;
            
            fire_map[target_index] = new_intensity;
        }
    }
}

void render_rect(SDL_Renderer *renderer, CustomFRect custom_rect) {
    SDL_Color rect_color = custom_rect.color;
    float x = custom_rect.x;
    float y = custom_rect.y;
    float w = custom_rect.w;
    float h = custom_rect.h;

    SDL_Color stroke_color = custom_rect.stroke.color;
    float stroke_width = custom_rect.stroke.width;

    const SDL_FRect border_rect = {
        x, y,
        w, h
    };
    const SDL_FRect rect = {
        x + stroke_width, y + stroke_width,
        w - stroke_width * 2, h - stroke_width * 2
    };
    
    SDL_SetRenderDrawColor(renderer, stroke_color.r, stroke_color.g,
                            stroke_color.b, stroke_color.a);
    SDL_RenderFillRect(renderer, &border_rect);

    SDL_SetRenderDrawColor(renderer, rect_color.r, rect_color.g,
                            rect_color.b, rect_color.a);
    SDL_RenderFillRect(renderer, &rect);
}

#ifdef DEBUG
void init_text_cache(TextCache *cache, SDL_Renderer *renderer, TTF_Font *font) {
    char intensity_text[4];
    
    // Pre-render all possible intensity values
    for (int i = 0; i <= MAX_INTENSITY_LEVEL; i++) {
        snprintf(intensity_text, sizeof(intensity_text), "%d", i);
        
        SDL_Surface *text_surface = TTF_RenderText_Blended(font, intensity_text, 0,
                                                           DEBUG_TEXT_COLOR);
        
        if (!text_surface) {
            SDL_Log("Failed to render text for intensity %d: %s", i, SDL_GetError());
            cache->textures[i] = NULL;
            continue;
        }
        
        cache->textures[i] = SDL_CreateTextureFromSurface(renderer, text_surface);
        cache->widths[i] = text_surface->w;
        cache->heights[i] = text_surface->h;
        
        SDL_DestroySurface(text_surface);
        
        if (!cache->textures[i]) {
            SDL_Log("Failed to create texture for intensity %d: %s", i, SDL_GetError());
        }
    }
}

void destroy_text_cache(TextCache *cache) {
    for (int i = 0; i <= MAX_INTENSITY_LEVEL; i++) {
        if (cache->textures[i]) {
            SDL_DestroyTexture(cache->textures[i]);
            cache->textures[i] = NULL;
        }
    }
}

void render_fire(SDL_Renderer *renderer, int *fire_map, TextCache *cache) {
    CustomFRect debug_rect = {
        .color = DEBUG_RECT_COLOR,
        .w = CELL_WIDTH,
        .h = CELL_HEIGHT,
        .x = 0,
        .y = 0,
        .stroke = {
            .color = DEBUG_STROKE_COLOR,
            .width = 1
        }
    };

    SDL_FRect text_dst_rect;

    for (int k = 0; k < MAP_COLUMNS; k++) {
        for (int j = 0; j < MAP_ROWS; j++) {
            int index = j * MAP_COLUMNS + k;
            int intensity = fire_map[index];
            int rx = CELL_WIDTH * k;
            int ry = CELL_HEIGHT * j;

            debug_rect.x = rx;
            debug_rect.y = ry;

            // Use cached texture
            SDL_Texture *text_texture = cache->textures[intensity];
            
            if (text_texture) {
                text_dst_rect.w = cache->widths[intensity];
                text_dst_rect.h = cache->heights[intensity];
                text_dst_rect.x = rx + (CELL_WIDTH - text_dst_rect.w) / 2.0f;
                text_dst_rect.y = ry + (CELL_HEIGHT - text_dst_rect.h) / 2.0f;

                render_rect(renderer, debug_rect);
                SDL_RenderTexture(renderer, text_texture, NULL, &text_dst_rect);
            } else {
                render_rect(renderer, debug_rect);
            }
        }
    }
}
#else
void render_fire(SDL_Renderer *renderer, int *fire_map) {
    for (int k = 0; k < MAP_COLUMNS; k++) {
        for (int j = 0; j < MAP_ROWS; j++) {
            int index = j * MAP_COLUMNS + k;
            SDL_Color color = FIRE_INTENSITY[fire_map[index]];
            
            SDL_FRect cell_rect = {
                .x = CELL_WIDTH * k,
                .y = CELL_HEIGHT * j,
                .w = CELL_WIDTH,
                .h = CELL_HEIGHT
            };
            
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &cell_rect);
        }
    }
}
#endif

void close_app(SDL_Window *window, SDL_Renderer *renderer,
               int *fire_map
               #ifdef DEBUG
               , TTF_Font *font,
               TextCache *cache
               #endif
) {
    #ifdef DEBUG
    destroy_text_cache(cache);

    if (font) TTF_CloseFont(font);
    #endif
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    free(fire_map);

    #ifdef DEBUG
    TTF_Quit();
    #endif
    SDL_Quit();
}
