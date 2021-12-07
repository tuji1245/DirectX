#ifndef __TEXT_H__
#define __TEXT_H__

#include <DirectXMath.h>
#include <System/Graphics/Font.h>
#include <System/Graphics/DXBuffer.h>

class Text
{
private:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};
public:
	Text();
	~Text();

	HRESULT Create(const int textCount);
	void Release();

	void SetPos(const DirectX::XMFLOAT3& pos);
	void Draw(const char* text);

private:
	DirectX::XMFLOAT3 m_pos;
	Vertex *m_pVertex;
	UINT m_textCount;
	DXBuffer *m_pBuffer;
	Font *m_pFont;
};

#endif // __RENDER_TEXT_H__