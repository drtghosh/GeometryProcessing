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

    std::vector<tg::pos3> vs_coo;
    auto pos_vec = position.to_vector();

    //1. Calculate centre of origin
    const int size = vs.size();

    tg::pos3 coo = { 0.0,0.0,0.0 };
    for (int i = 0;i < size ;i++) {
        auto pos = pos_vec[vs[i].idx.value];
        vs_coo.push_back(pos);
        coo += pos;
    }
    coo /= size;

    //2. Move centre of gravity to origin
    for (int i = 0;i < size ;i++) {
        vs_coo[i]+=(-coo); //TODO does vs_coo change?
    }
    //3. Calculate inertia tensor
    auto xx = 0.0;
    auto yy = 0.0;
    auto zz = 0.0;
    auto xy = 0.0;
    auto xz = 0.0;
    auto yz = 0.0;

    for (int i = 0; i < size;i++) {
        xx += pow(vs_coo[i].x,2);
        yy += pow(vs_coo[i].y, 2);
        zz += pow(vs_coo[i].z, 2);

        xy += (vs_coo[i].x * vs_coo[i].y);
        xz += (vs_coo[i].x * vs_coo[i].z);
        yz += (vs_coo[i].y * vs_coo[i].z);
    }
    tg::mat3 inertia_tensor;
    inertia_tensor[0][0] = xx;
    inertia_tensor[1][1] = yy;
    inertia_tensor[2][2] = zz;

    inertia_tensor[0][1] = xy, inertia_tensor[1][0] = xy;
    inertia_tensor[0][2] = xz, inertia_tensor[2][0] = xz;
    inertia_tensor[1][2] = yz, inertia_tensor[2][1] = yz;

    //4. Find eigenvector for min eigenvalue as normal
    auto eigen_decomp = tg::eigen_decomposition_symmetric(inertia_tensor);

    float min_eigenvalue = FLT_MAX;
    tg::dir3 min_eigenvector;
    for (auto value : eigen_decomp._values) {
        auto eigenvalue = value.eigenvalue;
        auto eigenvector = value.eigenvector;
        if (eigenvalue < min_eigenvalue) {
            min_eigenvalue = eigenvalue;
            min_eigenvector = { eigenvector.x, eigenvector.y, eigenvector.z};
        }
    }

    normal = {min_eigenvector.x, min_eigenvector.y, min_eigenvector.z};

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

    auto normal_vec = normal.to_vector();
    auto position_vec = position.to_vector();

    auto n0 = normal_vec[v0.idx.value];
    auto n1 = normal_vec[v1.idx.value];

    auto pos0 = position_vec[v0.idx.value];
    auto pos1 = position_vec[v1.idx.value];

    float alpha = 0.0; //can be adjusted to consider vertex distance

    auto pos = pos1 - pos0;
    float norm = sqrt(pow(pos.x,2) + pow(pos.y, 2) + pow(pos.z, 2));
 
    weight = (1 - abs(tg::dot(n0,n1)))+alpha*norm;

    // ----- %< -------------------------------------------------------
    /*
     *
     * Insert your own code above.
     * NO CHANGES BEYOND THIS POINT!
     *
     */

    return weight;
}
