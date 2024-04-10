#include "my_app.h"

int main(int argc, char *argv[]) {
	app::Game game;
	MyApp app(&game);
	app.Main();
	return 0;
}
