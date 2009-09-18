/** 
    
    This file is part of NxOgre.
    
    Copyright (c) 2009 Robin Southern, http://www.nxogre.org
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
    
*/

                                                                                       

#include "NxOgreStable.h"
#include "NxOgreSphericalJoint.h"
#include "NxOgreJoint.h"
#include "NxOgreRigidBody.h"
#include "NxOgrePrototypeFunctions.h"
#include "NxOgreScene.h"
#include "NxOgreReason.h"
#include "NxPhysics.h"

                                                                                       

namespace NxOgre
{

                                                                                       

SphericalJoint::SphericalJoint(RigidBody* first, RigidBody* second, const SphericalJointDescription& desc)
: Joint(first, second), mSphericalJoint(0)
{
 NxSphericalJointDesc description;
 mRigidBodies[0] = first;
 mRigidBodies[1] = second;
 description.actor[0] = mRigidBodies[0]->getNxActor();
 if (mRigidBodies[1])
  description.actor[1] = mRigidBodies[1]->getNxActor();
 else
  description.actor[1] = 0;

 Functions::PrototypeFunctions::SphericalJointDescriptionToNxSphericalJointDescription(desc, description);
 
 if (description.isValid() == false)
 {
  SharedStringStream message(SharedStringStream::_LARGE); 
  
  message << "An error occured whilst creating a SphericalJoint.\nThe reason(s) and cause(s) could be:\n\n";
  message << Reason::whyAsStream(description).get();
  NxOgre_ThrowError(message.get());
  return;
 }

 mJoint =  first->getScene()->getScene()->createJoint(description);
 mSphericalJoint = mJoint->isSphericalJoint();
}

SphericalJoint::~SphericalJoint(void)
{
 mRigidBodies[0]->getScene()->getScene()->releaseJoint(*mJoint);
}

void SphericalJoint::setProjectionMode(Enums::JointProjectionMode mode)
{
 mSphericalJoint->setProjectionMode(NxJointProjectionMode(int(mode)));
}

Enums::JointProjectionMode SphericalJoint::getProjectionMode(void) const
{
 return Enums::JointProjectionMode(int(mSphericalJoint->getProjectionMode()));
}

Enums::JointFunctionType SphericalJoint::getJointType()
{
 return Enums::JointFunctionType_Spherical;
}


                                                                                       

} // namespace NxOgre

                                                                                       
