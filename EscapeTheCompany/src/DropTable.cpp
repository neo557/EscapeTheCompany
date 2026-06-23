#include "DropTable.h"

DropTable::DropTable() {
	loadDefaultDrops();
}

void DropTable::loadDefaultDrops() {
	dropTable[1] = {
		{1,100},
		{2,90}
	};

	dropTable[2] = {
		{1,70},
		{2,30}
	};
}

const std::vector<DropEntry>& DropTable::getDrops(int enemyId) const {
	static const std::vector<DropEntry> empty;
	auto it = dropTable.find(enemyId);
	if (it != dropTable.end()) return it->second;
	return empty;
}