#include "TrajectoryRecorder.h"

static uint8_t speedToByte(DisplayManager::SpeedMode s) {
  switch (s) {
    case DisplayManager::SpeedMode::Fast:   return 0;
    case DisplayManager::SpeedMode::Slow: return 1;
    default: return 1;
  }
}

void TrajectoryRecorder::begin() {
  _prefs.begin("btrv", false); // namespace "btrv"
}

bool TrajectoryRecorder::hasSavedTrack() {
  return _prefs.getUShort("segCount", 0) > 0;
}

void TrajectoryRecorder::startRecording(DisplayManager::SpeedMode speed) {
  _mode = Mode::Recording;
  _count = 0;

  _curLeft = 0;
  _curRight = 0;
  _curSpeed = speedToByte(speed);

  _segStartMs = millis();
}

void TrajectoryRecorder::stopRecording() {
  if (_mode != Mode::Recording) return;

  // finalize last segment
  finalizeCurrentSegment(millis());

  // Save
  saveToFlash();

  _mode = Mode::Idle;
}

bool TrajectoryRecorder::startPlayback() {
  if (!loadFromFlash()) return false;
  if (_count == 0) return false;

  _mode = Mode::Playing;
  _playIdx = 0;
  _playSegStartMs = millis();
  return true;
}

void TrajectoryRecorder::stopPlayback() {
  _mode = Mode::Idle;
}

void TrajectoryRecorder::recordTick(int left, int right, DisplayManager::SpeedMode speed) {
  if (_mode != Mode::Recording) return;

  // If command changed (or speed changed), close current segment and start new
  uint8_t sp = speedToByte(speed);

  // small deadband to avoid noise creating tons of segments
  const int DB = 6;
  bool changed =
      (abs(left - _curLeft) > DB) ||
      (abs(right - _curRight) > DB) ||
      (sp != _curSpeed);

  if (changed) {
    unsigned long now = millis();
    finalizeCurrentSegment(now);

    // start new segment
    _curLeft = (int16_t)left;
    _curRight = (int16_t)right;
    _curSpeed = sp;
    _segStartMs = now;
  }
}

bool TrajectoryRecorder::playbackTick(int &outLeft, int &outRight) {
  if (_mode != Mode::Playing) return false;
  if (_playIdx >= _count) {
    stopPlayback();
    return false;
  }

  Segment &seg = _buf[_playIdx];

  // output current segment values
  outLeft = seg.left;
  outRight = seg.right;

  unsigned long now = millis();
  unsigned long elapsed = now - _playSegStartMs;

  if (elapsed >= seg.dt_ms) {
    _playIdx++;
    _playSegStartMs = now;
  }

  // still valid this tick
  return (_mode == Mode::Playing);
}

void TrajectoryRecorder::finalizeCurrentSegment(unsigned long nowMs) {
  if (_count >= MAX_SEGMENTS) return;

  uint16_t dt = (uint16_t)min<unsigned long>(65535, (nowMs - _segStartMs));

  // avoid storing 0ms segments
  if (dt < 5) return;

  Segment &s = _buf[_count++];
  s.left = _curLeft;
  s.right = _curRight;
  s.dt_ms = dt;
  s.speedMode = _curSpeed;
}

bool TrajectoryRecorder::saveToFlash() {
  // store count + raw bytes
  _prefs.putUShort("segCount", _count);

  size_t bytes = (size_t)_count * sizeof(Segment);
  return _prefs.putBytes("segData", _buf, bytes) == bytes;
}

bool TrajectoryRecorder::loadFromFlash() {
  uint16_t n = _prefs.getUShort("segCount", 0);
  if (n == 0 || n > MAX_SEGMENTS) {
    _count = 0;
    return false;
  }

  size_t bytes = (size_t)n * sizeof(Segment);
  size_t got = _prefs.getBytes("segData", _buf, bytes);

  if (got != bytes) {
    _count = 0;
    return false;
  }

  _count = n;
  return true;
}
