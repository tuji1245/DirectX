//----------
// @file Font.h
// @brief winapiで文字テクスチャ作成
// @author ryota.ssk
//----------
#ifndef __FONT_H__
#define __FONT_H__

#include <System/Graphics/Texture.h>
#include <vector>

class Font
{
public:
	struct Info
	{
		UINT	code;		// 文字コード
		POINT	adjust;		// 調整位置
		SIZE	size;		// 文字サイズ
		float	uv[2];		// 
	};

public:
	Font();
	~Font();

	//----------
	// 
	// 
	//----------
	bool Create(const char* pText);
	void Release();
	Texture* GetTexture();

	const Info& GetInfo(const char* pChara, BYTE* pReadByte);

	/**
	 * @brief 現在のフォント情報で描画サイズを計算
	 * @param[in] text 文字列
	 * @return 描画サイズ
	 */
	SIZE CalcSize(const char *text);

private:



	//Code GetCode(const char *c);
	//int FindInfo(const Code &code);
	//Info CreateInfo(const Code &code);

private:
	UINT m_size;
	UINT m_edge;
	//FontInfoList m_fontList;
	Texture* m_pTexture;
};

#endif // __WIN_FONT_TEXTURE_H__