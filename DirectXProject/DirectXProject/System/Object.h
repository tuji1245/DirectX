#pragma once

#include <vector>
#ifndef __COMPONENT_H__
#include "Component/Component.h"
#endif
#include "Component/Transform.h"
#include "Graphics/DirectX.h"
#include <string>
#include <typeinfo>

class Component;
class Transform;

class Object
{
public:
	Object(std::string name);
	virtual ~Object();

	// フレーム処理関数群
	/**
	* @brief 主に変数の更新を行う
	* @details 1フレームに1度だけ呼ばれる
	*/
	virtual void Update();
	/**
	* @brief 描画の設定を行う
	* @details 1フレームに1度だけ呼ばれる
	*/
	virtual void Draw();
	/**
	* @brief 自分が破壊された時の処理
	* @details 破壊されたフレームの終了時に呼ばれる
	*/
	virtual void OnDestroy();

	// コンポーネント関数群
	/**
	* @brief オブジェクトにコンポーネントを追加する
	* @return T 成功で追加したコンポーネント、失敗でnullptrが返る
	* @note 同じコンポーネントは複数持てない。
	*		今後複数持てるコンポーネントも追加する予定
	*/
	template<typename T> T* AddComponent();
	/**
	* @brief オブジェクトについているコンポーネントを取得する
	* @return 成功で指定したコンポーネント、失敗でnullptrが返る
	*/
	template<typename T> inline T* GetComponent();
	/**
	* @brief オブジェクトについている対応するコンポーネントをすべて取得する
	* @return 指定したコンポーネントをリストに格納し、見つからない場合nullptrが返る
	* @note 同じコンポーネントは複数持てない。
	*		今後複数持てるコンポーネントも追加する予定
	*/
	template<typename T> inline std::vector<T*> GetComponents();

	// 自身の状態に関する関数群
	virtual bool ActiveFlag() final;
	virtual void ActiveFlag(bool flag) final;
	virtual bool DestroyFlag() final;
	virtual void DestroyFlag(bool flag) final;

	// オブジェクト関数群
	static void Destoroy(Object* const& obj);


public:
	Transform m_transform;
	std::string m_name;

private:
	bool m_active;
	bool m_destoroy;
	std::vector<Component*> m_listComponent;
};

template<typename T>
T* Object::AddComponent()
{
	// コンポーネントでないものを排除
	// guess: コンパイル時エラーの法がいい？
	if (std::is_base_of(Component, T) == false)
		return nullptr;

	for (const auto& element : m_listComponent)
	{// 同じコンポーネントを排除
		if (typeid(*element) != typeid(T)) continue;
		return nullptr;
	}

	// コンポーネント追加
	T* buff = new T(this);
	m_listComponent.push_back(buff);
	return buff;
}
template<typename T>
T* Object::GetComponent()
{
	for (auto& element : m_listComponent)
	{
		T* buff = dynamic_cast<T*>(element);
		if (buff != nullptr)
			return buff;
	}
	return nullptr;
}

template<typename T>
std::vector<T*> Object::GetComponents()
{
	std::vector<T*> returnBuff;
	for (auto& element : m_listComponent)
	{
		T* buff = dynamic_cast<T*>(element);
		if (buff != nullptr)
			returnBuff.push_back(buff);
	}
	return returnBuff;
}