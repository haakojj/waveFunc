#include "libs.h"
#include "misc.h"
#include "myClock.h"
#include "myWindow.h"

int main(void)
{
	MyWindow window("OpenGL test", 1024, 1024);

	unsigned int frameCount = 1;
	float secondAccumulator = 0.0f;
	float dt;
	MyClock clock;

	float r = 20.0f;
	for (int x = 0; x < window.getWidth(); x++) {
		for (int y = 0; y < window.getHeight(); y++) {
			float d = sqrt((pow((float)x - (float)window.getWidth() / 4.0f, 2.0f) + pow((float)y - (float)window.getHeight() / 4.0f, 2.0f)));
			if (d < r) {
				window.setPixel(x, y, 1000.0f);
			}
			else {
				window.setPixel(x, y, 0.0f);
			}
		}
	}
	window.update();

	clock.getDeltaTime();
	do {

		dt = clock.getDeltaTime();

		secondAccumulator += dt;
		if (secondAccumulator >= 1.0f) {
			std::cout << std::fixed << std::setprecision(1) << (float)frameCount / secondAccumulator << std::endl;
			frameCount = 1;
			secondAccumulator = 0.0f;
		}
		else {
			frameCount++;
		}

		window.draw(dt);
		window.poolEvents();
	}
	while (glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window.getWindow()) == 0);

	return 0;
}

