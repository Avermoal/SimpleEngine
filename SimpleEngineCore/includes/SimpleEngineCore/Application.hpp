#ifndef INCLUDES_SIMPLE_ENGINE_CORE_APPLICATION_HPP
#define INCLUDES_SIMPLE_ENGINE_CORE_APPLICATION_HPP

#include <memory>

namespace SimpleEngine {
	class Application {
	public:
		Application();
		virtual ~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator =(const Application&) = delete;
		Application& operator =(const Application&&) = delete;

		virtual int start(unsigned int win_width, unsigned int win_height, const char* title);

		virtual void on_update() {};

	private:
		std::unique_ptr<class Window> m_window;
	};
}

#endif//INCLUDES_SIMPLE_ENGINE_CORE_APPLICATION_HPP