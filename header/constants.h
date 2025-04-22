#ifndef CONSTANTS_H
#define CONSTANTS_H

// Basic Ray Tracing Constants
const int IMAGE_WIDTH = 512;
const int IMAGE_HEIGHT = 368;
const float FOCAL_LENGTH = 0.5;
const float FILM_PLANE_WIDTH = 1.33;
const float FILM_PLANE_HEIGHT = 1;
const float CAMERA_POS_X = 0.0;
const float CAMERA_POS_Y = 1.0;
const float CAMERA_POS_Z = 7.0;
const float CAMERA_UP_X = 0.0;
const float CAMERA_UP_Y = 1.0;
const float CAMERA_UP_Z = 0.0;
const float CAMERA_LOOKAT_X = 0.0;
const float CAMERA_LOOKAT_Y = 0.0;
const float CAMERA_LOOKAT_Z = 0.0;
const float BACKGROUND_RADIANCE_RED = 25.49;
const float BACKGROUND_RADIANCE_GREEN = 60.39;
const float BACKGROUND_RADIANCE_BLUE = 95.29;

// Tone Reproduction
const int WHICH_TONE_REPRODUCTION = 0;
const bool REINHARD_USE_LOG_AVERAGE = false;
const bool REINHARD_USE_USER_CONSTANT = true;
const float MAXIMUM_IRRADIANCE = 100.0;
const int REINHARD_KEY_VALUE_PIXEL_X = 124;
const int REINHARD_KEY_VALUE_PIXEL_Y = 350;
const float REINHARD_USER_CONSTANT = 78.0f;

// Math Constants
const float PI = 3.14159265358979323846;
const float INDEX_REFRACTION_AIR = 1.0f;

// Supersampling toggle
const bool SUPERSAMPLING = true;

// KD-Tree Stuff
const bool USE_KD_TREES = false;
const bool USE_SURFACE_AREA_HEURISTIC = true;
const bool IGNORE_MIN_SPLIT_PLANE = true;
const float COST_OF_TRAVERSAL = 1.0f;
const bool USE_BINNING = true;
const int NUMBER_OF_BINS = 128;
const bool TERMINATE_WITH_LEVELS = true;
const bool TERMINATE_WITH_VOLUME = true;
const bool TERMINATE_WITH_PRIMITIVES = false;
const float VOXEL_TERMINAL_VOLUME = 0.3f;
const int MAX_PRIMITIVES_PER_VOXEL = 5;
const int KD_MAX_RECURSION_LEVEL = 3;

#endif