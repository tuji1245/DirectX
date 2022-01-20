#pragma once

#include <System\Singleton.hpp>
#include <App\FBXModel.h>

class ModelList final : public Singleton<ModelList>
{
public:
	enum class Type
	{
		Smple,
		MAX
	};

public:
	ModelList();
	~ModelList();

	//GUESS: inline ������ƓW�J���ɖ������O���Q�ƁH
	const FBXModel* GetModel(Type type) const;

private:
	static constexpr uint8_t MAX = static_cast<uint8_t>(Type::MAX);
	FBXModel* m_modelList[MAX];
};
