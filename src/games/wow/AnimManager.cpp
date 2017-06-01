/*
 * AnimManager.cpp
 *
 *  Created on: 19 oct. 2013
 *
 */
#include "AnimManager.h"
#include "WoWModel.h"
#include "logger/Logger.h"

#include "wow_enums.h"

AnimManager::AnimManager(ModelAnimation *anim)
{
  AnimIDSecondary = -1;
  SecondaryCount = UPPER_BODY_BONES;
  AnimIDMouth = -1;
  anims = anim;
  Count = 1;
  PlayIndex = 0;
  CurLoop = 0;
  animList[0].AnimID = 0;
  animList[0].Loops = 0;

  if (anims != NULL)
  {
    Frame = anims[0].timeStart;
    TotalFrames = anims[0].timeEnd - anims[0].timeStart;
  }
  else
  {
    Frame = 0;
    TotalFrames = 0;
  }
  Speed = 1.0f;
  mouthSpeed = 1.0f;
  Paused = false;
}

AnimManager::~AnimManager()
{
  anims = NULL;
}

void AnimManager::SetCount(int count)
{
  Count = count;
}

void AnimManager::AddAnim(unsigned int id, short loops)
{
  if (Count > 3)
    return;

  animList[Count].AnimID = id;
  animList[Count].Loops = loops;
  Count++;
}

void AnimManager::SetAnim(short index, unsigned int id, short loops)
{
  // error check, we currently only support 4 animations.
  if (index > 3)
    return;

  animList[index].AnimID = id;
  animList[index].Loops = loops;

  // Just an error check for our "auto animate"
  if (index == 0)
  {
    Count = 1;
    PlayIndex = index;
    Frame = anims[id].timeStart;
    TotalFrames = anims[id].timeEnd - anims[id].timeStart;
  }

  if (index+1 > Count)
    Count = index+1;
}

void AnimManager::Play()
{
  if (!Paused)
    return;
  Paused = false;
  TotalFrames = GetFrameCount();
  Paused = false;
}

void AnimManager::Stop()
{
  Paused = true;
  PlayIndex = 0;
  SetFrame(anims[animList[0].AnimID].timeStart);
  CurLoop = animList[0].Loops;
  TotalFrames = GetFrameCount();
}

void AnimManager::Pause(bool force)
{
  if (Paused && force == false)
    Paused = false;
  else
    Paused = true;
}

void AnimManager::Next()
{
  if(CurLoop == 1)
  {
    PlayIndex++;
    if (PlayIndex >= Count)
    {
      Stop();
      return;
    }
    CurLoop = animList[PlayIndex].Loops;
  }
  else if(CurLoop > 1)
  {
    CurLoop--;
  }
  else if(CurLoop == 0)
  {
    PlayIndex++;
    if (PlayIndex >= Count)
      PlayIndex = 0;
  }

  Frame = anims[animList[PlayIndex].AnimID].timeStart;
  TotalFrames = GetFrameCount();
}

void AnimManager::Prev()
{
  if(CurLoop >= animList[PlayIndex].Loops)
  {
    PlayIndex--;

    if (PlayIndex < 0)
    {
      Stop();
      return;
    }
    CurLoop = animList[PlayIndex].Loops;
  }
  else if(CurLoop < animList[PlayIndex].Loops)
  {
    CurLoop++;
  }

  Frame = anims[animList[PlayIndex].AnimID].timeEnd;
  TotalFrames = GetFrameCount();
}

int AnimManager::Tick(int time)
{
  if((Count < PlayIndex) )
    return -1;

  Frame += int(time*Speed);

  // animate our mouth animation
  if (AnimIDMouth > -1)
  {
    FrameMouth += (time*mouthSpeed);

    if (FrameMouth >= anims[AnimIDMouth].timeEnd)
      FrameMouth -= (anims[AnimIDMouth].timeEnd - anims[AnimIDMouth].timeStart);
    else if (FrameMouth < anims[AnimIDMouth].timeStart)
      FrameMouth += (anims[AnimIDMouth].timeEnd - anims[AnimIDMouth].timeStart);
  }

  // animate our second (upper body) animation
  if (AnimIDSecondary > -1)
  {
    FrameSecondary += (time*Speed);

    if (FrameSecondary >= anims[AnimIDSecondary].timeEnd)
      FrameSecondary -= (anims[AnimIDSecondary].timeEnd - anims[AnimIDSecondary].timeStart);
    else if (FrameSecondary < anims[AnimIDSecondary].timeStart)
      FrameSecondary += (anims[AnimIDSecondary].timeEnd - anims[AnimIDSecondary].timeStart);
  }

  if (Frame >= anims[animList[PlayIndex].AnimID].timeEnd)
  {
    Next();
    return 1;
  }
  else if (Frame < anims[animList[PlayIndex].AnimID].timeStart)
  {
    Prev();
    return 1;
  }
  return 0;
}

size_t AnimManager::GetFrameCount()
{
  return (anims[animList[PlayIndex].AnimID].timeEnd - anims[animList[PlayIndex].AnimID].timeStart);
}


void AnimManager::NextFrame()  // Only called by the animation controls
{
  ssize_t TimeDiff;
  ssize_t id = animList[PlayIndex].AnimID;
  Frame += ((anims[id].timeEnd - anims[id].timeStart) / 60);
  TimeDiff = ((anims[id].timeEnd - anims[id].timeStart) / 60);
  ForceModelUpdate(TimeDiff);
}

void AnimManager::PrevFrame()  // Only called by the animation controls
{
  ssize_t TimeDiff;
  ssize_t id = animList[PlayIndex].AnimID;
  Frame -= ((anims[id].timeEnd - anims[id].timeStart) / 60);
  TimeDiff = ((anims[id].timeEnd - anims[id].timeStart) / 60) * -1;
  ForceModelUpdate(TimeDiff);
}

void AnimManager::SetFrame(size_t f)  // Only called by the animation slider, or if Stop is called
{
  ssize_t TimeDiff;
  TimeDiff = f - Frame;
  ssize_t id = animList[PlayIndex].AnimID;

  // ideal frame interval:
  int frameInterval = (anims[id].timeEnd - anims[id].timeStart) / 60;

  // Update the model approx. ideal frame intervals, or rendering may not be smooth if slider is moved quickly...
  size_t i = round(abs(TimeDiff / frameInterval));  // Number of chunks to break it up into
  if (i > 1)
  {
    for (size_t j = 0; j < i; j++)
    {
      ForceModelUpdate((float) TimeDiff/i);
    }
  }
  else
    ForceModelUpdate(TimeDiff);
  Frame = f;
}

void AnimManager::Clear()
{
  Stop();
  Paused = true;
  PlayIndex = 0;
  Count = 0;
  CurLoop = 0;
  Frame = 0;
}

void AnimManager::ForceModelUpdate(float dt)
{
  model->update(dt);
  model->drawParticles();
  model->draw();
}

