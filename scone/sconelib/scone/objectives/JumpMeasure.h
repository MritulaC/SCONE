#pragma once

#include "Measure.h"

#include "scone/core/PropNode.h"
#include "scone/optimization/ParamSet.h"
#include "scone/model/Model.h"

namespace scone
{
	class SCONE_API JumpMeasure : public Measure
	{
	public:
		JumpMeasure( const PropNode& props, ParamSet& par, Model& model, const Locality& area );
		virtual ~JumpMeasure();

		virtual double GetResult( Model& model ) override;
		virtual UpdateResult UpdateAnalysis( const Model& model, double timestamp ) override;
		virtual String GetClassSignature() const override;

	private:
		enum State { Prepare, Takeoff, Flight, Landing, Recover };
		enum JumpType { NoJumpType, HighJump, LongJump };

		double GetHighJumpResult( const Model& m );
		double GetLongJumpResult( const Model& m );
		static double GetLandingDist( const Vec3& pos, const Vec3& vel, double floor_height = 0.0 );

		State state;
		Body* target_body;
		Real termination_height;
		bool terminate_on_peak;
		Vec3 init_com;
		double init_min_x;
		Vec3 prepare_com;
		Vec3 peak_com;
		Vec3 peak_com_vel;
		Vec3 recover_com;
		TimeInSeconds recover_start_time;
		Real prepare_time;
		Real recover_time;
		Real recover_cop_dist = 1000.0;
		bool negate_result;
		int jump_type;
	};
}