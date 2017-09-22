#pragma once

#include "scone/core/memory_tools.h"

namespace scone
{
	SCONE_DECLARE_CLASS_AND_PTR( Function );

	SCONE_DECLARE_CLASS_AND_PTR( SimulationObjective );
	SCONE_DECLARE_CLASS_AND_PTR( ImitationObjective );
	SCONE_DECLARE_CLASS_AND_PTR( ModelObjective );
	SCONE_DECLARE_CLASS_AND_PTR( FeedForwardController );
	SCONE_DECLARE_CLASS_AND_PTR( ReflexController );
	SCONE_DECLARE_CLASS_AND_PTR( MuscleReflex );
	SCONE_DECLARE_CLASS_AND_PTR( Reflex );
	SCONE_DECLARE_CLASS_AND_PTR( Measure );
	SCONE_DECLARE_CLASS_AND_PTR( HeightMeasure );
	SCONE_DECLARE_CLASS_AND_PTR( MetaReflexController );
	SCONE_DECLARE_CLASS_AND_PTR( MetaReflexDof );
	SCONE_DECLARE_CLASS_AND_PTR( MetaReflexMuscle );
	SCONE_DECLARE_CLASS_AND_PTR( MetaReflexVirtualMuscle );

	// forward declarations
	SCONE_DECLARE_CLASS_AND_PTR( Simulation );
	SCONE_DECLARE_CLASS_AND_PTR( Model );
	SCONE_DECLARE_CLASS_AND_PTR( Body );
	SCONE_DECLARE_CLASS_AND_PTR( Joint );
	SCONE_DECLARE_CLASS_AND_PTR( Dof );
	SCONE_DECLARE_CLASS_AND_PTR( Link );
	SCONE_DECLARE_CLASS_AND_PTR( Locality );
	SCONE_DECLARE_CLASS_AND_PTR( Actuator );
	SCONE_DECLARE_CLASS_AND_PTR( Muscle );
	SCONE_DECLARE_CLASS_AND_PTR( Controller );
	SCONE_DECLARE_CLASS_AND_PTR( Leg );
	SCONE_DECLARE_CLASS_AND_PTR( Sensor );
	SCONE_DECLARE_CLASS_AND_PTR( SensorDelayAdapter );

	// forward declarations
	SCONE_DECLARE_CLASS_AND_PTR( Optimizer );
	SCONE_DECLARE_CLASS_AND_PTR( Objective );
	//SCONE_DECLARE_CLASS_AND_PTR( Param );
	//SCONE_DECLARE_CLASS_AND_PTR( ParamSet );
}
