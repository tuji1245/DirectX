#ifndef __COMPONENT_H__
#define __COMPONENT_H__
/**
* @file Component.h
* @breif 
*
* @author 辻悠希
* @date 2021/06/28
* @details ※継承先のクラスで必ずコンポーネント制御の変数を適切に使用すること
*/

#include "Scripts/Object/Object.h"

class Object;

class Component
{
public:
	Component();
	virtual ~Component() {}

	virtual void Awake()		= 0;
	virtual void OnEnable()		= 0;
	virtual void Start()		= 0;
	virtual void Update()		= 0;
	virtual void FixedUpdate()	= 0;
	virtual void OnDisEnable()	= 0;
	virtual void Terminate()	= 0;

	virtual void SetParent(Object* parent) final { m_parent = parent; }

public:

protected:
	// コンポーネント自体の制御
	bool m_active;
	bool m_prevActive;
	bool m_StartFlag;

	Object* m_parent;
};

class A :public Component
{
public:
	void Awake() {};
	void OnEnable() {};
	void Start() {};
	void Update() {};
	void FixedUpdate() {};
	void OnDisEnable() {};
	void Terminate() {};
};
#endif // !__COMPONENT_H__