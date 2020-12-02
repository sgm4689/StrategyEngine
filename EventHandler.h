#pragma once
#include <unordered_map>
#include <thread>
#include <functional>

using voidFunc = std::function<bool()>;

enum Event {
	M1Down, M1Up, M1Hold, M2Down, M2Up, M2Hold, M3Down, M3Up, M3Hold,//Mouse Commands
	//KeyDown, KeyUp, KeyHold, KeyRelease//Keyboard Enums, currently unused
	customEvent
};

class EventHandler
{
public:
	//If usePriority is set to true, events will only be sent out until one uses the event.  Otherwise send to all instances
	//EX. if a unit is underneath a UI button, the button should be pressed and the unit shouldn't be selected
	bool DispatchEvent(Event event);

	//In order for this method to work with every class the compiler needs it to be written in the header file
	//Otherwise there's linker errors
	template <typename T>
	void BindEvent(Event event, bool(T::* method)(void), T* obj) {
		voidFunc vf = std::bind(method, obj);
		std::pair<Event, voidFunc> storage(event, vf);
		callbacks->insert(storage);
	}
	static EventHandler* GetInstance();
private:
	EventHandler();//Private constructor so there's only ever one
	std::unordered_multimap<Event, voidFunc>* callbacks;//An multimap of all functions to be called on an event being dispatched
};

