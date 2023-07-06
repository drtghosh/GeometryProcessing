#include "task.hh"

#include <typed-geometry/tg.hh>
#include <iostream>

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
    std::vector<tg::pos3> current_points;
    for (auto v : vs) {
        current_points.push_back(position[v]);
    }
    float X = 0.0, Y = 0.0, Z = 0.0;
    for (auto p : current_points) {
        X += p.x;
        Y += p.y;
        Z += p.z;
    }
    X /= current_points.size();
    Y /= current_points.size();
    Z /= current_points.size();
    tg::mat3 M;
    for (auto p : current_points) {
        tg::pos3 centered_point = { p.x - X, p.y - Y, p.z - Z };
        float x_squared = centered_point.x * centered_point.x;
        float y_squared = centered_point.y * centered_point.y;
        float z_squared = centered_point.z * centered_point.z;
        float x_pdt_y = centered_point.x * centered_point.y;
        float x_pdt_z = centered_point.x * centered_point.z;
        float y_pdt_z = centered_point.y * centered_point.z;
        M[0][0] += x_squared;
        M[0][1] += x_pdt_y;
        M[0][2] += x_pdt_z;
        M[1][0] += x_pdt_y;
        M[1][1] += y_squared;
        M[1][2] += y_pdt_z;
        M[2][0] += x_pdt_z;
        M[2][1] += y_pdt_z;
        M[2][2] += z_squared;
    }

    auto eigen = tg::eigen_decomposition_symmetric(M);
    auto sm_eigenvector = eigen[0].eigenvector;
    normal = { sm_eigenvector.x, sm_eigenvector.y, sm_eigenvector.z };

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
    tg::dir3 normal0 = normal[v0];
    tg::dir3 normal1 = normal[v1];
    float dot_pdt = (normal0.x * normal1.x) + (normal0.y * normal1.y) + (normal0.z * normal1.z);
    float alpha = 0.0f; //manually chosen
    tg::pos3 point0 = position[v0];
    tg::pos3 point1 = position[v1];
    tg::vec3 distance_vec = point1 - point0;
    float norm_l2 = sqrt(distance_vec.x * distance_vec.x + distance_vec.y * distance_vec.y + distance_vec.z * distance_vec.z);
    weight = (1 - abs(dot_pdt)) + alpha * norm_l2;

    // ----- %< -------------------------------------------------------
    /*
     *
     * Insert your own code above.
     * NO CHANGES BEYOND THIS POINT!
     *
     */

    return weight;
}
