#pragma once
#include "./enums.hpp"
#include <functional>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
#include <vector>

class StoneGroup {
  private:
    std::set<std::pair<int, int>> _stones;
    mutable int _liberty;
    mutable std::vector<std::vector<PointColor>>* _board;
    PointColor _color;

  public:
    StoneGroup(PointColor, std::vector<std::vector<PointColor>>*);
    StoneGroup(std::pair<int, int>, std::vector<std::vector<PointColor>>*);
    ~StoneGroup();

    void updateLiberty() const;
    void clearStones();
    void setBoardPtr(std::vector<std::vector<PointColor>>*) const;
    int liberty() const;
    int& libertyRef();
    const std::set<std::pair<int, int>>& constStonesRef() const;
    PointColor color() const;
    bool isIn(int, int) const;
    void addStone(std::pair<int, int>);
    void addGroup(StoneGroup&);
    void decrLiverty1() const;

    bool operator==(const StoneGroup&) const;
    void operator=(StoneGroup&);
};

namespace std {
template <> struct hash<StoneGroup> {
    size_t operator()(const StoneGroup& group) const {
        size_t h = 0;
        for (const auto& stone : group.constStonesRef()) { h ^= std::hash<int>()(stone.first) ^ (std::hash<int>()(stone.second) << 1); }
        return h;
    }
};
} 
