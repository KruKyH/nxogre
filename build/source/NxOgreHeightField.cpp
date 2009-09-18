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
#include "NxOgreHeightField.h"
#include "NxOgreResourceSystem.h"
#include "NxOgrePhysXStream.h"
#include "NxOgreXFunctions.h"
#include "NxOgreManualHeightField.h"
#include "NxOgrePhysXUserEntityReport.h"

#include "NxPhysics.h"

                                                                                       

namespace NxOgre
{

                                                                                       

HeightField::HeightField()
{
 mHeightField = 0;
}

HeightField::HeightField(Resource* resource)
{
 mHeightField = 0;
 load(resource);
}

HeightField::HeightField(const String& name, NxHeightField* hf)
 : mName(name),
   mHeightField(hf)
{
}

HeightField::~HeightField(void)
{
 if (mHeightField == 0)
  return;
 
 NxPhysicsSDK* sdk = NxGetPhysicsSDK();
 sdk->releaseHeightField(*mHeightField);
}


bool HeightField::isLoaded(void) const
{
 return mHeightField != 0;
}

bool HeightField::isUsed(void) const
{
 return (mHeightField && mHeightField->getReferenceCount() != 0);
}

void HeightField::load(Resource* resource)
{
 if (!Functions::XFunctions::isX(resource))
 {
  SharedStringStream ss;
  ss << "Error: " << resource->getArchiveResourceIdentifier().getArchive() << ":" << resource->getArchiveResourceIdentifier().getResourceName() << " is not a NxOgre file. Reasons(s) are:\n";
  ss << Functions::XFunctions::whyIsNotX(resource);
  NxOgre_ThrowError(ss.get());
  return;
 }

 if (!Functions::XFunctions::getXType(resource) == Enums::XType_HeightField)
 {
  SharedStringStream ss;
  ss << "Error: " << resource->getArchiveResourceIdentifier().getArchive() << ":" << resource->getArchiveResourceIdentifier().getResourceName() << " is not a NxOgre heightfield file\n";
  ss << Functions::XFunctions::whyIsNotX(resource);
  NxOgre_ThrowError(ss.get());
  return;
 }

 resource->seekBeginning();
 resource->seek(8);

 // hasMaterialAlias (Currently not implemented)
 bool hasMaterialAlias = resource->readBool();

 // nbRows (Number of Rows)
 unsigned short nbRows = resource->readShort();

 // nbColumns (Number of Columns)
 unsigned short nbColumns = resource->readShort();

 unsigned int wh = nbRows * nbColumns;
 
 ManualHeightField heightfield;
 heightfield.begin(nbColumns, nbRows);
 for (unsigned int i=0; i < wh; i++)
 {
  HeightFieldSample sample;
  resource->readCharArray( (char*) &sample, sizeof(NxHeightFieldSample));
  heightfield.sample(sample);
 }
 
 mHeightField = heightfield.cook();
}

void HeightField::setName(const char* name)
{
 mName = name;
}

void HeightField::setName(const String& name)
{
 mName = name;
}

String HeightField::getName() const
{
 return mName;
}

NxHeightField* HeightField::getHeightField(void)
{
 return mHeightField;
}


                                                                                       

} // namespace NxOgre

                                                                                       
