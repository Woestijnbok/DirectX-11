#include "pch.h"
#include "Camera.h"
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

using namespace dae;

Camera::Camera(const Vector3& origin, float fovDegrees, float ascpectRatio) :
	m_FieldOfView{ tanf((fovDegrees * TO_RADIANS) / 2.0f) },
	m_AspectRatio{ ascpectRatio },
	m_Origin{ origin },
	m_Forward{ Vector3::UnitZ },
	m_Up{ Vector3::UnitY },
	m_Right{ Vector3::UnitX },
	m_TotalPitch{ 0.0f, },
	m_TotalYaw{ 0.0f, },
	m_ViewMatrix{},
	m_ProjectionMatrix{},
	m_NearPlane{ 1.0f },
	m_FarPlane{ 1000.0f },
	m_MovementSpeed{ 50.0f },
	m_RotationSpeed{ 45.0f }
{
	CalculateProjectionMatrix();
}

void Camera::CalculateViewMatrix()
{
	m_Right = Vector3{ Vector3::Cross(Vector3::UnitY, m_Forward).Normalized() };
	m_Up = Vector3{ Vector3::Cross(m_Forward, m_Right).Normalized() };
	m_ViewMatrix = Matrix{ m_Right, m_Up, m_Forward, m_Origin }.Inverse();

	//DirectX Implementation => https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
}

void Camera::CalculateProjectionMatrix()
{
	m_ProjectionMatrix = Matrix
	{
		Vector4{ ((1.0f) / (m_AspectRatio * m_FieldOfView)), 0.0f, 0.0f, 0.0f },
		Vector4{ 0.0f, ((1.0f) / (m_FieldOfView)), 0.0f, 0.0f},
		Vector4{ 0.0f, 0.0f, ((m_FarPlane) / (m_FarPlane - m_NearPlane)), 1.0f },
		Vector4{ 0.0f, 0.0f, -((m_FarPlane * m_NearPlane) / (m_FarPlane - m_NearPlane)), 0.0f }
	};

	//DirectX Implementation => https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh
}

void Camera::Update(float deltaSeconds)
{
	//Camera Update Logic
	float movementSpeed{ m_MovementSpeed };

	//Keyboard Input
	const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);

	if (pKeyboardState[SDL_SCANCODE_LSHIFT])
	{
		movementSpeed *= 2.0f;
	}
	if (pKeyboardState[SDL_SCANCODE_W] || pKeyboardState[SDL_SCANCODE_UP])
	{
		m_Origin += m_Forward * movementSpeed * deltaSeconds;
	}
	if (pKeyboardState[SDL_SCANCODE_S] || pKeyboardState[SDL_SCANCODE_DOWN])
	{
		m_Origin -= m_Forward * movementSpeed * deltaSeconds;
	}
	if (pKeyboardState[SDL_SCANCODE_D] || pKeyboardState[SDL_SCANCODE_RIGHT])
	{
		m_Origin += m_Right * movementSpeed * deltaSeconds;
	}
	if (pKeyboardState[SDL_SCANCODE_A] || pKeyboardState[SDL_SCANCODE_LEFT])
	{
		m_Origin -= m_Right * movementSpeed * deltaSeconds;
	}

	//Mouse Input
	int mouseX{}, mouseY{};
	const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

	const bool LeftMouseButtonDown{ bool(mouseState & SDL_BUTTON_LMASK) };
	const bool RightMouseButtonDown{ bool(mouseState & SDL_BUTTON_RMASK) };

	if (LeftMouseButtonDown)
	{
		if (RightMouseButtonDown)
		{
			m_Origin += m_Up * float(mouseY) * movementSpeed * deltaSeconds;
		}
		else
		{
			m_Origin -= m_Forward * float(mouseY) * movementSpeed * deltaSeconds;
			m_TotalYaw += float(mouseX) * m_RotationSpeed * deltaSeconds;
		}
	}
	else if (RightMouseButtonDown)
	{
		m_TotalYaw += float(mouseX) * m_RotationSpeed * deltaSeconds;
		m_TotalPitch -= float(mouseY) * m_RotationSpeed * deltaSeconds;
	}

	Matrix finalRotation{ Matrix::CreateRotation(m_TotalPitch, m_TotalYaw, 0.0f) };
	m_Forward = finalRotation.TransformVector(Vector3::UnitZ);
	m_Forward.Normalize();

	CalculateViewMatrix();
}

const Vector3& Camera::GetPosition() const
{
	return m_Origin;
}

const Matrix& Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

const Matrix& Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}