#include "stdafx.h"
#include "MeshGenerators.hpp"

namespace Graphics
{
	Mesh MeshGenerators::Quad(OpenGL* gl, Vector2 pos, Vector2 size /*= Vector2(1,1)*/)
	{
		Vector<SimpleVertex> verts =
		{
			{ { 0.0f,  size.y, 0.0f }, { 0.0f, 0.0f } },
			{ { size.x, 0.0f,  0.0f }, { 1.0f, 1.0f } },
			{ { size.x, size.y, 0.0f }, { 1.0f, 0.0f } },

			{ { 0.0f,  size.y, 0.0f }, { 0.0f, 0.0f } },
			{ { 0.0f,  0.0f,  0.0f }, { 0.0f, 1.0f } },
			{ { size.x, 0.0f,  0.0f }, { 1.0f, 1.0f } },
		};

		for(auto& v : verts)
		{
			v.pos += pos;
		}

		Mesh mesh = MeshRes::Create(gl);
		mesh->SetData(verts);
		mesh->SetPrimitiveType(PrimitiveType::TriangleList);
		return mesh;
	}

	void MeshGenerators::GenerateSimpleXYQuad(Rect3D r, Rect uv, Vector<SimpleVertex>& out)
	{
		Vector<MeshGenerators::SimpleVertex> verts =
		{
			{ { r.Left(),  r.Top(),     0.0f },{ uv.Left(), uv.Top() } },
			{ { r.Right(), r.Bottom(),  0.0f },{ uv.Right(), uv.Bottom() } },
			{ { r.Right(), r.Top(),     0.0f },{ uv.Right(), uv.Top() } },

			{ { r.Left(),  r.Top(),     0.0f },{ uv.Left(), uv.Top() } },
			{ { r.Left(),  r.Bottom(),  0.0f },{ uv.Left(), uv.Bottom() } },
			{ { r.Right(), r.Bottom(),  0.0f },{ uv.Right(), uv.Bottom() } },
		};
		for(auto& v : verts)
			out.Add(v);
	}
	void MeshGenerators::GenerateSimpleXZQuad(Rect3D r, Rect uv, Vector<MeshGenerators::SimpleVertex>& out)
	{
		Vector<MeshGenerators::SimpleVertex> verts =
		{
			{ { r.Left(),  0.0f, r.Top(),    },{ uv.Left(), uv.Top() } },
			{ { r.Right(), 0.0f, r.Bottom(), },{ uv.Right(), uv.Bottom() } },
			{ { r.Right(), 0.0f, r.Top(),    },{ uv.Right(), uv.Top() } },
						   
			{ { r.Left(),  0.0f, r.Top(),    },{ uv.Left(), uv.Top() } },
			{ { r.Left(),  0.0f, r.Bottom(), },{ uv.Left(), uv.Bottom() } },
			{ { r.Right(), 0.0f, r.Bottom(), },{ uv.Right(), uv.Bottom() } },
		};
		for(auto& v : verts)
			out.Add(v);
	}
	Mesh MeshGenerators::Hex(OpenGL *gl, Vector2 pos, float width, float thickness)
	{
		float w = width;
		float i = thickness;
		float x = w / (2 + sqrt(3.0));
		float h = i * sqrt(3.0) / 2;
		float j = i / 2.0;
		float l = w - 2.0 * x;

		Vector<MeshGenerators::SimpleVertex> verts =
				{
						{{x, 0.0, 0.0}, {0.0, 0.0}},
						{{x + l, 0.0, 0.0}, {0.0, 0.0}},
						{{x + j, h, 0.0}, {1.0, 1.0}},

						{{x + l - j, h, 0.0}, {1.0, 1.0}},
						{{x + j, h, 0.0}, {1.0, 1.0}},
						{{x + l, 0.0, 0.0}, {0.0, 0.0}},

						// bottom-right
						{{x + l, 0.0, 0.0}, {0.0, 0.0}},
						{{w, w / 2.0f, 0.0}, {0.0, 0.0}},
						{{x + l - j, h, 0.0}, {1.0, 1.0}},

						{{w - i, w / 2.0f, 0.0}, {1.0, 1.0}},
						{{x + l - j, h, 0.0}, {1.0, 1.0}},
						{{w, w / 2.0f, 0.0}, {0.0, 0.0}},

						// top-right
						{{w, w / 2.0f, 0.0}, {0.0, 0.0}},
						{{x + l, w, 0.0}, {0.0, 0.0}},
						{{w - i, w / 2.0f, 0.0}, {1.0, 1.0}},

						{{x + l - j, w - h, 0.0}, {1.0, 1.0}},
						{{w - i, w / 2.0f, 0.0}, {1.0, 1.0}},
						{{x + l, w, 0.0}, {0.0, 0.0}},

						// top
						{{x + l, w, 0.0}, {0.0, 0.0}},
						{{x, w, 0.0}, {0.0, 0.0}},
						{{x + l - j, w - h, 0.0}, {1.0, 1.0}},

						{{x + j, w - h, 0.0}, {1.0, 1.0}},
						{{x + l - j, w - h, 0.0}, {1.0, 1.0}},
						{{x, w, 0.0}, {0.0, 0.0}},

						// top left
						{{x, w, 0.0}, {0.0, 0.0}},
						{{0.0, w / 2.0f, 0.0}, {0.0, 0.0}},
						{{x + j, w - h, 0.0}, {1.0, 1.0}},

						{{i, w / 2.0f, 0.0}, {1.0, 1.0}},
						{{x + j, w - h, 0.0}, {1.0, 1.0}},
						{{0.0, w / 2.0f, 0.0}, {0.0, 0.0}},

						// bottom left
						{{0.0, w / 2.0f, 0.0}, {0.0, 0.0}},
						{{x, 0.0, 0.0}, {0.0, 0.0}},
						{{i, w / 2.0f, 0.0}, {1.0, 1.0}},

						{{x + j, h, 0.0}, {1.0, 1.0}},
						{{i, w / 2.0f, 0.0}, {1.0, 1.0}},
						{{x, 0.0, 0.0}, {0.0, 0.0}},
				};
		for (auto &v : verts)
		{
			v.pos += pos;
		}

		Mesh mesh = MeshRes::Create(gl);
		mesh->SetData(verts);
		mesh->SetPrimitiveType(PrimitiveType::TriangleList);
		return mesh;
	}
}
