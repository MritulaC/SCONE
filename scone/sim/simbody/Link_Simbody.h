#pragma once

#include "simbody.h"
#include "..\Link.h"


namespace scone
{
	namespace sim
	{
		class SIM_SIMBODY_API Link_Simbody : public Link
		{
		public:
			Link_Simbody( ) { };
			Link_Simbody( const class Model& model ) { };
			virtual ~Link_Simbody() { };

			virtual Vec3 GetPos() override;
			virtual Quat GetOri() override;

			virtual Vec3 GetLinVel() override;
			virtual Vec3 GetAngVel() override;

			virtual class Model& GetModel() override;

			virtual size_t GetParentJointCount() override;
			virtual class Joint& GetParentJoint( size_t idx ) override;
			virtual size_t GetChildJointCount() override;
			virtual class Joint& GetChildJoint( size_t idx ) override;

			virtual size_t GetParentLinkCount() override;
			virtual size_t GetParentLink( size_t idx ) override;
			virtual size_t GetChildLinkCount() override;
			virtual size_t GetChildLink( size_t idx ) override;
		};
	}
}
