#include "DofReflex.h"
#include "scone/model/Dof.h"
#include "scone/model/Side.h"
#include "scone/model/Actuator.h"
#include "scone/model/Model.h"
#include "scone/model/SensorDelayAdapter.h"
#include "scone/model/Sensors.h"

namespace scone
{
	DofReflex::DofReflex( const PropNode& props, Params& par, Model& model, const Locality& area ) :
	Reflex( props, par, model, area ),
	m_DelayedPos( model.AcquireDelayedSensor< DofPositionSensor >( *FindByNameTrySided( model.GetDofs(), props.get< String >( "source" ), area.side ) ) ),
	m_DelayedVel( model.AcquireDelayedSensor< DofVelocitySensor >( *FindByNameTrySided( model.GetDofs(), props.get< String >( "source" ), area.side ) ) ),
	m_DelayedRootPos( model.AcquireDelayedSensor< DofPositionSensor >( *FindByName( model.GetDofs(), "pelvis_tilt" ) ) ),
	m_DelayedRootVel( model.AcquireDelayedSensor< DofVelocitySensor >( *FindByName( model.GetDofs(), "pelvis_tilt" ) ) ),
	m_bUseRoot( m_DelayedRootPos.GetName() != m_DelayedPos.GetName() )
	{
		auto src_name = props.get< String >( "source" );
		String par_name = GetParName( props );
		name = GetReflexName( m_Target.GetName(), src_name );
		ScopedParamSetPrefixer prefixer( par, par_name + "." );

		INIT_PARAM_NAMED( props, par, target_pos, "P0", 0.0 );
		INIT_PARAM_NAMED( props, par, target_vel, "V0", 0.0 );
		INIT_PARAM_NAMED( props, par, pos_gain, "KP", 0.0 );
		INIT_PARAM_NAMED( props, par, vel_gain, "KV", 0.0 );
		INIT_PARAM_NAMED( props, par, constant_u, "C0", 0.0 );
		INIT_PROP( props, condition, 0 );

		INIT_PROP( props, filter_cutoff_frequency, 0.0 );
		if ( filter_cutoff_frequency != 0.0 )
			m_Filter = xo::make_lowpass_butterworth_2nd_order( filter_cutoff_frequency / 1000.0 ); // TODO: use actual update frequency

		//log::TraceF( "DofReflex TRG=%s KP=%.2f KV=%.2f C0=%.2f", m_Target.GetName().c_str(), target_pos, target_vel, constant_u );
	}

	DofReflex::~DofReflex()
	{
	}

	void DofReflex::ComputeControls( double timestamp )
	{
		Real pos = m_DelayedPos.GetValue( delay );
		Real vel = m_DelayedVel.GetValue( delay );

		if ( m_bUseRoot )
		{
			// TODO: Add world coordinate option to Body
			Real root_pos = m_DelayedRootPos.GetValue( delay );
			Real root_vel = m_DelayedRootVel.GetValue( delay );
			pos += root_pos;
			vel += root_vel;
		}

		if ( filter_cutoff_frequency != 0.0 )
		{
			pos = m_Filter( pos );
			vel = m_Filter.velocity() * 1000;
		}

		auto delta_pos = target_pos - pos;
		auto delta_vel = target_vel - vel;

		if ( condition == 0 || ( condition == -1 && delta_pos < 0 && delta_vel < 0 ) || condition == 1 && delta_pos > 0 && delta_vel > 0 )
		{
			u_p = pos_gain * delta_pos;
			u_d = vel_gain * delta_vel;
			AddTargetControlValue( constant_u + u_p + u_d );
		}
		else u_p = u_d = 0.0;
	}

	void DofReflex::StoreData( Storage<Real>::Frame& frame, const StoreDataFlags& flags ) const
	{
		frame[ name + ".RP" ] = u_p;
		frame[ name + ".RD" ] = u_d;
	}
}