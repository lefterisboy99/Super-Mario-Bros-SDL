#include <event.h>

namespace app {

	Event::~Event() {}

	Dispatcher::Dispatcher(App *app) {
		this->app = app;
	}

	void Dispatcher::Register(const std::string& descriptor, Slot&& callback) {
		slots[descriptor].push_back(callback);
	}

	void Dispatcher::Post(const Event& event) const {

		auto desc = event.GetDescription();
		if (slots.find(desc) == slots.end()) return;

		auto&& observers = slots.at(desc);

		for (auto&& observer : observers) {
			observer(*app, event);
		}
	}
}