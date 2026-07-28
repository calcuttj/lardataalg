////////////////////////////////////////////////////////////////////////
// \file NullPositionDistorter.h
//
// \brief IPositionDistorter that applies no distortion: Distort() and
//        Correct() return the input point unchanged (identity transform).
//
////////////////////////////////////////////////////////////////////////
#ifndef LARDATAALG_DETINFO_NULLPOSITIONDISTORTER_H
#define LARDATAALG_DETINFO_NULLPOSITIONDISTORTER_H

#include "lardataalg/DetectorInfo/IPositionDistorter.h"

#include "fhiclcpp/types/Atom.h"

#include <string>

namespace fhicl {
  class ParameterSet;
}

namespace detinfo {

  class NullPositionDistorter : public IPositionDistorter {
  public:
    struct Config {
      using Name = fhicl::Name;
      using Comment = fhicl::Comment;
      // No parameters: the identity distorter is fully specified by its type.
    };

    explicit NullPositionDistorter(fhicl::ParameterSet const& pset);

    /// Returns the point unchanged.
    geo::Point_t Distort(geo::Point_t const& point) const override { return point; }

    /// Returns the point unchanged.
    geo::Point_t Correct(geo::Point_t const& point) const override { return point; }

  }; // class NullPositionDistorter
} // namespace detinfo

#endif // LARDATAALG_DETINFO_NULLPOSITIONDISTORTER_H
