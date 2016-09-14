#pragma once

#include "scone/sim/Model.h"
#include "simvis/scene.h"
#include "scone/cs/SimulationObjective.h"
#include "scone/opt/Objective.h"

#include "simvis/arrow.h"
#include <thread>
#include <mutex>

namespace scone
{
	SCONE_DECLARE_CLASS_AND_PTR( StudioModel );

	class StudioModel
	{
	public:
		StudioModel( vis::scene &s, const String& par_file );
		virtual ~StudioModel();

		void UpdateVis( TimeInSeconds t );
		void EvaluateObjective();

		const Storage<>& GetData() { std::lock_guard< std::mutex > lock( eval_mutex ); return data; }
		sim::Model& GetSimModel() { return so->GetModel(); }

		bool IsEvaluating() { return is_evaluating; }

	private:
		void InitModel( const String& par_file );
		void InitVis( vis::scene& s );
		void SetModelStateFromDataFrame( const Storage< Real, TimeInSeconds >::Frame& f );

		Storage<> data;
		cs::SimulationObjectiveUP so;
		String filename;

		std::vector< size_t > state_data_index;

		vis::material bone_mat;
		vis::material arrow_mat;
		vis::material muscle_mat;

		std::thread eval_thread;
		std::mutex eval_mutex;
		bool is_evaluating;

		vis::group root;
		std::vector< std::vector< vis::mesh > > body_meshes;
		std::vector< vis::mesh > joints;
		std::vector< std::pair< vis::path, vis::material > > muscles;
		std::vector< vis::arrow > forces;
		vis::mesh com;
		void InitStateDataIndices();
	};
}