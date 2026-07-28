////////////////////////////////////////////////////////////////////////
// \file DetectorProperties.h
//
// \brief pure virtual base interface for detector properties
//
// \author jpaley@fnal.gov
//
////////////////////////////////////////////////////////////////////////
#ifndef LARDATA_DETINFO_DETECTORPROPERTIES_H
#define LARDATA_DETINFO_DETECTORPROPERTIES_H

#include "lardataalg/DetectorInfo/DetectorPropertiesData.h"

#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"

#include "TVector3.h"

/// General LArSoft Utilities
namespace detinfo {

  class DetectorClocksData;
  class DetectorProperties {
  public:
    DetectorProperties() = default;
    DetectorProperties(const DetectorProperties&) = delete;
    DetectorProperties(DetectorProperties&&) = delete;
    DetectorProperties& operator=(const DetectorProperties&) = delete;
    DetectorProperties& operator=(DetectorProperties&&) = delete;
    virtual ~DetectorProperties() = default;

    /**
     * @brief Returns the nominal electric field in the specified volume
     * @param planegap volume specification (default: 0, the big drift volume)
     * @return electric field in the volume, in kV/cm
     *
     * The electric field is "nominal", i.e., a completely uniform field is
     * assumed.
     *
     * The planegap argument identifies which volume to return the field value
     * for. The relation between planegap and readout plane is not perfectly
     * formalized yet. In general, a good rule is that planegap N describes
     * the volume on the cathode side of wire plane N. This rule is formally
     * valid also for ArgoNeuT/LArIAT, where three wire planes are present.
     * But only two of them are instrumented and read, that are called
     * "readout plane 0" and "readout plane 1", but effectively correspond to
     * planegap 1 and 2.
     *
     * Note that all TPCs are assumed to have the same electric field values.
     */
    virtual double PerPlaneEfield(unsigned int planegap = 0) const = 0;

    /**
     * @brief Returns the electric field vector at a spatial point
     * @param point position in the detector [cm] (ROOT TVector3)
     * @return electric field vector at that point, in kV/cm
     *
     * The value is supplied by a pluggable IElectricFieldProvider, so
     * different (possibly non-uniform) field models can be selected by
     * configuration.
     */
    virtual TVector3 Efield(TVector3 const& point) const = 0;

    /**
     * @brief Maps a true position to its distorted (reconstructed-space) position
     * @param point true position in the detector [cm]
     * @return the distorted position [cm]
     *
     * The transform is supplied by a pluggable IPositionDistorter, so different
     * (space-charge-style) distortion models can be selected by configuration.
     */
    virtual geo::Point_t Distort(geo::Point_t const& point) const = 0;

    /**
     * @brief Maps a distorted position back to its corrected (true-space) position
     * @param point distorted position in the detector [cm]
     * @return the corrected position [cm]
     *
     * The inverse of Distort(), supplied by the same pluggable IPositionDistorter.
     */
    virtual geo::Point_t Correct(geo::Point_t const& point) const = 0;

    virtual double DriftVelocity(double efield = 0., double temperature = 0.) const = 0;

    /// dQ/dX in electrons/cm, returns dE/dX in MeV/cm.
    virtual double BirksCorrection(double dQdX) const = 0;
    virtual double BirksCorrection(double dQdX, double EField) const = 0;
    virtual double ModBoxCorrection(double dQdX) const = 0;
    virtual double ModBoxCorrection(double dQdX, double EField) const = 0;

    /**
     * @brief Returns the attenuation constant for ionization electrons.
     * @return the attenuation constant [&micro;s]
     *
     * The returned constant &tau; can be used to know the attenuation the
     * ionization charge undergoes after drifting for a certain time _t_:
     * @f$ A(t) = e^{-t/\tau} @f$ (_t_ is measured in microseconds).
     *
     * This is a uniform, constant value for the detector.
     */
    virtual double ElectronLifetime() const = 0;

    /**
     * @brief Returns argon density at a given temperature
     * @param temperature the temperature in kelvin
     * @return argon density in g/cm^3
     */
    virtual double Density(double temperature) const = 0;
    virtual double Temperature() const = 0;

    /**
     * @brief Restricted mean energy loss (@f$ dE/dx @f$)
     * @param mom  momentum of incident particle [GeV/c]
     * @param mass mass of incident particle [GeV/c^2]
     * @param tcut maximum kinetic energy of delta rays [MeV]; 0 for unlimited
     * @return the restricted mean energy loss (dE/dx) in units of MeV/cm
     *
     * Returned value is always positive.
     * For unrestricted mean energy loss, set tcut = 0 (special case),
     * or tcut large.
     */
    virtual double Eloss(double mom, double mass, double tcut) const = 0;

    /**
     * @brief Energy loss fluctuation (@f$ \sigma_{E}^2 / x @f$)
     * @param mom  momentum of incident particle in [GeV/c]
     * @param mass mass of incident particle [GeV/c^2]
     * @return energy loss fluctuation in MeV^2/cm
     */
    virtual double ElossVar(double mom, double mass) const = 0;

    /// Returns argon density at the temperature from Temperature()
    virtual double Density() const { return Density(Temperature()); }

    virtual double ElectronsToADC() const = 0;
    virtual unsigned int NumberTimeSamples() const = 0;
    virtual unsigned int ReadOutWindowSize() const = 0;
    virtual double TimeOffsetU() const = 0;
    virtual double TimeOffsetV() const = 0;
    virtual double TimeOffsetZ() const = 0;
    virtual double TimeOffsetY() const
    {
      throw std::runtime_error("DetectorProperties::TimeOffsetY() not implemented");
    }

    // To set the model we want for the optical boundary simulation Simple or G4
    // default
    virtual bool SimpleBoundary() const = 0;

    // Returns data object that corresponds to data specific for a
    // given clock-data object instance.
    virtual DetectorPropertiesData DataFor(DetectorClocksData const& clockData) const = 0;

  }; // class DetectorProperties
} // namespace detinfo

#endif // LARDATA_DETINFO_DETECTORPROPERTIES_H
