
#pragma once
class ShadowMap {
	int shadowMapHandle;

public:
	ShadowMap();
	~ShadowMap();

public:
	void Start();
	void Update();
	void Render();

	void SetUp();
	void RoundShadowMap();
	void SpotShadowMap();

	int GetShadowMapHandle() const { return shadowMapHandle; }

	void Apply();
	void Disable();
};

