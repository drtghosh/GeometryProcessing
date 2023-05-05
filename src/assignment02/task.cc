#include "task.hh"

#include <typed-geometry/tg.hh>

tg::dir3 task::compute_normal(std::vector<pm::vertex_handle> const& vs, pm::vertex_attribute<tg::pos3> const& position)
{
    // the normal to be computed
    tg::dir3 normal;

    /*
     * INSERT YOUR OWN CODE BETWEEN THE HORIZONTAL LINES BELOW.
     * DO NOT CHANGE CODE ANYWHERE ELSE.
     *
     * Note that there is another function below this function
     * where you have to insert code as well.
     *
     * This function should compute a regression plane for the
     * supplied sequence of points and return an arbitrary one
     * of the two normal vectors of that plane. (The orientation
     * doesn't matter at this point.)
     *
     * Hints:
     *   use tg::mat3 as 3x3 (column major) matrix representation
     *   tg::eigen_decomposition_symmetric may save you a lot of time
     *
     */
    // ----- %< -------------------------------------------------------


    // ----- %< -------------------------------------------------------
    /*
     *
     * Insert your own code above.
     * NO CHANGES BEYOND THIS POINT!
     *
     */

    return normal;
}

float task::compute_mst_weight(pm::vertex_handle v0, pm::vertex_handle v1, pm::vertex_attribute<tg::pos3> const& position, pm::vertex_attribute<tg::dir3> const& normal)
{
    // this is the weight that you should overwrite
    float weight;

    /*
     * INSERT YOUR OWN CODE BETWEEN THE HORIZONTAL LINES BELOW.
     * DO NOT CHANGE CODE ANYWHERE ELSE.
     *
     * This function should compute the weight of the edge between
     * the two supplied vertices for the purpose of the normal
     * orientation propagation algorithm using the minimum spanning tree.
     *
     */
    // ----- %< -------------------------------------------------------


    // ----- %< -------------------------------------------------------
    /*
     *
     * Insert your own code above.
     * NO CHANGES BEYOND THIS POINT!
     *
     */

    return weight;
}
