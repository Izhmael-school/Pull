#include "Transform.h"
#include "../Definition/Const/VECTORConst.h"
#include "../Definition/CommonModule/MyMath.h"
#include <cmath>

Transform::Transform() 
	:position(VZero)
	,rotation(VZero)
	,scale(VOne)
	,matrix(MGetIdent())
	,isActive(true)
{}

void Transform::Update() {
	if (!isActive) return;

	// À•WA‰ñ“]AŠgk‚©‚çs—ñ‚ğ‹‚ß‚é
	MATRIX mRotX = MGetRotX(MyMath::Deg2Rad(rotation.x));
	MATRIX mRotY = MGetRotY(MyMath::Deg2Rad(rotation.y));
	MATRIX mRotZ = MGetRotZ(MyMath::Deg2Rad(rotation.z));

	// ‰ñ“]s—ñ‚Ìì¬
	MATRIX mRotXYZ = MMult(MMult(mRotZ, mRotX), mRotY);

	// Šgks—ñ‚Ìì¬
	MATRIX mScale = MGetScale(scale);

	// •½sˆÚ“®s—ñ‚ğæ“¾‚·‚é
	MATRIX mTranslate = MGetTranslate(position);

	// ‰ñ“]¨Šgk¨•½sˆÚ“®‚Ì‡‚ÅŠ|‚¯‚é
	matrix = MMult(MMult(mScale, mRotXYZ), mTranslate);
}

