/*
 * GameFile.cpp
 *
 *  Created on: 27 oct. 2014
 *      Author: Jerome
 */


#include "GameFile.h"

#include <cstring> // memcpy

#include "logger\Logger.h"

size_t GameFile::read(void* dest, size_t bytes)
{
  if (eof)
    return 0;

  size_t rpos = pointer + bytes;
  if (rpos > size) {
    bytes = size - pointer;
    eof = true;
  }

  memcpy(dest, &(buffer[pointer]), bytes);

  pointer = rpos;

  return bytes;
}

bool GameFile::isEof()
{
    return eof;
}

void GameFile::seek(size_t offset) {
  pointer = offset;
  eof = (pointer >= size);
}

void GameFile::seekRelative(size_t offset)
{
  pointer += offset;
  eof = (pointer >= size);
}

bool GameFile::close()
{
  delete[] originalBuffer;
  originalBuffer = 0;
  buffer = 0;
  eof = true;
  chunks.clear();
  return true;
}

void GameFile::allocate(unsigned int s)
{
  if (originalBuffer)
    delete[] originalBuffer;

  size = s;



  originalBuffer = new unsigned char[size];
  buffer = originalBuffer;

  if (size == 0)
    eof = true;
  else
    eof = false;
}

bool GameFile::setChunk(std::string chunkName)
{
  LOG_INFO << "Setting chunk to" << chunkName.c_str();
  bool result = false;
  for (auto it : chunks)
  {
    if (it.magic == chunkName)
    {
      LOG_INFO << "Found chunk" << chunkName.c_str();
      buffer = originalBuffer + it.start;
      pointer = 0;
      size = it.size;
      result = true;
      break;
    }
  }
  return result;
}

size_t GameFile::getSize()
{
  return size;
}

size_t GameFile::getPos()
{
  return pointer;
}

unsigned char* GameFile::getBuffer()
{
  return buffer;
}

unsigned char* GameFile::getPointer()
{
  return buffer + pointer;
}
