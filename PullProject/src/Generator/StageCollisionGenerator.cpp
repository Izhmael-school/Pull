#include "StageCollisionGenerator.h"
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#include "../Definition/CommonModule/MyJson.h"
#include "../Component/Collider/Collider.h"

//  Block構造体
struct Block
{
    int x = 0;
    int y = 0;
    int z = 0;
    std::string type = "";
};

//  座標をキーにするための構造体
struct Key
{
    int x, y, z;

    Key(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}

    bool operator==(const Key& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
};

// ハッシュ関数
struct KeyHash
{
    std::size_t operator()(const Key& k) const
    {
        return ((k.x * 73856093) ^
            (k.y * 19349663) ^
            (k.z * 83492791));
    }
};


// AABB生成のためのグリッドベースのグリーディアルゴリズム
void StageCollisionGenerator::Generate(const std::string& path, CollisionManager& manager)
{
    auto json = MyJson::LoadJsonFile(path);
    if (json.is_null())
    {
        printfDx("JSON load failed");
        return;
    }

    std::vector<Block> blocks;

    for (auto& b : json["blocks"])
    {
        Block block;
        block.x = b["x"];
        block.y = b["y"];
        block.z = b["z"];
        block.type = b["type"];
        blocks.push_back(block);
    }

    int ground = 0, tree = 0, bridge = 0;

    for (auto& b : blocks)
    {
        if (b.type == "ground") ground++;
        if (b.type == "tree") tree++;
        if (b.type == "bridge") bridge++;
    }

    std::unordered_set<Key, KeyHash> grid;

    for (auto& b : blocks)
    {
        //if (b.type != "ground") continue;
        grid.insert(Key(b.x, b.y, b.z));
    }

	//  スケール
    float scale = 100.0f;
    
	//  座標をソートするためのベクターにコピー
    std::vector<Key> cells(grid.begin(), grid.end());

    std::sort(cells.begin(), cells.end(),
        [](const Key& a, const Key& b)
        {
            if (a.y != b.y) return a.y < b.y;
            if (a.z != b.z) return a.z < b.z;
            return a.x < b.x;
        });

	//  使用済みの座標を記録するためのセット
    std::unordered_set<Key, KeyHash> used;

    int colCount = 0;

    for (auto& cell : cells)
    {
        if (used.count(cell)) continue;

        int x = cell.x;
        int y = cell.y;
        int z = cell.z;

		
        int w = 1;
        while (grid.count(Key(x + w, y, z)) &&
            !used.count(Key(x + w, y, z)))
        {
            w++;
        }

        int d = 1;
        bool expandZ = true;

        while (expandZ)
        {
            for (int dx = 0; dx < w; dx++)
            {
                if (!grid.count(Key(x + dx, y, z + d)) ||
                    used.count(Key(x + dx, y, z + d)))
                {
                    expandZ = false;
                    break;
                }
            }
            if (expandZ) d++;
        }

        int h = 1;
        bool expandY = true;

        while (expandY)
        {
            for (int dz = 0; dz < d; dz++)
            {
                for (int dx = 0; dx < w; dx++)
                {
                    if (!grid.count(Key(x + dx, y + h, z + dz)) ||
                        used.count(Key(x + dx, y + h, z + dz)))
                    {
                        expandY = false;
                        break;
                    }
                }
                if (!expandY) break;
            }
            if (expandY) h++;
        }

		// 使用済みの座標を記録
        for (int dy = 0; dy < h; dy++)
            for (int dz = 0; dz < d; dz++)
                for (int dx = 0; dx < w; dx++)
                    used.insert(Key(x + dx, y + dy, z + dz));

		// AABBの座標を計算
        // 通常座標
        float minX = (x * scale);
        float maxX = ((x + w) * scale);

        float minZ = (z * scale);
        float maxZ = ((z + d) * scale);
        float minY = y * scale;
        float maxY = (y + h) * scale;

		// 反転座標
        minX = -minX;
        maxX = -maxX;
        minZ = -minZ;
        maxZ = -maxZ;

		//  反転座標のminとmaxを入れ替え
        if (minX > maxX) std::swap(minX, maxX);
        if (minZ > maxZ) std::swap(minZ, maxZ);

		//  AABBコライダーを生成して登録
        AABBCollider* col = new AABBCollider();

        col->SetMin(VGet(minX, minY, minZ));
        col->SetMax(VGet(maxX, maxY, maxZ));

        manager.Register(col);
        colCount++;
    }

	//  デバッグ情報を出力
    printfDx("=== Collision Debug ===");
    printfDx("ground : %d", ground);
    printfDx("tree   : %d", tree);
    printfDx("bridge : %d", bridge);
    printfDx("collider: %d", colCount);
    printfDx("scale : %.2f", scale);
}
