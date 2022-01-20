#include "ModelList.h"

ModelList::ModelList()
{
	HRESULT hr = S_OK;

	const std::string entryPath = "Assets/Model/";
	const std::string fileType = ".fbx";
	const std::string modelPath[] =
	{
		"normalSample"
	};
	// ƒ‚ƒfƒ‹“Ç‚Ýž‚Ý
	for (uint8_t cnt = 0; auto & element : modelPath)
	{
		m_modelList[cnt] = new FBXModel;
		hr = m_modelList[cnt]->Load((entryPath + element + fileType).c_str());
		if (FAILED(hr))[[unlikely]]
		{
			// TODO: Ž¸”sŽž‚Ç‚¤‚·‚é‚©
		}
		cnt++;
	}
}

ModelList::~ModelList()
{
	for (auto& element : m_modelList)
		safe_delete(element);
}

const FBXModel* ModelList::GetModel(Type type) const
{
	return m_modelList[static_cast<uint8_t>(type)];
}

