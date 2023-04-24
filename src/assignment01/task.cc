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

    //1. Find line in standard form (NXx+NYy=C) from two points:
    //find normal n=(NX,NY)^T for point pairs (a,b) and (a,c)
    tg::vec<2, tg::f32> n1 = { a.x - b.x, a.y - b.y };
    tg::vec<2, tg::f32> n2 = { a.x - c.x,a.y - c.y };
    //2. Calculate midpoints of ab and ac:
    tg::pos2 mid_ab = { (a.x+b.x) / 2, (a.y+b.y) / 2 };
    tg::pos2 mid_ac = { (a.x+c.x) / 2, (a.y+c.y) / 2 };

    //insert midpoints to find corresponding C
    float c1 = (n1.x * mid_ab.x) + (n1.y * mid_ab.y);
    float c2 = (n2.x * mid_ac.x) + (n2.y * mid_ac.y);

    //=>Line equations through midpoint perpendicular to the edges ab and ac: n1*(x,y)^T=c1 and n2*(x,y)^T=c2

    //3. Calculate intersection of lines:
    float determinant = ((n1.x * n2.y) - (n2.x * n1.y));

    if (determinant == 0) { //lines are parallel
        return false;
    }

    float x = (n2.y * c1) - (n1.y * c2);
    float y = (n1.x * c2) - (n2.x * c1);
    tg::pos2 circumcenter = {x / determinant, y / determinant};

    //4. Calculate circumradius
    tg::vec<2, tg::f32> v = a-circumcenter;
    float circumradius = tg::length(v);
    
    //5. Calculate distance from center for d
    tg::vec<2, tg::f32> test_vertex = d - circumcenter;
    float dist = tg::length(test_vertex);

    if (dist < circumradius) {
        result = false;
    }

    //TODO for testing only
    //std::cout << result << std::endl;

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

    std::queue<polymesh::edge_handle> edge_queue;
    std::vector<polymesh::edge_handle> visited_edges;

    //1. Find closest edges

    int new_vertex = v.idx.value;

    for (auto vertex : mesh.vertices()) {
        if (vertex.idx.value == new_vertex) {
            auto faces=vertex.all_faces();
            for (auto face : faces) {
                for (auto edge : face.edges()) { 
                    if (edge.vertexA().idx.value!=new_vertex && edge.vertexB().idx.value != new_vertex) {
                        edge_queue.push(edge);
                    }
                }
            }
        }
    }

    while (!edge_queue.empty()) {
        auto edge = edge_queue.front();
        edge_queue.pop();

        //edge has already been looked at
        if (std::find(visited_edges.begin(), visited_edges.end(),edge) != visited_edges.end()) { 
            continue; 
        }
        visited_edges.push_back(edge);

        if (edge.is_boundary()) {
            continue;
        }
        

        if (!is_delaunay(edge, position)) {
            auto faceA = edge.faceA();
            auto faceB = edge.faceB();

            for (size_t i = 0; i < faceA.edges().size();i++) {
                //add edges of triangles that were changed
                auto edgeA = faceA.edges().to_vector().at(i);
                if (edgeA != edge) {
                    edge_queue.push(edgeA);
                }
                auto edgeB = faceB.edges().to_vector().at(i);
                if (edgeB != edge) {
                    edge_queue.push(edgeB);
                }
            }
            mesh.edges().flip(edge);
        }
    }

    //--- end strip ---

    return v;
}
