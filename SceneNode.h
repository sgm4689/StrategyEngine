#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include "Entity.h"
#include "Transform.h"
#include "Camera.h"

class SceneNode
{
public:
	SceneNode();//Global root node
	SceneNode(Entity* ent);
	SceneNode(Transform* transform);
	SceneNode(Entity* ent, Transform* transform);
	~SceneNode();
	Transform* GetWorldTransform();
	Transform* GetLocalTransform();
	Entity* GetEntity();
	void SetEnt(Entity* e);

	//Add children to the scene
	void AddChild(SceneNode* s);
	SceneNode* AddChild(Entity* ent);//returns the new node so the user can interact with it
	SceneNode* AddChild(Transform* t);//returns the new node so the user can interact with it
	bool RemoveChild(SceneNode* s);
	bool RemoveChild(int position);
	SceneNode* GetChild(int position);

	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime, Camera* camera, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	std::vector <SceneNode*>::const_iterator GetChildIteratorStart();
	std::vector <SceneNode*>::const_iterator GetChildIteratorEnd();
	int GetChildCount();
protected:
	SceneNode* parent;
	Entity* ent;
	Transform* worldTransform;
	std::vector <SceneNode*> children;
};

