#pragma once

/*
 * @file Singleton.h
 * @author Sekino
*/

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

protected:
	Singleton() = default;
	~Singleton() = default;

};