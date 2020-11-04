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
		callback->second();//Call each function pointer
	}
	return true;
}
