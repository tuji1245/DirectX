#pragma once

#include "Component.h"
#include <System\Model\ModelList.h>
#include <memory>

class Model	final : public Component
{
public:
	Model(Object* pObject);
	~Model();

	void Draw() override;

	void SetModel(ModelList::Type type);

private:
	std::weak_ptr<const FBXModel> m_model;

};