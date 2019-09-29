#include "handmade.h"

internal void
RenderWeirdGradeint(game_offscreen_buffer *Buffer, int XOffset, int YOffset) {
  uint8 *Row = (uint8 *)Buffer->Memory;

  for(int Y = 0; Y < Buffer->Height; Y++) {
    uint32 *Pixel = (uint32 *)Row;

    for(int X = 0; X < Buffer->Width; X++) {
      uint8 Blue = (uint8)(X + XOffset);
      uint8 Green = (uint8)(Y + YOffset);
      // 0xXXRRGGBB
      *Pixel++ = (Green << 16) | Blue;
    }

    Row = Row + Buffer->Width * Buffer->BytesPerPixel;
  }
}

internal void
RenderSineWave(
  game_state *State,
  game_sound_buffer *Buffer
) {
  int ToneHz = State->ToneHz;
  int WavePeriod = Buffer->SamplesPerSecond / ToneHz;

  int16* Output = Buffer->Memory;
  for(int SampleIndex = 0; SampleIndex < Buffer->SampleCount; SampleIndex++) {
    int16 Value = (int16)(sinf(State->tSine) * Buffer->ToneVolume);
    *Output++ = Value;
    *Output++ = Value;
    State->tSine += (real32)(2.0f * Pi32 * (1.0f / (real32)WavePeriod));
    if(State->tSine > 2.0f * Pi32) {
      State->tSine -= (real32)(2.0f * Pi32);
    }
  }
}

extern "C" GAME_UPDATE_VIDEO(GameUpdateVideo) {
  Assert((&Input->Controllers[0].Terminator - &Input->Controllers[0].Buttons[0]) == ArrayCount(Input->Controllers[0].Buttons))

  game_state *State = (game_state *)Memory->PermanentStorage;

  Assert(sizeof(game_state) <= Memory->PermanentStorageSize)
  if(!Memory->IsInitialized) {
    char *Filename = __FILE__;

    debug_read_file_result File = Memory->DebugPlatformReadFile(Filename);
    if(File.Memory)
    {
        Memory->DebugPlatformWriteFile("test.out", File.Memory, File.Size);
        Memory->DebugPlatformFreeFileMemory(File.Memory);
    }

    State->ToneHz = 256;
    State->tSine = 0;
    Memory->IsInitialized = true;
  }

  int delta = 5;
  game_controller_input *Controller = &Input->Controllers[0];
  if(Controller->MoveUp.IsEndedDown) {
    State->YOffset += delta;
  }
  if(Controller->MoveDown.IsEndedDown) {
    State->YOffset -= delta;
  }
  if(Controller->MoveLeft.IsEndedDown) {
    State->XOffset -= delta;
  }
  if(Controller->MoveRight.IsEndedDown) {
    State->XOffset += delta;
  }
  if(State->YOffset <= 1000 && State->YOffset >= -1000) {
    int Tmp = (int)(((real32)State->YOffset / 1000) * 256) + 512;
    State->ToneHz = Tmp;
  }

  RenderWeirdGradeint(Buffer, State->XOffset, State->YOffset);
}

extern "C" GAME_UPDATE_AUDIO(GameUpdateAudio) {
  game_state *State = (game_state *)Memory->PermanentStorage;
  RenderSineWave(State, SoundBuffer);
}
