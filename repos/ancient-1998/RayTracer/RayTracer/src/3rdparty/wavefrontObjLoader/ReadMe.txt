Micah Taylor
AIM: kixornet
email: admin@kixor.net

http://test.kixor.net/school/2008fall/comp870/objloader/

Included with the project are two sample files: 'test.obj' and 
'test.mtl'. These are examples of the wavefront obj file format.
'test.obj' contains the geometry definitions, while 'test.mtl'
contains the material definitions.

There are extensions to the parser to support common
raytracing objects. There is also support for lights and
camera data. See the example files for details on the
specifications.



Wavefront (obj) loader
Overview

This is a basic .obj loader written in C. A C++ wrapper is included. It can parse vertices, texture coordinates, normals, 3 or 4 vertex faces, and .mtl files. There is also support for non-standard object types that are relevant to raytracting.

Please contact me if you have any suggestions for improvements.

The code and tests for the loader is available here. Some sample scenes are also available:

test.obj and test.mtl
cornell_box.obj and cornell_box.mtl
Storage data structures
obj_scene_data
This structure is used for scene access in C. The user is responsible for passing a pointer of this type to int parse_obj_scene(...). The memory allocated to scene storage may be freed by calling void delete_obj_data(...).
obj_vector **vertex_list	Array of vertex vector pointers
obj_vector **vertex_normal_list	Array of normal vector pointers
obj_vector **vertex_texture_list	Array of texture vector pointers
obj_face **face_list	Array of face pointers
obj_sphere **sphere_list	Array of sphere pointers
obj_plane **plane_list	Array of plane pointers
obj_light_point **light_point_list	Array of point light pointers
obj_light_quad **light_quad_list	Array of quad light pointers
obj_light_disc **light_disc_list	Array of disc light pointers
obj_material **material_list	Array of material pointers
int vertex_count	Length of vertex array
int vertex_normal_count	Length of normal vector array
int vertex_texture_count	Length of texture vector array
int face_count	Length of face array
int sphere_count	Length of sphere array
int plane_count	Length of plane array
int light_point_count	Length of point light array
int light_quad_count	Length of quad light array
int light_disc_count	Length of disc light array
int material_count	Length of material array
obj_camera *camera	Pointer to a camera

objLoader
This class can be used in C++. It does very little other than hiding the allocation and freeing of resources.
int load(char *filename)	Call this to parse a scene and fill in the data
obj_vector **vertexList	Array of vertex vector pointers
obj_vector **normalList	Array of normal vector pointers
obj_vector **textureList	Array of texture vector pointers
obj_face **faceList	Array of face pointers
obj_sphere **sphereList	Array of sphere pointers
obj_plane **planeList	Array of plane pointers
obj_light_point **lightPointList	Array of point light pointers
obj_light_quad **lightQuadList	Array of quad light pointers
obj_light_disc **lightDiscList	Array of disc light pointers
obj_material **materialList	Array of material pointers
int vertexCount	Length of vertex array
int normalCount	Length of normal vector array
int textureCount	Length of texture vector array
int faceCount	Length of face array
int sphereCount	Length of sphere array
int planeCount	Length of plane array
int lightPointCount	Length of point light array
int lightQuadCount	Length of quad light array
int lightDiscCount	Length of disc light array
int materialCount	Length of material array
obj_camera *camera	Pointer to a camera
Scene data structures
obj_vector
These are used to describe vertex, normal, and texture vectors.
double e[3]	Values for x, y, z in 3 space

obj_face
A polygon. Currently limited to triangles and quads.
int vertex_index[MAX_VERTEX_COUNT]	Indices of the position vertices
int normal_index[MAX_VERTEX_COUNT]	Indices of each vertex normal
int texture_index[MAX_VERTEX_COUNT]	Indices of each vertex texture coordinate
int vertex_count	Number of vertices in the face
int material_index	Index of the assigned material

obj_sphere
A sphere. The length of the up normal can be used as the radius.
int pos_index	Index of position vertex
int up_normal_index	Index of up normal
int equator_normal_index	Index of equator normal
int texture_index[MAX_VERTEX_COUNT]	Indices of texture coordinates
int material_index	Index of the assigned material

obj_plane
Infinite plane.
int pos_index	Index of position vertex
int normal_index	Index of the face normal
int rotation_normal_index	Index of the plane rotation normal
int texture_index[MAX_VERTEX_COUNT]	Indices of texture coordinates
int material_index	Index of the assigned material


obj_light_point
A simple point light source.
int pos_index	Index of the position vertex
int material_index	Index of the assigned material

obj_light_disc
An area light shaped like a disc.
int pos_index	Index of the position vertex
int normal_index	Index of the direction normal. The length can be used as the disc radius
int material_index	Index of the assigned material

obj_light_quad
An area light shaped like a quad.
int vertex_index[MAX_VERTEX_COUNT]	Indices of the position vertices
int material_index	Index of the assigned material

obj_camera
A simple camera.
int camera_pos_index	Index of the position vertex
int camera_look_point_index	Index of the focus vertex
int camera_up_norm_index	Index of the up normal

obj_material
Material definition. This is created when parsing a .mtl file.
char name[MATERIAL_NAME_SIZE]	String name of the material
char texture_filename[OBJ_FILENAME_LENGTH]	Filename of the texture map
double amb[3]	Ambient reflectance values
double diff[3]	Diffuse reflectance values
double spec[3]	Specular reflectance values
double reflect	Amount of reflection
double refract	Amount of refraction
double trans	Amount of transparency
double shiny	Sharpness of specular highlight
double glossy	Glossiness of reflection
double refract_index	Refractive index of the material
Example code

C example:

obj_scene_data data;
if( !parse_obj_scene(&data, "test.obj") )
	return 0;

// read the scene dta into your data structures

delete_obj_data(&data);

C++ example:

objLoader *objData = new objLoader();
objData->load("test.obj");

// read the scene dta into your data structures

delete(objData);

Example obj data

A quick list of what the parser can handle. For details on the format refer to the obj spec or the test file included with the code.

Vertex:
A vertex is a point in 3d space.

v 0.123 1.23 12.3
v 0.314 3.14 31.4

Normal:
A normal is used as a vertex normal in standard obj. This parser also uses normals in certain object definitions (spheres for instance).

vn 0 1 0
vn 0 0 -1

Texture Coordinate:
Texture coordinates are used to align textures to objects.

vt 5 5 0
vt -5 5 0

Material Library:
A file containing the scene material descriptions.

mtllib location_of_material_file.mtl

Material Reference:
A directive to begin assigning a certain material to all following objects. The mtl file must have already been processed.

usemtl a_material_name

Face:
Polygon faces are defined by a list of position vectices.

v  0.0 0.0 0.0
v  5.0 0.0 0.0
v  5.0 5.0 0.0
v  0.0 5.0 0.0
f 1 2 3
f 1 3 4

Sphere (non-standard):
Spheres are defined with a position vertex and two normals: one for the up normal and one for the equator normal. The length of either normal can be chosen for the sphere radius.

v 10 10 10
vn 1 0 0
vn 1 0 0
sp 1 1 2

Plane (non-standard):
Planes are defined with a position vertex and two normals: one for the rotation normal and one for the plane normal.

v 0 0 -10
vn 0 0 1
vn 1 0 0
pl 1 1 2

Light, point (non-standard):
A simple point light source. Use a material definition to set the output values.

v 10 50 0
lp 1

Light, quad (non-standard):
A 4 cornered area light.

v 0 50 0
v 10 50 0
v 10 50 10
v 0 50 10
lq 1 2 3 4

Light, disc (non-standard):
A disc-shaped arealight source. The normal specifies the direction and size of the disc.

v 0 50 0
vn 0 -1 0
ld 1 1

Camera (non-standard):
Used to define a simple camera. The 2 vertices define the camera position and the point the camera is focusing on. The normal defines the up vector.

v 0 0 -20
v 0 5 5
vn 0 1 0
c 1 2 1

Example mtl data

Object materials can be specified in a separate .mtl file. For details, refer to the mtl spec or the test file that comes with the code. A brief example follows.

newmtl material_name_goes_here
Ka  0.1 0 0      # ambient values
Kd  1 0 0        # diffuse values
Ks  1 1 1        # specular values
Ns  10           # sharpness of specular highlight
map_Ka test.png  # texture map filename
r 0.5            # reflection amount (non-standard)
sharpness 30     # sharpness of reflection
d  0.5           # transparency amount
Ni  1.33         # index of refraction
