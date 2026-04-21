#include <SFML/Graphics.hpp>

enum class SpringType {
	None,
	Normal,
	Ice,
	Electric,
	Fire
};

class Spring {
public:
	SpringType type;

	//戦闘用
	float damageMultiplier = 1.0f; //ダメージ倍率
	float statusChance = 0.0f; //状態異常付与確率

	//フィールド
	bool ignoreFireFloor = false; //床判定を無視するか
	bool ignoreIceFloor = false; //床判定を無視するか
	bool ignoreElectricFloor = false; //床判定を無視するか

	Spring(SpringType t);
}; 
