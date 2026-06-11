#pragma once
#include "../Character.h"
#include "../../../Definition/Enum/EnemyState.h"

class EnemyBase : public Character{
protected:

	struct Ray_Fan {
		float rayLenght = 1000.0f;	// レイの長さ
		float rayAngle = 100.0f;		// レイの角度
		int rayCount = 30;			// レイの数
		float raySpan = 0.1f;		// レイが更新される間隔
		float rayTime = raySpan;	// レイが更新される時間
	};
private:
	struct Point {
		VECTOR position;
	};

	Point point;

	struct Fan {
		VECTOR position;		// 中心
		float rangeDegree;		// 範囲
		float length;			// 長さ
		float directionDegree;	// 方向
	};
	// 視界
	Ray_Fan vision;

	bool rayAnswer;

private:
	VECTOR spawnPoint;
	VECTOR wanderingGoalPos;

protected:
	EnemyActionState currentState;
	EnemyActionState nextState;
	float moveSpeed = 1000;
	float standbyTime;
	bool isAttacking;

public:
	EnemyBase(int _modelHandle, VECTOR _pos);
	~EnemyBase();

protected:
	virtual void Start() override;

public:
	virtual void Update() override;
	virtual void Render() override;
	virtual void Setup() override;

protected:	// 行動
	/// <summary>
	/// 移動
	/// </summary>
	void Move(VECTOR targetPos);

	/// <summary>
	/// 徘徊
	/// </summary>
	void WanderingAction();

	/// <summary>
	/// 追跡
	/// </summary>
	void TracingAction();

	/// <summary>
	/// 攻撃
	/// </summary>
	void AttackAction();

private:
	// 扇形の視界
	bool VisionFan(VECTOR target);
	// Vision_Fanのデバッグ表示
	void DrawVisionFanDebug();
};