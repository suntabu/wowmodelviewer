﻿/*
 * CharDetailsFrame.h
 *
 *  Created on: 21 dec. 2014
 *      Author: Jeromnimo
 */

#ifndef _CHARDETAILSFRAME_H_
#define _CHARDETAILSFRAME_H_

#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/window.h>
class wxSpinButton;
class wxSpinEvent;
class wxStaticText;


#include "CharDetails.h"

#include "metaclasses/Observer.h"

enum {
  SPIN_SKIN_COLOR,
  SPIN_FACE_TYPE,
  SPIN_HAIR_COLOR,
  SPIN_HAIR_STYLE,
  SPIN_FACIAL_HAIR,

  NUM_SPIN_BTNS,

  ID_SKIN_COLOR,
  ID_FACE_TYPE,
  ID_HAIR_COLOR,
  ID_HAIR_STYLE,
  ID_FACIAL_HAIR,
  ID_CHAR_RANDOMISE,
};

class CharDetailsFrame : public wxWindow, public Observer
{
  public:
    CharDetailsFrame(wxWindow* parent);

    void refresh();
    void randomiseChar();
    void setModel(CharDetails & details);


  protected:


  private:
    DECLARE_CLASS(CharDetailsFrame)
      DECLARE_EVENT_TABLE()

    wxSpinButton *spins[NUM_SPIN_BTNS];
    wxStaticText *spinLabels[NUM_SPIN_BTNS];
    wxFlexGridSizer * charCustomizationGS;

    void onSpin(wxSpinEvent &event);
    void onRandomise(wxCommandEvent &event);

    virtual void onEvent(Event *);

    void addControl(int type, int id, wxString caption);

    CharDetails * m_details;
};


#endif /* _CHARDETAILSFRAME_H_ */
