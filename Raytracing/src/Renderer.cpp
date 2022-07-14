#include "Renderer.h"

#include "Walnut/Random.h"

void Renderer::Render()
{
	float aspectRatio = m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight();
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec2 coordinate = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			coordinate = coordinate * 2.0f - 1.0f;
			coordinate.x *= aspectRatio;
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
	float sphereRadius = 1.0f;
	glm::vec3 sphereOrigin = glm::vec3(0, 0, 0);
	glm::vec3 lightdir = glm::vec3(2, -2, -2);
	lightdir = glm::normalize(lightdir);

	float a = glm::dot(rayDirection, rayDirection);
	float b = 2 * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - sphereRadius * sphereRadius;

	// Quadratic formula discriminant
	//b^2-4ac
	float discriminant = b * b - 4.0f * a * c;

	if (discriminant >= 0) 
	{
		float t[] = 
		{
			(-b - glm::sqrt(discriminant)) / (2.0f * a),
			(-b + glm::sqrt(discriminant)) / (2.0f * a)
		};
		for (int i = 0; i < 2; i++)
		{
			glm::vec3 hitPosition = rayOrigin + rayDirection * t[i];
			glm::vec3 normal = hitPosition - sphereOrigin;
			normal = glm::normalize(normal);

			float lightIntensity = glm::max(glm::dot(normal, -lightdir),0.0f);
			uint32_t pixelColor = 0;
			float alpha = 255;
			float red = lightIntensity * 128;
			float green = lightIntensity * 128;
			float blue = lightIntensity * 128;
			pixelColor |= (int8_t)alpha << 24;
			pixelColor |= (int8_t)red << 16;
			pixelColor |= (int8_t)green << 8;
			pixelColor |= (int8_t)blue;
			return pixelColor;
		}
			
		
	}

	return 0xff330036;
}
