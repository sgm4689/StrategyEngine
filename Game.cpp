#include "Game.h"
#include "Vertex.h"
#include <iostream>

// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Note: Since we're using smart pointers (ComPtr),
	// we don't need to explicitly clean up those DirectX objects
	// - If we weren't using smart pointers, we'd need
	//   to call Release() on each DirectX object
	for (auto& m : meshVector) { delete m; }
	for (auto& e : entityVector) { delete e; }
	for (auto& e : materialVector) { delete e; }
	delete camera;
	delete pixelShader;
	delete vertexShader;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();

	camera = new Camera(XMFLOAT3(0.5f, 0.5f, -1), XMFLOAT3(0, 0, 0), this->width / this->height, 3.1415f / 2);
	mainCamera->SetMain(camera);
	directional1 = PointLight();
	directional1.position = XMFLOAT3(0, 0, 1);
	directional1.range = 100.0f;

	directional2 = DirectionalLight();
	directional2.ambientColor = XMFLOAT3(0.1f, 0.1f, 0.1f);
	directional2.diffuseColor = XMFLOAT3(0.25, 0.25, 0.25f);
	directional2.direction = XMFLOAT3(0, 0, -1);

	directional3 = DirectionalLight();
	directional3.direction = XMFLOAT3(1, 0, 0);

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	SetTextures();
	CreateBasicGeometry();
	eventHandler->BindEvent(M1Down, &Game::TestGrid, this);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"VertexShader.cso").c_str());
	pixelShader = new SimplePixelShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"PixelShader.cso").c_str());
	normalVS = new SimpleVertexShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"NormalMapVS.cso").c_str());
	normalPS = new SimplePixelShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"NormalMapPS.cso").c_str());
}

bool Game::TestGrid() {
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(hWnd, &mouse);
	XMFLOAT2* position = camera->ClientToWorld(&mouse);
	std::cout << "Position: " << position->x << ", " << position->y << "\n";
	if (map->MoveAbsolute(entityVector[1], position->x, position->y)) {
		return true;
	}
	else {
		//std::cout << "Entity Move Failed";
		return false;
	}
}

void Game::SetTextures() {
	// Texture releated init
	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),	// Passing in the context auto-generates mipmaps!!
		GetFullPathTo_Wide(L"../../Assets/Textures/grid.png").c_str(),
		nullptr, // We don't need the texture ref ourselves
		SRVGrid.GetAddressOf()); // We do need an SRV

	D3D11_SAMPLER_DESC gridDesc = {};
	gridDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	gridDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	gridDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	gridDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	gridDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device.Get()->CreateSamplerState(&gridDesc, sampleGrid.GetAddressOf());

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),	// Passing in the context auto-generates mipmaps!!
		GetFullPathTo_Wide(L"../../Assets/Textures/button.png").c_str(),
		nullptr, // We don't need the texture ref ourselves
		SRVButton.GetAddressOf()); // We do need an SRV

	D3D11_SAMPLER_DESC buttonDesc = {};
	gridDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	gridDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	gridDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	gridDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	gridDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device.Get()->CreateSamplerState(&buttonDesc, sampleButton.GetAddressOf());
}


void TestButton() {
	std::cout << "Button Pressed";
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 yellow = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 cyan = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 white = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	map = new Map(12, 12, 0.05f);


#pragma region Meshes

	//Mesh* mesh = map->CreateHex(device);
	Mesh* mesh = new Mesh(GetFullPathTo("../../Assets/Models/cube.obj").c_str(), device);

	Vertex vertices[] =
	{
		{ DirectX::XMFLOAT3(+1.0f / 2.0f, +1.0f / 2.0f, -0.1f), DirectX::XMFLOAT3(+1.0f / 2.0f, +1.0f / 2.0f, -0.1f), DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT2(1,0)},
		{ DirectX::XMFLOAT3(+1.0f / 2.0f, -1.0f / 2.0f, -0.1f), DirectX::XMFLOAT3(+1.0f / 2.0f, -1.0f / 2.0f, -0.1f), DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT2(1,1)},
		{ DirectX::XMFLOAT3(-1.0f / 2.0f, -1.0f / 2.0f, -0.1f), DirectX::XMFLOAT3(-1.0f / 2.0f, -1.0f / 2.0f, -0.1f), DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT2(0,1)},
		{ DirectX::XMFLOAT3(-1.0f / 2.0f, +1.0f / 2.0f, -0.1f), DirectX::XMFLOAT3(-1.0f / 2.0f, +1.0f / 2.0f, -0.1f), DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT2(0,0)},
	};

	UINT indices[] = { 0, 1, 2, 3, 0, 2 };


	Mesh* mesh2 = new Mesh(vertices, 4, indices, 6, device);

	buttons = new Button*[3];
#pragma endregion

#pragma region Materials
	Material* mat = new Material(white, pixelShader, vertexShader, SRVGrid, sampleGrid, 0);
	Material* mat2 = new Material(white, pixelShader, vertexShader, SRVButton, sampleButton, 0);
	buttons[0] = new Button(mesh2, mat2, 1.0f, 1.0f, 0.5f, 0.5f, &TestButton, hWnd);
	buttons[1] = new Button(mesh2, mat2, 1.0f, 1.0f, 0.5f, 0.5f, &TestButton, hWnd);
	buttons[2] = new Button(mesh2, mat2, 1.0f, 1.0f, 0.5f, 0.5f, &TestButton, hWnd);
	dropdown = new DropdownMenu(buttons, 3, mesh2, mat2, 1, 1, 0,0,hWnd);

	materialVector.push_back(mat);

#pragma endregion

#pragma region Entities

	//Object to visualize grid
	entityVector.push_back(new Entity(mesh2, mat));
	entityVector[0]->GetTransform()->Scale(2.0f, 2.0f, 0.1f);
	entityVector[0]->GetTransform()->MoveRelative(0.5f, 0.5f, 0);

	Material* mat3 = new Material(red, pixelShader, vertexShader, SRVGrid, sampleGrid, 0);
	//Object to represent unit
	entityVector.push_back(new Entity(mesh, mat3));
	entityVector[1]->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	entityVector[1]->GetTransform()->MoveRelative(0.5f, 0.5f, -0.1f);
	if (map->Add(entityVector[1])) {
		POINT position = map->GetGridPosition(entityVector[1]);
		std::cout << "Position: " << position.x << ", " << position.y << "\n";
	}
	else
		std::cout << "Add to Grid Failed\n";


#pragma endregion
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	if (camera != nullptr)
		camera->UpdateProjectionMatrix(this->width / this->height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	camera->Update(deltaTime, this->hWnd);
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	//TestGrid(this->hWnd);
	inputManager->Update(deltaTime, this->hWnd);
} 

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	normalPS->SetData("light1", &directional1, sizeof(DirectionalLight));
	normalPS->SetData("light2", &directional2, sizeof(DirectionalLight));
	normalPS->SetData("light3", &directional3, sizeof(DirectionalLight));
	normalPS->CopyAllBufferData();

	pixelShader->SetData("light1", &directional1, sizeof(DirectionalLight));
	pixelShader->SetData("light2", &directional2, sizeof(DirectionalLight));
	pixelShader->SetData("light3", &directional3, sizeof(DirectionalLight));
	pixelShader->CopyAllBufferData();


	for (int x = 0; x < entityVector.size(); x++) {

		SimplePixelShader* psData = entityVector[x]->GetMaterial()->GetPixelShader();

		psData->SetFloat("specular", entityVector[x]->GetMaterial()->GetSpecularExponent());
		psData->SetFloat3("position", camera->GetTransform().GetPosition());

		// Set the vertex and pixel shaders to use for the next Draw() command
		//  - These don't technically need to be set every frame
		//  - Once you start applying different shaders to different objects,
		//    you'll need to swap the current shaders before each draw
		entityVector[x]->GetMaterial()->GetVertexShader()->SetShader();
		entityVector[x]->GetMaterial()->GetPixelShader()->SetShader();

		SimpleVertexShader* vsData = entityVector[x]->GetMaterial()->GetVertexShader();
		vsData->SetFloat4("colorTint", entityVector[x]->GetMaterial()->GetColor());
		vsData->SetMatrix4x4("world", entityVector[x]->GetTransform()->GetWorldMatrix());
		vsData->SetMatrix4x4("view", camera->M4_view);
		vsData->SetMatrix4x4("projection", camera->M4_projection);
		psData->SetShaderResourceView("diffuseTexture", entityVector[x]->GetMaterial()->GetResource().Get());
		if (entityVector[x]->GetMaterial()->HasNormalMap())
			psData->SetShaderResourceView("normalMap", entityVector[x]->GetMaterial()->GetNormalMap().Get());
		psData->SetSamplerState("samplerOptions", entityVector[x]->GetMaterial()->GetState().Get());

		vsData->CopyAllBufferData();

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		//  - for this demo, this step *could* simply be done once during Init(),
		//    but I'm doing it here because it's often done multiple times per frame
		//    in a larger application/game
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		context->IASetVertexBuffers(0, 1, entityVector[x]->GetMesh()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(entityVector[x]->GetMesh()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			entityVector[x]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}

	for (int x = 0; x < 3; x++) {

		SimplePixelShader* psData = buttons[x]->GetMaterial()->GetPixelShader();

		psData->SetFloat("specular", buttons[x]->GetMaterial()->GetSpecularExponent());
		psData->SetFloat3("position", camera->GetTransform().GetPosition());

		// Set the vertex and pixel shaders to use for the next Draw() command
		//  - These don't technically need to be set every frame
		//  - Once you start applying different shaders to different objects,
		//    you'll need to swap the current shaders before each draw
		buttons[x]->GetMaterial()->GetVertexShader()->SetShader();
		buttons[x]->GetMaterial()->GetPixelShader()->SetShader();

		SimpleVertexShader* vsData = buttons[x]->GetMaterial()->GetVertexShader();
		vsData->SetFloat4("colorTint", buttons[x]->GetMaterial()->GetColor());
		vsData->SetMatrix4x4("world", buttons[x]->GetTransform()->GetWorldMatrix());
		vsData->SetMatrix4x4("view", camera->M4_view);
		vsData->SetMatrix4x4("projection", camera->M4_projection);
		psData->SetShaderResourceView("diffuseTexture", buttons[x]->GetMaterial()->GetResource().Get());
		if (buttons[x]->GetMaterial()->HasNormalMap())
			psData->SetShaderResourceView("normalMap", buttons[x]->GetMaterial()->GetNormalMap().Get());
		psData->SetSamplerState("samplerOptions", buttons[x]->GetMaterial()->GetState().Get());

		vsData->CopyAllBufferData();

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		//  - for this demo, this step *could* simply be done once during Init(),
		//    but I'm doing it here because it's often done multiple times per frame
		//    in a larger application/game
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		context->IASetVertexBuffers(0, 1, buttons[x]->GetMesh()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(buttons[x]->GetMesh()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			buttons[x]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}

	SimplePixelShader* psData = dropdown->GetMaterial()->GetPixelShader();

	psData->SetFloat("specular", dropdown->GetMaterial()->GetSpecularExponent());
	psData->SetFloat3("position", camera->GetTransform().GetPosition());

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	dropdown->GetMaterial()->GetVertexShader()->SetShader();
	dropdown->GetMaterial()->GetPixelShader()->SetShader();

	SimpleVertexShader* vsData = dropdown->GetMaterial()->GetVertexShader();
	vsData->SetFloat4("colorTint", dropdown->GetMaterial()->GetColor());
	vsData->SetMatrix4x4("world", dropdown->GetTransform()->GetWorldMatrix());
	vsData->SetMatrix4x4("view", camera->M4_view);
	vsData->SetMatrix4x4("projection", camera->M4_projection);
	psData->SetShaderResourceView("diffuseTexture", dropdown->GetMaterial()->GetResource().Get());
	if (dropdown->GetMaterial()->HasNormalMap())
		psData->SetShaderResourceView("normalMap", dropdown->GetMaterial()->GetNormalMap().Get());
	psData->SetSamplerState("samplerOptions", dropdown->GetMaterial()->GetState().Get());

	vsData->CopyAllBufferData();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	//  - for this demo, this step *could* simply be done once during Init(),
	//    but I'm doing it here because it's often done multiple times per frame
	//    in a larger application/game
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, dropdown->GetMesh()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(dropdown->GetMesh()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	context->DrawIndexed(
		dropdown->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}