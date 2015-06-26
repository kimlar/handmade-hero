#ifndef HANDMADE_H__
#define HANDMADE_H__

#include <math.H>
#include <stdint.h>

#define internal static
#define local_persist static
#define global_variable static

//TODO: Implements sinf by myself
#define Pi32 3.14159265359f

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

#define ArrayCount(Array) (sizeof((Array)) / sizeof((Array)[0]))

#if HANDMADE_DEBUG
  #define Assert(expression) if(!(expression)){ *(int *)0 = 0; }
#elif
  #define Assert(expression)
#endif

#define Kilobytes(value) ((value)*1024LL)
#define Megabytes(value) (Kilobytes(value)*1024LL)
#define Gigabytes(value) (Megabytes(value)*1024LL)
#define Terabytes(value) (Gigabytes(value)*1024LL)





uint32
SafeTruncateUint64(uint64 Value)
{
  Assert( Value <= 0xffffffff );
  uint32 Result = (uint32)Value;
  return Result;
}





struct thread_context {
  int PlaceHolder;
};

struct game_offscreen_buffer
{
  void *Memory;
  int Width;
  int Height;
  int Pitch;
  int BytesPerPixel;
};

struct game_sound_buffer
{
  int SampleCount;
  int16 *Samples;
  int SamplesPerSecond;
};

struct game_button_state
{
  int HalfTransitionCount;
  bool EndedDown;
};

struct game_controller_input
{
  bool IsAnalog;
  bool IsConnected;
  real32 StickAverageX;
  real32 StickAverageY;

  union
  {
    game_button_state Buttons[12];
    struct
    {
      game_button_state MoveUp;
      game_button_state MoveDown;
      game_button_state MoveLeft;
      game_button_state MoveRight;

      game_button_state ActionUp;
      game_button_state ActionDown;
      game_button_state ActionLeft;
      game_button_state ActionRight;

      game_button_state LeftShoulder;
      game_button_state RightShoulder;

      game_button_state Start;
      game_button_state Back;

      //NOTE: All buttons must be added above terminator
      game_button_state Terminator;
    };
  };
};

struct game_input
{
  real32 TimeForFrame;
  game_button_state MouseButtons[5];
  int MouseX, MouseY, MouseZ;
  game_controller_input Controllers[5];
};

inline game_controller_input *GetController(game_input *Input, unsigned int Index)
{
  Assert(Index < ArrayCount(Input->Controllers));
  game_controller_input *Result = &Input->Controllers[Index];
  return Result;
}

#if HANDMADE_INTERNAL
/*
NOTE: These are NOT for doing anything for the shipping game
      they are blocking and the write doesn't protect against lost data
 */
struct debug_read_file_result
{
  uint32 ContentSize;
  void *Content;
};


#define DEBUG_PLATFORM_READ_FILE(name) debug_read_file_result name(thread_context *Context, char *Filename)
typedef DEBUG_PLATFORM_READ_FILE(debug_platform_read_file);

#define DEBUG_PLATFORM_WRITE_FILE(name) bool name(thread_context *Context, char *Filename, uint32 Size, void *Content)
typedef DEBUG_PLATFORM_WRITE_FILE(debug_platform_write_file);

#define DEBUG_PLATFORM_FREE_FILE_MEMORY(name) void name(thread_context *Context, void *Memory)
typedef DEBUG_PLATFORM_FREE_FILE_MEMORY(debug_platform_free_file_memory);

#endif
//NOTE: this memory should be initialized to zero by platform!
struct game_memory
{
  bool IsInitialized;
  uint64 PermanentStorageSize;
  void *PermanentStorage;

  uint64 TransientStorageSize;
  void *TransientStorage;

  debug_platform_read_file *DEBUGPlatformReadFile;
  debug_platform_write_file *DEBUGPlatformWriteFile;
  debug_platform_free_file_memory *DEBUGPlatformFreeFileMemory;
};

struct game_state
{
  real32 PlayerX;
  real32 PlayerY;

  int32 TileMapX;
  int32 TileMapY;
};

struct tile_map
{
  uint32 *Tiles;
};

struct world
{
  tile_map *TileMaps;
  real32 UpperLeftX;
  real32 UpperLeftY;
  real32 TileWidth;
  real32 TileHeight;
  int32 TileCountX;
  int32 TileCountY;
  int32 TileMapCountX;
  int32 TileMapCountY;
};

struct raw_pos
{
  int32 TileMapX;
  int32 TileMapY;

  // these values are relative to game window
  real32 X;
  real32 Y;
};

struct canonical_pos
{
  int32 TileMapX;
  int32 TileMapY;

  int32 TileX;
  int32 TileY;

  // these values are reltavie to tile
  real32 X;
  real32 Y;
};


#define GAME_UPDATE_VIDEO(name) void name(thread_context *Context, game_memory *Memory, game_input *Input, game_offscreen_buffer *Buffer)
typedef GAME_UPDATE_VIDEO(game_update_video);

#define GAME_UPDATE_AUDIO(name) void name(thread_context *Context, game_memory *Memory, game_sound_buffer *SoundBuffer)
typedef GAME_UPDATE_AUDIO(game_update_audio);


#endif
