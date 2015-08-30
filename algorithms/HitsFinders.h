
#include "Physics.h"

/* 
 * File:   HitsFinders.h
 * Author: towelenee
 *
 * Created on August 30, 2015, 1:20 AM
 */
#pragma once

std::vector<std::pair<double, Hit> > findBestHits(  
  const EventInfo& event,
  const std::function<double(Hit)> function
)
{
  std::vector<std::pair<double, Hit> > distances;
  std::vector<Hit> sensorsBest(event.sensorNum);
  std::vector<double> currentMax(event.sensorNum);
  std::vector<bool> init(event.sensorNum);
  for (const Hit& hit : event.hits)
  {
    double value = function(hit);
    if (!init[hit.sensorId] || (currentMax[hit.sensorId] > value) )
    {
      sensorsBest[hit.sensorId] = hit;
      currentMax[hit.sensorId] = value;
      init[hit.sensorId] = true;
    }
  }
  for (const Hit& hit: sensorsBest)
  {
    if (init[hit.sensorId])
    {
      distances.emplace_back(function(hit), hit);
    }
  }
  return distances;
}

std::vector<Track> findHits(
  const std::vector<TrackPure>& tracks,
  const EventInfo& event
)
{
  const std::vector<Hit>& hits = event.hits;
  std::vector<Track> extendedTracks;
  extendedTracks.reserve(tracks.size());
  std::set<std::vector<uint32_t> > tracksSet;
  std::vector<bool> used(hits.size(), false);
  int cnt = 0;
  for (const TrackPure& track: tracks)
  {
    if (++cnt % 1000 == 0)
    {
      std::cerr << cnt << " tracks proceed" << std::endl;
    }
    std::vector<std::pair<double, Hit> > distances = findBestHits(event,
      std::bind(getDistance, track, std::placeholders::_1)
    );
    Track extended;
    std::sort(distances.begin(), distances.end(), 
      [](const std::pair<double, Hit>& a, const std::pair<double, Hit>& b) -> bool
      {
        return a.first < b.first;
      }
    );
    {
      double zStart = distances[0].second.z;
      extended.addHit(distances[0].second.id);
      for (size_t i = 1; i < 3; i++)
      {
        //if (isFit(track, distances[i].second, zStart))
        {
          extended.addHit(distances[i].second.id);
        }
      }
    }
    if (extended.hitsNum > 2)
    {
      extendedTracks.push_back(extended);
    }
  }
        
  return extendedTracks;
}
