////////////////////////////////////////////////////////////////////////
// \file IDistortion.h
//
// \brief pure virtual interface for a single position-distortion element
//        usable as a link in a chain-of-responsibility distorter.
//
// Framework-agnostic (art-free), like detinfo::IPositionDistorter, but with
// applicability folded in: a link answers PointValidForDistort/Correct so a
// first-match chain (detinfo::ChainedDistortion) can route each point to the
// first link that claims it. A link is NOT itself an IPositionDistorter; the
// chain aggregates links and presents the total IPositionDistorter interface
// to consumers.
//
// Applicability is evaluated in the space of the point handed to each method:
//   - PointValidForDistort(p): p is a TRUE-space position (forward transform).
//   - PointValidForCorrect(p): p is a DISTORTED-space position (inverse).
//
////////////////////////////////////////////////////////////////////////
#ifndef LARDATAALG_DETINFO_IDISTORTION_H
#define LARDATAALG_DETINFO_IDISTORTION_H

#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"

namespace detinfo {

  class IDistortion {
  public:
    IDistortion(IDistortion const&) = delete;
    IDistortion(IDistortion&&) = delete;
    IDistortion& operator=(IDistortion const&) = delete;
    IDistortion& operator=(IDistortion&&) = delete;
    virtual ~IDistortion() = default;

    /// Maps a true position to its distorted (reconstructed-space) position.
    virtual geo::Point_t Distort(geo::Point_t const& point) const = 0;

    /// Maps a distorted position back to its corrected (true-space) position.
    virtual geo::Point_t Correct(geo::Point_t const& point) const = 0;

    /// True if this link applies the forward distortion at true-space \p point.
    virtual bool PointValidForDistort(geo::Point_t const& point) const = 0;

    /// True if this link applies the inverse at distorted-space \p point.
    virtual bool PointValidForCorrect(geo::Point_t const& point) const = 0;

  protected:
    IDistortion() = default;

  }; // class IDistortion
} // namespace detinfo

#endif // LARDATAALG_DETINFO_IDISTORTION_H
