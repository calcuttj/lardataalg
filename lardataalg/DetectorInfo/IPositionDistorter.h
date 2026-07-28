////////////////////////////////////////////////////////////////////////
// \file IPositionDistorter.h
//
// \brief pure virtual base interface for a spatial position distortion
//
// Modeled on spacecharge::SpaceCharge and detinfo::IElectricFieldProvider:
// a framework-agnostic (art-free), pure-virtual base living in its own
// header/namespace, mapping a spatial point to its distorted (Distort) or
// corrected (Correct) position.
//
////////////////////////////////////////////////////////////////////////
#ifndef LARDATAALG_DETINFO_IPOSITIONDISTORTER_H
#define LARDATAALG_DETINFO_IPOSITIONDISTORTER_H

#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"

namespace detinfo {

  class IPositionDistorter {
  public:
    IPositionDistorter(IPositionDistorter const&) = delete;
    IPositionDistorter(IPositionDistorter&&) = delete;
    IPositionDistorter& operator=(IPositionDistorter const&) = delete;
    IPositionDistorter& operator=(IPositionDistorter&&) = delete;
    virtual ~IPositionDistorter() = default;

    /// Maps a true position to its distorted (reconstructed-space) position.
    virtual geo::Point_t Distort(geo::Point_t const& point) const = 0;

    /// Maps a distorted position back to its corrected (true-space) position.
    virtual geo::Point_t Correct(geo::Point_t const& point) const = 0;

  protected:
    IPositionDistorter() = default;

  }; // class IPositionDistorter
} // namespace detinfo

#endif // LARDATAALG_DETINFO_IPOSITIONDISTORTER_H
