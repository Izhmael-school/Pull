/*
 * @file Animator.h
 * @author Sekino
 */

#include <Vector>
#include <string>
#include <functional>
#include <memory>

#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

template <class T = void, class... R>

/*
 * @brief アニメーションイベント構造体
 */
struct AnimationEvent {
	std::function<T(R...)> animEvent;	// 指定の時間になった時に発生するイベント
	float eventTime;			// イベントが発生する時間
	bool isAction;				// イベントが発生したかどうか
};

template <class T = void, class... R>
/*
 * @brief アニメーションクリップ構造体
 */
struct AnimationClip {
	int animationHandle;	// アニメーションハンドル
	float playSpeed;	// 再生速度
	int animIndex;		// アニメーションファイル内の番号
	float totalTime;	// アニメーションの終了時間
	float playTime;		// アニメーションの再生時間
	bool isLoop;		// ループするかどうか
	bool canInterrupt;	// 割り込み可能か

	std::string name;	// アニメーションの名前
	std::vector<AnimationEvent<void>> events; // イベント 

	/*
	 * @brief コンストラクタ
	 * @param[out] int& _animHandle
	 * @param[in]  bool _isLoop = false
	 * @param[in]  int _transition
	 */
	AnimationClip(int _animHandle, std::string _name, bool _canInterrupt, bool _isLoop = false)
		:animationHandle(_animHandle)
		, playTime(0.0f)
		, totalTime(0.0f)
		, playSpeed(1.0f)
		, animIndex(0)
		, canInterrupt(_canInterrupt)
		, isLoop(_isLoop)
		, name(_name)
		, events() {
	}

	~AnimationClip() = default;

	// アニメーションイベントの作成
	void SetEvent(std::function<T(R...)> _event, float _eventTime) {
		AnimationEvent e;
		e.animEvent = _event;
		e.eventTime = _eventTime;
		e.isAction = false;
		events.push_back(e);
	}
};


/*
 * @brief アニメーションを管理するクラス
 * @tips  アニメーション毎にファイルが分かれているモデル用
 */
class Animator {
private:
	int animationModelHandle;
	std::vector<std::unique_ptr<AnimationClip<>>> pAnimations;
	int currentAnimation;
	bool isPlaying;

public:
	Animator(int modelHandle);
	~Animator();

public:
	/*
	 * @brief 更新
	 */
	void Update();

	/*
	 * @brief アニメーションの読み込み
	 */
	void Load(std::string _filePath, bool _canInterrupt, bool _isLoop = false);

	/*
	 * @brief アニメーションの再生
	 */
	void Play(int _index, float _speed = 0.3f);
	int Play(std::string _name, float _speed = 0.3f);

	inline void Reset() {
		pAnimations.erase(pAnimations.begin(), pAnimations.begin() + pAnimations.size());
	}

private:

	/*
	 * @brief 名前に必要ない部分を消す
	 * @brief Blenderで出力したアニメーションにはArmature|~という感じになるため
	 * @brief Armature|を消すために存在する
	 * @return Armature|のないファイル名
	 */
	std::string DeleteUnnecessaryName(std::string _name);

public:
	/*
	 * @brief アニメーションの取得
	 */
	inline AnimationClip<>* GetAnimation(int _index) const { return pAnimations[_index].get(); }
	/*
	 * @brief アニメーションの取得
	 */
	AnimationClip<>* GetAnimation(std::string _name) const;
	/*
	 * @brief 現在再生中のアニメーションIDの取得
	 * @return 現在再生中のアニメーションID
	 */
	inline int GetCurrentAnimation() const { return currentAnimation; }
	/*
	 * @brief アニメーションが再生中か
	 * @return true : 再生中 | false : 停止中
	 */
	inline bool IsPlaying() const { return isPlaying; }
	/*
	 * @brief 指定アニメーションの再生速度変更
	 */
	void ChangeSpeed(int index, float _speed);
	/*
	 * @brief 指定アニメーションの再生速度変更
	 */
	void ChangeSpeed(std::string animName, float _speed);
	/*
	 * @brief 指定アニメーションの再生速度を取得
	 * @return 再生速度
	 */
	float GetAnimSpeed(int index);
	/*
	 * @brief 指定アニメーションの再生速度を取得
	 * @return 再生速度
	 */
	float GetAnimSpeed(std::string animName);
	/*
	 * @brief 指定アニメーションの総再生時間を取得
	 * @return 総再生時間
	 */
	float GetTotalTime(int index);
	/*
	 * @brief 指定アニメーションの総再生時間を取得
	 * @return 総再生時間
	 */
	float GetTotalTime(std::string animName);
	/*
	 * @brief 指定アニメーションのインデックス取得
	 */
	int GetAnimationIndex(std::string animName);

};
#endif // !_ANIMATOR_H_