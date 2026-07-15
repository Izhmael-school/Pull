/*
 *	@file	UIImage.h
 *  @author oorui
 */

#ifndef _UIIMAGE_H_
#define _UIIMAGE_H_

#include "../UIObject.h"


 /*
  *	汎用画像UIオブジェクト
  */
class UIImage : public UIObject {
private:
	int m_graphHandle;	// 画像ハンドル


public:

	UIImage();
	UIImage(int handle, const Vector2& position);
	virtual ~UIImage() override = default;

protected:
	void OnInit() override;
	void OnUpdate(float deltaTime)override;
	void OnDraw()override;
	void OnEnd() override;

public:
	void SetGraphHandle(int handle) { m_graphHandle = handle; }
	int GetGraphHandle() const { return m_graphHandle; }

};





#endif // !_UIIMAGE_H_