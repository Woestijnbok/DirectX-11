#ifndef CAMERA
#define CAMERA

class Camera final
{
public:
	Camera(const dae::Vector3& origin, float fovDegrees, float ascpectRatio);

	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
	Camera(Camera&&) = delete;
	Camera& operator=(Camera&&) = delete;

	void Update(float deltaSeconds);
	const dae::Vector3& GetPosition() const;
	const dae::Matrix& GetViewMatrix() const;
	const dae::Matrix& GetProjectionMatrix() const;

private:
	const float m_FieldOfView;
	const float m_AspectRatio;
	dae::Vector3 m_Origin;
	dae::Vector3 m_Forward;
	dae::Vector3 m_Up;
	dae::Vector3 m_Right;
	float m_TotalPitch;
	float m_TotalYaw;
	dae::Matrix m_ViewMatrix;
	dae::Matrix m_ProjectionMatrix;
	const float m_NearPlane;
	const float m_FarPlane;
	const float m_MovementSpeed;
	const float m_RotationSpeed;

	void CalculateViewMatrix();
	void CalculateProjectionMatrix();
};

#endif