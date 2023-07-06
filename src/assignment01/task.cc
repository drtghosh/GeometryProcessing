#include "task.hh"

#include <iostream>
#include <queue>

#include <typed-geometry/feature/matrix.hh>
#include <typed-geometry/feature/std-interop.hh>
#include <typed-geometry/feature/vector.hh>

bool task::is_delaunay(polymesh::edge_handle edge, pm::vertex_attribute<tg::pos2> const& position)
{
    auto const va = edge.halfedgeA().next().vertex_to();
    auto const vc = edge.halfedgeA().vertex_to();
    auto const vd = edge.halfedgeB().next().vertex_to();
    auto const vb = edge.halfedgeB().vertex_to();

    /* These are the four points of the triangles incident to edge _eh
            a
           / \
          /   \
        b ----- c
          \   /
           \ /
            d
    */
    tg::pos2 const& a = position[va];
    tg::pos2 const& b = position[vb];
    tg::pos2 const& c = position[vc];
    tg::pos2 const& d = position[vd];

    bool result = true;

    // IMPORTANT: DO NOT ADD ANY CODE OUTSIDE OF THE MARKED CODE STRIPS
    // INSERT CODE:
    // is the edge delaunay or not?
    // -> circum-circle test of the four points (a,b,c,d) OR check if the projected paraboloid is convex
    //--- start strip ---
    tg::pos2 const& mid_ab = { (a.x + b.x) / 2, (a.y + b.y) / 2 };
    float slope_normal_ab = std::numeric_limits<float>::infinity();
    float intercept_normal_ab = std::numeric_limits<float>::infinity();
    if (b.y != a.y) {
        slope_normal_ab = -(b.x - a.x) / (b.y - a.y);
        intercept_normal_ab = mid_ab.y - (slope_normal_ab * mid_ab.x);
    }

    tg::pos2 const& mid_ac = { (a.x + c.x) / 2, (a.y + c.y) / 2 };
    float slope_normal_ac = 0.f;
    float intercept_normal_ac = 0.f;
    if (c.y != a.y) {
        slope_normal_ac = -(c.x - a.x) / (c.y - a.y);
        intercept_normal_ac = mid_ac.y - (slope_normal_ac * mid_ac.x);
    }

    if (intercept_normal_ab == intercept_normal_ac) {
        result = false;
    }
    else {
        float center_x = 0.f;
        tg::pos2 circumcenter = { center_x, center_x };
        if (b.y == a.y) {
            center_x = mid_ab.x;
            circumcenter = { center_x, (slope_normal_ac * center_x) + intercept_normal_ac };
        } else if (c.y == a.y) {
            center_x = mid_ac.x;
            circumcenter = { center_x, (slope_normal_ab * center_x) + intercept_normal_ab };
        }
        else {
            center_x = (intercept_normal_ac - intercept_normal_ab) / (slope_normal_ab - slope_normal_ac);
            circumcenter = { center_x, (slope_normal_ab * center_x) + intercept_normal_ab };
        }
        float radius = tg::length(a - circumcenter);
        float dist_d_center = tg::length(d - circumcenter);
        if (dist_d_center < radius) {
            result = false;
        }
        std::cout << "Circumcenter: " << circumcenter << "Radius: " << radius << "Distance: " << dist_d_center << std::endl;
    }
    std::cout << "Lines info: " << slope_normal_ab << intercept_normal_ab << slope_normal_ac << intercept_normal_ac << std::endl;
    std::cout << "Delauney? " << result << std::endl;
    //--- end strip ---
    
    return result;
}

polymesh::vertex_index task::insert_vertex(polymesh::Mesh& mesh, pm::vertex_attribute<tg::pos2>& position, tg::pos2 const& vertex_position, polymesh::face_handle face)
{
    // add vertex and assign it its position
    auto const v = mesh.vertices().add();
    position[v] = vertex_position;

    if (face.is_valid())
    {
        mesh.faces().split(face, v);
        std::cout << "[delaunay] 1:3 Split: vertex " << v.idx.value << " at position " << vertex_position << " inside triangle " << face.idx.value << std::endl;
    }
    else
        return {};

    // IMPORTANT: DO NOT ADD ANY CODE OUTSIDE OF THE MARKED CODE STRIPS
    // INSERT CODE:
    // re-establish Delaunay property
    // ... find edges opposite to the inserted vertex
    // ... are these edges ok? otherwise: flip'em (use mesh.edges().flip(the_edge_to_flip))
    // ... propagate if necessary
    // Hint:
    //   Use is_delaunay(...) (see above) to check the delaunay criterion.
    //   Do not check boundary edges as they do not neighbor two triangles.
    //   You can check if an edge e is boundary by calling e.is_boundary()
    //   You can use an std::queue as a container for edges
    //--- start strip ---
    std::queue<polymesh::edge_handle> edges_to_check;
    auto visited = mesh.edges().make_attribute<bool>();
    for (auto e : mesh.edges()) {
        visited[e] = false;
    }
    
    //auto inserted_vertex = v.idx.value;

    for (auto f : v.all_faces()) {
        for (auto e : f.edges()) {
            if (e.vertexA() != v && e.vertexB() != v) {
                edges_to_check.push(e);
            }
        }
    }

    while (!edges_to_check.empty()) {
        polymesh::edge_handle current_edge = edges_to_check.front();
        auto current_edge_idx = current_edge.idx.value;
        edges_to_check.pop();
        std::cout << "Current edge index: " << current_edge_idx << " and edge visited: " << visited[current_edge] << std::endl;
        
        if (visited[current_edge]) {
            continue;
        }
        else {
            visited[current_edge] = true;
            if (current_edge.is_boundary()) continue;
            if (is_delaunay(current_edge, position)) {
                continue;
            }
            auto edges_faceA = current_edge.faceA().edges();
            auto edges_faceB = current_edge.faceB().edges();
            auto it1 = edges_faceA.begin();
            auto it2 = edges_faceB.begin();
            for (; it1 != edges_faceA.end(); ++it1, ++it2) {
                if (*it1 != current_edge) {
                    //visited[e] = false;
                    edges_to_check.push(*it1);
                }
                if (*it2 != current_edge) {
                    //visited[e] = false;
                    edges_to_check.push(*it2);
                }
            }
            mesh.edges().flip(current_edge);
        }
    }
    //--- end strip ---

    return v;
}
