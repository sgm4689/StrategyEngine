#include "EventHandler.h"

EventHandler* eventHandler = nullptr;

EventHandler::EventHandler() {
	callbacks = new std::unordered_multimap<Event, voidFunc>();
}

EventHandler* EventHandler::GetInstance() {
	if (eventHandler == nullptr)
		eventHandler = new EventHandler();
	return eventHandler;
}

bool EventHandler::DispatchEvent(Event event) {
	auto list = callbacks->equal_range(event);
	if (list.first == list.second)
		return false;//No functions were found bound to the called event
	for (auto callback = list.first; callback != list.second; callback++) {
		if (callback->second())
			break;//If a function handles the event, don't give that event to other methods
	}
	return true;
}
