/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Description
    A function object that marks cells in the domain that get phase
    during the course of the simulation.

SourceFiles
    phaseCellsFunctionObject.C

Authors: 
    Tomislav Maric tomislav.maric@gmx.com

\*---------------------------------------------------------------------------*/

#ifndef phaseCellsFunctionObject_H
#define phaseCellsFunctionObject_H

#include "functionObject.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         A templated functionObject
\*---------------------------------------------------------------------------*/

class phaseCellsFunctionObject
    :
        public functionObject
{
    // Private data

        //- Time.
        const Time& time_; 

        //- Mesh const reference.
        const fvMesh& mesh_; 

        //- Name of the phase indicator field.
	word fieldName_;

	//- Reference to the phase indicator. 
        const volScalarField& alpha1_; 

        //- Phase cells field.
        volScalarField phaseCells_; 

        //- Phase cell tolerance. 
        scalar phaseTolerance_; 

        //- Percent of cells that have contained the phase.
        scalar phaseDomainPercent_; 

public:

    //- Runtime type information
    TypeName("phaseCells");

    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        phaseCellsFunctionObject
        (
            const word& name,
            const Time& time,
            const dictionary& dict
        );

    //- Destructor
     ~phaseCellsFunctionObject();


    // Member Functions

        //- Access time
        const Time& getTime() const
        {
            return time_; 
        }

        //- Read the system calls
         bool read(const dictionary&);

        //- Execute the "executeCalls" at each time-step
	 bool execute();

	//- Do nothing
	 bool write();

        //- Calculate phase cells
         virtual void calcWettedCells(); 

        //- Test if volume fraction value marks the cell phase. 
         bool isWetted(scalar s)
         {
             return (s > phaseTolerance_); 
         } 


        //- Compute phase domain percent.
         virtual void calcWettedDomainPercent(); 
        
         //- Compute phase domain percent.
         virtual void clearMarked(); 

        //- Get phase domain perceintage.
         scalar getWettedDomainPercent()
         {
             return phaseDomainPercent_; 
         } 

        //- Report 
        virtual void report(); 
        
        //- Called at the start of the time-loop
         bool start();

        //- Execute the "endCalls" at the final time-loop
         bool end();

        //- Called when time was set at the end of the Time::operator++
         //bool timeSet();

        //- Update for changes of mesh
         void updateMesh(const mapPolyMesh&);

        //- Update for changes of mesh
         void movePoints(const polyMesh&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
