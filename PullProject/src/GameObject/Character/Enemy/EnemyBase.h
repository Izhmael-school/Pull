#pragma once
#include "../Character.h"
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

	Fan fan;

	// 視界
	Ray_Fan vision;

	bool rayAnswer;

private:
	VECTOR spawnPoint;


private:
	virtual void Start() override;

public:
	virtual void Update() override;

private:
	// 扇形の視界
	bool VisionFan(VECTOR target);
	// Vision_Fanのデバッグ表示
	void DrawVisionFanDebug();
};