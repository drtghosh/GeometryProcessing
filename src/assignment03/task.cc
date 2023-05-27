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

            auto v0 = position[eh.vertexA()];
            auto v1 = position[eh.vertexB()];

            auto edge_0 = eh.halfedgeA();
            auto edge_1 = eh.halfedgeB();

            auto v2 = position[edge_0.next().vertex_to()]; 
            auto v3 = position[edge_1.next().vertex_to()]; 

            auto dir20 = tg::normalize(v0 - v2);
            auto dir21 = tg::normalize(v1 - v2);
            auto dir30 = tg::normalize(v0 - v3);
            auto dir31 = tg::normalize(v1 - v3);

            //cot(a, b) = (a * b) / |a x b|
            float cot1 = tg::dot(dir20, dir21) / tg::length(tg::cross(dir20, dir21));
            float cot2= tg::dot(dir30, dir31) / tg::length(tg::cross(dir30, dir31));

            weights(eh) = ((cot1 + cot2) / 2);

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

                //boundary;
                if (locked[vh])
                    continue;

                // Iterating over one ring neighbourhood of the vertex
                for (auto edge : vh.outgoing_halfedges())
                {
                    //discretized Laplacian: sum over j wij*(pi-pj)
                    float w_ij = edge_weight[edge.edge()];
                    auto v = edge.vertex_to();
                    auto s = edge.vertex_from();
                    auto p = position[edge.vertex_to()] - position[edge.vertex_from()];
                    u += (w_ij * p);
                }

                u /= vh.outgoing_halfedges().size();
                new_position[vh] = position[vh] + 0.5 * u; 

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

            // 1st: compute Laplaces of positions
            auto laplace = mesh.vertices().make_attribute<tg::vec3>(); // Creating a vertex attribute of vectors to store the Laplace operator values

            for (auto vh : mesh.vertices())
            {
                auto u = tg::vec3::zero;

                //boundary;
                if (locked[vh])
                    continue;

                // Iterating over one ring neighbourhood of the vertex
                for (auto edge : vh.outgoing_halfedges())
                {
                    //discretized Laplacian: sum over j wij*(pi-pj)
                    float w_ij = edge_weight[edge.edge()];
                    auto p = position[edge.vertex_from()] - position[edge.vertex_to()];
                    u += (w_ij * p);
                }

                u /= vh.outgoing_halfedges().size();
                laplace[vh] = u;
            }

            // 2nd: compute Laplaces of Laplacian vectors of all one-ring neighbors
            for (auto vh : mesh.vertices())
            {
                auto u = tg::vec3::zero;

                //boundary;
                if (locked[vh])
                    continue;

                // Iterating over one ring neighbourhood of the vertex
                for (auto edge : vh.outgoing_halfedges())
                {
                    //discretized Laplacian: sum over j wij*(pi-pj)
                    float w_ij = edge_weight[edge.edge()];
                    auto p = laplace[edge.vertex_from()] - laplace[edge.vertex_to()];
                    u += (w_ij * p);
                }

                u /= vh.outgoing_halfedges().size();
                // 3rd: store updated positions in new_position (use damping factor 0.25 for stability)
                new_position[vh] = position[vh] - 0.25 * u;
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
