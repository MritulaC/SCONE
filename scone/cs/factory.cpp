#include "stdafx.h"
#include "factory.h"
#include "FeedForwardController.h"
#include "JumpingMeasure.h"
#include "EnergyMeasure.h"
#include "..\opt\CmaOptimizer.h"

namespace scone
{
	//Factory< cs::Objective > g_ObjectiveFactory;
	//template<> Factory< cs::Objective >& GetFactory() { return g_ObjectiveFactory; }


	namespace factory
	{
		template<>
		sim::Simulation* Create( const PropNode& props )
		{
			sim::Simulation* simptr;
			
			String type = props.GetStr( "type" );
			if ( type == "Simbody" )
				simptr = new sim::Simulation_Simbody();
			else SCONE_THROW( "Unknown simulation type: " + type );
			
			simptr->ProcessProperties( props );

			return simptr;
		}

		template<>
		cs::ParameterizableController* Create( const PropNode& props )
		{
			cs::ParameterizableController* con;

			String type = props.GetStr( "type" );
			if ( type == "FeedForwardController" )
				con = new cs::FeedForwardController();
			else SCONE_THROW( "Unknown controller type: " + type );

			con->ProcessProperties( props );

			return con;
		}

		template<>
		cs::Measure* Create( const PropNode& props )
		{
			cs::Measure* m;

			String type = props.GetStr( "type" );
			if ( type == "JumpingMeasure" )
				m = new cs::JumpingMeasure();
			if ( type == "EnergyMeasure" )
				m = new cs::EnergyMeasure();
			else SCONE_THROW( "Unknown measure type: " + type );

			m->ProcessProperties( props );

			return m;
		}

		template<>
		cs::SimulationObjective* Create( const PropNode& props )
		{
			cs::SimulationObjective* o = new cs::SimulationObjective();
			o->ProcessProperties( props );
			return o;
		}

		template<>
		opt::Optimizer* Create( const PropNode& props )
		{
			opt::Optimizer* o;

			String type = props.GetStr( "type" );
			if ( type == "CMA" )
				o = new opt::CmaOptimizer();
			else SCONE_THROW( "Unknown optimizer type: " + type );

			o->ProcessProperties( props );

			return o;
		}
	}
}