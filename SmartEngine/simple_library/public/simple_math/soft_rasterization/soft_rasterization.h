#include "../../simple_library.h"

#define VARIABLE(...)

namespace soft_rasterization
{
	////////////////////////////����
	//�����Ϊ����
	class fobject
	{
		friend class fengine;
	public:
		fobject();

	public:
		virtual void build(float in_time) {}
		virtual void tick(float in_time) {}
		virtual void clear() {}

	public:
		template<class t>
		t *create_object()
		{
			gobject_array.push_back(new t());

			return dynamic_cast<t*>(gobject_array[gobject_array.size() - 1]);
		}

	private:
		static void gobject_array_init(float in_time);
		static void gobject_array_tick(float in_time);
		static void gobject_array_clear();

	private:
		static std::vector<fobject*> gobject_array;
	};

	//�������
	class fcomponent : public fobject
	{
	public:
		fcomponent* parent;
		std::vector<fcomponent*> children;
	};

	//λ�����
	class ftransform_component :public fcomponent
	{
	public:
		ftransform_component();

		virtual void build(float in_time) {}
		virtual void tick(float in_time);
		virtual void clear() {}
	public:
		fvector_3d position;
		frotator rotation;
		fvector_3d scale;

		fmatrix_4x4 view_matrix;
		fmatrix_4x4 viewProj_matrix;
	};

	//Mesh���
	class fmesh_component :public ftransform_component
	{
	public:
		std::vector<fvector_3d> vertex_data;
	};

	//�ܿ����Ļ���
	class factor :public fobject
	{
		VARIABLE(category = actor, visibleanywhere, blueprintreadonly, meta = (allowprivateaccess = "true"))
		ftransform_component* transform;//root component

	public:
		factor();
		virtual ~factor();

		FORCEINLINE ftransform_component* get_transform() { return transform; }
	};

	//�����
	class fcamera :public factor
	{
	public:
	};

	//ģ��Actor
	class fmesh_actor :public factor
	{
		VARIABLE(category = actor, visibleanywhere, blueprintreadonly, meta = (allowprivateaccess = "true"))
		fmesh_component* mesh_component;

	public:
		fmesh_actor();
		~fmesh_actor();

		FORCEINLINE fmesh_component* get_mesh() { return mesh_component; }
	};

	//�ӿ�����
	struct fviewport_config
	{
		fviewport_config()
			:viewport_size(1920, 1080)
			, fov(0.5f * 3.1415926f)
			, aspect_ratio(viewport_size.x / viewport_size.y)
			, near_z(1.f)
			, far_z(1000.f)
		{
		}

		fvector_2d viewport_size;

		float fov;
		float aspect_ratio;
		float near_z;
		float far_z;
	};
	////////////////////////////Ҫ��Ⱦ������
	struct frender_data_3d
	{
		std::vector<fvector_3d> vertex_data;
		std::vector<uint16_t> index_data;

		fmatrix_4x4 matrix;
	};

	struct frender_data_2d
	{
		std::vector<fvector_2d> vertex_data;
		std::vector<uint16_t> index_data;
	};

	////////////////////////////����
	class fengine
	{
	public:
		virtual void init(float in_time);
		virtual void tick(float in_time);
		virtual void exit();
	};

	class frender_engine :public fengine
	{
		typedef fengine super;

	public:
		virtual void init(float in_time);
		virtual void tick(float in_time);
		virtual void exit();

	public:
		void build_input_path(const std::wstring& in_path);
		void build_draw_object(const std::vector<fmesh_actor*>& in_objs);
		void build_camera(const fvector_3d& in_position, const fviewport_config& in_config);
	
	public:
		void strat_update(float in_time);
		void draw(float in_time);
		void end_update(float in_time);

	protected:
		fcamera camera;
		fviewport_config viewport_config;

		std::vector<fmesh_actor*> draw_obj;

		//ÿһ֡Ҫ���Ƶ�����
		std::vector<frender_data_3d> frame_render_data3;
		std::wstring wpath;

		//֡��
		int index;//
	};

	class fengine_factory
	{
	public:
		template<class t>
		t* create_engine()
		{
			return new t();
		}
	};
}

/*
ʹ�÷���
#include "simple_library/public/simple_math/soft_rasterization/soft_rasterization.h"
using namespace soft_rasterization;
int main()
{
	class ftest_mesh_actor :public soft_rasterization::fmesh_actor
	{
		typedef soft_rasterization::fmesh_actor super;
	public:
		virtual void tick(float in_time)
		{
			super::tick(in_time);

			// λ��
			//get_transform()->position.x += 2.f;
			// ��ת
			//get_transform()->rotation.roll += 6.f;
			// ����
			//get_transform()->scale.x = 1.f;
			//get_transform()->scale.y = 1.f;
			//get_transform()->scale.z = 1.f;
		}
	};

	fengine_factory factory;
	if (frender_engine* render_enigne = factory.create_engine<frender_engine>())
	{
		std::vector<fmesh_actor*> in_objs;
		{
			in_objs.push_back(new ftest_mesh_actor());
			fmesh_actor *in_actor = in_objs[in_objs.size() - 1];

			//������ ��Ƭ
			in_actor->get_mesh()->vertex_data.push_back(fvector_3d(0.f, 0.f, 0.f));
			in_actor->get_mesh()->vertex_data.push_back(fvector_3d(0.f, 10.f, 10.f));
			in_actor->get_mesh()->vertex_data.push_back(fvector_3d(10.f, 0.f, 0.f));
		}

		//1.ת·��
		char path_buff[1024] = { 0 };
		char path[] = "../Math/render/a_%i.bmp";
		get_full_path(path_buff,1024 ,path);

		wchar_t dst_wchar_t[1024] = { 0 };
		char_to_wchar_t(dst_wchar_t,1024, path_buff);

		fviewport_config config;
		render_enigne->build_draw_object(in_objs);
		render_enigne->build_camera(fvector_3d(1.f,40.f,1.f),config);
		render_enigne->build_input_path(dst_wchar_t);

		render_enigne->init(0.2f);

		while (1)
		{
			render_enigne->tick(0.03f);
		}

		render_enigne->exit();
	}

	return 0;
}
*/