#include <Windows.h>
#include <random>
#include "Window.h"
#include "D3D.h"
#include "Utils.h"
#include "Mesh.h"
#include "Camera.h"
#include "Time.h"
#include "Material.h"
#include "Light.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	INT error = 0;
	UINT width = 1024;
	UINT height = 768;
	BOOL isFullscreen = FALSE;

	// 1. create window
	Window window = {};
	error = window.init(hInstance, width, height);
	CheckError(error);

	// 2. connection to Direct3D11
	D3D d3d = {};
	error = d3d.init(window.getWindowHandle(), width, height, isFullscreen);
	CheckError(error);

	// 3. create mesh/object
	Mesh mesh = {};
	error = mesh.init(d3d.getDevice());
	CheckError(error);

	// 4. create camera
	Camera camera = {};
	error = camera.init(width, height);
	CheckError(error);

	// 5. set up time
	Time time = {};
	error = time.init();
	CheckError(error);

	// 6. create material
	Material material = {};
	error = material.init(d3d.getDevice(), TEXT("wall.jpg"));
	CheckError(error);

	// 7. create light
	Light::LightData lightData = {};
	lightData.lightDirection = { -1.0f, -1.0f, 1.0f };
	lightData.lightDiffuseColor = { 0.8f, 0.8f, 0.8f, 1.0f };
	lightData.lightIntensity = 1.0f;
	Light light = {};
	error = light.init(d3d.getDevice(), lightData);
	CheckError(error);

	// 8. run application
	while (window.run())
	{
		// 8.1 update objects
		time.update();
		camera.update(time.getDeltaTime());
		mesh.update(time.getDeltaTime());

		// 8.2 draw objects
		d3d.beginScene(0.0f, 0.0f, 0.0f);

		material.render(d3d.getDeviceContext(), mesh.getWorldMatrix(), camera.getViewMatrix(), camera.getProjectionMatrix());
		light.render(d3d.getDeviceContext());
		mesh.render(d3d.getDeviceContext());

		d3d.endScene();
	}

	// 9. tidy up
	light.deInit();
	material.deInit();
	time.deInit();
	camera.deInit();
	mesh.deInit();
	d3d.deInit();
	window.deInit();

	return 0;
}