#pragma once

const int  MORPHING_RATE = 500;
const int CUBE_SIZE = 100000;
const int CUBE_SCALE = 5;
const int POINT_SIZE = 6;	

const int LOAD_MODE1 = 1;
const int LOAD_MODE2 = 2;

const unsigned int CHARACTER_NUM = 10;

// Bounding volume types
const int BOUNDING_CIRCLE = 0;
const int BOUNDING_LINE = 1;

// Waiting time to generate a new non-player
const int WAIT_TIME_MIN = 5;
const int WAIT_TIME_MAX = 10;

// Life cycel of a non-player
const int LIFE_TIME_MIN = 10;
const int LIFE_TIME_MAX = 20;

// Stop gap for a non-player
const int STOP_GAP_MIN = 5;
const int STOP_GAP_MAX = 30;

const int CHARACTER_PLAYER = 0;
const int CHARACTER_BOT = 1;

// status for bot
const int AWAKE = 1;
const int SLEEP = 2;

const float BOUNCE_OFFSET = 0.3f;

const float PARTICLE_SYSTEM_LIFE = 5.0f;