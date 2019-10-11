#pragma once

#include "ShaderFlat.h"

class CScene;

class CShaderPhong : public CShaderFlat
{
public:
	/**
	 * @brief Constructor
	 * @param scene Reference to the scene
	 * @param color The color of the object
	 * @param ka The ambient coefficient
	 * @param kd The diffuse reflection coefficients
	 * @param ks The specular refelection coefficients
	 * @param ke The shininess exponent
	 */
	CShaderPhong(CScene& scene, Vec3f color, float ka, float kd, float ks, float ke)
		: CShaderFlat(color)
		, m_scene(scene)
		, m_ka(ka)
		, m_kd(kd)
		, m_ks(ks)
		, m_ke(ke)
	{}
	virtual ~CShaderPhong(void) = default;

	virtual Vec3f Shade(const Ray& ray) const override
	{
		// --- PUT YOUR CODE HERE ---
		Ray inc; 
		Ray lig; 
		Vec3f specular = 0;
		Vec3f difference = 0;
		Vec3f constantS;
		Vec3f constantA;
		Vec3f constantD; 

		int i;

		for (i = 0; i < m_scene.m_vpLights.size(); i++)
		{
			lig.t = std::numeric_limits<float>::infinity();
			lig.org = (ray.t * ray.dir) + ray.org;
			std::optional<Vec3f> lightRadiance = m_scene.m_vpLights[i]->Illuminate(lig);
			if (!m_scene.Occluded(lig)) 
			{
				if (lightRadiance) 
				{
					difference += *lightRadiance * (max(lig.dir.dot(ray.hit->GetNormal(ray)), 0.0f));
				}
			}
		}
		for (int i = 0; i < m_scene.m_vpLights.size(); i++)
		{
			inc.org = (ray.t * ray.dir) + ray.org;
			inc.t = std::numeric_limits<float>::infinity();
			std::optional<Vec3f> lightRadiance = m_scene.m_vpLights[i]->Illuminate(inc);
			if (!m_scene.Occluded(lig)) 
			{
				if (lightRadiance) 
				{
					Vec3f reflectedDir = incident.dir - 2 * (incident.dir.dot(ray.hit->GetNormal(ray))) * ray.hit->GetNormal(ray);
					specular += *lightRadiance * pow((max(ray.dir.dot(reflectedDir), 0.0f)), m_ke);
				} 
			}
		}
		constantS = RGB(1, 1, 1).mul(spec) * m_ks;
		constantA = CShaderFlat::Shade(ray) * m_ka;
		constantD = CShaderFlat::Shade(ray).mul(diff) * m_kd;
		

		return constantS + constantA + constantD;
	}

	
private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};
