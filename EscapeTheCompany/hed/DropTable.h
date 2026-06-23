#pragma once
#include <unordered_map>
#include <vector>

struct DropEntry {
	int itemId;
	int probability;
};

class DropTable {
public:
	DropTable();

	const std::vector<DropEntry>& getDrops(int enemyId) const;

private:
	std::unordered_map<int, std::vector<DropEntry>> dropTable;
	void loadDefaultDrops();
};