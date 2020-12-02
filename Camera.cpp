#include "Camera.h"
Camera::Camera(XMFLOAT3 position, XMFLOAT3 orientation, float aspect, float FOV) {
	transform = Transform(position, orientation, XMFLOAT3(1.0f, 1.0f, 1.0f));
	this->FOV = FOV;
	this->nearClip = 0.01f;
	this->farClip = 500.0f;
	this->moveSpeed = 3.0f;
	this->lookSpeed = 0.025f;

	eventHandler = EventHandler::GetInstance();
	UpdateViewMatrix();
	UpdateProjectionMatrix(aspect);
	eventHandler->BindEvent(M1Down, &Camera::OnClick, this);
	eventHandler->BindEvent(M1Hold, &Camera::OnDrag, this);
}

Camera::Camera(XMFLOAT3 position, XMFLOAT3 orientation, float aspect, float FOV, float nearClip, float farClip, float moveSpeed, float lookSpeed) {
	transform = Transform(position, orientation, XMFLOAT3(1.0f, 1.0f, 1.0f));
	this->FOV = FOV;
	this->nearClip = nearClip;
	this->farClip = farClip;
	this->moveSpeed = moveSpeed;
	this->lookSpeed = lookSpeed;

	UpdateViewMatrix();
	UpdateProjectionMatrix(aspect);
}

Camera::~Camera() {

}

void Camera::UpdateViewMatrix() {
	XMVECTOR position = XMVectorSet(transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z,0.0f);

	//Get camera forward
	XMVECTOR absolute = XMVectorSet(0, 0, 1, 0.0f);
	XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(transform.GetPitchYawRoll().x, transform.GetPitchYawRoll().y, transform.GetPitchYawRoll().z);
	XMVECTOR rotated = XMVector3Rotate(absolute, rotation);

	XMVECTOR up = XMVectorSet(0.0f,1.0f,0.0f,0.0f);
	XMMATRIX view = XMMatrixLookToLH(position, rotated, up);
	XMStoreFloat4x4(&M4_view, view);
}

void Camera::UpdateProjectionMatrix(float aspect) {
	XMMATRIX projection = XMMatrixPerspectiveFovLH(FOV, aspect, nearClip, farClip);
	XMStoreFloat4x4(&M4_projection, projection);
}

Transform* Camera::GetTransform() {
	return &transform;
}

void Camera::Update(float dt, HWND windowHandle) {
	float currentSpeed = moveSpeed;//Rather than writing 2 seperate sets of if statements, changing the speed based on if shift is pressed...

	wh = windowHandle;

	//Keyboard input
	if (GetAsyncKeyState('W') & 0x8000) { transform.MoveAbsolute(0, currentSpeed * dt, 0); }
	else if (GetAsyncKeyState('S') & 0x8000) { transform.MoveAbsolute(0, -currentSpeed * dt, 0); }
	if (GetAsyncKeyState('A') & 0x8000) { transform.MoveRelative(-currentSpeed * dt,0,0); }
	else if (GetAsyncKeyState('D') & 0x8000) { transform.MoveRelative(currentSpeed * dt,0,0); }
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) { transform.MoveAbsolute(0, currentSpeed * dt, 0); }
	else if (GetAsyncKeyState('X') & 0x8000) { transform.MoveAbsolute(0, -currentSpeed * dt, 0); }
}

XMFLOAT2* Camera::ClientToWorld(LPPOINT clientP) {
	XMMATRIX projection = XMLoadFloat4x4(&M4_projection);
	XMMATRIX view = XMLoadFloat4x4(&M4_view);
	float depth = M4_view._43;
	XMMATRIX combine = projection * view;
	combine = XMMatrixInverse(&DirectX::XMMatrixDeterminant(view * projection), combine);

	RECT window;
	//attempting to use clientP without binding them to variables always returns 0...  For reasons
	float x = clientP->x;
	float y = clientP->y;

	GetWindowRect(wh, &window);
	//Mouse Pos, clamped from -1 to 1.
	XMFLOAT4 F4_range = XMFLOAT4(
		2 * depth * (x  / (window.right - window.left))-depth,
		-(2 * depth * (y / (window.bottom - window.top))-depth),
		1.0f,//Since screen points are only X and Y, Z and W don't matter
		1.0f
	);
	XMVECTOR V_range = XMLoadFloat4(&F4_range);
	XMVECTOR pos = XMVector4Transform(V_range, combine);

	XMFLOAT2* value = new XMFLOAT2(
		XMVectorGetByIndex(pos, 0),
		XMVectorGetByIndex(pos, 1)
	);

	return value;
}

//Set mouse position to wherever the user clicked
bool Camera::OnClick() {
	//Mouse position
	POINT mousePos = {};
	GetCursorPos(&mousePos);	
	ScreenToClient(wh, &mousePos);

	P_lastPos = mousePos;
	return false;//Moving the camera shouldn't block other events, so always return false
}

bool Camera::OnDrag() {
	//Mouse position
	POINT mousePos = {};
	GetCursorPos(&mousePos);
	ScreenToClient(wh, &mousePos);

	float x = (P_lastPos.x - mousePos.x) * lookSpeed;
	float y = -(P_lastPos.y - mousePos.y) * lookSpeed;
	transform.MoveAbsolute(x, y, 0);

	P_lastPos = mousePos;
	UpdateViewMatrix();
	return false;//Moving the camera shouldn't block other events, so always return false
}