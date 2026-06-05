#pragma once
/// <summary>
/// シングルトンクラス
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class Singleton {
public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	inline static T& GetInstance() {
		static T instance;
		return instance;
	}

	// 複製・代入の禁止
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	// Dxlib関係のデリート関数など削除するタイミングが決まってるものがあればここに
	virtual void DeleteData() {}
protected:
	Singleton() = default;
	~Singleton() = default;

};