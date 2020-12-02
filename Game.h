#pragma once
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <vector>
#include "StrategyEngine.h"
#include "Army.h"

class Game 
	: public DXCore
{
public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	bool TestGrid();
	bool TestButton();
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateBasicGeometry();
	void SetTextures();

	
	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//    Component Object Mode, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Buffers to hold actual geometry data

	// Shaders and shader-related constructs
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimpleVertexShader* normalVS;
	SimplePixelShader* normalPS;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRVGrid;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampleGrid;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRVButton;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampleButton;


	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	EventHandler* eventHandler = EventHandler::GetInstance();
	InputManager* inputManager = InputManager::GetInstance();
	MainCamera* mainCamera = MainCamera::GetInstance();

	Camera* camera = nullptr;

	Map* map;
	Army* allyTeam;
	Army* enemyTeam;
	Button* button;
	int buttonCount = 0;
	DropdownMenu* dropdown;

	PointLight directional1;
	DirectionalLight directional2;
	DirectionalLight directional3;

	//Meshes
	std::vector<Mesh*> meshVector;

	//Entities
	SceneNode* root;

	//Materials
	std::vector<Material*> materialVector;
};

