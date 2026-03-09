#ifndef _KMAS_COMX_BASE_TAG_CODE_HXX
#define _KMAS_COMX_BASE_TAG_CODE_HXX

namespace KMAS{
namespace tag_code{

	enum tag_code_enum_t
	{
        GL_PRIMITIVE = -2,
		ANY = -1,
		POINT = 0,
		VERTEX = 1,
		CURVE = 2,
		FACE = 3,
		MESH_FACE = 4,
		MESH_BORDER = 5,
		MESH_NODE = 6,
		MESH_ELEMENT = 7,
		MESH_POINT = 8,
		GL_DB_RECORD = 9,
		EDGE = 10,
		CAD_FUNCTION = 11,
		RENDER_OBJECT = 12,
		KUL_TOOLBAR = 13,
		POLY_LINE = 14
	};

}// end namespace tag_code.
}// end namespace KMAS.

#endif
