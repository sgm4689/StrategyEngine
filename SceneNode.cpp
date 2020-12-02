#include "SceneNode.h"

//Creates a node centered around 0,0
SceneNode::SceneNode() {
	ent = nullptr;
	worldTransform = new Transform();
	parent = nullptr;
}

//Creates a node centered around the given entity
SceneNode::SceneNode(Entity* ent) {
	this->ent = ent;
	worldTransform = ent->GetTransform();
	parent = nullptr;  //Set in AddChild
}

//Creates a node centered around the given transform
SceneNode::SceneNode(Transform* transform) {
	this->ent = nullptr;
	worldTransform = transform;
	parent = nullptr;  //Set in AddChild
}

//sets a node centered around the given transform
SceneNode::SceneNode(Entity* ent, Transform* transform) {
	this->ent = ent;
	this->worldTransform = ent->GetTransform();
	parent = nullptr;  //Set in AddChild
}

SceneNode::~SceneNode() {
	for (int i = 0; i < children.size(); i++) {
		delete children[i];
	}
}

void SceneNode::AddChild(SceneNode* s) {
	children.push_back(s);
	s->worldTransform = ent->GetTransform();
	s->parent = this;
}

SceneNode* SceneNode::AddChild(Entity* e) {
	SceneNode* child = new SceneNode(e);
	child->parent = this;
	child->worldTransform = worldTransform;
	children.push_back(child);
	return child;
}


SceneNode* SceneNode::AddChild(Transform* t) {
	SceneNode* child = new SceneNode(t);
	child->parent = this;
	children.push_back(child);
	return child;
}


bool SceneNode::RemoveChild(SceneNode* s) {
	auto size = children.size();
	children.erase(std::remove(children.begin(), children.end(), s));
	if (size == children.size())
		return false;//no items were removed
	return true;
}

bool SceneNode::RemoveChild(int position) {
	if (position > 0 && position < children.size()) {
		children.erase(children.begin() + position);
		return true;
	}
	return false;
}

SceneNode* SceneNode::GetChild(int position) {
	return children[position];
}


Transform* SceneNode::GetWorldTransform() {
	return worldTransform;
}

Entity* SceneNode::GetEntity() {
	return ent;
}

void SceneNode::SetEnt(Entity* ent) {
	this->ent = ent;
}

void SceneNode::Update(float deltaTime, float totalTime) {
	if (worldTransform != nullptr && ent != nullptr) { // check if root
		XMVECTOR world = XMLoadFloat3(&worldTransform->GetPosition());
		XMVECTOR local = XMLoadFloat3(&ent->GetLocalTransform()->GetPosition());
		XMFLOAT3* newPos = new XMFLOAT3;
		XMStoreFloat3(newPos, world + local);
		ent->GetTransform()->SetPosition(newPos->x, newPos->y, newPos->z);
	}
	for (std::vector <SceneNode*>::const_iterator i = children.begin(); i != children.end(); i++) {
		(*i)->Update(deltaTime, totalTime);
	}
}

void SceneNode::Draw(float deltaTime, float totalTime, Camera* camera, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) {
	//Draw the ent in this level
	if (ent != nullptr) {
		SimplePixelShader* psData = ent->GetMaterial()->GetPixelShader();

		psData->SetFloat("specular", ent->GetMaterial()->GetSpecularExponent());
		psData->SetFloat3("position", camera->GetTransform()->GetPosition());

		// Set the vertex and pixel shaders to use for the next Draw() command
		//  - These don't technically need to be set every frame
		//  - Once you start applying different shaders to different objects,
		//    you'll need to swap the current shaders before each draw
		ent->GetMaterial()->GetVertexShader()->SetShader();
		ent->GetMaterial()->GetPixelShader()->SetShader();

		SimpleVertexShader* vsData = ent->GetMaterial()->GetVertexShader();
		vsData->SetFloat4("colorTint", ent->GetMaterial()->GetColor());
		vsData->SetMatrix4x4("world", ent->GetTransform()->GetWorldMatrix());
		vsData->SetMatrix4x4("view", camera->M4_view);
		vsData->SetMatrix4x4("projection", camera->M4_projection);
		psData->SetShaderResourceView("diffuseTexture", ent->GetMaterial()->GetResource().Get());
		if (ent->GetMaterial()->HasNormalMap())
			psData->SetShaderResourceView("normalMap", ent->GetMaterial()->GetNormalMap().Get());
		psData->SetSamplerState("samplerOptions", ent->GetMaterial()->GetState().Get());

		vsData->CopyAllBufferData();

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		context->IASetVertexBuffers(0, 1, ent->GetMesh()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(ent->GetMesh()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(
			ent->GetMesh()->GetIndexCount(),
			0,
			0);
	}

	//Then call draw on all its children
	for (std::vector <SceneNode*>::const_iterator i = children.begin(); i != children.end(); i++) {
		(*i)->Draw(deltaTime, totalTime, camera, context);
	}

}

std::vector <SceneNode*>::const_iterator SceneNode::GetChildIteratorStart() {
	return children.begin();
}

std::vector <SceneNode*>::const_iterator SceneNode::GetChildIteratorEnd() {
	return children.end();
}

int SceneNode::GetChildCount() {
	return children.size();
}