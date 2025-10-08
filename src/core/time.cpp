// Implementation of time management
#include "time.h"

std::chrono::high_resolution_clock::time_point Time::start_time_;
std::chrono::high_resolution_clock::time_point Time::last_frame_time_;
float Time::delta_time_ = 0.0f;
float Time::total_time_ = 0.0f;
float Time::fps_ = 0.0f;
int Time::frame_count_ = 0;
float Time::fps_timer_ = 0.0f;

void Time::Initialize() {
    start_time_ = std::chrono::high_resolution_clock::now();
    last_frame_time_ = start_time_;
    delta_time_ = 0.0f;
    total_time_ = 0.0f;
    fps_ = 0.0f;
    frame_count_ = 0;
    fps_timer_ = 0.0f;
}

void Time::Update() {
    auto current_time = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        current_time - last_frame_time_).count();
    delta_time_ = static_cast<float>(duration) / 1000000.0f; // Convert to seconds
    
    duration = std::chrono::duration_cast<std::chrono::microseconds>(
        current_time - start_time_).count();
    total_time_ = static_cast<float>(duration) / 1000000.0f;
    
    last_frame_time_ = current_time;
    
    // Calculate FPS
    frame_count_++;
    fps_timer_ += delta_time_;
    
    if (fps_timer_ >= 1.0f) {
        fps_ = static_cast<float>(frame_count_) / fps_timer_;
        frame_count_ = 0;
        fps_timer_ = 0.0f;
    }
}

float Time::GetDeltaTime() {
    return delta_time_;
}

float Time::GetTotalTime() {
    return total_time_;
}

float Time::GetFPS() {
    return fps_;
}


