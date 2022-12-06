#pragma once
#include "Walnut/Image.h"
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"
#include <memory>
#include <glm/glm.hpp>

class Renderer 
{
public:
	Renderer() = default;
	void Render(const Scene& scene, const Camera& camera);
	void CreateOrResizeImage(uint32_t width, uint32_t height);
	std::shared_ptr<Walnut::Image> GetFinalImage();

private:

	struct HitPayload
	{
		float HitDistance;
		glm::vec3 WorldPosition;
		glm::vec3 WorldNormal;

		int ObjectIndex;
	};

	//Methods
	glm::vec4 PerPixel(uint32_t x, uint32_t y); //RayGen in the GPU API naming
	HitPayload TraceRay(const Ray& ray);
	HitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
	HitPayload Miss(const Ray& ray);

	// Member variables
	std::shared_ptr<Walnut::Image> m_FinalImage;
	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;
	uint32_t* m_ImageData = nullptr;


};