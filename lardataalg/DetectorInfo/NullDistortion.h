////////////////////////////////////////////////////////////////////////
// \file NullDistortion.h
//
// \brief IDistortion link that applies no distortion: Distort()/Correct()
//        return the input point unchanged, and it applies everywhere.
//
// Useful as an explicit catch-all at the end of a ChainedDistortion (its
// PointValidFor* always return true, so it claims any point the earlier
// links declined). The chain already falls back to identity when no link
// matches, so this link is optional.
//
////////////////////////////////////////////////////////////////////////
#ifndef LARDATAALG_DETINFO_NULLDISTORTION_H
#define LARDATAALG_DETINFO_NULLDISTORTION_H

#include "lardataalg/DetectorInfo/IDistortion.h"

#include "fhiclcpp/types/Atom.h"

namespace fhicl {
  class ParameterSet;
}

namespace detinfo {

  class NullDistortion : public IDistortion {
  public:
    struct Config {
      using Name = fhicl::Name;
      using Comment = fhicl::Comment;
      // No parameters: the identity distortion is fully specified by its type.
    };

    explicit NullDistortion(fhicl::ParameterSet const& pset);

    /// Returns the point unchanged.
    geo::Point_t Distort(geo::Point_t const& point) const override { return point; }

    /// Returns the point unchanged.
    geo::Point_t Correct(geo::Point_t const& point) const override { return point; }

    /// Applies everywhere (catch-all).
    bool PointValidForDistort(geo::Point_t const&) const override { return true; }
    bool PointValidForCorrect(geo::Point_t const&) const override { return true; }

  }; // class NullDistortion
} // namespace detinfo

#endif // LARDATAALG_DETINFO_NULLDISTORTION_H
