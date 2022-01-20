#pragma once

#include "Component.h"
#include <System\Model\ModelList.h>

class Model	final : public Component
{
public:
	Model(Object* pObject);
	~Model();

	void Draw() override;

	void SetModel(ModelList::Type type);

private:
	const FBXModel* m_model;
};