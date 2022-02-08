#include "ModelList.h"

ModelList::ModelList():
	m_modelList()
{
}

ModelList::~ModelList()
{
	Release();
}

HRESULT ModelList::Load()
{
	HRESULT hr = S_OK;

	const std::string entryPath = "Assets/Model/";
	const std::string fileType = ".fbx";
	const std::string modelPath[] =
	{
		"normalSample"
	};
	// ÉÇÉfÉãì«Ç›çûÇ›
	for (uint8_t cnt = 0; auto & element : modelPath)
	{
		m_modelList[cnt] = std::make_shared<FBXModel>();
		hr = m_modelList[cnt]->Load((entryPath + element + fileType).c_str());
		if (FAILED(hr)) [[unlikely]]
		{
			// TODO: é∏îséûÇ«Ç§Ç∑ÇÈÇ©
		}
		cnt++;
	}
}

void ModelList::Release()
{
	for (auto& element : m_modelList)
		element.reset();
}

const std::weak_ptr<const FBXModel> ModelList::GetModel(Type type) const
{
	// guess: shard -> weak cast?
	return m_modelList[static_cast<uint8_t>(type)];
}
