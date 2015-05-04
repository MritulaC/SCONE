#pragma once

#include "Measure.h"
#include "cs.h"
#include "../sim/sim.h"

namespace scone
{
	namespace cs
	{
		class CS_API CompositeMeasure : public Measure
		{
		public:
			CompositeMeasure( const PropNode& props, opt::ParamSet& par, sim::Model& model, const sim::Area& area );
			virtual ~CompositeMeasure();

			virtual void UpdateControls( sim::Model& model, double timestamp ) override;
			virtual double GetResult( sim::Model& model ) override;
			virtual PropNode GetReport() override;
			virtual String GetSignature() override;

			struct Term
			{
				Term() : weight( 0.0 ), measure( nullptr ) {};
				Term( Term&& other ) : name( other.name ), weight( other.weight ), measure( std::move( other.measure ) ) {};
				String name;
				double weight;
				MeasureUP measure;
			private:
				Term( const Term& other );
			};

		private:
			CompositeMeasure( CompositeMeasure& other );
			CompositeMeasure& operator=( CompositeMeasure& other );

			std::vector< Term > m_Terms;
			PropNode m_Report;
		};
	}
}
