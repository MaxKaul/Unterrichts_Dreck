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

	// 2. connection to Direct3D9
	D3D d3d = {};
	error = d3d.init(window.getWindowHandle(), width, height, isFullscreen);
	CheckError(error);

	d3d.getDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	d3d.getDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	d3d.getDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
	d3d.getDevice()->SetRenderState(D3DRS_COLORVERTEX, FALSE);
	d3d.getDevice()->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

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
	D3DLIGHT9 lightData = {};
	lightData.Type = D3DLIGHT_POINT;
	//lightData.Direction = { -1.0f, -1.0f, 1.0f };
	lightData.Position = { 0.0f, 0.0f, -5.0f };
	lightData.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	lightData.Diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	lightData.Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	lightData.Range = 10.0f;
	// attenuation = a0 + a1 * distance + a2 * distance * distance
	lightData.Attenuation0 = 1.0f; // constant
	lightData.Attenuation1 = 0.2f; // linear
	lightData.Attenuation2 = 0.1f; // quadratic
	Light light = {};
	error = light.init(lightData);
	CheckError(error);
	
	// 8. run application
	while (window.run())
	{
		// 8.1 update objects
		time.update();
		mesh.update(time.getDeltaTime());
		
		// 8.2 draw objects
		// random color
		//static std::default_random_engine e;
		//static std::uniform_int_distribution<INT> d(0, 255);
		//d3d.beginScene(D3DCOLOR_XRGB(d(e), d(e), d(e)));

		d3d.beginScene(D3DCOLOR_XRGB(0, 0, 0));

		camera.render(d3d.getDevice());
		material.render(d3d.getDevice());
		light.render(d3d.getDevice());
		mesh.render(d3d.getDevice());

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