#include "task.hh"

#include <typed-geometry/tg.hh>

namespace task
{
pm::edge_attribute<float> compute_weights(pm::Mesh& mesh, pm::vertex_attribute<tg::pos3>& position, bool cotan_weights)
{
    auto weights = mesh.edges().make_attribute<float>();

    // Uniform weighting
    for (auto eh : mesh.edges())
        weights(eh) = 1.0f;

    if (cotan_weights) // Cotangent weighting
    {
        for (auto eh : mesh.edges())
        {
            if (eh.is_boundary())
                continue;

            // INSERT CODE:
            // Compute the cotan weights and store them in the weights attribute
            //--- start strip ---

            //--- end strip ---
        }
    }

    return weights;
}


void compute_new_positions(pm::Mesh& mesh,
                           pm::vertex_attribute<tg::pos3>& position,
                           pm::edge_attribute<float> const& edge_weight,
                           const pm::vertex_attribute<bool>& locked,
                           bool simple_laplace,
                           int iterations)
{
    // Compute new positions using Laplace or Laplace^2 smoothing

    auto new_position = mesh.vertices().make_attribute<tg::pos3>();

    for (int i = 0; i < iterations; ++i)
    {
        // Laplace
        if (simple_laplace)
        {
            for (auto vh : mesh.vertices())
            {
                auto u = tg::vec3::zero;

                // INSERT CODE:
                // Compute the Laplace vector and store the updated position in new_position:
                // new_position(v) = position(v) + 0.5* Laplace(v)
                //--- start strip ---

                //--- end strip ---
            }
        }
        else // bilaplacian smoothing
        {
            // INSERT CODE:
            // Compute the squared Laplacian update
            // 1st: compute Laplaces of positions
            // 2nd: compute Laplaces of Laplacian vectors of all one-ring neighbors
            // 3rd: store updated positions in new_position (use damping factor 0.25 for stability)
            //--- start strip ---

            //--- end strip ---
        }

        // set new positions
        for (auto vh : mesh.vertices())
            if (!locked(vh))
                position(vh) = new_position(vh);
    }
}

}
