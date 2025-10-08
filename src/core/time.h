// Time management and delta time calculation
#pragma once

#include <chrono>

class Time {
public:
    static void Initialize();
    static void Update();
    
    static float GetDeltaTime();
    static float GetTotalTime();
    static float GetFPS();
    
private:
    static std::chrono::high_resolution_clock::time_point start_time_;
    static std::chrono::high_resolution_clock::time_point last_frame_time_;
    static float delta_time_;
    static float total_time_;
    static float fps_;
    static int frame_count_;
    static float fps_timer_;
};


