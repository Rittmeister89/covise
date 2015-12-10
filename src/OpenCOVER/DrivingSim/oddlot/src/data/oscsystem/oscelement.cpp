/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

/**************************************************************************
** ODD: OpenDRIVE Designer
**   Frank Naegele (c) 2010
**   <mail@f-naegele.de>
**   02.02.2010
**
**************************************************************************/

#include "oscelement.hpp"

//#include "oscObject.h";

//using namespace OpenScenario;

/*! \brief The constructor does nothing special.
*
*/
OSCElement::OSCElement(OpenScenario::oscObject *oscObject)
    : DataElement() 
	, oscObject_(oscObject)
 //   , rSystemElementChanges_(0x0)
{
}

OSCElement::~OSCElement()
{
}

/*! \brief Accepts a visitor.
*
* With autotraverse: visitor will be send to roads, fiddleyards, etc.
* Without: accepts visitor as 'this'.
*/
void
OSCElement::accept(Visitor *visitor)
{
    visitor->visit(this);
}


//##################//
// Observer Pattern //
//##################//

/*! \brief Called after all Observers have been notified.
*
* Resets the change flags to 0x0.
*/
/*void
RSystemElement::notificationDone()
{
    rSystemElementChanges_ = 0x0;
    DataElement::notificationDone(); // pass to base class
}*/

/*! \brief Add one or more change flags.
*
*/
/*void
RSystemElement::addRSystemElementChanges(int changes)
{
    if (changes)
    {
        rSystemElementChanges_ |= changes;
        notifyObservers();
    }
}*/
