#include <iostream>
#include <vector>
#include <unordered_set>

bool CheckSequence(const std::vector<uint8_t>& stack) {
  if (stack.size() != 9) {
    return false;
  }
  for (auto i = 0; i < 9; ++i) {
    if (stack[i] != 8 - i) {
      return false;
    }
  }
  return true;
}

using Vector = std::vector<std::vector<uint8_t>>;

struct State {
  Vector status;
  bool used = false;
  bool empty = false;

  explicit State(const Vector& other) {
    status.resize(8, std::vector<uint8_t>(9));
    for (auto i = 0; i < 8; ++i) {
      for (auto j = 0; j < 9; ++j) {
        status[i][j] = other[i][j];
      }
    }
  }

  std::vector<State> GetNextState() {
    std::vector<State> moves;
    for (auto i = 0; i < status.size(); ++i) {
      if (!status[i].empty()) {
        uint8_t card = status[i].back(); // Top card
        for (auto j = 0; j < status.size(); ++j) {
          if (i != j && (status[j].empty() || status[j].back() > card)) {
            State new_state(status);
            new_state.status[j].push_back(i);
            new_state.status[i].pop_back();
            moves.push_back(new_state);
          }
        }
      }
    }
    return moves;
  }

  void RemoveSequance() {
    std::vector<int8_t> cnt;
    int8_t step = 0;
    for (auto i : status) {
      if (CheckSequence(i)) {
        cnt.push_back(step);
      }
      ++step;
    }

    for (auto j : cnt) {
      for(auto i : status[j]) {
        status[j].pop_back();
      }
    }

    for (auto i : status) {
      if (!i.empty()) {
        return;
      }
    }
    empty = true;
  }
};

int DFS(State& v, int num) {
  v.RemoveSequance();
  if (v.empty) {
    return num;
  }
  v.used = 1;
  std::vector<State> neighbours = v.GetNextState();
  for (auto u : neighbours) {
    if (!u.used) {
      DFS(u, num + 1);
    }
  }
  return -1;
}


int main() {
  Vector matrix(8, std::vector<uint8_t>(9));
  for (auto i = 0; i < 8; ++i) {
    for (auto j = 0; j < 9; ++j) {
      std::cin >> matrix[i][j];
    }
  }
  State matr(matrix);
  std::cout << DFS(matr, 0);
}