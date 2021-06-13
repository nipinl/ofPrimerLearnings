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

\*---------------------------------------------------------------------------*/
#include<iostream>
#include "phaseCellsFunctionObject.H"
#include "addToRunTimeSelectionTable.H"
#include "error.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(phaseCellsFunctionObject, 0);
addToRunTimeSelectionTable(functionObject, phaseCellsFunctionObject, dictionary); 

// * * * * * * * * * * * * * * * Local Functions * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

phaseCellsFunctionObject::phaseCellsFunctionObject
(
    const word& name,
    const Time& time,
    const dictionary& dict
)
:
    functionObject(name),
    time_(time),
    mesh_(time.lookupObject<fvMesh>(polyMesh::defaultRegion)),
    fieldName_(dict.lookup("phaseIndicator")),
    alpha1_
    (
        mesh_.lookupObject<volScalarField>
        (
           fieldName_  
        )
    ),
    phaseCells_ 
    (
        IOobject
        (
            "phaseCells", 
            time.timeName(), 
            time, 
            IOobject::NO_READ, 
            IOobject::AUTO_WRITE
        ), 
        mesh_,  
        dimensionedScalar
        (
            "zero", 
            dimless,
            0.0
        )
    ),
    phaseTolerance_(dict.get<scalar>("phaseTolerance")),
    phaseDomainPercent_(0)
{}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

phaseCellsFunctionObject::~phaseCellsFunctionObject()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool phaseCellsFunctionObject::read(const dictionary& dict)
{
    notImplemented("phaseCellsFunctionObject::read(const dictionary&)"); 
    return execute(); 
}

bool phaseCellsFunctionObject::execute()
{
    //clearMarked(); //if we want to clear the phaseCells
    calcWettedCells();
    calcWettedDomainPercent();
    report();
    if(phaseDomainPercent_ >=6){
        time_.stopAt(Time::saWriteNow);//stop and write as phaseDomainPercent_ >=6%
    }

    return true;

}


void phaseCellsFunctionObject::calcWettedCells()
{
    forAll (alpha1_, cellI)
    {
        if (isWetted(alpha1_[cellI]))
        {
            phaseCells_[cellI] = 1; 
        }
    }
}

void phaseCellsFunctionObject::calcWettedDomainPercent()
{
    scalar phaseCellsSum = 0;  

    forAll (phaseCells_, cellI)
    {
        if (phaseCells_[cellI] == 1)
        {
            phaseCellsSum += 1; 
        }
    }

    phaseDomainPercent_ = 
        100. * phaseCellsSum / alpha1_.size(); 
}

void phaseCellsFunctionObject::clearMarked()
{
    forAll (phaseCells_, cellI)
    {
        phaseCells_[cellI] = 0;
    }
}

void phaseCellsFunctionObject::report()
{
    Info << "Phase " << fieldName_ << " covers " << phaseDomainPercent_ 
        << " % of the solution domain." << endl << endl; 
}

bool phaseCellsFunctionObject::start()
{
    execute(); 
    return true;
}


bool phaseCellsFunctionObject::end()
{
    return execute();
}

void phaseCellsFunctionObject::updateMesh(const mapPolyMesh& map)
{
    execute(); 
}

void phaseCellsFunctionObject::movePoints(const polyMesh& mesh)
{
    execute();  
}

bool phaseCellsFunctionObject::write()
{
    if (time_.writeTime())
    {
        phaseCells_.write();
    }
    return true;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
} // End namespace Foam


// ************************************************************************* //
