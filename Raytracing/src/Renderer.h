#pragma once
#include "Walnut/Image.h"
#include <memory>
#include <glm/glm.hpp>

class Renderer 
{
public:
	Renderer() = default;

	void Render();
	void CreateOrResizeImage(uint32_t width, uint32_t height);
	std::shared_ptr<Walnut::Image> GetFinalImage();
private:
	uint32_t GetPixelColor(glm::vec2 coordinate);
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
};