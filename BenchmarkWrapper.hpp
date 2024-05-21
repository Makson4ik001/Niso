#pragma once
#include <chrono>

template<typename Callable, typename ...Args>
auto benchmark_function(Callable function, Args ...args) {
   const auto start {std::chrono::steady_clock::now()};
   auto val = function(args...);
   const auto end {std::chrono::steady_clock::now()};
   auto res = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
   return std::pair{res, val};
}

template<typename Callable, typename ...Args>
auto benchmark_procedure(Callable procedure, Args ...args) {
   const auto start {std::chrono::steady_clock::now()};
   procedure(args...);
   const auto end {std::chrono::steady_clock::now()};
   auto res = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
   return res;
}
