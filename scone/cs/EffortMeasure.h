#pragma once
#include "Measure.h"
#include "../core/StringMap.h"
#include "../core/Statistic.h"

namespace scone
{
	namespace cs
	{
		class EffortMeasure : public Measure
		{
		public:
			EffortMeasure( const PropNode& props, opt::ParamSet& par, sim::Model& model, const sim::Area& area );
			virtual ~EffortMeasure();

			enum EnergyMeasureType { UnknownMeasure, TotalForce, Wang2012 };
			static StringMap< EnergyMeasureType > m_MeasureNames;

			virtual UpdateResult UpdateAnalysis( const sim::Model& model, double timestamp ) override;
			virtual double GetResult( sim::Model& model ) override;

			virtual PropNode GetReport() override;

			EnergyMeasureType measure_type;
			bool use_cost_of_transport;

		protected:
			virtual String GetClassSignature() const override;

		private:
			Real m_Wang2012BasalEnergy;
			Statistic< double > m_Energy;
			Vec3 m_InitComPos;
			PropNode m_Report;

			double GetEnergy( const sim::Model& model ) const;
			double GetWang2012( const sim::Model& model ) const;
			double GetTotalForce( const sim::Model& model ) const;
		};
	}
}
