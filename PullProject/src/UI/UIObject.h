/*
 *	@file	UIObject.h
 *  @author oorui
 */

#ifndef _UIOBJECT_H_
#define _UIOBJECT_H_



#include <vector>
#include <memory>

#include "../Definition/CommonModule/MyMath.h"

 /*
  *	UIで扱うオブジェクトの基本クラス
  */
class UIObject {
protected:

	/// <summary>
	/// UIを表示する座標
	/// </summary>
	Vector2 m_position;

	/// <summary>
	/// 表示フラグ
	/// </summary>
	bool m_visible;

	/// <summary>
	/// 親子階層の親のオブジェクト
	/// </summary>
	UIObject* m_parent;

	/// <summary>
	/// 親子階層の子のオブジェクト
	/// </summary>
	std::vector<std::unique_ptr<UIObject>> m_children;

public:
	UIObject();
	virtual ~UIObject() = default;

public:

	/*
	 *	初期設定
	 */
	void Init();

	/*
	 *	更新処理
	 */
	void Update(float deltaTime);

	/*
	 *	描画処理
	 */
	void Draw();

	/*
	 *	終了処理
	 */
	void End();

protected:

	/*
	 * 空の関数で、個別で処理したいことがあればoverrideして記載
	 */
	virtual void OnInit() {}

	/*
	 * 空の関数で、個別で処理したいことがあればoverrideして記載
	 */
	virtual void OnUpdate(float deltaTime) {}

	/*
	 * 空の関数で、個別で処理したいことがあればoverrideして記載
	 */
	virtual void OnDraw() {}

	/*
	 * 空の関数で、個別で処理したいことがあればoverrideして記載
	 */
	virtual void OnEnd() {}

public:

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector2& pos) { m_position = pos; }

	/// <summary>
	/// 座標を取得する
	/// </summary>
	/// <returns></returns>
	const Vector2& GetPosition() const { return m_position; }

	/// <summary>
	/// ワールド座標を取得する
	/// メンバ変数を返すわけではないので値渡しにしている
	/// </summary>
	/// <returns></returns>
	Vector2 GetWorldPosition() const;

public:
	/*
	 *	表示フラグを設定
	 */
	void SetVisible(bool visible) { m_visible = visible; }

	/*
	 *	表示フラグを取得
	 */
	bool IsVisible() const { return m_visible; }

	/*
	 * 親階層の表示状態を考慮した結果を取得
	 */
	bool IsVisibleInHierarchy() const;

	/*
	 * 子オブジェクトの追加
	 */
	void AddChild(std::unique_ptr<UIObject> child);

	/*
	 * 親オブジェクトの取得
	 */
	UIObject* GetParent() const { return m_parent; }
};
#endif // !_UIOBJECT_H_