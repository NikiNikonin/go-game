#pragma once
#include "./enums.hpp"
#include <functional>
#include <memory>
#include <set>
#include <utility>
#include <vector>

class StoneGroup {
  private:
    std::set<std::pair<int, int>> _stones;
    mutable int _liberty;
    std::vector<std::vector<State>>* _board;
    State _color;

  public:
    StoneGroup(State, std::vector<std::vector<State>>*);
    StoneGroup(std::pair<int, int>, std::vector<std::vector<State>>*);
    ~StoneGroup();

    void updateLiberty() const;
    int liberty() const;
    int& libertyRef();
    const std::set<std::pair<int, int>>& constStonesRef() const;
    State color() const;
    bool isIn(int, int) const;
    void addStone(std::pair<int, int>);
    void addGroup(StoneGroup&&);
    void decrLiverty1() const;

    bool operator==(const StoneGroup&) const;
};

namespace std {
template <> struct hash<StoneGroup>;
}
