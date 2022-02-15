#ifndef __SCENE_BASE_H__
#define __SCENE_BASE_H__

#include <Windows.h>
#include <string>
#include <vector>
#include <System/Object.h>

#define SCENE_DECLARATION(scene) \
public:\
	scene();\
	virtual ~scene();\
	HRESULT Load();\
	void Release();\
	void Update(float tick);\
	void Draw() // ; あえてつけてない

class Scene abstract
{
public:
	Scene(const char *sceneName);
	virtual ~Scene();

	// 基本動作
	/**
	* @brief シーンの読み込み
	* @return HRESULT 成功時 0, 失敗時 0より小さい値
	* @details そのシーンに必要なものをすべて読み込む
	*/
	virtual HRESULT Load() = 0;
	/**
	* @brief シーンの破棄
	* @details Load()で読み込んだものをすべて解放
	*/
	virtual void Release() = 0;
	/**
	* @brief 1フレームに1度だけ呼ばれる
	* @deteils オブジェクトをすべて更新する
	*/
	virtual void Update(float tick) = 0;
	/**
	* @brief 描画を行う
	* @details 描画設定を行う
	*/
	virtual void Draw() = 0;
	/**
	* @brief フレーム終了時処理
	* @detail DestroyFlagを参照してオブジェクト破棄
	*/
	virtual void EndFrame() final;

protected:
	std::vector<Object*> m_listpObject;

private:
	std::string m_name;
};

#endif // __SCENE_BASE_H__