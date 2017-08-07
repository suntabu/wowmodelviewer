/*
 * GameFile.h
 *
 *  Created on: 27 oct. 2014
 *      Author: Jerome
 */

#ifndef _GAMEFILE_H_
#define _GAMEFILE_H_

#include <stdio.h>
#include <string>

#include "metaclasses/Component.h"

#ifdef _WIN32
#    ifdef BUILDING_CORE_DLL
#        define _GAMEFILE_API_ __declspec(dllexport)
#    else
#        define _GAMEFILE_API_ __declspec(dllimport)
#    endif
#else
#    define _GAMEFILE_API_
#endif

class _GAMEFILE_API_ GameFile : public Component
{
  public:
    GameFile(QString path, int id = -1) :eof(true), buffer(0), pointer(0), size(0), filepath(path), m_fileDataId(id) {}
    virtual ~GameFile() {}

    size_t read(void* dest, size_t bytes);
    size_t getSize();
    size_t getPos();
    unsigned char* getBuffer();
    unsigned char* getPointer();
    bool isEof();
    void seek(size_t offset);
    void seekRelative(size_t offset);
    virtual bool open() = 0;
    virtual bool close();

    void setFullName(const QString & name) { filepath = name; }
    QString fullname() const { return filepath; }
    int fileDataId() { return m_fileDataId; }

  protected:
    bool eof;
    unsigned char *buffer;
    size_t pointer, size;
    QString filepath;

  private:
    // disable copying
    GameFile(const GameFile &);
    void operator=(const GameFile &);

    int m_fileDataId;
};



#endif /* _GAMEFILE_H_ */
