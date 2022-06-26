#include "Renderer.h"

#include "Walnut/Random.h"

void Renderer::Render()
{
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec2 coordinate = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			coordinate = coordinate * 2.0f - 1.0f;
			m_ImageData[x+y * m_FinalImage->GetWidth()] = GetPixelColor(coordinate);
		}
	}
	

	m_FinalImage->SetData(m_ImageData);
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

uint32_t Renderer::GetPixelColor(glm::vec2 coordinate)
{
	glm::vec3 rayOrigin = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::vec3 rayDirection = glm::vec3(coordinate.x, coordinate.y, -1.0f);
	rayDirection = glm::normalize(rayDirection);
	float sphereRadius = 0.5f;

	float a = glm::dot(rayDirection, rayDirection);
	float b = 2 * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - sphereRadius * sphereRadius;

	// Quadratic formula discriminant
	//b^2-4ac
	float discriminant = b * b - 4.0f * a * c;

	if (discriminant >= 0) 
	{
		return 0xff0000ff;
	}

	return 0xff777777;
}
