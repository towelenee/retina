#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>

const int GRID_SIZE_X_ON_Z0 = 10;
const int GRID_SIZE_Y_ON_Z0 = 10;
const int GRID_SIZE_DX_OVER_DZ = 10;
const int GRID_SIZE_DY_OVER_DZ = 10;

const double RETINA_SHARPNESS_COEFFICIENT = 10;
const size_t MAX_TRACK_SIZE = 24;
        
struct Hit {
  float x;
  float y;
  float z;
  uint32_t id;
  uint32_t sensorId;
  Hit(float x, float y, float z, uint32_t id, uint32_t sensorId) :
    x(x),
    y(y),
    z(z),
    id(id),
    sensorId(sensorId)
  {
  }
  Hit() = default;
  Hit(const Hit& other) : Hit(other.x, other.y, other.z, other.id, other.sensorId)
  {
      
  }
};

struct TrackPure { 
    //coefficients of lineEquation x = track.xOnZ0 + track.dxOverDz * z
    //                            y = track.yOnZ0 + track.dyOverDz * z;
public:
  float xOnZ0;
  float yOnZ0;
  float dxOverDz;
  float dyOverDz;
  TrackPure(float x0, float y0, float tx, float ty) noexcept : xOnZ0(x0), yOnZ0(y0), dxOverDz(tx), dyOverDz(ty)  {}
  TrackPure() = default;
};

TrackPure operator*(const TrackPure& one, const double alpha);

TrackPure operator+(const TrackPure& one, const TrackPure& other);

struct Track {
public:
  int hitsNum;
  int hits[MAX_TRACK_SIZE];
  inline void addHit(int hitId) noexcept
  {
    if(hitsNum < MAX_TRACK_SIZE)
      hits[hitsNum++] = hitId;
  }
  Track() :hitsNum(0) { }
  
};

double getDistanceFromTrackToHit(const TrackPure& track, const Hit& hit) noexcept;