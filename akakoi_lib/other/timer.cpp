class Timer {
  chrono::time_point<chrono::high_resolution_clock> s;
  Timer() : start_timepoint(chrono::high_resolution_clock::now()) {}
  void reset() {
    s = chrono::high_resolution_clock::now();
  }
  double elapsed() const {
    auto e = chrono::high_resolution_clock::now();
    auto start = chrono::time_point_cast<chrono::microseconds>(s).time_since_epoch().count();
    auto end = chrono::time_point_cast<chrono::microseconds>(e).time_since_epoch().count();
    return (end - start) * 0.001;
  }
};