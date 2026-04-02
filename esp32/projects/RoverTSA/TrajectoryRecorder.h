#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include "DisplayManager.h"

class TrajectoryRecorder {
public:
  enum class Mode : uint8_t { Idle, Recording, Playing };

  void begin();

  // Recording control
  void startRecording(DisplayManager::SpeedMode speed);
  void stopRecording();                 // also saves to flash
  bool isRecording() const { return _mode == Mode::Recording; }

  // Playback control
  bool startPlayback();                 // loads from flash
  void stopPlayback();
  bool isPlaying() const { return _mode == Mode::Playing; }

  // Feed current drive command during recording (call every loop in Status mode)
  void recordTick(int left, int right, DisplayManager::SpeedMode speed);

  // Get what to apply during playback (call every loop)
  // returns true if output updated / valid
  bool playbackTick(int &outLeft, int &outRight);

  // Info
  uint16_t segmentCount() const { return _count; }
  bool hasSavedTrack();

private:
  static const uint16_t MAX_SEGMENTS = 600; // adjust (600 segments ~ lots of driving)

  struct Segment {
    int16_t left;
    int16_t right;
    uint16_t dt_ms;
    uint8_t speedMode;
  };

  Mode _mode = Mode::Idle;
  Preferences _prefs;

  Segment _buf[MAX_SEGMENTS];
  uint16_t _count = 0;

  // Recording state
  int16_t _curLeft = 0, _curRight = 0;
  uint8_t _curSpeed = 1;
  unsigned long _segStartMs = 0;

  // Playback state
  uint16_t _playIdx = 0;
  unsigned long _playSegStartMs = 0;

  void finalizeCurrentSegment(unsigned long nowMs);
  bool saveToFlash();
  bool loadFromFlash();
};
