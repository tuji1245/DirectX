#pragma once

#include "Scripts/Object/Object.h"
#include <vector>

#define SCENE_DECLARATION(scene) \
	scene();\
	virtual ~scene();\
	void Init() override;\
	void Uninit() override;\
	void Update() override;\
	void Draw() override// ; ‚ ‚¦‚Ä‚Â‚¯‚Ä‚È‚¢

class Scene abstract
{
protected:
	std::vector<Object*> m_pObjectArray;

public:
	Scene() = default; 
	virtual ~Scene() = default;

	virtual void Create() {};
	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};
};