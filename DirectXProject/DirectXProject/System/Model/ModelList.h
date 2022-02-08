#pragma once

#include <System\Singleton.hpp>
#include <App\FBXModel.h>
#include <memory>

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

	[[nodiscard]] HRESULT Load();
	void Release();

	//GUESS: inline をつけると展開時に未解決外部参照？
	const std::weak_ptr<const FBXModel> GetModel(Type type) const;

private:
	static constexpr uint8_t MAX = static_cast<uint8_t>(Type::MAX);
	std::shared_ptr<FBXModel> m_modelList[MAX];
};
