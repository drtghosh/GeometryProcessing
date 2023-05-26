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
            auto v0 = position[eh.vertexA()]; // First vertex of the edge
            auto v1 = position[eh.vertexB()]; // Second vertex of the edge
            float weight = 0.f; // Initializing the cotan weight for the edge
            auto heh0 = eh.halfedgeA();
            auto heh1 = eh.halfedgeB();
            auto nextVertex0 = position[heh0.next().vertex_to()]; // First opposite vertex position
            auto nextVertex1 = position[heh1.next().vertex_to()]; // Second opposite vertex position
            auto dir00 = tg::normalize(nextVertex0 -v0); // Direction from first vertex of the edge to the first opposite vertex
            auto dir01 = tg::normalize(nextVertex0 -v1); // Direction from second vertex of the edge to the first opposite vertex
            weight += tg::dot(dir00,dir01) / tg::length(tg::cross(dir00,dir01)); // Computing first cotangent (cot alpha_ij) and adding to the initial weight
            auto dir10 = nextVertex1 -v0; // Direction from first vertex of the edge to the second opposite vertex
            auto dir11 = nextVertex1 -v1; // Direction from second vertex of the edge to the second opposite vertex
            weight += tg::dot(dir10,dir11) / tg::length(tg::cross(dir10,dir11)); // Computing second cotangent (cot beta_ij) and adding to the current weight
            weights(eh) = (weight/2); // Averaging the cotangent weights
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
                float weightSum = 0.0f; // Initialing the sum of weights for one ring neighbourhood

                // Checking if vertex is on boundary just to be safe and ignore if true
                if (locked[vh])
                    continue;

                // Iterating over one ring neighbourhood of the vertex
                for (auto heh: vh.outgoing_halfedges())
                {
                    auto edgeVector = position[heh.vertex_to()] - position[heh.vertex_from()]; // Computing the edge vector of the current half edge
                    float edgeWeight = edge_weight[heh.edge()]; // Collecting the weight associated with the current edge
                    u += (edgeWeight* edgeVector); // Weighted average of the edge vectors
                    weightSum += edgeWeight; // Updating sum of weights for one ring neighbourhood
                }
                u /= weightSum; // Normalizing the weighted average by sum of weights to get the Laplace value
                new_position[vh] = position[vh] + 0.5 * u; // Computing new positions using Laplace smoothing (iterative)
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
            auto laplace = mesh.vertices().make_attribute<tg::vec3>(); // Creating a vertex attribute of vectors to store the Laplace operator values

            // Iterating over all vertices
            for (auto vh : mesh.vertices())
            {
                auto ub = tg::vec3::zero; // Initialing the Laplacian for current vertex

                float weightSumb = 0.0f; // Initialing the sum of weights for one ring neighbourhood

                // Checking if vertex is on boundary just to be safe and ignore if true
                if (locked[vh])
                    continue;

                // Iterating over one ring neighbourhood of the vertex
                for (auto heh: vh.outgoing_halfedges())
                {
                    auto edgeVector = position[heh.vertex_to()] - position[heh.vertex_from()]; // Computing the edge vector of the current half edge
                    float edgeWeight = edge_weight[heh.edge()]; // Collecting the weight associated with the current edge
                    ub += (edgeWeight* edgeVector); // Weighted average of the edge vectors
                    weightSumb += edgeWeight; // Updating sum of weights for one ring neighbourhood
                }
                ub /= weightSumb; // Normalizing the weighted average by sum of weights to get the Laplace value
                laplace[vh] = ub; // Storing the Laplace value for current vertex
            }

            // Iterating over all vertices
            for (auto vh : mesh.vertices())
            {
                auto ub2 = tg::vec3::zero; // Initialing the Bi-Laplacian for current vertex

                float weightSumb = 0.0f; // Initialing the sum of weights for one ring neighbourhood

                // Checking if vertex is on boundary just to be safe and ignore if true
                if (locked[vh])
                    continue;

                // Iterating over one ring neighbourhood of the vertex
                for (auto heh: vh.outgoing_halfedges())
                {
                    auto edgeVector2 = laplace[heh.vertex_to()] - laplace[heh.vertex_from()]; // Computing the edge vector of the current half edge by taking difference of Laplace values
                    float edgeWeight = edge_weight[heh.edge()]; // Collecting the weight associated with the current edge
                    ub2 += (edgeWeight* edgeVector2); // Weighted average of the Laplace edge vectors
                    weightSumb += edgeWeight; // Updating sum of weights for one ring neighbourhood
                }
                ub2 /= weightSumb; // Normalizing the weighted average by sum of weights to get the Laplace value
                new_position[vh] = position[vh] - 0.25 * ub2; // Computing new positions using Laplace² smoothing (iterative)
            }
            //--- end strip ---
        }

        // set new positions
        for (auto vh : mesh.vertices())
            if (!locked(vh))
                position(vh) = new_position(vh);
    }
}

}
