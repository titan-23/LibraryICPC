struct Timer {
  chrono::time_point<chrono::high_resolution_clock> s;
  Timer() : s(chrono::high_resolution_clock::now()) {}
  void reset() { s = chrono::high_resolution_clock::now(); }
  double elapsed() { // [ms]
    using namespace chrono;
    return duration_cast<duration<double, milli>>(high_resolution_clock::now() - s).count();
  }
};