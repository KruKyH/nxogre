/** File: NxOgreD6Joint.cpp
    Created on: 21-Apr-09
    Author: Robin Southern "betajaen"
    

    � Copyright, 2008-2009 by Robin Southern, http://www.nxogre.org

    This file is part of NxOgre.

    NxOgre is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NxOgre is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with NxOgre.  If not, see <http://www.gnu.org/licenses/>.
*/

                                                                                       

#include "NxOgreStable.h"
#include "NxOgreD6Joint.h"
#include "NxOgreJoint.h"
#include "NxOgreRigidBody.h"
#include "NxOgrePrototypeFunctions.h"
#include "NxOgreScene.h"
#include "NxOgreReason.h"
#include "NxPhysics.h"

                                                                                       

namespace NxOgre
{

                                                                                       

D6Joint::D6Joint(RigidBody* first, RigidBody* second, const D6JointDescription& desc)
: Joint(first, second), mD6Joint(0)
{
 NxD6JointDesc description;
 mRigidBodies[0] = first;
 mRigidBodies[1] = second;
 description.actor[0] = mRigidBodies[0]->getNxActor();
 if (mRigidBodies[1])
  description.actor[1] = mRigidBodies[1]->getNxActor();
 else
  description.actor[1] = 0;

 Functions::PrototypeFunctions::D6JointDescriptionToNxD6JointDescription(desc, description);
 
 if (description.isValid() == false)
 {
  SharedStringStream message(SharedStringStream::_LARGE); 
  
  message << "An error occured whilst creating a D6Joint.\nThe reason(s) and cause(s) could be:\n\n";
  message << Reason::whyAsStream(description).get();
  NxOgre_ThrowError(message.get());
  return;
 }

 mJoint =  first->getScene()->getScene()->createJoint(description);
 mD6Joint = mJoint->isD6Joint();
}

D6Joint::~D6Joint(void)
{
 mRigidBodies[0]->getScene()->getScene()->releaseJoint(*mJoint);
}

Enums::JointFunctionType D6Joint::getJointType()
{
 return Enums::JointFunctionType_D6;
}


                                                                                       

} // namespace NxOgre

                                                                                       
