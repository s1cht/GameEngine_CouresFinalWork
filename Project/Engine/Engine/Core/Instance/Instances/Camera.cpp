#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
    m_position = { 0.f, 0.f, 0.f };
    m_rotation = { 0.f, 0.f, 0.f };
	m_Name = L"Camera";
	m_velocity = Vector3::Zero();
	m_ID = INT();
	m_viewMatrix = XMMATRIX();

	m_upVector = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	m_forwardVector = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	m_rightVector = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	m_positionVector = XMVECTOR();
	m_lookAtVector = XMVECTOR();
}

Camera::~Camera()
{
}

void Camera::SetPosition(Vector3 position)
{
    m_position = position;
}

void Camera::SetRotation(Vector3 rotation)
{
    m_rotation = rotation;
}

void Camera::SetVelocity(Vector3 vel)
{
	m_velocity = vel;
}

Vector3 Camera::GetPosition()
{
    return m_position;
}

Vector3 Camera::GetRotation()
{
    return m_rotation;
}

Vector3 Camera::GetUpVector()
{
	return Vector3(m_upVector.m128_f32[0], m_upVector.m128_f32[1], m_upVector.m128_f32[2]);
}

Vector3 Camera::GetForwardpVector()
{
	return Vector3(m_forwardVector.m128_f32[0], m_forwardVector.m128_f32[1], m_forwardVector.m128_f32[2]);
}

Vector3 Camera::GetRightVector()
{
	return Vector3(m_rightVector.m128_f32[0], m_rightVector.m128_f32[1], m_rightVector.m128_f32[2]);
}

void Camera::Render(FLOAT Pitch, FLOAT Yaw)
{
	XMFLOAT3 up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	position.x =			m_position.X;
	position.y =			m_position.Y;
	position.z =			m_position.Z;

	m_positionVector =		XMLoadFloat3(&position);

	lookAt.x =				0.0f;
	lookAt.y =				0.0f;
	lookAt.z =				1.0f;

	m_lookAtVector =		XMLoadFloat3(&lookAt);

	pitch =					math::ToRadians(m_rotation.X);
	yaw =					math::ToRadians(m_rotation.Y);
	roll =					math::ToRadians(m_rotation.Z);

	rotationMatrix =		XMMatrixRotationRollPitchYaw(math::ToRadians(Pitch), math::ToRadians(Yaw), roll);

	m_lookAtVector =		XMVector3TransformCoord(m_lookAtVector, rotationMatrix);

	m_upVector =			XMVector3TransformCoord(DEFAULT_UP_VECTOR, rotationMatrix);
	m_forwardVector =		XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, rotationMatrix);
	m_rightVector =			XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, rotationMatrix);

	m_lookAtVector =		XMVectorAdd(m_positionVector, m_lookAtVector);

	m_viewMatrix =			XMMatrixLookAtLH(m_positionVector, m_lookAtVector, m_upVector);
}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
    viewMatrix = m_viewMatrix;
}

inline void Camera::SetParent(INSTANCE parent)
{
	m_Parent = parent;
}

inline void Camera::SetName(wstring name)
{
	m_Name = name;
}

void Camera::SetDevices(ID3D11Device*, ID3D11DeviceContext*)
{
}

void Camera::SetID(INT id)
{
	if (!m_idInited)
	{
		m_ID = id;
		m_idInited = true;
	}
}

void Camera::AddChild(INSTANCE child)
{
	m_Children.push_back(child);
}

void Camera::DeleteChild(INT id)
{
	if (m_Children.empty())
		return;
	for (auto it = m_Children.begin(); it != m_Children.end(); )
		if ((*it)->GetID() == id)
		{
			m_Children.erase(it);
			break;
		}
		else
			it++;
}

INSTANCE Camera::GetParent()
{
	return m_Parent;
}

wstring Camera::GetName()
{
	return m_Name;
}

ClassName Camera::GetInstanceClassName()
{
	return m_ClassName;
}

std::vector<INSTANCE> Camera::GetChildren()
{
	return m_Children;
}

INT Camera::GetID()
{
	return m_ID;
}

INSTANCE Camera::operator[](const wstring childName)
{
	for (auto& child : m_Children)
		if (child->GetName() == childName)
			return child;
	return nullptr;
}

INSTANCE Camera::operator[](size_t child)
{
	if (m_Children[child])
		return m_Children[child];
	return nullptr;
}

INSTANCE Camera::GetChildByID(INT id)
{
	if (m_ID == id)
		return shared_from_this();

	if (m_Children.empty())
		return nullptr;

	if (id > (INT)m_Children.capacity())
		return (m_Children.end() - 1)->get()->GetChildByID(id);

	for (size_t i = 0; i < m_Children.capacity(); i++)
	{
		if (m_Children[i]->GetID() < id)
		{
			if (m_Children[i]->GetID() == id)
				return m_Children[i];
		}
		else if (m_Children[i]->GetID() == id)
			return m_Children[i];
		else
		{
			return m_Children[i - 1]->GetChildByID(id);
		}
	}

	return nullptr;
}

void Camera::Destroy()
{
}