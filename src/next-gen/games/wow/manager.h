#ifndef MANAGER_H
#define MANAGER_H

// wxWidgets
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// STL
#include <string>
#include <map>

#include "OpenGLHeaders.h"

#include <iostream>

// base class for manager objects

#ifdef _WIN32
#    ifdef BUILDING_WOW_DLL
#        define _MANAGEDITEM_API_ __declspec(dllexport)
#    else
#        define _MANAGEDITEM_API_ __declspec(dllimport)
#    endif
#else
#    define _DISPLAYABLE_API_
#endif

class _MANAGEDITEM_API_ ManagedItem {
	int refcount;
public:
	wxString wxname;
	ManagedItem(wxString n): refcount(0), wxname(n) { }
	virtual ~ManagedItem() {}

	void addref()
	{
		++refcount;
	}

	bool delref()
	{
		return --refcount==0;
	}
	
};



template <class IDTYPE>
class Manager {
public:
	std::map<wxString, IDTYPE> names;
	std::map<IDTYPE, ManagedItem*> items;

	Manager()
	{
	}

	virtual IDTYPE add(wxString name) = 0;

	virtual void del(IDTYPE id)
	{
		if (items.find(id) == items.end())  {
			doDelete(id);
			return; // if we can't find the item id, delete the texture
		}

		if (items[id]->delref()) {
			ManagedItem *i = items[id];

			if (!i)
				return;

#ifdef _DEBUG
				std::cout << "Unloading Texture: " << i->name.c_str();
#endif

			doDelete(id);
			names.erase(names.find(i->wxname));
			items.erase(items.find(id));

			wxDELETE(i);
		}
	}

	void delbyname(wxString name)
	{
		if (has(name)) 
			del(get(name));
	}

	virtual void doDelete(IDTYPE) {}

	bool has(wxString name)
	{
		return (names.find(name) != names.end());
	}

	IDTYPE get(wxString name)
	{
		return names[name];
	}

	wxString get(IDTYPE id)
	{
	  return "";
		//return names[id];
	}

	void clear()
	{
		/*
		for (std::map<IDTYPE, ManagedItem*>::iterator it=items.begin(); it!=items.end(); ++it) {
			ManagedItem *i = (*it);
			
			wxDELETE(i);
		}
		*/

		for (size_t i=0; i<50; i++) {
			if(items.find((const unsigned int)i) != items.end()) {
				del((GLuint)i);
			}
		}
		
		names.clear();
		items.clear();
	}

protected:
	void do_add(wxString name, IDTYPE id, ManagedItem* item)
	{
		names[name] = id;
		item->addref();
		items[id] = item;
	}
};

class SimpleManager : public Manager<int> {
	int baseid;
public:
	SimpleManager() : baseid(0)
	{
	}

protected:
	int nextID()
	{
		return baseid++;
	}
};

#endif
