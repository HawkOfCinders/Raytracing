#include "Renderer.h"

#include "Walnut/Random.h"

namespace Utils
{
	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{

		uint8_t r(color.r * 255.0f);
		uint8_t g(color.g * 255.0f);
		uint8_t b(color.b * 255.0f);
		uint8_t a(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
}

void Renderer::Render(const Scene& scene, const Camera& camera)
{
	Ray ray;
	ray.Origin = camera.GetPosition();

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			ray.Direction = camera.GetRayDirections()[x + y * (float)m_FinalImage->GetWidth()];

			glm::vec4 color = TraceRay(scene, ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
		}
	}


	m_FinalImage->SetData(m_ImageData);
}


glm::vec4 Renderer::TraceRay(const Scene& scene, const Ray& ray)
{
	glm::vec4 clearColor = glm::vec4(scene.ClearColor, 1.0f);
	glm::vec3 lightdir = glm::vec3(-1, -1, -1);
	lightdir = glm::normalize(lightdir);

	if (scene.Spheres.size() <= 0)
	{
		return clearColor;
	}

	const Sphere* closestSphere = nullptr;
	float hitDistance = std::numeric_limits<float>::max();

	for (const Sphere& sphere : scene.Spheres) {
		glm::vec3 origin = ray.Origin - sphere.Position;

		float a = glm::dot(ray.Direction, ray.Direction);
		float b = 2.0f * glm::dot(origin, ray.Direction);
		float c = glm::dot(origin, origin) - sphere.Radius * sphere.Radius;


		// Quadratic formula discriminant
		//b^2-4ac
		float discriminant = b * b - 4.0f * a * c;

		if (discriminant < 0)
		{
			continue;
		}

		float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
		float furtherT = (-b + glm::sqrt(discriminant)) / (2.0f * a);
		if (closestT < hitDistance) 
		{
			hitDistance = closestT;
			closestSphere = &sphere;
		}
	}

	if (closestSphere == nullptr)
	{
		return clearColor;
	}

	glm::vec3 origin = ray.Origin - closestSphere->Position;

	glm::vec3 hitPosition = origin + ray.Direction * hitDistance;
	glm::vec3 normal = glm::normalize(hitPosition);

	float lightIntensity = glm::max(glm::dot(normal, -lightdir), 0.0f);
	return glm::vec4(closestSphere->Albedo * lightIntensity, 1.0f);

}

void Renderer::CreateOrResizeImage(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
		{
			return;
		}

		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];

}

std::shared_ptr<Walnut::Image> Renderer::GetFinalImage()
{
	return m_FinalImage;
}