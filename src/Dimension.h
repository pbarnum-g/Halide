#ifndef HALIDE_DIMENSION_H
#define HALIDE_DIMENSION_H

/** \file
 * Defines the Dimension utility class for Halide pipelines
 */

#include "Func.h"
#include "Parameter.h"

namespace Halide {
namespace Internal {

class Dimension {
public:
    /** Get an expression representing the minimum coordinates of this image
     * parameter in the given dimension. */
    EXPORT Expr min() const;

    /** Get an expression representing the extent of this image
     * parameter in the given dimension */
    EXPORT Expr extent() const;

    /** Get an expression representing the maximum coordinates of
     * this image parameter in the given dimension. */
    EXPORT Expr max() const;

    /** Get an expression representing the stride of this image in the
     * given dimension */
    EXPORT Expr stride() const;

    /** Get the estimate of the minimum coordinate of this image parameter
     * in the given dimension. Return an undefined expr if the estimate is
     * never specified. */
    EXPORT Expr min_estimate() const;

    /** Get the estimate of the extent of this image parameter in the given
     * dimension. Return an undefined expr if the estimate is never specified. */
    EXPORT Expr extent_estimate() const;

    /** Set the min in a given dimension to equal the given
     * expression. Setting the mins to zero may simplify some
     * addressing math. */
    EXPORT Dimension set_min(Expr min);

    /** Set the extent in a given dimension to equal the given
     * expression. Images passed in that fail this check will generate
     * a runtime error. Returns a reference to the ImageParam so that
     * these calls may be chained.
     *
     * This may help the compiler generate better
     * code. E.g:
     \code
     im.dim(0).set_extent(100);
     \endcode
     * tells the compiler that dimension zero must be of extent 100,
     * which may result in simplification of boundary checks. The
     * value can be an arbitrary expression:
     \code
     im.dim(0).set_extent(im.dim(1).extent());
     \endcode
     * declares that im is a square image (of unknown size), whereas:
     \code
     im.dim(0).set_extent((im.dim(0).extent()/32)*32);
     \endcode
     * tells the compiler that the extent is a multiple of 32. */
    EXPORT Dimension set_extent(Expr extent);

    /** Set the stride in a given dimension to equal the given
     * value. This is particularly helpful to set when
     * vectorizing. Known strides for the vectorized dimension
     * generate better code. */
    EXPORT Dimension set_stride(Expr stride);

    /** Set the min and extent in one call. */
    EXPORT Dimension set_bounds(Expr min, Expr extent);

    /** Set the min and extent estimates in one call. These values are only
     * used by the auto-scheduler. */
    EXPORT Dimension set_bounds_estimate(Expr min, Expr extent);

    /** Get a different dimension of the same buffer */
    // @{
    EXPORT Dimension dim(int i);
    EXPORT const Dimension dim(int i) const;
    // @}

private:
    friend class ::Halide::OutputImageParam;
    template<typename T2> friend class GeneratorInput_Buffer;
    template<typename T2> friend class GeneratorOutput_Buffer;

    /** Construct a Dimension representing dimension d of some
     * Internal::Parameter p. Only friends may construct
     * these. */
    EXPORT Dimension(const Internal::Parameter &p, int d, Func f);

    /** Only friends may copy these, too. This prevents
     * users removing constness by making a non-const copy. */
    Dimension(const Dimension &) = default;

    Parameter param;
    int d;
    Func f;
};

}  // namespace Internal
}  // namespace Halide

#endif
