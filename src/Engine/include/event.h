#pragma once

#include <string>
#include <map>
#include <vector>
#include <functional>

namespace app {
	
	class App;

	class Event {
	public:
		virtual ~Event();
		virtual std::string GetDescription() const = 0;
	};

	class Dispatcher {
	public:
		explicit Dispatcher(App *app);

		using Slot = std::function<void(App&, const Event&)>;

		void Register(const std::string& descriptor, Slot&& callback);
		void Post(const Event& event) const;

	private:
		std::map<std::string, std::vector<Slot>> slots;
		App *app;
	};
}